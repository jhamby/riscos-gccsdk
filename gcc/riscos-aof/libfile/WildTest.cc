#include <iostream>
#include "Path.h"

int main(int argc, char **argv)
{
	List<BString> res = Path::getMatchingFiles(argv[1]);
	Listiter<BString> iter(res);
	BString *str;

	while(str = iter.next())
		cout << (*str) << endl;
}
