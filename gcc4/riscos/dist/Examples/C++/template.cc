/* Template repository demonstration.  */

#include <cstdio>

template<class T> class stack {
  T* v;
  T* p;
  int sz;
public:
  stack(int);
  ~stack();

  void push (T);
  T pop();

  int size() const;
};

template<class T> void stack<T>::push(T a)
{
  *p++ = a;
}

template<class T> stack<T>::stack(int s)
{
  v = p = new T[sz=s];
}

template<class T> stack<T>::~stack()
{
  delete[] v;
}

template<class T> T stack<T>::pop()
{
  return *--p;
}

template<class T> int stack<T>::size() const
{
  return p-v;
}

stack<int> ip(200);

void f(stack<int>& sc)
{
  sc.push(10);
  int z = 4*sc.pop();

  stack<char>*p = 0;
  p = new stack<char>(800);

  for (int i = 0; i < 400; i++) {
    p->push(i);
  }
}

int main (void)
{
  f (ip);
  printf ("It worked\n");
  return 0;
}
