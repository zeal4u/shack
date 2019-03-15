#include "list_node.h"

#include <stdio.h>

ListNode* ListNode::BuildList(const std::vector<int>& nums) 
{
  ListNode* head = new ListNode;
  ListNode* node = head;
  for (int i = 0; i < nums.size(); ++i) {
    if (node->val == 0) node->val = nums[i];
    else {
      ListNode* nex = new ListNode;
      nex->val = nums[i];
      node->next = nex;
      node = nex;
    } 
  }
  return head;
}

void ListNode::Destroy(ListNode* head)
{
  if (head != nullptr) {
    Destroy(head->next);
    delete head;
    head = nullptr;
  }
}

ListNode* ListNode::DeleteNode(ListNode** node) 
{
  if (*node != nullptr) {
    ListNode* ret = *node;
    *node = (*node)->next;
    return ret;
  }
  return nullptr;
}

void ListNode::PrintList(ListNode* list)
{
  while(list != nullptr) {
    printf("%d->", list->val);
    list = list->next;
  }
  printf("null\n");
}

