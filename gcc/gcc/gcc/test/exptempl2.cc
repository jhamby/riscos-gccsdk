class Bar
{
  public:
    template<class T>
    T* create()
      { return new T; }
};

int main (void)
{
  Bar b;
  Foo *f = b.template create<Foo>();
}
