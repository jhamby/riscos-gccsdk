/*
 * This file is part of Sargasso, http://zamez.org/sargasso
 * Licensed under the GNU General Public License,
 *                http://www.opensource.org/licenses/gpl-license
 * Copyright 2006 James Bursa <james@zamez.org>
 */

#ifndef FEED_H
#define FEED_H

#include <stdbool.h>
#include <curl/curl.h>
#include <libxml/parser.h>
#include <openssl/ssl.h>


#define FEED_MAX_ITEMS 200
#define MAX_DESC_ITEMS 32
#define MAX_CERTS   10


 /* the ciphersuites we are willing to use */
 #define CIPHER_LIST                                             \
         /* disable everything */                                \
         "-ALL:"                                                 \
         /* enable TLSv1.2 PFS suites */                         \
         "EECDH+AES+TLSv1.2:EDH+AES+TLSv1.2:"                    \
         /* enable PFS AES GCM suites */                         \
         "EECDH+AESGCM:EDH+AESGCM:"                              \
         /* Enable PFS AES CBC suites */                         \
         "EECDH+AES:EDH+AES:"                                    \
         /* Enable non-PFS fallback suite */                     \
         "AES128-SHA:"                                           \
         /* Remove any PFS suites using weak DSA key exchange */ \
         "-DSS"

typedef enum { FEED_NEW, FEED_FETCHING, FEED_OK, FEED_ERROR, FEED_PAUSED, FEED_UPDATE }
		feed_status;

 /** SSL certificate info */
 struct cert_info {
         X509 *cert;             /**< Pointer to certificate */
         long err;               /**< OpenSSL error code */
 };

struct feed {
	char *url;
	feed_status status;
	bool updated;
	const char *error;
	CURL *curl;
	struct curl_slist *headers;
	char *status_line;
	char *etag;
	unsigned int redirect_count;
	char *redirect;
	char *data;
	size_t data_size;
	xmlChar *title;
	xmlChar *description;
	xmlChar *link;
	xmlChar *copyright;
	xmlChar *pub_date;
	xmlChar *category;
	struct {
		xmlChar *title;
		xmlChar *description;
		xmlChar *link;
		xmlChar *author;
		xmlChar *pub_date;
		xmlChar *category;
		xmlChar *guid;
		bool new_item;
		xmlChar *paragraph[MAX_DESC_ITEMS];
		unsigned int paragraph_style[MAX_DESC_ITEMS];
	} item[FEED_MAX_ITEMS];
	unsigned int item_count;
	struct cert_info cert_data[MAX_CERTS]; /**< HTTPS certificate data */
};

extern struct feed *feeds;
extern unsigned int feed_count;
extern unsigned int max_fetches;
extern bool feed_work_needed;
extern const char *feed_error;
extern char http_proxy[255];

extern char feed1[255];
extern char feed2[255];
extern char feed3[255];
extern char feed4[255];

extern char type1[7];
extern char type2[7];
extern char type3[7];
extern char type4[7];

extern char exclude1[1030];
extern char exclude2[1030];
extern char exclude3[1030];
extern char exclude4[1030];

bool feed_init(void);
void feed_quit(void);
bool feed_add(const char *url);
bool feed_remove(unsigned int i);
bool feed_work(void);
void feed_update(void);
bool feed_list_load(const char *path);
bool feed_list_save(const char *path);
void feed_print(struct feed *feed);
void feed_set_status (struct feed *feed, feed_status status);


#endif
