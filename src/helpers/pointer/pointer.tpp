#include "pointer.hpp"

template <class Type>
safe_pointer<Type>::safe_pointer() {
  pointer = nullptr;
  destroyed = false;
}

template <class Type>
safe_pointer<Type>::safe_pointer(Type * pointer) {
  this->pointer = pointer;
  destroyed = false;
}

template <class Type>
safe_pointer<Type>::~safe_pointer() {
  if (!destroyed) delete pointer;
}

template <class Type>
void safe_pointer<Type>::destroy() {
  if (!destroyed) delete pointer;
}

template <class Type>
safe_pointer<Type> safe_pointer<Type>::operator=(Type * pointer) {
  this->pointer = pointer;
  destroyed = false;
}

template <class Type>
safe_pointer<Type>::operator void* () {
  return (void*)pointer;
}

template <class Type>
Type safe_pointer<Type>::operator*() {
  if (!destroyed) return *pointer;
  return Type();
}

template <class Type>
Type * safe_pointer<Type>::operator->() {
  if (!destroyed) return pointer;
  return new Type();
}
