#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>
#include <assert.h>
#include <iostream>

template <class T> class Const_listiter;
template <class T> class Listiter;

class Listiter_Base;

class List_Base
{
 public:
 	int length() const
 	{
 	 return(m_size);
 	}

 	void remember(Listiter_Base *l)
	{
 	 m_numIters++;
	 if (m_numIters)
	 {
		 m_iters=(Listiter_Base **) realloc(m_iters,
		 	m_numIters*sizeof(Listiter_Base *));
	 	 assert(m_iters);
	 }
	 else
	 {
		 m_iters = 0;
	}

 	 m_iters[m_numIters-1]=l;
 	}

 	void forget(Listiter_Base *l)
 	{
 	 int i,pos=-1;
 	 for(i=0;i<m_numIters;i++)
 	  if(m_iters[i]==l) pos=i;
 	 assert(pos!=-1);
 	 m_iters[pos]=m_iters[m_numIters-1];
 	 m_numIters--;

 	 if (m_numIters)
 	 {
		 m_iters=(Listiter_Base **) realloc(m_iters,
			m_numIters*sizeof(Listiter_Base *));
		 assert(m_iters);
	}
	else
	{
		m_iters = 0;
	}
 	}

 	void *operator[](unsigned i)
 	{
 	 assert((i>=0)&&(i<m_size));
 	 return(m_data[i]);
 	}

 	void insert(int a_pos)
 	{
 	 assert(a_pos<=m_size);
 	 int i;
 	 int n=m_size++;
 	 m_data=(void **) realloc(m_data,m_size*sizeof(void *));
 	 assert(m_data);
	 for(i=a_pos;i<n;i++)
	  m_data[i+1]=m_data[i];
 	}

	virtual void inserted(int a_pos)=0;
	virtual void removed(int a_pos)=0;
	virtual void remove(int a_pos)=0;

	void sort(int (*f)(const void *,const void *))
	{
		qsort(m_data,m_size,sizeof(void **),f);
	}

// 	void remove(int a_pos)
// 	{
// 	 int i;
// 	 delete m_data[a_pos];
// 	 for(i=a_pos;i<(m_size-1);i++)
// 	  m_data[i]=m_data[i+1];
// 	 m_size--;
// 	 m_data=(T **) realloc(m_data,m_size*sizeof(T*));
// 	 assert(m_data);
//	 for(i=0;i<m_numIters;i++)
//	  m_iters[i]->removed(a_pos);
// 	}

 	int m_dataSize;
 	void **m_data;
 	int m_size;
 	Listiter_Base **m_iters;
 	int m_numIters;
};

class Listiter_Base
{
	friend class List_Base;

 public:

 	List_Base *m_list;
 	int m_pos;

	void listDied()
	{
	 m_list=NULL;
	}

	void inserted(int a_pos)
	{
	 if(m_pos>a_pos) m_pos++;
	}

	void removed(int a_pos)
	{
	 if(m_pos>a_pos) m_pos--;
	}

	void reset(unsigned i=0)
	{
	 int l=m_list->length();
	 if(i>l) i=l;
	 m_pos=i;
	}

};

