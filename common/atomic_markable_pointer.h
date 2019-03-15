#ifndef _ATOMIC_MARKABLE_POINTER_H_
#define _ATOMIC_MARKABLE_POINTER_H_

#include <atomic>
#include <stdint.h>

namespace safe{

template <class Pointer>
class AtomicMarkablePointer {
private:
  std::atomic<Pointer> ptr;
  const int64_t Marked = 0x00000001;
  const int64_t UnMraked = 0x00000000;

public:
  explicit AtomicMarkablePointer(Pointer p, bool mark) {
    ptr.store((Pointer)((int64_t)p | (int64_t)mark));
  }
  AtomicMarkablePointer(const AtomicMarkablePointer &a) {
    ptr.store(a.ptr.load());
  }
  AtomicMarkablePointer operator=(const AtomicMarkablePointer &a) {
    if (*this != a) {
      ptr.store(a.ptr.load());
    }
    return *this;
  }
  Pointer getReference() const {
    Pointer p = ptr.load();
    return (bool)((int64_t)p & Marked) ? (Pointer)((int64_t)(p) & ~Marked) : p;
  }
  bool isMarked() const {
    Pointer p = ptr.load();
    return (bool)((int64_t)p & Marked);
  }
  Pointer get(bool &b) const {
    Pointer p = ptr.load();
    b = (bool)((int64_t)p & Marked);
    return b ? (Pointer)((int64_t)(p) & ~Marked) : p;
  }
  bool compareAndSet(Pointer expectedPointer, Pointer newPointer,
                     bool expectedMark, bool newMark) {
    Pointer p = ptr.load();
    bool b = (bool)((int64_t)p & Marked);
    if (b == expectedMark) {
      expectedPointer = (Pointer)((int64_t)expectedPointer | (int64_t)b);
      return ptr.compare_exchange_strong(
          expectedPointer, (Pointer)((int64_t)newPointer | (int64_t)newMark));
    }
    return false;
  }
  void set(Pointer newPointer, bool newMark) {
    newPointer = (Pointer)((int64_t)newPointer | (int64_t)newMark);
    ptr.exchange(newPointer);
  }
  bool attemptMark(Pointer expectedPointer, bool newMark) {
    Pointer newPointer = (Pointer)((int64_t)expectedPointer | (int64_t)newMark);
    expectedPointer = isMarked() ? (Pointer)((int64_t)expectedPointer | Marked)
                                 : expectedPointer;
    return ptr.compare_exchange_strong(expectedPointer, newPointer);
  }
};
}// namespace safe
#endif // _ATOMIC_MARKABLE_POINTER_H_
