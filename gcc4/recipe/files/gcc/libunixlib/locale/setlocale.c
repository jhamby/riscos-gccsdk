/* setlocale ()
 * Written by Nick Burrett, 20 July 1997.
 * Copyright (c) 1997-2012 UnixLib Developers
 */

#include <ctype.h>
#include <errno.h>
#include <locale.h>
#include <string.h>
#include <stddef.h>
#include <swis.h>

#include <pthread.h>
#include <internal/unix.h>
#include <internal/os.h>

/* Locale information types. These should correspond to the #defines
   in <locale.h>.  */
static const char * const locale_names[] = {
  "LC_COLLATE", "LC_CTYPE", "LC_MESSAGES", "LC_MONETARY", "LC_NUMERIC",
  "LC_TIME", "LC_ALL"
  };

typedef struct locale_entry
{
  const char *territory_name;
  const char *locale_name;
} locale_entry;

/* Table of RISC OS territories and the corresponding locale name.  */
static const locale_entry locale_table[] =
{
  { "uk", "en-GB" },
  { "germany", "de-DE" },
  { "france", "fr-FR" },
  { "netherlands", "nl-NL" },
  { "italy", "it-IT" },
  { "spain", "es-ES" },
  { "portugal", "pt-PT" },
  { "greece", "el-GR" },
  { "sweden", "sv-SE" },
  { "finland", "fi-FI" },
  { "denmark", "da-DK" },
  { "norway", "no-NO" },
  { "iceland", "is-IS" },
  { "canada", "en-CA" },
  { "turkey", "tr-TR" },
  { "arabic", "ar-SA" },
  { "ireland", "en-IE" },
  { "hong kong", "zh-HK" },
  { "russia", "ru-RU" },
  { "israel", "he-IL" },
  { "mexico", "es-MX" },
  { "australia", "en-AU" },
  { "austria", "de-AT" },
  { "belgium", "nl-BE" },
  { "japan", "ja-JP" },
  { "switzerland", "de-CH" }, /* Could also be italian or french */
  { "usa", "en-US" },
  { "china", "zh-CN" },
  { "brazil", "pt-BR" },
  { "safrica", "af-ZA" },
  { "korea", "ko-KR" },
  { "taiwan", "zh-TW" },
  { "faroe", "fo-FO" },
  { "albania", "sq-AL" },
  { "bulgaria", "bg-BG" },
  { "czech", "cs-CZ" },
  { "farsi", "fa-IR" },
  { "gujarati", "gu-IN" },
  { "estonia", "et-EE" },
  { "hungary", "hu-HU" },
  { "latvia", "lv-LV" },
  { "lithuania", "lt-LT" },
  { "macedonia", "mk-MK" },
  { "poland", "pl-PL" },
  { "punjabi", "pa-IN" },
  { "romania", "ro-RO" },
  { "slovak", "sk-SK" },
  { "slovene", "sl-SL" },
  { "tamil", "ta-IN" },
  { "ukraine", "uk-UA" },
  { "swiss1", "fr-CH" },
  { "swiss2", "de-CH" },
  { "swiss3", "it-CH" },
  { NULL, NULL }
};

/* Translate a RISC OS territory as listed here:

   <http://www.riscosopen.org/wiki/documentation/show/Territory%20Numbers>

   into a locale of the form "<language>-<region>". For example, in the
   case of the United Kingdom, the Territory module returns UK which is
   translated into "en-GB".
   Return NULL if the lookup failed.  */
static const char *
territory_to_locale (const char *territory)
{
  const locale_entry *entry;
  for (entry = locale_table;
       entry->territory_name != NULL && stricmp (entry->territory_name, territory) != 0;
       entry++)
    /* Empty Loop */;

  return entry->locale_name; /* NULL if the end of the table reached.  */
}

/* Translate a locale of the form "<language>-<region>.<character encoding>"
   into a RISC OS territory. If the character encoding is present, then we
   ignore it.
   Note that the lookup is case insensitive, but the territory module for the
   current locale must be loaded for strnicmp to work correctly.
   Return NULL if the lookup failed.  */
