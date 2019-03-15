#include <stdio.h>

#include "list_node.h"

int main(int argc, char* argv[])
{
  ListNode* list = ListNode::BuildList({1,2,3,4,5}); 
  ListNode::DeleteNode(&(list->next));
  ListNode::DeleteNode(&(list->next->next));
  ListNode::DeleteNode(&(list->next->next));
  ListNode::PrintList(list);
  ListNode::Destroy(list);
  return 0;
}
