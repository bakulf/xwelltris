#ifndef BILIST_H
#define BILIST_H

template < class TC > class bilist
{
  TC          object;
  bool        usable;
  bilist<TC>  *next,*prev;
 public:
  bilist<TC>() { next=prev=0;usable=false;};
  bilist<TC>(TC& obj) { next=prev=0;object=obj;usable=true;};

  bool add(bilist<TC> *plist);
  bilist<TC>* del(TC& obj);
  bool del_self() 
    { 
      if(next) 
	next->set_prev(prev); 
      if(prev) 
	prev->set_next(next);
      return true;
    };

  bool is_last() {return next==0;};
  bool is_first() {return prev==0;};

  bilist<TC> *get_next() { return next;};
  bilist<TC> *get_prev() { return prev;};

  void set_prev(bilist<TC> *plist) { prev=plist;};
  void set_next(bilist<TC> *plist) { next=plist;};

  TC& get_object() { return object;};
};

//here we are using while for searching end of list
template <class TC> bool bilist<TC>::add(bilist<TC> *plist)
{
  bilist<TC> *ptr=this;
  while(ptr->get_next()!=0)
    ptr=ptr->get_next();
  ptr->set_next(plist);
  plist->set_prev(ptr);
  return true;
}

//and here lets make recursion for diff :)
template <class TC> bilist<TC>* bilist<TC>::del(TC& obj)
{
  if(obj==object)
  {
    del_self();
    return this;
  }
  if(next)
    return next->del(obj);
  return 0;  //Can't find object in list.
}

#endif

