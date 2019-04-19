#ifndef _LISTNODE_H_
#define _LISTNODE_H_

#include<vector>

class ListNode {
 public:
  int val;
  ListNode* next;
  ListNode(): val(0), next(nullptr) {} 

  static ListNode* BuildList(const std::vector<int>& nums);
 
  static void Destroy(ListNode* head);
  
  static ListNode* DeleteNode(ListNode** node);
 
  static void PrintList(ListNode* list);
};
#endif //_LISTNODE_H_
