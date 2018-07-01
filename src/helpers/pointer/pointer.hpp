#ifndef pointer_hpp
#define pointer_hpp

template <class Type>
class safe_pointer {
private:
  Type * pointer;
  bool destroyed;
public:
  safe_pointer();
  safe_pointer(Type * pointer);
  ~safe_pointer();
  void destroy();
  safe_pointer operator=(Type * pointer);
  operator void* ();
  Type operator*();
  Type * operator->();
};

#include "pointer.tpp"

#endif