template <class T> class List : public List_Base
{
 friend class Const_listiter<T>;
 friend class Listiter<T>;

 public:
 	List()
 	{
	 m_dataSize=sizeof(T);
 	 m_data=NULL;
 	 m_size=0;
 	 m_numIters=0;
 	 m_iters=NULL;
 	}

 	List(const T &t1)
 	{
	 m_dataSize=sizeof(T);
 	 m_data=NULL;
 	 m_size=0;
 	 put(t1);
 	 m_numIters=0;
 	 m_iters=NULL;
 	}

 	~List()
 	{
 	 freeList();
 	 int i;
 	 for(i=0;i<m_numIters;i++)
 	 	m_iters[i]->listDied();
 	 if(m_iters) free(m_iters);
 	}

 	List(const List<T> &tl)
 	{
 	 m_iters=NULL;
 	 m_numIters=0;
 	 if((&tl)!=this)
 	 {
	  m_data=NULL;
	  m_size=0;
	  copyList(tl);
	 }
 	}

 	List<T> &operator=(const List<T> &tl)
 	{
 	 if((&tl)!=this)
 	 {
	  freeList();
	  copyList(tl);
	  int i;
	  for(i=0;i<m_numIters;i++)
	  	m_iters[i]->reset();
	 }
	 return(*this);
 	}

 	operator void*() const
 	{
 	 return((void *) m_data);
 	}

 	List<T> operator+(const T &t)
 	{
 	 List<T> ret=*this;
 	 ret.put(t);
 	 return(ret);
 	}

 	List<T> operator+(const List<T> &tl)
 	{
 	 List<T> ret=*this;
 	 ret.copyList(tl);
	 return(ret);
 	}

 	T* head() const
 	{
 	 if(m_size>0)
	  return((T*) (m_data[0]));
	 else
	  return(NULL);
 	}

 	T* tail() const
 	{
 	 if(m_size>0)
	  return((T*) (m_data[m_size-1]));
	 else
	  return(NULL);
 	}

 	List<T> &put(const T &t)
 	{
 	 insert(m_size,t);
 	 return(*this);
 	}

 	List<T> &put(const List<T> &tl)
 	{
 	 copyList(tl);
 	 return(*this);
 	}

 	T unput()
 	{
 	 T res;
 	 unput(res);
	 return(res);
 	}

 	int unput(T& t)
 	{
	 int res=0;
 	 if(m_size>0)
 	 {
	  t=*tail();
 	  remove(m_size-1);
 	  res=1;
 	 }
	 return(res);
 	}

 	T get()
 	{
	  T* t=(T*) (m_data[0]);
	  remove(0);
 	  return(*t);
 	}

 	int get(T &t)
 	{
	 int res;
 	 if(m_size>0)
 	 {
 	  t=get();
 	  res=1;
 	 }
 	 else
 	  res=0;
	 return(res);
 	}

 	List<T> &unget(const T &t)
 	{
 	 insert(0,t);
 	 return(*this);
 	}

 	List<T> &unget(const List<T> &tl)
 	{
 	 int i,l=tl.length();
 	 for(i=l-1;i>=0;i--)
 	  insert(0,*((T*) (tl.m_data[i])));
 	 return(*this);
 	}

 	T &operator[](unsigned i)
 	{
 	 assert((i>=0)&&(i<m_size));
 	 return(*((T*) (m_data[i])));
 	}

 	const T &operator[](unsigned i) const
 	{
 	 assert((i>=0)&&(i<m_size));
 	 return(*((T*) (m_data[i])));
 	}

 	void sort(int (*f)(const T&,const T&))
 	{
	 // List_Base::sort(
	 cerr << "Sort nicht implementiert." << endl;
	 f=f;
 	 assert(0);
 	}

 protected:
	//----------------------------------------------------
	// intern

	void inserted(int a_pos)
	{
	 int i;
	 for(i=0;i<m_numIters;i++)
	  m_iters[i]->inserted(a_pos);
	}

	void removed(int a_pos)
	{
	 int i;
	 for(i=0;i<m_numIters;i++)
	  m_iters[i]->removed(a_pos);
	}

 	void insert(int a_pos,const T& t)
 	{
	 List_Base::insert(a_pos);
	 m_data[a_pos]=new T(t);
	 inserted(a_pos);
 	}

 	void remove(int a_pos)
 	{
 	 int i;
 	 delete (T*) (m_data[a_pos]);
 	 for(i=a_pos;i<(m_size-1);i++)
 	  m_data[i]=m_data[i+1];
 	 m_size--;

	if (m_size)
	{
		m_data=(void **) realloc(m_data,m_size*sizeof(void *));
	}
	else
	{
		m_data = 0;
 	}

	if(m_size>0) assert(m_data);
	removed(a_pos);
 	}

 	void freeList()
 	{
 	 int i;
 	 for(i=0;i<m_size;i++)
 	 	delete (T*) (m_data[i]);
 	 if(m_data) free(m_data);
 	 m_size=0;
 	 m_data=NULL;
 	}

 	void copyList(const List<T> &tl)
 	{
 	 int i;
 	 for(i=0;i<tl.m_size;i++)
 	 	put(*((T*) (tl.m_data[i])));
 	}
};

template <class T> class List_of_p : public List<T *>
{
 public:
	T *head()
	{
	 return(*(List<T *>::head()));
	}

	T *tail()
	{
	 return(*(List<T *>::tail()));
	}

 	void sort(int (*lessThan)(const T* &,const T* &))
 	{
		int i,j;
		for(j=0;j<m_size;j++)
			for(i=1;i<m_size;i++)
			{
				const T *pt1=*((T **) m_data[i]);
				const T *pt2=*((T **) m_data[i-1]);
				if(lessThan(pt1,pt2))
				{
					T **tmp=(T **) m_data[i];
					m_data[i]=m_data[i-1];
					m_data[i-1]=tmp;
				}
			}
 	}

};

