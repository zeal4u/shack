#ifndef _SAFE_SET_H_
#define _SAFE_SET_H_

#include <mutex>
#include <atomic>

#include "atomic_markable_pointer.h"

namespace safe{

typedef struct Node {
  void* item;
  int key;
  struct Node* next;
  Node(int key): item(nullptr), key(key), next(nullptr) {}
}Node;

class AbstractSafeSet {
 public:
  virtual bool add(int i) = 0;
  virtual bool remove(int i) = 0;
  virtual bool contains(int i) = 0;

  AbstractSafeSet(const AbstractSafeSet&) = delete;
  AbstractSafeSet& operator=(const AbstractSafeSet&) = delete;

  AbstractSafeSet() {}
  virtual ~AbstractSafeSet() {}
};

class CoarseSafeSet: public AbstractSafeSet {
 public:
  CoarseSafeSet();
  ~CoarseSafeSet();

  virtual bool add(int i) override;
  virtual bool remove(int i) override;
  virtual bool contains(int i) override;

  void print();
  
  CoarseSafeSet(const CoarseSafeSet&) = delete;
  CoarseSafeSet& operator=(const CoarseSafeSet&) = delete;

 private:
  Node* head_;
  std::mutex mu_; 
};

typedef struct LockNode {
  std::mutex mu;
  void* item;
  int key;
  struct LockNode* next;
  LockNode(int key): mu(), item(nullptr), key(key), next(nullptr) {}
} LockNode;


class FineSafeSet: public AbstractSafeSet {
 public:
  FineSafeSet();
  ~FineSafeSet();

  virtual bool add(int i) override;
  virtual bool remove(int i) override;
  virtual bool contains(int i) override;
  void print();

 private:
  LockNode* head_;
};

typedef struct MarkedNode {
  void* item;
  int key;
  safe::AtomicMarkablePointer<MarkedNode*> next; 
  MarkedNode(int key): item(nullptr), key(key), next(nullptr, false) {}
} MarkedNode;

typedef struct Window {
  MarkedNode* pre;
  MarkedNode* cur;
  Window(MarkedNode* p, MarkedNode* c):pre(p), cur(c) {}
}Window;

class UnblockingSafeSet: public AbstractSafeSet {
 public:
  UnblockingSafeSet();
  ~UnblockingSafeSet();

  virtual bool add(int i) override;
  virtual bool remove(int i) override;
  virtual bool contains(int i) override;
  void print();
  Window find(MarkedNode* head, int key); 
  
 private:
  MarkedNode* head_; 
};

template<class T>
void delete_set(T set);

} //namespace safe
#endif //_SAFE_SET_H_
