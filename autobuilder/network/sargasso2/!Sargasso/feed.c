/*
 * This file is part of Sargasso, http://zamez.org/sargasso
 * Licensed under the GNU General Public License,
 *                http://www.opensource.org/licenses/gpl-license
 * Copyright 2006 James Bursa <james@zamez.org>
 */

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef DEBUG1
#include <libxml/debugXML.h>
#endif
#include <pcre.h>
#include <openssl/ssl.h>
#include "feed.h"
#ifdef FORTIFY
#include "fortify.h"
#endif

#define OVECCOUNT 30

unsigned int feed_count = 0;
struct feed *feeds = 0;
bool feed_work_needed = false;
const char *feed_error = 0;
static const char *feed_status_name[] = { "NEW", "FETCHING", "OK", "ERROR", "PAUSED" };

static CURLM *curl_multi_handle;
static unsigned int fetching = 0;

static void feed_work_feed (struct feed *feed);
static void feed_create_fetch (struct feed *feed);
static void feed_start_fetch (struct feed *feed);
static size_t feed_header_callback (void *ptr, size_t size, size_t nmemb,
				    void *stream);
static size_t feed_write_callback (void *ptr, size_t size, size_t nmemb,
				   void *stream);
static void feed_fetched (struct feed *feed, CURLcode result);
static void feed_parse (struct feed *feed);
static void feed_parse_item (struct feed *feed, xmlNode * node);
static void feed_free_item (struct feed *feed, unsigned int i);
static void feed_clean_text (xmlChar * text, bool process);
static CURLcode fetch_curl_sslctxfun (CURL * curl_handle, void *_sslctx,
				      void *p);
static int fetch_curl_verify_callback (int preverify_ok,
				       X509_STORE_CTX * x509_ctx);

 /**
  * cURL SSL setup callback
  *
  * \param curl_handle The curl handle to perform the ssl operation on.
  * \param _sslctx The ssl context.
  * \param parm The callback context.
  * \return A curl result code.
  */
static CURLcode
fetch_curl_sslctxfun (CURL * curl_handle, void *_sslctx, void *parm)
{
  SSL_CTX *sslctx = _sslctx;
  long options = SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3;

  /* set verify callback for each certificate in chain */
  SSL_CTX_set_verify (sslctx, SSL_VERIFY_PEER | SSL_VERIFY_CLIENT_ONCE,
		      fetch_curl_verify_callback);
  SSL_CTX_set_verify_depth (sslctx, MAX_CERTS + 1);

  SSL_CTX_set_options (sslctx, options);

  return CURLE_OK;
}

/**
 * Initialise the feed module.
 */

bool
feed_init (void)
{
  CURLcode code;

  code = curl_global_init (CURL_GLOBAL_ALL);
  if (code != CURLE_OK)
    {
      feed_error = curl_easy_strerror (code);
      return false;
    }

  curl_multi_handle = curl_multi_init ();
  if (!curl_multi_handle)
    {
      feed_error = "Failed to initialise curl";
      return false;
    }

  curl_multi_setopt(curl_multi_handle, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);

  xmlInitParser ();

  return true;
}


/**
 * Quit the feed module.
 */

void
feed_quit (void)
{
  while (feed_count)
    feed_remove (0);
  free (feeds);
  feeds = 0;

  xmlCleanupParser ();

  curl_multi_cleanup (curl_multi_handle);
}


/**
 * Add a new feed.
 */

bool
feed_add (const char *url)
{
  struct feed *feeds1;
  struct feed *feed;
  char *url1;
  unsigned int i, j = 0;

  assert (url);

  feeds1 = realloc (feeds, sizeof *feed * (feed_count + 1));
  if (!feeds1)
    {
      feed_error = "Out of memory";
      return false;
    }
  feeds = feeds1;

  url1 = strdup (url);
  if (!url1)
    {
      feed_error = "Out of memory";
      free (url1);
      url1 = 0;
      return false;
    }

  feed = &feeds[feed_count];
  feed->url = url1;
  feed->status = FEED_NEW;
  feed->error = 0;
  feed->status_line = 0;
  feed->etag = 0;
  feed->redirect = 0;
  feed->data = 0;
  feed->data_size = 0;
  feed->title = 0;
  feed->description = 0;
  feed->link = 0;
  feed->copyright = 0;
  feed->pub_date = 0;
  feed->category = 0;
  for (i = 0; i != FEED_MAX_ITEMS; i++)
    {
      feed->item[i].title = 0;
      feed->item[i].description = 0;
      feed->item[i].link = 0;
      feed->item[i].author = 0;
      feed->item[i].pub_date = 0;
      feed->item[i].category = 0;
      feed->item[i].guid = 0;
      feed->item[i].new_item = false;
      for (j = 0; j != MAX_DESC_ITEMS; j++)
	{
	  feed->item[i].paragraph[j] = NULL;
	  feed->item[i].paragraph_style[j] = 0;
	}
    }
  feed->item_count = 0;

  feed_count++;
  feed_work_needed = true;

  printf ("added feed %s\n", url);

  return true;
}


/**
 * Remove a feed.
 */

bool
feed_remove (unsigned int i)
{
  unsigned int j = 0;

  assert (i < feed_count);

  if (feeds[i].status == FEED_FETCHING)
    {
      curl_multi_remove_handle (curl_multi_handle, feeds[i].curl);
      curl_easy_cleanup (feeds[i].curl);
      curl_slist_free_all (feeds[i].headers);
      feeds[i].headers = 0;
    }

  for (j = 0; j != feeds[i].item_count; j++)
    feed_free_item (&feeds[i], j);

  free (feeds[i].url);
  feeds[i].url = 0;
  free (feeds[i].status_line);
  feeds[i].status_line = 0;

  if (feeds[i].etag)
    {
      free (feeds[i].etag);
      feeds[i].etag = 0;
    }
  if (feeds[i].redirect)
    {
      free (feeds[i].redirect);
      feeds[i].redirect = 0;
    }

  if (feeds[i].data)
    {
      free (feeds[i].data);
      feeds[i].data = 0;
    }

  if (feeds[i].title)
    xmlFree (feeds[i].title);
  if (feeds[i].description)
    xmlFree (feeds[i].description);
  if (feeds[i].link)
    xmlFree (feeds[i].link);
  if (feeds[i].copyright)
    xmlFree (feeds[i].copyright);
  if (feeds[i].pub_date)
    xmlFree (feeds[i].pub_date);
  if (feeds[i].category)
    xmlFree (feeds[i].category);

  if (i != feed_count - 1)
    memmove (&feeds[i], &feeds[i + 1],
	     (sizeof feeds[0]) * (feed_count - i - 1));

  feed_count--;

  return true;
}


/**
 * Set the status of a feed.
 */

void
feed_set_status (struct feed *feed, feed_status status)
{
  feed->status = status;
  feed->updated = true;
  printf ("status %s %s => %s\n", feed->url,
	  feed_status_name[feed->status], feed_status_name[status]);

}


/**
 * Do some work on the feeds.
 */

