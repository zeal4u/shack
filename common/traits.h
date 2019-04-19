#ifndef _TRAITS_H_
#define _TRAITS_H_
template <class T>
struct TypeTraits {
  typedef T ValueType;
  typedef T* Pointer;
  typedef T& Reference;
};

template <class T>
struct TypeTraits<T*> {
  typedef T ValueType;
  typedef T* Pointer;
  typedef T& Reference;
};

template <class T>
struct TypeTraits<const T*> {
  typedef T ValueType;
  typedef const T* Pointer;
  typedef const T& Reference;
};

#endif //_TRAITS_H_
