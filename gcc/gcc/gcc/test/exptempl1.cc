template<class T>
T* create()
{
  return new T;
}

int main (void)
{
  Foo *f = create<Foo>();
  return 0;
}