bool
feed_work (void)
{
  unsigned int i;
  int running;
  int queue;
  CURLMsg *msg;
  CURLcode res;
  struct feed *feed;

  for (i = 0; i != feed_count; i++)
    {
      feeds[i].updated = false;
      feed_work_feed (&feeds[i]);
    }

  feed_work_needed = false;

  while (curl_multi_perform (curl_multi_handle, &running) ==
	 CURLM_CALL_MULTI_PERFORM)
    continue;

  if ((msg = curl_multi_info_read (curl_multi_handle, &queue)))
    {
      if (msg->msg == CURLMSG_DONE)
	{
	  res =
	    curl_easy_getinfo ((CURL *) msg->easy_handle, CURLINFO_PRIVATE,
			       &feed);
	  if ((CURLE_OK == res) && feed)
	    feed_fetched (feed, msg->data.result);
	}
    }

  for (i = 0; i != feed_count; i++)
    if (feeds[i].status == FEED_NEW ||
	feeds[i].status == FEED_FETCHING || feeds[i].status == FEED_UPDATE)
      feed_work_needed = true;

  for (i = 0; i != feed_count; i++)
    if (feeds[i].updated)
      return true;
  return false;
}


/**
 * Do some work on a feed.
 */

void
feed_work_feed (struct feed *feed)
{
  assert (feed);

  if ((feed->status == FEED_NEW || feed->status == FEED_UPDATE) &&
      fetching < max_fetches)
    {
      feed_create_fetch (feed);
      if (feed->status != FEED_ERROR)
	feed_start_fetch (feed);
    }
}


/**
 * Create a fetch for a feed.
 */

