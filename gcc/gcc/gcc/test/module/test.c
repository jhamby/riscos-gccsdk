int test (void)
{
  static int a = 0, b = 1;
  static char buf[256];

  if (! buf[0])
    {
      buf[0] = 'c';
      a = 5;
      b = 6;

      return 1;
    }
  return 0;
}
