#include <iostream.h>
#include "Librarian.h"
#include "BError.h"

int main(int argc,char **argv)
{
	BError *err;

	TRY
	{
		Librarian libFile(argc,argv);
		libFile.run();
	}
	CATCH(err)
	{
	 	cerr << err;
	}

	return 0;
}
