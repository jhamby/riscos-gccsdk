extern int factorial (int argc, char *argv[]);
extern int powers (int argc, char *argv[]);

int main (int argc, char *argv[])
{
  powers (argc, argv);
  factorial (argc, argv);

  return 0;
}
