
#include <iostream.h>

void t(int r)
{
	if (r) throw "EXCEPTION";
}

int main (void)
{
  try
  {
  	t(0);
  	t(1);
  }
  catch (char /*const*/ * cp)
  {
  	cerr << cp <<endl;
  	return 1;
  }
  catch (...)
  {
  	cerr << "?" << endl;
  	return 2;
  }
  return 0;
}