template <class T> class Const_listiter : public Listiter_Base
{
 friend class List<T>;

 public:
        Const_listiter()
        {
         m_list=NULL;
         m_pos=0;
        }

 	Const_listiter(const List<T> &l)
 	{
 	 m_list=(List<T> *) &l;
 	 m_pos=0;
 	 m_list->remember(this);
 	}

 	~Const_listiter()
 	{
 	 if(m_list)
 	  m_list->forget(this);
 	}

 	Const_listiter(const Const_listiter<T> &it)
 	{
 	 m_list->forget(this);
 	 m_list=it.m_list;
 	 m_pos=it.m_pos;
 	 m_list->remember(this);
 	}

 	Const_listiter<T> &operator=(const Const_listiter<T> &it)
 	{
 	 m_list->forget(this);
 	 m_list=it.m_list;
 	 m_pos=it.m_pos;
 	 m_list->remember(this);
 	 return(*this);
 	}

 	int operator==(const Const_listiter<T> &it) const
 	{
 	 return((m_list==it.m_list)&&(m_pos==it.m_pos));
 	}

 	int operator!=(const Const_listiter<T> &it) const
 	{
	 return(!((m_list==it.m_list)&&(m_pos==it.m_pos)));
 	}

	const List<T> *the_list()
	{
	 return((List<T> *) m_list);
	}

	int at_head() const
	{
	 return(m_pos==0);
	}

	int at_end() const
	{
	 return(m_pos==m_list->length());
	}

	int position() const
	{
	 return(m_pos);
	}

	void end_reset(unsigned i=0)
	{
	 int l=m_list->length();
	 if(i>l) i=l;
	 m_pos=l-i;
	}

	int find_next(const T& t)
	{
	 int i,l=m_list->length(),res=0;
	 for(i=m_pos;i<l;i++)
	  if(*((T*) ((m_list->m_data)[i]))==t)
	  {
	   m_pos=i;
	   res=1;
	  }
	 return(res);
	}

	int find_prev(const T& t)
	{
	 int i,res=0;
	 for(i=m_pos-1;i>=0;i--);
	  if(*((T*) ((m_list->m_data)[i]))==t)
	  {
	   m_pos=i+1;
	   res=1;
	  }
	 return(res);
	}

	int step_next()
	{
	 int l=m_list->length();
	 int oldPos=m_pos;
	 if(m_pos<l)
	  m_pos++;
	 return(oldPos!=m_pos);
	}

	int step_prev()
	{
	 int oldPos=m_pos;
	 if(m_pos>0)
	  m_pos--;
	 return(oldPos!=m_pos);
	}

	int peek_next(T &t) const
	{
	 int res=0;
	 if(m_pos<m_list->length())
	 {
	  t=*((T*) ((m_list->m_data)[m_pos]));
	  res=1;
	 }
	 return(res);
	}

	int peek_prev(T &t) const
	{
	 if(m_pos>0)
	 {
	  t=*((T*) ((m_list->m_data)[m_pos-1]));
	 }
	 return(0);
	}

	int peek_next(T *&t) const
	{
	 int res=0;
	 if(m_pos<m_list->length())
	 {
	  t=((T*) ((m_list->m_data)[m_pos]));
	  res=1;
	 }
	 return(res);
	}

	int peek_prev(T *&t) const
	{
	 if(m_pos>0)
	 {
	  t=((T*) ((m_list->m_data)[m_pos-1]));
	 }
	 return(0);
	}

	T *peek_next() const
	{
	 T *res=NULL;
	 if(m_pos<m_list->length())
	  res=((T*) ((m_list->m_data)[m_pos]));
	 return(res);
	}

	T *peek_prev() const
	{
	 T *res=NULL;
	 if(m_pos>0)
	  res=((T*) ((m_list->m_data)[m_pos-1]));
	 return(res);
	}

	int next(T &t)
	{
	 int res=0;
	 T *tp=next();
	 if(tp)
	 {
	  t=*tp;
	  res=1;
	 }
	 return(res);
	}

	int prev(T &t)
	{
	 int res=0;
	 T *tp=prev();
	 if(tp)
	 {
	  t=*tp;
	  res=1;
	 }
	 return(res);
	}

	int next(T *&t)
	{
	 return((int) (t=next()));
	}

	int prev(T *&t)
	{
	 return((int) (t=prev()));
	}

	T *next()
	{
	 T *res=NULL;
	 if(m_pos<m_list->length())
	  res=((T*) ((m_list->m_data)[m_pos++]));
	 return(res);
	}

	T *prev()
	{
	 T *res=NULL;
	 if(m_pos>0)
	  res=((T*) ((m_list->m_data)[--m_pos]));
	 return(res);
	}

};

