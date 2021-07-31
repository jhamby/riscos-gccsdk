#ifndef TEST_DATA_PART1_HEADER_INCLUDED
#define TEST_DATA_PART1_HEADER_INCLUDED

extern char *char_ptr;
extern const char *const_char_ptr;
extern const char * const const_char_const_ptr;

extern char char_array[];
extern const char const_char_array[];

extern int an_int;
extern const int a_const_int;

extern const char *func1(void);
extern const char *func2(void);

extern const char *(*func1_ptr)(void);
extern const char *(* const const_func2_ptr)(void);

extern const char *(*func1_array[2])(void);
extern const char *(* const const_func2_array[2])(void);

#endif
