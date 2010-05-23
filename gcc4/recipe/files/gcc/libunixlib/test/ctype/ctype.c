#include <ctype.h>
#include <stdio.h>

#define TEST_PURE_SCL

int
main(int argc, char *argv[])
{
  for (int c = -1; c <= 255; ++c)
    {
      int is_alnum = isalnum (c);
      int is_alpha = isalpha (c);
      int is_cntrl = iscntrl (c);
      int is_digit = isdigit (c);
      int is_graph = isgraph (c);
      int is_lower = islower (c);
      int is_upper = isupper (c);
      int is_print = isprint (c);
      int is_punct = ispunct (c);
      int is_space = isspace (c);
      int is_xdigit = isxdigit (c);
#ifndef TEST_PURE_SCL
      int is_ascii = isascii (c);
#endif

      printf ("%02x : %c%c%c%c%c%c%c%c%c%c%c"
#ifndef TEST_PURE_SCL
              "%c"
#endif
              " : %02x %02x"
#ifndef TEST_PURE_SCL
              " %02x %02x %02x"
#endif
              "\n",
              c,
              is_alnum ? 'X' : '.', is_alpha ? 'X' : '.', is_cntrl ? 'X' : '.', is_digit ? 'X' : '.',
              is_graph ? 'X' : '.', is_lower ? 'X' : '.', is_upper ? 'X' : '.', is_print ? 'X' : '.',
              is_punct ? 'X' : '.', is_space ? 'X' : '.', is_xdigit ? 'X' : '.',
#ifndef TEST_PURE_SCL
              is_ascii ? 'X' : '.',
#endif
              toupper (c), tolower (c)
#ifndef TEST_PURE_SCL
              , _toupper (c), _tolower (c),
              toascii (c)
#endif
              );
    }

  return 0;
}