template <class T> class Listiter : public Const_listiter<T>
{
 friend class List<T>;

 public:
 	Listiter()
 	{
 	 m_list=NULL;
 	 m_pos=0;
 	}

 	Listiter(List<T> &l):Const_listiter<T>(l)
 	{
 	}

 	Listiter(Listiter<T> &l):Const_listiter<T>(l)
 	{
 	}

 	~Listiter()
 	{
 	}

	Listiter<T> &operator=(const Listiter<T> &l)
	{
	 Const_listiter<T>::operator=(l);
	 return(*this);
	}

	int operator==(const Listiter<T> &l) const
	{
	 return(Const_listiter<T>::operator==(l));
	}

	int operator!=(const Listiter<T> &l) const
	{
	 return(Const_listiter<T>::operator!=(l));
	}

	List<T> *the_list()
	{
	 return((List<T> *) m_list);
	}

	void insert_next(const T &t)
	{
	 m_list->insert(m_pos);
	 (m_list->m_data)[m_pos]=new T(t);

	 m_list->inserted(m_pos);
	}

	void insert_prev(const T &t)
	{
	 m_list->insert(m_pos-1);
	 (m_list->m_data)[m_pos-1]=new T(t);

	 m_list->inserted(m_pos-1);
	}

	int replace_next(const T &t)
	{
	 int res=0;
	 if(m_pos<m_list->length())
	 {
	  *((T*) ((m_list->m_data)[m_pos]))=t;
	  res=1;
	 }
	 return(res);
	}

	int replace_prev(const T &t)
	{
	 int res=0;
	 if(m_pos>0)
	 {
	  *((T*) ((m_list->m_data)[m_pos-1]))=t;
	  res=1;
	 }
	 return(res);
	}

	int remove_next()
	{
	 int res=0;
	 if(m_pos<m_list->length())
	 {
	  m_list->remove(m_pos);
	  res=1;
	 }
	 return(res);
	}

	int remove_prev()
	{
	 int res=0;
	 if(m_pos>0)
	 {
	  m_list->remove(m_pos-1);
	  res=1;
	 }
	 return(res);
	}

	int remove_next(T &t)
	{
	 int res=0;
	 if(m_pos<m_list->length())
	 {
	  t=*((T*) ((m_list->m_data)[m_pos]));
	  m_list->remove(m_pos);
	  res=1;
	 }
	 return(res);
	}

	int remove_prev(T &t)
	{
	 int res=0;
	 if(m_pos>0)
	 {
	  t=*((T*) ((m_list->m_data)[m_pos-1]));
	  m_list->remove(m_pos-1);
	  res=1;
	 }
	 return(res);
	}
};

template <class T> class Const_list_of_piter : public Const_listiter<T *>
{
 public:
 	Const_list_of_piter(const List<T *> &l):Const_listiter<T *>(l)
 	{
 	}

	T *peek_next() const
	{
	 T **dp=Const_listiter<T *>::peek_next();
	 T *res=NULL;
	 if(dp)
	  res=*dp;
	 return(res);
	}

	T *peek_prev() const
	{
	 T **dp=Const_listiter<T *>::peek_prev();
	 T *res=NULL;
	 if(dp)
	  res=*dp;
	 return(res);
	}

	T* next()
	{
	 T **dp=Const_listiter<T *>::next();
	 T *res=NULL;
	 if(dp)
	  res=*dp;
	 return(res);
	}

	T* prev()
	{
	 T **dp=Const_listiter<T *>::prev();
	 T *res=NULL;
	 if(dp)
	  res=*dp;
	 return(res);
	}
};

template <class T> class List_of_piter : public Listiter<T *>
{
 public:
 	List_of_piter(List<T *> &l):Listiter<T *>(l)
	{
	}

	T *peek_next() const
	{
	 T **dp=Listiter<T *>::peek_next();
	 T *res=NULL;
	 if(dp)
	  res=*dp;
	 return(res);
	}

	T *peek_prev() const
	{
	 T **dp=Listiter<T *>::peek_prev();
	 T *res=NULL;
	 if(dp)
	  res=*dp;
	 return(res);
	}

	T* next()
	{
	 T **dp=Listiter<T *>::next();
	 T *res=NULL;
	 if(dp)
	  res=*dp;
	 return(res);
	}

	T* prev()
	{
	 T **dp=Listiter<T *>::prev();
	 T *res=NULL;
	 if(dp)
	  res=*dp;
	 return(res);
 	}
};

#endif