void
feed_create_fetch (struct feed *feed)
{
  CURL *curl;
  struct curl_slist *headers = 0;
  struct curl_slist *headers2 = 0;

  curl = curl_easy_init ();
  if (!curl)
    {
      feed_set_status (feed, FEED_ERROR);
      feed->error = "Failed to create curl session";
      return;
    }

  headers2 = curl_slist_append (headers, "Accept: "
				"application/rss+xml, application/xml, text/xml");
  if (!headers2)
    {
      curl_easy_cleanup (feed);
      curl_slist_free_all (headers);
      feed_set_status (feed, FEED_ERROR);
      feed->error = "Out of memory";
      return;
    }
  headers = headers2;

  if (feed->etag)
    {
      size_t n = 20 + strlen (feed->etag);
      char if_none_match[n];
      snprintf (if_none_match, n, "If-None-Match: %s", feed->etag);
      headers2 = curl_slist_append (headers, if_none_match);
      if (!headers2)
	{
	  curl_easy_cleanup (curl);
	  curl_slist_free_all (headers);
	  feed_set_status (feed, FEED_ERROR);
	  feed->error = "Out of memory";
	  return;
	}
      headers = headers2;
    }

  curl_easy_setopt (curl, CURLOPT_FORBID_REUSE, 1L);
  curl_easy_setopt (curl, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt (curl, CURLOPT_URL, feed->url);
  curl_easy_setopt (curl, CURLOPT_PRIVATE, feed);
  curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, feed_write_callback);
  curl_easy_setopt (curl, CURLOPT_WRITEDATA, feed);
  curl_easy_setopt (curl, CURLOPT_HEADERFUNCTION, feed_header_callback);
  curl_easy_setopt (curl, CURLOPT_HEADERDATA, feed);
  curl_easy_setopt (curl, CURLOPT_USERAGENT,
		    "Sargasso (http://zamez.org/sargasso)");
  curl_easy_setopt (curl, CURLOPT_ENCODING, "gzip");
  curl_easy_setopt (curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt (curl, CURLOPT_LOW_SPEED_LIMIT, 1L);
  curl_easy_setopt (curl, CURLOPT_LOW_SPEED_TIME, 60L);
  curl_easy_setopt (curl, CURLOPT_NOSIGNAL, 1L);
  curl_easy_setopt (curl, CURLOPT_CONNECTTIMEOUT, 30L);
  curl_easy_setopt (curl, CURLOPT_SSL_CTX_FUNCTION, *fetch_curl_sslctxfun);
  curl_easy_setopt (curl, CURLOPT_SSL_CTX_DATA, feed);
  curl_easy_setopt (curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
  curl_easy_setopt (curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt (curl, CURLOPT_MAXREDIRS, 2L);
  curl_easy_setopt (curl, CURLOPT_PROXY, http_proxy);
  curl_easy_setopt (curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
  curl_easy_setopt (curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);

  feed->curl = curl;
  feed->headers = headers;
  feed->redirect_count = 0;
}


/**
 * Start fetching a feed.
 */

void
feed_start_fetch (struct feed *feed)
{
  CURLMcode mcode;

  mcode = curl_multi_add_handle (curl_multi_handle, feed->curl);
  if (mcode != CURLM_OK)
    {
      feed_set_status (feed, FEED_ERROR);
      feed->error = curl_multi_strerror (mcode);
      curl_easy_cleanup (feed->curl);
      curl_slist_free_all (feed->headers);
      feed->curl = NULL;
      feed->headers = NULL;
      return;
    }
  if (feed->status_line)
    {
      free (feed->status_line);
      feed->status_line = 0;
      feed->data_size = 0;	// FIXME was NULL
      feed->error = NULL;
    }

  feed_set_status (feed, FEED_FETCHING);

  printf ("fetching feed %s\n", feed->url);

  fetching++;
  feed_work_needed = true;
}


/**
 * Callback for receiving headers for a feed.
 */

size_t
feed_header_callback (void *ptr, size_t size, size_t nmemb, void *stream)
{
  struct feed *feed = (struct feed *) stream;
  size_t n = size * nmemb;
  char header[n + 1];
  char *value;

  strncpy (header, ptr, n);
  header[n] = 0;
  while (0 < n && header[n - 1] <= 32)
    header[--n] = 0;

  if (!feed->status_line)
    {
      feed->status_line = strdup (header);
      return size * nmemb;
    }

  value = strchr (header, ':');
  if (!value)
    return size * nmemb;
  *value = 0;
  value++;
  while (isspace (*value))
    value++;

  if (strcasecmp (header, "ETag") == 0 && value[0] == '"')
    {
      if (feed->etag)
	{
	  free (feed->etag);
	  feed->etag = 0;
	}
      feed->etag = strdup (value);
    }
  else if (strcasecmp (header, "Location") == 0)
    {
      if (feed->redirect)
	{
	  free (feed->redirect);
	  feed->redirect = 0;
	}
      feed->redirect = strdup (value);
    }

  return size * nmemb;
}


/**
 * Callback for receiving data for a feed.
 */

size_t
feed_write_callback (void *ptr, size_t size, size_t nmemb, void *stream)
{
  struct feed *feed = (struct feed *) stream;
  char *data;

  printf ("received %u for %s\n", (unsigned int) (size * nmemb), feed->url);

  data = realloc (feed->data, feed->data_size + size * nmemb);
  if (!data)
    {
      feed_set_status (feed, FEED_ERROR);
      feed->error = "Out of memory";
      return 0;
    }

  memcpy (data + feed->data_size, ptr, size * nmemb);
  feed->data = data;
  feed->data_size += size * nmemb;

  return size * nmemb;
}


/**
 * Process a complete feed fetch.
 */

void
feed_fetched (struct feed *feed, CURLcode result)
{
  long http_code;

  printf ("finished %s with result %i %s\n",
	  feed->url, result, curl_easy_strerror (result));

  fetching--;

  if (result == CURLE_OK)
    {
      curl_easy_getinfo (feed->curl, CURLINFO_RESPONSE_CODE, &http_code);
      printf ("HTTP code %li\n", http_code);
      if (http_code == 0 || http_code == 200 /* OK */ )
	{
	  feed_parse (feed);
	}
      else if (http_code == 300 /* Multiple Choices */  ||
	       http_code == 301 /* Moved Permanently */  ||
	       http_code == 302 /* Found */  ||
	       http_code == 303 /* See Other */  ||
	       http_code == 307 /* Temporary Redirect */ )
	{
	  if (feed->redirect_count++ == 5)
	    {
	      feed_set_status (feed, FEED_ERROR);
	      feed->error = "Too many redirects.";
	    }
	  else if (feed->redirect)
	    {
	      curl_multi_remove_handle (curl_multi_handle, feed->curl);
	      curl_easy_setopt (feed->curl, CURLOPT_URL, feed->redirect);
	      feed_start_fetch (feed);
	      if (http_code == 301 /* Moved Permanently */ )
		{
		  free (feed->url);
		  feed->url = 0;
		  feed->url = feed->redirect;
		  feed->redirect = 0;
		}
	      return;
	    }
	  else
	    {
	      feed_set_status (feed, FEED_ERROR);
	      feed->error = "Invalid redirect.";
	    }
	}
      else if (http_code == 304 /* Not Modified */ )
	{
	  feed_set_status (feed, FEED_OK);
	}
      else
	{
	  feed_set_status (feed, FEED_ERROR);
	  if (feed->status_line)
	    feed->error = feed->status_line;
	  else
	    feed->error = "Response not understood.";
	}

    }
  else
    {
      feed_set_status (feed, FEED_ERROR);
      if (!feed->error)
	feed->error = curl_easy_strerror (result);
    }

  curl_multi_remove_handle (curl_multi_handle, feed->curl);
  curl_easy_cleanup (feed->curl);
  feed->curl = 0;
  curl_slist_free_all (feed->headers);
  feed->headers = 0;
  if (feed->data)
    {
      free (feed->data);
      feed->data = 0;
    }
}


/**
 * Parse a feed's XML.
 */

void
feed_parse (struct feed *feed)
{
  xmlDoc *doc;
  xmlNode *rss;
  xmlNode *channel;
  xmlNode *node;
  bool process;

  assert (feed);
  assert (feed->status == FEED_FETCHING);

#ifdef DEBUG1

	  fprintf (stderr, "Feed: %s\n", feed->data);
	  fflush (stderr);
#endif
  doc = xmlReadMemory (feed->data, feed->data_size, feed->url, 0, 0);
  if (!doc)
    {
      feed_set_status (feed, FEED_ERROR);
      feed->error = "failed to parse XML";
      return;
    }

#ifdef DEBUG
  xmlDebugDumpDocument (stdout, doc);
#endif

  if (feed->data)
    {
      free (feed->data);
      feed->data = 0;
    }

  for (rss = doc->children; rss; rss = rss->next)
    if ((rss->type == XML_ELEMENT_NODE
	 && !xmlStrcmp (rss->name, (const xmlChar *) "rss"))
	|| (rss->type == XML_ELEMENT_NODE
	    && !xmlStrcmp (rss->name, (const xmlChar *) "rdf"))
	|| (rss->type == XML_ELEMENT_NODE
	    && !xmlStrcmp (rss->name, (const xmlChar *) "feed")))
      break;
  if (!rss)
    {
      feed_set_status (feed, FEED_ERROR);
      feed->error = "rss element not found";
      xmlFreeDoc (doc);
      return;
    }

  if (rss->type == XML_ELEMENT_NODE
      && !xmlStrcmp (rss->name, (const xmlChar *) "feed"))
    {
      for (node = rss->last; node; node = node->prev)
	{
	  if (node->type != XML_ELEMENT_NODE)
	    continue;

	  if (!xmlStrcmp (node->name, (const xmlChar *) "title"))
	    {
	      if (feed->title)
		xmlFree (feed->title);
	      feed->title = xmlNodeGetContent (node);
	    }
	  else if (!xmlStrcmp (node->name, (const xmlChar *) "summary"))
	    {
	      if (feed->description)
		xmlFree (feed->description);
	      feed->description = xmlNodeGetContent (node);

	    }
	  else if (!xmlStrcmp (node->name, (const xmlChar *) "link"))
	    {
	      if (feed->link)
		xmlFree (feed->link);
	      feed->link = xmlGetProp (node, (const xmlChar *) "href");
	    }
	  else if (!xmlStrcmp (node->name, (const xmlChar *) "copyright"))
	    {
	      if (feed->copyright)
		xmlFree (feed->copyright);
	      feed->copyright = xmlNodeGetContent (node);

	    }
	  else if (!xmlStrcmp (node->name, (const xmlChar *) "updated"))
	    {
	      if (feed->pub_date)
		xmlFree (feed->pub_date);
	      feed->pub_date = xmlNodeGetContent (node);

	    }
	  else if (!xmlStrcmp (node->name, (const xmlChar *) "category"))
	    {
	      if (feed->category)
		xmlFree (feed->category);
	      feed->category = xmlNodeGetContent (node);

	    }
	  else if (!xmlStrcmp (node->name, (const xmlChar *) "entry"))
	    {
	      feed_parse_item (feed, node);
	    }
	}

    }
  else
    {
      for (channel = rss->children; channel; channel = channel->next)
	if (channel->type == XML_ELEMENT_NODE &&
	    !xmlStrcmp (channel->name, (const xmlChar *) "channel"))
	  break;
      if (!channel)
	{
	  feed_set_status (feed, FEED_ERROR);
	  feed->error = "channel element not found";
	  xmlFreeDoc (doc);
	  return;
	}

      for (node = channel->last; node; node = node->prev)
	{
	  if (node->type != XML_ELEMENT_NODE)
	    continue;

	  if (!xmlStrcmp (node->name, (const xmlChar *) "title"))
	    {
	      if (feed->title)
		xmlFree (feed->title);
	      feed->title = xmlNodeGetContent (node);

	    }
	  else if (!xmlStrcmp (node->name, (const xmlChar *) "description"))
	    {
	      if (feed->description)
		xmlFree (feed->description);
	      feed->description = xmlNodeGetContent (node);

	    }
	  else if (!xmlStrcmp (node->name, (const xmlChar *) "link"))
	    {
	      if (feed->link)
		xmlFree (feed->link);
	      feed->link = xmlNodeGetContent (node);

	    }
	  else if (!xmlStrcmp (node->name, (const xmlChar *) "copyright"))
	    {
	      if (feed->copyright)
		xmlFree (feed->copyright);
	      feed->copyright = xmlNodeGetContent (node);

	    }
	  else if (!xmlStrcmp (node->name, (const xmlChar *) "pubDate"))
	    {
	      if (feed->pub_date)
		xmlFree (feed->pub_date);
	      feed->pub_date = xmlNodeGetContent (node);

	    }
	  else if (!xmlStrcmp (node->name, (const xmlChar *) "category"))
	    {
	      if (feed->category)
		xmlFree (feed->category);
	      feed->category = xmlNodeGetContent (node);

	    }
	  else if (!xmlStrcmp (node->name, (const xmlChar *) "item"))
	    {
	      feed_parse_item (feed, node);
	    }
	}

    }
  if (doc)
    xmlFreeDoc (doc);

  process = true;

  if (feed->description)
    {
      if (*feed->description == '<')
	{
#ifdef DEBUG
	  fprintf (stderr, "Pre Clean1: %s\n", feed->description);
	  fflush (stderr);
#endif
	  process = false;
	}
    }

  if (feed->title != NULL)
    {
      if (strlen ((const char *) feed->title) == 0)
	{
	  feed->title = 0;
	}
    }

  if (feed->description != NULL)
    {
      if (strlen ((const char *) feed->description) == 0)
	{
	  feed->description = 0;
	}
    }
  feed_clean_text (feed->title, process);
  feed_clean_text (feed->pub_date, process);

#ifdef DEBUG
  fprintf (stderr, "Pre Clean2: %d %s\n", process, feed->description);
  fprintf (stderr, "\n");
  fflush (stderr);
#endif
  feed_clean_text (feed->description, process);
  feed_clean_text (feed->link, process);
  feed_clean_text (feed->copyright, process);
  feed_clean_text (feed->category, process);

  feed_set_status (feed, FEED_OK);
}


void
feed_parse_item (struct feed *feed, xmlNode *node)
{
  xmlNode *child;
  xmlChar *title = NULL;
  xmlChar *description = NULL;
  xmlChar *link = NULL;
  xmlChar *author = NULL;
  xmlChar *pub_date = NULL;
  xmlChar *category = NULL;
  xmlChar *guid = NULL;
  xmlDoc *pdoc = NULL;
  xmlNode *pnode = NULL;
  xmlNode *nodetmp = NULL;
  char *d;
  unsigned int i, j;		// , k;
  bool process, skip;
  
  pcre *re;
  const char *error;
  int erroffset;
  int ovector[OVECCOUNT];
  int rc;

  for (child = node->children; child; child = child->next)
    {
      if (child->type != XML_ELEMENT_NODE)
	continue;
      if (!xmlStrcmp (child->name, (const xmlChar *) "title"))
	title = xmlNodeGetContent (child);
      else if (!xmlStrcmp (child->name, (const xmlChar *) "description"))
	description = xmlNodeGetContent (child);
      else if (!xmlStrcmp (child->name, (const xmlChar *) "summary"))
	description = xmlNodeGetContent (child);
      else if (!xmlStrcmp (child->name, (const xmlChar *) "link"))
	{
	  if (!(link = xmlGetProp (child, (const xmlChar *) "href")))
	    link = xmlNodeGetContent (child);
	}
      else if (!xmlStrcmp (child->name, (const xmlChar *) "author"))
	author = xmlNodeGetContent (child);
      else if (!xmlStrcmp (child->name, (const xmlChar *) "username"))
	author = xmlNodeGetContent (child);
      else if (!xmlStrcmp (child->name, (const xmlChar *) "pubDate"))
	pub_date = xmlNodeGetContent (child);
      else if (!xmlStrcmp (child->name, (const xmlChar *) "updated"))
	pub_date = xmlNodeGetContent (child);
      else if (!xmlStrcmp (child->name, (const xmlChar *) "category"))
	category = xmlNodeGetContent (child);
      else if (!xmlStrcmp (child->name, (const xmlChar *) "id"))
	guid = xmlNodeGetContent (child);
      else if (!xmlStrcmp (child->name, (const xmlChar *) "guid"))
	guid = xmlNodeGetContent (child);
    }

  process = true;

  if (description)
    {
      if (*description == '<')
	{
#ifdef DEBUG
	  fprintf (stderr, "Pre Clean: %s\n", description);
	  fprintf (stderr, "\n");
	  fflush (stderr);
#endif
	  process = false;
	}
    }

  feed_clean_text (title, process);
  feed_clean_text (pub_date, process);
  feed_clean_text (description, process);
#ifdef DEBUG
  fprintf (stderr, "Post Clean1a: %d %s %s title: %s\n", process, description, feed->title);
  fprintf (stderr, "\n");
  fflush (stderr);
#endif
  feed_clean_text (link, process);
  feed_clean_text (author, process);
  feed_clean_text (category, process);
  feed_clean_text (guid, process);

#ifdef DEBUG
  fprintf (stderr, "Title: %s\n", title);
  fprintf (stderr, "\n");
  fflush (stderr);

  fprintf (stderr, "Author: %s\n", author);
  fprintf (stderr, "\n");
  fflush (stderr);

  fprintf (stderr, "Exclude: %s %s %s %s %s %s %s %s\n", type1, exclude1,
	   type2, exclude2, type3, exclude3, type4, exclude4);
  fprintf (stderr, "\n");
  fflush (stderr);
#endif

  skip = false;

  if (strcmp((const char *) type1, "title") == 0)
	  if (strcmp((const char *) title, exclude1) == 0)
		  skip = true;

  if (strcmp ((const char *) type1, "title") == 0 && skip == false) {
  
     re = pcre_compile(
     exclude1,     /* the pattern */
     0,           /* default options */
     &error,      /* for error message */
     &erroffset,  /* for error offset */
     NULL);       /* use default character tables */
     
     rc = pcre_exec(
     re,          /* the compiled pattern */
     NULL,        /* we didn't study the pattern */
     title,     /* the subject string */
     (int)strlen(title), /* the length of the subject */
     0,           /* start at offset 0 in the subject */
     0,           /* default options */
     ovector,     /* vector for substring information */
     OVECCOUNT);  /* number of elements in the vector */
     
    if (rc > 0)
      skip = true;      
  }

  if (strcmp((const char *) type2, "title") == 0)
	  if (strcmp((const char *) title, exclude2) == 0)
		  skip = true;

  if (strcmp ((const char *) type2, "title") == 0 && skip == false) {
  
     re = pcre_compile(
     exclude2,     /* the pattern */
     0,           /* default options */
     &error,      /* for error message */
     &erroffset,  /* for error offset */
     NULL);       /* use default character tables */
     
     rc = pcre_exec(
     re,          /* the compiled pattern */
     NULL,        /* we didn't study the pattern */
     title,     /* the subject string */
     (int)strlen(title), /* the length of the subject */
     0,           /* start at offset 0 in the subject */
     0,           /* default options */
     ovector,     /* vector for substring information */
     OVECCOUNT);  /* number of elements in the vector */
     
    if (rc > 0)
      skip = true; 
      
  }

  if (strcmp((const char *) type3, "title") == 0)
	  if (strcmp((const char *) title, exclude3) == 0)
		  skip = true;

  if (strcmp ((const char *) type3, "title") == 0 && skip == false) {
  
     re = pcre_compile(
     exclude3,     /* the pattern */
     0,           /* default options */
     &error,      /* for error message */
     &erroffset,  /* for error offset */
     NULL);       /* use default character tables */
     
     rc = pcre_exec(
     re,          /* the compiled pattern */
     NULL,        /* we didn't study the pattern */
     title,     /* the subject string */
     (int)strlen(title), /* the length of the subject */
     0,           /* start at offset 0 in the subject */
     0,           /* default options */
     ovector,     /* vector for substring information */
     OVECCOUNT);  /* number of elements in the vector */
     
    if (rc > 0)
      skip = true; 
      
  }

  if (strcmp((const char *) type4, "title") == 0)
	  if (strcmp((const char *) title, exclude4) == 0)
		  skip = true;

  if (strcmp ((const char *) type4, "title") == 0 && skip == false) {
  
     re = pcre_compile(
     exclude4,     /* the pattern */
     0,           /* default options */
     &error,      /* for error message */
     &erroffset,  /* for error offset */
     NULL);       /* use default character tables */
     
     rc = pcre_exec(
     re,          /* the compiled pattern */
     NULL,        /* we didn't study the pattern */
     title,     /* the subject string */
     (int)strlen(title), /* the length of the subject */
     0,           /* start at offset 0 in the subject */
     0,           /* default options */
     ovector,     /* vector for substring information */
     OVECCOUNT);  /* number of elements in the vector */
     
    if (rc > 0)
      skip = true; 
      
  }


  if (strcmp((const char *) type1, "author") == 0)
	  if (strcmp((const char *) author, exclude1) == 0)
		  skip = true;

      if (strcmp ((const char *) type1, "author") == 0 && skip == false) {
      
     re = pcre_compile(
     exclude1,     /* the pattern */
     0,           /* default options */
     &error,      /* for error message */
     &erroffset,  /* for error offset */
     NULL);       /* use default character tables */
     
     rc = pcre_exec(
     re,          /* the compiled pattern */
     NULL,        /* we didn't study the pattern */
     author,     /* the subject string */
     (int)strlen(author), /* the length of the subject */
     0,           /* start at offset 0 in the subject */
     0,           /* default options */
     ovector,     /* vector for substring information */
     OVECCOUNT);  /* number of elements in the vector */
     
     if (rc > 0)
       skip = true; 
	  
	}


  if (strcmp((const char *) type2, "author") == 0)
	  if (strcmp((const char *) author, exclude2) == 0)
		  skip = true;

    if (strcmp ((const char *) type2, "author") == 0 && skip == false) {
      
     re = pcre_compile(
     exclude2,     /* the pattern */
     0,           /* default options */
     &error,      /* for error message */
     &erroffset,  /* for error offset */
     NULL);       /* use default character tables */
     
     rc = pcre_exec(
     re,          /* the compiled pattern */
     NULL,        /* we didn't study the pattern */
     author,     /* the subject string */
     (int)strlen(author), /* the length of the subject */
     0,           /* start at offset 0 in the subject */
     0,           /* default options */
     ovector,     /* vector for substring information */
     OVECCOUNT);  /* number of elements in the vector */
     
     if (rc > 0)
       skip = true; 
	  
	}
	 
  if (strcmp((const char *) type3, "author") == 0)
	  if (strcmp((const char *) author, exclude3) == 0)
		  skip = true;

      if (strcmp ((const char *) type3, "author") == 0 && skip == false) {
      
     re = pcre_compile(
     exclude3,     /* the pattern */
     0,           /* default options */
     &error,      /* for error message */
     &erroffset,  /* for error offset */
     NULL);       /* use default character tables */
     
     rc = pcre_exec(
     re,          /* the compiled pattern */
     NULL,        /* we didn't study the pattern */
     author,     /* the subject string */
     (int)strlen(author), /* the length of the subject */
     0,           /* start at offset 0 in the subject */
     0,           /* default options */
     ovector,     /* vector for substring information */
     OVECCOUNT);  /* number of elements in the vector */
     
     if (rc > 0)
       skip = true; 
	  
	}

  if (strcmp((const char *) type4, "author") == 0)
	  if (strcmp((const char *) author, exclude4) == 0)
		  skip = true;

      if (strcmp ((const char *) type4, "author") == 0 && skip == false) {
      
     re = pcre_compile(
     exclude4,     /* the pattern */
     0,           /* default options */
     &error,      /* for error message */
     &erroffset,  /* for error offset */
     NULL);       /* use default character tables */
     
     rc = pcre_exec(
     re,          /* the compiled pattern */
     NULL,        /* we didn't study the pattern */
     author,     /* the subject string */
     (int)strlen(author), /* the length of the subject */
     0,           /* start at offset 0 in the subject */
     0,           /* default options */
     ovector,     /* vector for substring information */
     OVECCOUNT);  /* number of elements in the vector */
     
     if (rc > 0)
       skip = true; 
	  
	}


  if (skip == false)
    {

      for (i = 0; i != feed->item_count; i++)
	{
	  if (guid)
	    {
	      if (feed->item[i].guid && !xmlStrcmp (feed->item[i].guid, guid))
		break;
	    }
	  else if (link)
	    {
	      if (feed->item[i].link && !xmlStrcmp (feed->item[i].link, link))
		break;
	    }
	}

      if (i != feed->item_count)
	{

	  /* old item */
	  feed_free_item (feed, i);
	  feed->item[i].title = title;
	  feed->item[i].description = description;

	  if (feed->item[0].description)
	    {
	      if (*feed->item[i].description == '<')
		{
#ifdef DEBUG
		  fprintf (stderr, "Post Update: %s\n",
			   feed->item[i].description);
		  fprintf (stderr, "\n");
		  fflush (stderr);
#endif
		  d =
		    malloc (strlen ((char *) feed->item[i].description) + 20);
		  if (d == NULL)
		    {
		      fprintf (stderr, "Memory Error\n");
		      fflush (stderr);
		    }
		  else
		    {
		      strncpy (d, "<node>", 7);
		      memcpy (d + strlen (d),
			      (char *) feed->item[i].description,
			      strlen ((char *) feed->item[i].description) +
			      1);
		      strncat (d, "</node>", 8);
		      pdoc =
			xmlReadMemory ((const char *) d, strlen (d), 0, 0,
				       1 | 32 | 64);
		      if (pdoc)
			{
			  for (pnode = pdoc->children; pnode;
			       pnode = pnode->next)
			    if (pnode->type == XML_ELEMENT_NODE
				&& !xmlStrcmp (pnode->name,
					       (const xmlChar *) "node"))
			      break;
			  j = 0;
			  for (nodetmp = pnode->children; nodetmp;
			       nodetmp = nodetmp->next)
			    {
			      if (nodetmp->type != XML_ELEMENT_NODE)
				continue;
			      if (!xmlStrcmp
				  (nodetmp->name, (const xmlChar *) "p"))
				{
				  feed->item[i].paragraph[j] =
				    xmlNodeGetContent (nodetmp);
#ifdef DEBUG1
				  fprintf (stderr, "p: %s\n",
					   feed->item[i].paragraph[j]);
				  fprintf (stderr, "\n");
				  fflush (stderr);
#endif
				}
			      if (!xmlStrcmp
				  (nodetmp->name, (const xmlChar *) "ol"))
				{
				  feed->item[i].paragraph[j] =
				    xmlNodeGetContent (nodetmp);
#ifdef DEBUG
				  fprintf (stderr, "ol: %s\n",
					   feed->item[i].paragraph[j]);
				  fprintf (stderr, "\n");
				  fflush (stderr);
#endif
				}
			      if (!xmlStrcmp
				  (nodetmp->name, (const xmlChar *) "li"))
				{
				  feed->item[i].paragraph[j] =
				    xmlNodeGetContent (nodetmp);
#ifdef DEBUG
				  fprintf (stderr, "li: %s\n",
					   feed->item[i].paragraph[j]);
				  fprintf (stderr, "\n");
				  fflush (stderr);
#endif
				}
			      if (!xmlStrcmp
				  (nodetmp->name, (const xmlChar *) "pre"))
				{
				  feed->item[i].paragraph[j] =
				    xmlNodeGetContent (nodetmp);
#ifdef DEBUG1
				  fprintf (stderr, "pre: %s\n",
					   feed->item[i].paragraph[j]);
				  fprintf (stderr, "\n");
				  fflush (stderr);
#endif
				}
			      else
				if (!xmlStrcmp
				    (nodetmp->name, (const xmlChar *) "h4"))
				{
				  feed->item[i].paragraph[j] =
				    xmlNodeGetContent (nodetmp);
				  feed->item[i].paragraph_style[j] = 1;
#ifdef DEBUG
				  fprintf (stderr, "h4: %s\n",
					   feed->item[i].paragraph[j]);
				  fprintf (stderr, "\n");
				  fflush (stderr);
#endif
				}
			      else
				if (!xmlStrcmp
				    (nodetmp->name, (const xmlChar *) "bq"))
				{
				  feed->item[i].paragraph[j] =
				    xmlNodeGetContent (nodetmp);
				  feed->item[i].paragraph_style[j] = 2;
#ifdef DEBUG
				  fprintf (stderr, "bq: %s\n",
					   feed->item[i].paragraph[j]);
				  fprintf (stderr, "\n");
				  fflush (stderr);
#endif
				}
			      else
				if (!xmlStrcmp
				    (nodetmp->name, (const xmlChar *) "ul"))
				{
				  feed->item[i].paragraph[j] =
				    xmlNodeGetContent (nodetmp);
#ifdef DEBUG
				  fprintf (stderr, "ul: %s\n",
					   feed->item[i].paragraph[j]);
				  fprintf (stderr, "\n");
				  fflush (stderr);
#endif
				}
			      j++;
			      if (j > 31)
				j = 31;

			    }
			  // Clear the paragraphs
/*                 for (k = 0; k != MAX_DESC_ITEMS; k++) {
	            if (feed->item[i].paragraph[k] != NULL) {
			    feed->item[i].paragraph[k] = 0;
	            }

	          } */

			  if (pdoc)
			    {
			      xmlFreeDoc (pdoc);
			      pdoc = 0;
			    }

			  if (d)
			    {
			      free (d);
			      d = 0;
			    }
			}
		    }
		}
	    }
	  feed->item[i].link = link;
	  feed->item[i].author = author;
	  feed->item[i].pub_date = pub_date;
	  feed->item[i].category = category;
	  feed->item[i].guid = guid;

	}
      else
	{

	  /* new item */
	  if (feed->item_count == FEED_MAX_ITEMS)
	    feed_free_item (feed, FEED_MAX_ITEMS - 1);
	  memmove (feed->item + 1, feed->item,
		   sizeof *feed->item * (FEED_MAX_ITEMS - 1));

	  feed->item[0].title = title;
	  feed->item[0].description = description;

	  for (j = 0; j != MAX_DESC_ITEMS; j++)
	    {
	      if (feed->item[0].paragraph[j])
		feed->item[0].paragraph[j] = NULL;
	      feed->item[0].paragraph_style[j] = 0;
	    }

	  if (feed->item[0].description)
	    {
	      if (*feed->item[0].description == '<')
		{
		  d =
		    malloc (strlen ((char *) feed->item[0].description) + 20);
		  if (d == NULL)
		    {
		      fprintf (stderr, "Memory Error\n");
		      fflush (stderr);
		    }
		  else
		    {
		      strncpy (d, "<node>", 7);
		      memcpy (d + strlen (d),
			      (char *) feed->item[0].description,
			      strlen ((char *) feed->item[0].description) +
			      1);
		      strncat (d, "</node>", 8);
		      pdoc =
			xmlReadMemory ((const char *) d, strlen (d), 0, 0,
				       1 | 32 | 64);
		      if (pdoc)
			{
			  for (pnode = pdoc->children; pnode;
			       pnode = pnode->next)
			    if (pnode->type == XML_ELEMENT_NODE
				&& !xmlStrcmp (pnode->name,
					       (const xmlChar *) "node"))
			      break;
			  j = 0;
			  for (nodetmp = pnode->children; nodetmp;
			       nodetmp = nodetmp->next)
			    {
			      if (nodetmp->type != XML_ELEMENT_NODE)
				continue;
			      if (!xmlStrcmp
				  (nodetmp->name, (const xmlChar *) "p"))
				{
				  feed->item[0].paragraph[j] =
				    xmlNodeGetContent (nodetmp);
#ifdef DEBUG1
				  fprintf (stderr, "p: %s\n",
					   feed->item[0].paragraph[j]);
				  fprintf (stderr, "\n");
				  fflush (stderr);
#endif
				}
			      if (!xmlStrcmp
				  (nodetmp->name, (const xmlChar *) "li"))
				{
				  feed->item[0].paragraph[j] =
				    xmlNodeGetContent (nodetmp);
#ifdef DEBUG
				  fprintf (stderr, "li: %s\n",
					   feed->item[0].paragraph[j]);
				  fprintf (stderr, "\n");
				  fflush (stderr);
#endif
				}
			      if (!xmlStrcmp
				  (nodetmp->name, (const xmlChar *) "ol"))
				{
				  feed->item[0].paragraph[j] =
				    xmlNodeGetContent (nodetmp);
#ifdef DEBUG
				  fprintf (stderr, "ol: %s\n",
					   feed->item[0].paragraph[j]);
				  fprintf (stderr, "\n");
				  fflush (stderr);
#endif
				}
			      if (!xmlStrcmp
				  (nodetmp->name, (const xmlChar *) "pre"))
				{
				  feed->item[0].paragraph[j] =
				    xmlNodeGetContent (nodetmp);
#ifdef DEBUG1
				  fprintf (stderr, "pre:: %s\n",
					   feed->item[0].paragraph[j]);
				  fprintf (stderr, "\n");
				  fflush (stderr);
#endif
				}
			      else
				if (!xmlStrcmp
				    (nodetmp->name, (const xmlChar *) "h4"))
				{
				  feed->item[0].paragraph[j] =
				    xmlNodeGetContent (nodetmp);
				  feed->item[0].paragraph_style[j] = 1;
#ifdef DEBUG
				  fprintf (stderr, "h4: %s\n",
					   feed->item[0].paragraph[j]);
				  fprintf (stderr, "\n");
				  fflush (stderr);
#endif
				}
			      else
				if (!xmlStrcmp
				    (nodetmp->name, (const xmlChar *) "bq"))
				{
				  feed->item[0].paragraph[j] =
				    xmlNodeGetContent (nodetmp);
				  feed->item[0].paragraph_style[j] = 2;
#ifdef DEBUG
				  fprintf (stderr, "bq: %s\n",
					   feed->item[0].paragraph[j]);
				  fprintf (stderr, "\n");
				  fflush (stderr);
#endif
				}
			      else
				if (!xmlStrcmp
				    (nodetmp->name, (const xmlChar *) "ul"))
				{
				  feed->item[0].paragraph[j] =
				    xmlNodeGetContent (nodetmp);
#ifdef DEBUG
				  fprintf (stderr, "ul: %s\n",
					   feed->item[0].paragraph[j]);
				  fprintf (stderr, "\n");
				  fflush (stderr);
#endif
				}
			      j++;
			      if (j > 31)
				j = 31;

			    }

			  if (pdoc)
			    {
			      xmlFreeDoc (pdoc);
			      pdoc = 0;
			    }

			  if (d)
			    {
			      free (d);
			      d = 0;
			    }

			}
		    }
		}
	    }
	  feed->item[0].link = link;
	  feed->item[0].author = author;
	  feed->item[0].pub_date = pub_date;
	  feed->item[0].category = category;
	  feed->item[0].guid = guid;

	  feed->item[0].new_item = true;

	  if (feed->item_count != FEED_MAX_ITEMS)
	    feed->item_count++;
	}

    }

}

void
feed_free_item (struct feed *feed, unsigned int i)
{
  int j = 0;

  if (feed->item[i].title)
    xmlFree (feed->item[i].title);
  if (feed->item[i].description)
    xmlFree (feed->item[i].description);
  for (j = 0; j != MAX_DESC_ITEMS; j++)
    {
      if (feed->item[i].paragraph[j])
	{
	  feed->item[i].paragraph[j] = NULL;
	  feed->item[i].paragraph_style[j] = 0;
	}
    }
  if (feed->item[i].link)
    xmlFree (feed->item[i].link);
  if (feed->item[i].author)
    xmlFree (feed->item[i].author);
  if (feed->item[i].pub_date)
    xmlFree (feed->item[i].pub_date);
  if (feed->item[i].category)
    xmlFree (feed->item[i].category);
  if (feed->item[i].guid)
    xmlFree (feed->item[i].guid);
}


void
feed_clean_text (xmlChar * text, bool process)
{
  xmlChar *s, *d;

  if (!text)
    return;
  s = d = text;
  while (*s)
    {
      const xmlChar *gt;
      if (*s == '<' && (gt = xmlStrchr (s, '>')))
	{
	  if (s[1] == '/' && s[2] == 't' && s[3] == 'd')
	    *d++ = ' ', *d++ = '|', *d++ = ' ';
	  else if (s[1] == 'p' && process == false)
	    *d++ = '<', *d++ = 'p', *d++ = '>';
	  else if (s[1] == '/' && s[2] == 'p' && process == false)
	    *d++ = '<', *d++ = '/', *d++ = 'p', *d++ = '>';
/*			else if (s[1] == 'a' && process == false)
			        *d++ = '<', *d++ = 'a';
			else if (s[1] == '/' && s[2] == 'a' && process == false)
				*d++ = '<', *d++ = '/', *d++ = 'a', *d++ = '>'; */
	  else if (s[1] == 'u' && s[2] == 'l' && process == false)
	    *d++ = '<', *d++ = 'u', *d++ = 'l', *d++ = '>';
	  else if (s[1] == '/' && s[2] == 'u' && s[3] == 'l'
		   && process == false)
	    *d++ = '<', *d++ = '/', *d++ = 'u', *d++ = 'l', *d++ = '>';
	  else if (s[1] == 'h' && s[2] == '4' && process == false)
	    *d++ = '<', *d++ = 'h', *d++ = '4', *d++ = '>';
	  else if (s[1] == '/' && s[2] == 'h' && s[3] == '4'
		   && process == false)
	    *d++ = '<', *d++ = '/', *d++ = 'h', *d++ = '4', *d++ = '>';
	  else if (s[1] == 'l' && s[2] == 'i' && process == false)
	    *d++ = '<', *d++ = 'l', *d++ = 'i', *d++ = '>';
	  else if (s[1] == '/' && s[2] == 'l' && s[3] == 'i'
		   && process == false)
	    *d++ = '<', *d++ = '/', *d++ = 'l', *d++ = 'i', *d++ = '>';
	  else if (s[1] == 'o' && s[2] == 'l' && process == false)
//                              *d++ = '<', *d++ = 'o', *d++ = 'l', *d++ = '>';
	    *d++ = ' ';
	  else if (s[1] == '/' && s[2] == 'o' && s[3] == 'l'
		   && process == false)
	    *d++ = ' ';
//                              *d++ = '<', *d++ = '/', *d++ = 'o', *d++ = 'l', *d++ = '>';
	  else if (s[1] == 'p' && s[2] == 'r' && s[3] == 'e'
		   && process == false)
	    *d++ = '<', *d++ = 'p', *d++ = 'r', *d++ = 'e', *d++ = '>';
	  else if (s[1] == '/' && s[2] == 'p' && s[3] == 'r' && s[4] == 'e'
		   && process == false)
	    *d++ = '<', *d++ = '/', *d++ = 'p', *d++ = 'r', *d++ = 'e', *d++ =
	      '>';
	  else if (s[1] == 'b' && s[2] == 'l' && s[3] == 'o' && s[4] == 'c'
		   && s[5] == 'k' && s[6] == 'q' && s[7] == 'u' && s[8] == 'o'
		   && s[9] == 't' && s[10] == 'e' && process == false)
	    *d++ = '<', *d++ = 'b', *d++ = 'q', *d++ = '>';
	  else if (s[1] == '/' && s[2] == 'b' && s[3] == 'l' && s[4] == 'o' &&
		   s[5] == 'c' && s[6] == 'k' && s[7] == 'q' && s[8] == 'u' &&
		   s[9] == 'o' && s[10] == 't' && s[11] == 'e'
		   && process == false)
	    *d++ = '<', *d++ = '/', *d++ = 'b', *d++ = 'q', *d++ = '>';
	  else if (s[1] == 'b' && s[2] == 'r')
//	    *d++ = '&', *d++ = '#', *d++ = '1', *d++ = '0', *d++ = ';';
	    *d++ = '|', *d++ = 'M';
//	    *d++ = '<', *d++ = 'b', *d++ = 'r', *d++ = ' ', *d++ = '/', *d++ = '>';
	  s = gt + 1;
	}
      else if (*s == '&' && process == true)
	{
	  if (s[1] == '#' && s[2] == '3' && s[3] == '9' && s[4] == ';')
	    *d++ = '\'', s += 5;	/* Skip past the original string */
	  if (s[1] == '#' && s[2] == '4' && s[3] == '0' && s[4] == ';')
	    *d++ = '(', s += 5;
	  if (s[1] == '#' && s[2] == '1' && s[3] == '8' &&
	      s[4] == '7' && s[5] == ';')
	    *d++ = 0x00, s += 6;
	  if (s[1] == '#' && s[2] == '2' && s[3] == '1' &&
	      s[4] == '5' && s[5] == ';')
	    *d++ = 0xc3, *d++ = 0x97, s += 6;
	  if (s[1] == '#' && s[2] == '8' && s[3] == '2' &&
	      s[4] == '1' && s[5] == '1' && s[6] == ';')
	    *d++ = 0xe2, *d++ = 0x80, *d++ = 0x93, s += 7;
	  if (s[1] == '#' && s[2] == '8' && s[3] == '2' &&
	      s[4] == '1' && s[5] == '6' && s[6] == ';')
	    *d++ = 0xe2, *d++ = 0x80, *d++ = 0x98, s += 7;
	  if (s[1] == '#' && s[2] == '8' && s[3] == '2' &&
	      s[4] == '1' && s[5] == '7' && s[6] == ';')
	    *d++ = 0xe2, *d++ = 0x80, *d++ = 0x99, s += 7;
	  if (s[1] == '#' && s[2] == '8' && s[3] == '2' &&
	      s[4] == '1' && s[5] == '1' && s[6] == ';')
	    *d++ = 0xe2, *d++ = 0x80, *d++ = 0x91, s += 7;
	  if (s[1] == '#' && s[2] == '8' && s[3] == '2' &&
	      s[4] == '2' && s[5] == '0' && s[6] == ';')
	    *d++ = 0xe2, *d++ = 0x80, *d++ = 0x9c, s += 7;
	  if (s[1] == '#' && s[2] == '8' && s[3] == '2' &&
	      s[4] == '2' && s[5] == '1' && s[6] == ';')
	    *d++ = 0xe2, *d++ = 0x80, *d++ = 0x9d, s += 7;
	  if (s[1] == '#' && s[2] == '8' && s[3] == '2' &&
	      s[4] == '3' && s[5] == '0' && s[6] == ';')
	    *d++ = 0xe2, *d++ = 0x80, *d++ = 0xa6, s += 7;
	  if (s[1] == 'n' && s[2] == 'b' && s[3] == 's' &&
	      s[4] == 'p' && s[5] == ';')
	    *d++ = ' ', s += 6;
	  else if (s[1] == 'q' && s[2] == 'u' && s[3] == 'o' &&
		   s[4] == 't' && s[5] == ';')
	    *d++ = '"', s += 6;
/*			else if (s[1] == 'a' && s[2] == 'm' &&
                                        s[3] == 'p' && s[4] == ';' &&
					s[5] == 'n' && s[6] == 'b' && s[7] == 's' && s[8] == 'p' && s[9] == ';')
				*d++ = ' ', s += 10; */
	  else if (s[1] == 'a' && s[2] == 'm' && s[3] == 'p' && s[4] == ';')
	    *d++ = '&', s += 5;
//	  else if (s[1] == '<' && s[2] == 'C' && s[3] == 'R' && s[4] == '>')
//	    *d++ = 0x0d, *d++ = 0x0a, s += 5;
	  else if (s[1] == 'c' && s[2] == 'o' && s[3] == 'p' &&
		   s[4] == 'y' && s[5] == ';')
	    *d++ = 0xc2, *d++ = 0xa9, s += 6;
	  else
	    *d++ = *s++;
	}
      else
	*d++ = *s++;
    }
  *d = 0;

  /* collapse whitespace */
  s = d = text;
  while (*s == '\t' || *s == '\r' || *s == '\n' || *s == ' ')
    s++;
  while (*s)
    {
      while (*s && !(*s == '\t' || *s == '\r' || *s == '\n' || *s == ' '))
	*d++ = *s++;
      if (*s)
	*d++ = ' ';
      while (*s && (*s == '\t' || *s == '\r' || *s == '\n' || *s == ' '))
	s++;
    }
  *d = 0;

}


/**
 * Start updating all feeds.
 */

void
feed_update (void)
{
  unsigned int i;

  for (i = 0; i != feed_count; i++)
    if (feeds[i].status != FEED_PAUSED)
      feeds[i].status = FEED_UPDATE;

  feed_work_needed = true;
}


/**
 * Load list of feeds.
 */

bool
feed_list_load (const char *path)
{
  FILE *stream;
  char url[4000];

  stream = fopen (path, "r");
  if (!stream)
    {
      feed_error = strerror (errno);
      return false;
    }

  while (!feof (stream))
    {
      url[0] = 0;
      fgets (url, sizeof url, stream);
      if (url[0] == 0 || url[0] == '\n')
	continue;
      url[strlen (url) - 1] = 0;
      if (!feed_add (url))
	{
	  fclose (stream);
	  return false;
	}
    }

  if (fclose (stream))
    {
      feed_error = strerror (errno);
      return false;
    }

  return true;
}


/**
 * Save list of feeds.
 */

bool
feed_list_save (const char *path)
{
  FILE *stream;
  unsigned int i;

  stream = fopen (path, "w");
  if (!stream)
    {
      feed_error = strerror (errno);
      return false;
    }

  for (i = 0; i != feed_count; i++)
    {
      fputs (feeds[i].url, stream);
      fputc ('\n', stream);
    }

  if (fclose (stream))
    {
      feed_error = strerror (errno);
      return false;
    }

  return true;
}


/**
 * Output a feed.
 */

void
feed_print (struct feed *feed)
{
  unsigned int i;

  assert (feed);

  printf ("URL:         %s\n", feed->url);
  printf ("Status:      %s\n", feed_status_name[feed->status]);

  if (feed->status == FEED_OK)
    {
      printf ("Title:       %s\n", feed->title);
      printf ("Description: %s\n", feed->description);
      printf ("Link:        %s\n", feed->link);
      printf ("Copyright:   %s\n", feed->copyright);
      printf ("Publ'n date: %s\n", feed->pub_date);
      printf ("Category:    %s\n", feed->category);
      for (i = 0; i != feed->item_count; i++)
	{
	  printf ("    Title:       %s\n", feed->item[i].title);
	  printf ("    Description: %s\n", feed->item[i].description);
	  printf ("    Link:        %s\n", feed->item[i].link);
	  printf ("    Author:      %s\n", feed->item[i].author);
	  printf ("    Publ'n date: %s\n", feed->item[i].pub_date);
	  printf ("    Category:    %s\n", feed->item[i].category);
	  printf ("    GUID:        %s\n", feed->item[i].guid);
	  printf ("    New item:    %s\n", feed->item[i].new_item ?
		  "yes" : "no");
	}
    }
  else if (feed->status == FEED_ERROR)
    {
      printf ("Error:       %s\n", feed->error);
    }
}

static int
fetch_curl_verify_callback (int preverify_ok, X509_STORE_CTX * ctx)
{
  char buf[256];
  X509 *err_cert;
  int err, depth;

  err_cert = X509_STORE_CTX_get_current_cert (ctx);
  err = X509_STORE_CTX_get_error (ctx);
  depth = X509_STORE_CTX_get_error_depth (ctx);

  /*
   * Retrieve the pointer to the SSL of the connection currently treated
   * and the application specific data stored into the SSL object.
   */

  X509_NAME_oneline (X509_get_subject_name (err_cert), buf, 256);

  /*
   * Catch a too long certificate chain. The depth limit set using
   * SSL_CTX_set_verify_depth() is by purpose set to "limit+1" so
   * that whenever the "depth>verify_depth" condition is met, we
   * have violated the limit and want to log this error condition.
   * We must do it here, because the CHAIN_TOO_LONG error would not
   * be found explicitly; only errors introduced by cutting off the
   * additional certificates would be logged.
   */
  if (depth > MAX_CERTS)
    {
      preverify_ok = 0;
      err = X509_V_ERR_CERT_CHAIN_TOO_LONG;
      X509_STORE_CTX_set_error (ctx, err);
    }
  if (!preverify_ok)
    {
      printf ("verify error:num=%d:%s:depth=%d:verify_depth=%d:%s\n", err,
	      X509_verify_cert_error_string (err), depth, MAX_CERTS, buf);
    }
  else
    {
      printf ("depth=%d:%s\n", depth, buf);
    }

  /*
   * At this point, err contains the last verification error. We can use
   * it for something special
   */
  if (!preverify_ok && (err == X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT))
    {
      X509_NAME_oneline (X509_get_issuer_name (err_cert), buf, 256);
      printf ("issuer= %s\n", buf);
    }

    return preverify_ok;
}