static const char *
locale_to_territory (const char *locale)
{
  char *char_enc_dot = strchr (locale, '.');

  /* Ignore any character encoding specification when searching the locale
     table.  */
  int locale_len = char_enc_dot ?
		   (char_enc_dot - locale) :
		   strlen (locale);

  const locale_entry *entry;
  for (entry = locale_table;
       entry->locale_name != NULL && strnicmp (entry->locale_name, locale, locale_len) != 0;
       entry++)
    /* Empty Loop */;

  return entry->territory_name; /* NULL if the end of the table reached.  */
}

/* Convert a territory number into a name.  */
static void
territory_name (int territory, char *buffer, int size)
{
  /* Territory -1 is used as the C/POSIX locale.  */
  if (territory == -1)
    {
      buffer[0] = 'C';
      buffer[1] = 0;
    }
  else
    {
      int regs[10];
      const char *locale;

      regs[0] = territory;
      regs[1] = (int)buffer;
      regs[2] = size - 1;
      __os_swi (Territory_NumberToName, regs);
      locale = territory_to_locale (buffer);

      if (locale && strlen (locale) < size)
	strcpy (buffer, locale);
    }
}

/* Convert a territory name into a number.  */
static int
territory_number (const char *locale)
{
  int regs[10];

  if (*locale == 0)
    {
      /* Return the current territory number.  */
      __os_swi (Territory_Number, regs);
      return regs[0];
    }

  /* Check for the C or POSIX locale.  */
  if ((locale[0] == 'C' && locale[1] == 0)
      || strcmp (locale, "POSIX") == 0)
    return -1;

  const char *territory = locale_to_territory (locale);

  /* Fall back to the original locale name if the lookup fails. This
     means that RISC OS territory names are still recognised giving
     some backwards compatibility. However, specifying "UK" will match
     Ukraine rather than United Kingdom, so "en-GB" should be used in
     preference.   */
  if (territory == NULL)
    territory = locale;

  regs[0] = -1;
  regs[1] = (int)territory;
  /* If we can't find the locale, then this SWI will return zero
     in regs[0].  */
  __os_swi (Territory_NameToNumber, regs);
  return regs[0];
}

static void
do_lc_all (locale_t locobj, char *buffer, int size)
{
  char temp[64];
  int category, same = 1;

  /* We must return a string that can be passed into
     setlocale to set all locales.  If all locales are
     the same, then we can produce a very short string.  */
  for (category = 1; category < LC_ALL; ++category)
    if (locobj->locale_territory[0] != locobj->locale_territory[category])
      same = 0;

  if (same)
    {
      /* All locales are set to the same territory.  */
      territory_name (locobj->locale_territory[0], buffer, size);
    }
  else
    {
      /* Construct a composite name of the format:
	   LC_CATEGORY=country;LC_CATEGORY=country; ...  */
      for (category = 0; category < LC_ALL; ++category)
	{
	  territory_name (locobj->locale_territory[category], temp, sizeof (temp));
	  buffer = stpcpy (buffer, locale_names[category]);
	  *buffer++ = '=';
	  buffer = stpcpy (buffer, temp);
	  *buffer++ = ';';
	}
      /* Remove final semi-colon.  */
      buffer[-1] = '\0';
    }
}

