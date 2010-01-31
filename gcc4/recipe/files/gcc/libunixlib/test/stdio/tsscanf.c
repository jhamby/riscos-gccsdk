/* Test *scanf's width-limited character class handling */

#include <stdio.h>

int main(void)
{
	const char *input1 = "Fri, 16 Jun 2006 01:10:16 GMT";
	const char *input2 = "Fri, 16-Jun-2006 01:10:16 GMT";
	char buf[12];

	int assignments = sscanf(input1, "%[A-Za-z], "
				"%2[0-9]%1[ -]%3[A-Za-z]%1[ -]%4[0-9] "
				"%2[0-9]:%2[0-9]:%2[0-9] GMT",
				buf, buf, buf, buf, buf,
				buf, buf, buf, buf
			);

	printf("'%s' => %d (should be 9)\n", input1, assignments);

	assignments = sscanf(input2, "%3[A-Za-z], "
				"%2[0-9]%1[ -]%3[A-Za-z]%1[ -]%4[0-9] "
				"%2[0-9]:%2[0-9]:%2[0-9] GMT",
				buf, buf, buf, buf, buf,
				buf, buf, buf, buf);

	printf("'%s' => %d (should be 9)\n", input2, assignments);

	return 0;
}
