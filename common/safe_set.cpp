#include "safe_set.h"

#include <stdint.h>
#include <stdio.h>

namespace safe{

CoarseSafeSet::CoarseSafeSet(): mu_()
{
  head_ = new Node(INT32_MIN);
  head_->next = new Node(INT32_MAX);
}

void CoarseSafeSet::print()
{
  Node* node = head_;
  while (node->key != INT32_MAX) {
    if (node->key != INT32_MIN)
      printf("key 0x%x->", node->key);
    node = node->next;
  }
  printf("null\n");
}

bool CoarseSafeSet::contains(int key) 
{
  printf("CoarseSafeSet::contains is not well defined!");
  // TODO
  return false;
}

bool CoarseSafeSet::add(int key)
{
  std::lock_guard<std::mutex> lock(mu_);
  Node *pre = head_, *cur = head_->next;
  while (cur->key < key) {
    pre = cur;
    cur = cur->next;
  }
  if (key == cur->key) return false;
  else {
    Node* node = new Node(key);
    node->next = cur;
    pre->next = node;
    return true;
  }
} 

bool CoarseSafeSet::remove(int key)
{
  std::lock_guard<std::mutex> lock(mu_);
  Node *pre = head_, *cur = head_->next;
  while (cur->key < key) {
    pre = cur;
    cur = cur->next;
  }
  if (cur->key == key) {
    pre->next = cur->next;
    delete cur;
    cur = nullptr;
    return true;
  } else return false;
}

CoarseSafeSet::~CoarseSafeSet()
{
  //std::lock_guard<std::mutex> lock(mu_);
  Node* node = head_;
  Node* next = nullptr;
  while (node != nullptr) {
    next = node->next;
    if (node->item) delete reinterpret_cast<int*>(node->item);
    node->item = nullptr;
    delete node;
    node = next; 
  }
  head_ = nullptr;
}


// FineSafeSet Func
FineSafeSet::FineSafeSet() 
{
  head_ = new LockNode(INT32_MIN);
  head_->next = new LockNode(INT32_MAX);
}

bool FineSafeSet::add(int key)
{
  head_->mu.lock();
  LockNode *pre = head_, *cur = pre->next;
  cur->mu.lock();
  while (cur->key < key) {
    pre->mu.unlock();
    pre = cur;
    cur = cur->next;
    cur->mu.lock();
  }
  if (cur->key == key) {
    pre->mu.unlock();
    cur->mu.unlock();
    return false;
  } else {
    LockNode* node = new LockNode(key);
    pre->next = node;
    node->next = cur; 
    cur->mu.unlock();
    pre->mu.unlock();
    return true;
  }
}

bool FineSafeSet::remove(int key)
{
  head_->mu.lock();
  LockNode* pre = head_, *cur = head_->next;
  cur->mu.lock();
  while (cur->key < key) {
    pre->mu.unlock();
    pre = cur;
    cur = cur->next;
    cur->mu.lock();
  }
  if (cur->key == key) {
    pre->next = cur->next;
    cur->mu.unlock();
    delete cur;
    cur = nullptr;
    pre->mu.unlock();
    return true; 
  } else { 
    cur->mu.unlock();
    pre->mu.unlock();
    return false;
  }
}

bool FineSafeSet::contains(int key)
{
  return false;
}

FineSafeSet::~FineSafeSet()
{
  LockNode* node = head_;
  LockNode* next = nullptr;
  while (node != nullptr) {
    next = node->next;
    if (node->item) delete reinterpret_cast<int*>(node->item);
    node->item = nullptr;
    delete node;
    node = next; 
  }
  head_ = nullptr;
}

void FineSafeSet::print()
{
  LockNode* node = head_;
  while (node->key != INT32_MAX) {
    if (node->key != INT32_MIN)
      printf("key 0x%x->", node->key);
    node = node->next;
  }
  printf("null\n");
}
// FineSafeSet Func end

// UnblockingSafeSet Func
UnblockingSafeSet::UnblockingSafeSet()
{
  head_ = new MarkedNode(INT32_MIN);
  MarkedNode* tmp = nullptr;
  head_->next.compareAndSet(tmp, new MarkedNode(INT32_MAX), false, false);
}

Window UnblockingSafeSet::find(MarkedNode* head, int key)
{
  MarkedNode* pre = nullptr, *cur = nullptr, *succ = nullptr;
  bool marked = false; 
  while(true) {
    bool snip = true;
    pre = head;
    cur = pre->next.getReference();
    while (true) {
      succ = cur->next.get(marked);
      while (marked) {
        snip = pre->next.compareAndSet(cur, succ, false, false);
        if (snip) {
          cur = succ;
          succ = cur->next.get(marked);
        }
      }
      if (snip) {
        if (cur->key >= key)  return Window(pre, cur);
        pre = cur;
        cur = succ;
      }
    }
  }
}

bool UnblockingSafeSet::add(int key) 
{
  while (true) {
    Window win = find(head_, key);
    MarkedNode* pre = win.pre, *cur = win.cur; 
    if (cur->key == key) return false;
    else {
      MarkedNode* new_node = new MarkedNode(key);
      new_node->next.compareAndSet(nullptr, cur, false, false);
      if (pre->next.compareAndSet(cur, new_node, false, false))
         return true;
    }
  }
}

bool UnblockingSafeSet::remove(int key)
{
  bool snip = false;
  while (true) {
    Window win = find(head_, key);
    MarkedNode* pre = win.pre, *cur = win.cur;
    if (cur->key != key) return false;
    else {
      MarkedNode* succ = cur->next.getReference();
      snip = cur->next.compareAndSet(succ, succ, false, true);
      if (!snip) continue;
      pre->next.compareAndSet(cur, succ, false, false);
      return true;
    }
  }
}

bool UnblockingSafeSet::contains(int key)
{
  return false;
}

void UnblockingSafeSet::print()
{
  MarkedNode* node = head_;
  while (node->key != INT32_MAX) {
    if (node->key != INT32_MIN)
      printf("key 0x%x->", node->key);
    node = node->next.getReference();
  }
  printf("null\n");
}

UnblockingSafeSet::~UnblockingSafeSet()
{
  MarkedNode* node = head_;
  while (node != nullptr) {
    MarkedNode* next = node->next.getReference(); 
    delete node;
    node = next;
  }
}
// UnblockingSafeSet Func end

template<class T>
void delete_set(T set)
{
}
} // namespace safe