char *
__setlocale_l (locale_t locobj, int category, const char *locale)
{
  int new_territory, changed;
  static char old_locale[256];

  /* This tells localeconv to re-read data for the lconv structure.  */
  locobj->lc_needs_refresh = 1;

  if (locale == NULL)
    {
      /* Return the name of the current locale.  */
      if (category == LC_ALL)
	{
	  /* The locale string is specially encoded for LC_ALL so we
	     could restore all locales at any time.  */
	  do_lc_all (locobj, old_locale, sizeof (old_locale));
	  return old_locale;
	}

      territory_name (locobj->locale_territory[category], old_locale, sizeof (old_locale));
      return old_locale;
    }

  if (category == LC_ALL)
    {
      char *newnames[LC_ALL];

      /* We're either going to set all locale categories to one particular
	 locale, or we will set the locale categories according to the
	 previously encoded string that has been passed in.  */

      /* Set all name pointers to the argument name.  */
      for (category = 0; category < LC_ALL; ++category)
	newnames[category] = (char *)(int) locale;

      /* Encode the locale string, as we will be returning this
	 later.  Remember, setlocale returns the locale settings
	 that are about to be changed.  */
      do_lc_all (locobj, old_locale, sizeof (old_locale));

      /* Check for an encoded (composite) name.  Simply looking for
	 a semi-colon will verify this.  */
      if (strchr (locale, ';') != NULL)
	{
	  /* This is a composite name, split it up.  */
	  char *np = (char *)(int) locale;
	  char *cp;
	  int cnt;

	  while ((cp = strchr (np, '=')) != NULL)
	    {
	      for (cnt = 0; cnt < LC_ALL; ++cnt)
		if ((size_t) (cp - np) == strlen (locale_names[cnt])
		    && memcmp (np, locale_names[cnt], cp - np) == 0)
		  break;

	      if (cnt == LC_ALL)
		{
		  /* Bad category name.  */
		  (void) __set_errno (EINVAL);
		  return NULL;
		}

	      /* Found the category this clause sets.  */
	      newnames[cnt] = ++cp;
	      cp = strchr (cp, ';');
	      if (cp != NULL)
		{
		  /* Examine the next clause.  */
		  *cp = '\0';
		  np = cp + 1;
		}
	      else
		/* This was the last clause.  We are done.  */
		break;
	    }

	  for (cnt = 0; cnt < LC_ALL; ++cnt)
	    if (newnames[cnt] == locale)
	      {
		/* The composite name did not specify all categories.  */
		(void) __set_errno (EINVAL);
		return NULL;
	      }

	  /* Check that the specified locales do exist.  If any do not,
	     then we must fail the setlocale() call and ensure that no
	     locale categories have been altered.  */
	  for (category = 0; category < LC_ALL; ++category)
	    {
	      int territory = territory_number (newnames[category]);
	      if (territory == 0)
		{
		  (void) __set_errno (EINVAL);
		  return NULL;
		}
	    }

	  /* We now know all locales exist, so set them.  */
	  for (category = 0; category < LC_ALL; ++category)
	    locobj->locale_territory[category] = territory_number (newnames[category]);

	  /* Re-build the character type tables according to the new
	     locale settings.  */
	  __build_ctype_tables (locobj, locobj->locale_territory[LC_CTYPE]);
	  return old_locale;
	}
    }

  /* The user has either specified LC_ALL and not passed a composite
     string, or passed LC_MONETARY (or similar).  */
  new_territory = territory_number (locale);

  /* We can't do much with invalid territories.  */
  if (new_territory == 0)
    {
      (void) __set_errno (EINVAL);
      return NULL;
    }

  /* Mark that we don't really need to build new ctype tables.  */
  changed = 0;
  if (category == LC_ALL)
    {
      /* Change the locale for all categories. old_locale was created
	 when we previously checked for a composite string.  */
      for (category = 0; category < LC_ALL; ++category)
	if (locobj->locale_territory[category] != new_territory)
	  {
	    locobj->locale_territory[category] = new_territory;
	    changed = 1;
	  }
    }
  else
    {
      /* Change the locale for just one category.  */
      territory_name (locobj->locale_territory[category],
		      old_locale, sizeof (old_locale));

      if (locobj->locale_territory[category] != new_territory)
	{
	  locobj->locale_territory[category] = new_territory;
	  changed = 1;
	}
    }

  /* Re-build the character-type tables if they've changed.  */

  /* For efficiency, only build the new tables if the locale really
     is changing.  The GNU Java compiler is known to repeatedly call
     setlocale.  */
  if (changed && (category == LC_ALL || category == LC_CTYPE))
    __build_ctype_tables (locobj, new_territory);

  return old_locale;
}

char *
setlocale (int category, const char *locale)
{

  PTHREAD_UNSAFE

  return __setlocale_l(&__locale_global, category, locale);
}
