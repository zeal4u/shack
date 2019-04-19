#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <algorithm>
#include <utility>
#include <unordered_set>
#include <math.h>
#include <string>


#include "list_node.h"
#include "tree_node.h"
#include "min_stack.h"


bool is_rotate(const char* str1, const char* str2)
{
  if (str1 == nullptr && str2 == nullptr) return true;
  else if (str1 == nullptr || str2 == nullptr) return false;
  int len = strlen(str1);
  if (len != strlen(str2)) return false;
  for (int i = 0; i < len; ++i) {
    if (str1[i] != str2[len - i - 1]) return false;
  }
  return true;
}

void _test_is_rotate(const char* str1, const char* str2)
{
  printf("%s is %s rotate of %s\n", str1, is_rotate(str1, str2) ? "the" : "not", str2);
}

void test_is_rotate()
{
  _test_is_rotate("", "");
  _test_is_rotate("abc", "");
  _test_is_rotate("abc", "cba");
  _test_is_rotate("abba", "abba");
  _test_is_rotate("aaaa", "aaba");
  _test_is_rotate("aaaa", "aaaa");
}

int min_distance(TreeNode* root)
{
  if (root == nullptr) return -1;
  std::stack<TreeNode*> stk;
  std::vector<int> values;
  int ret = INT32_MAX; 
  while (root != nullptr || !stk.empty()) {
    while (root != nullptr) {
      stk.push(root);
      root = root->left;
    }
    TreeNode* node = stk.top();
    stk.pop();
    values.push_back(node->val);
    if (node->right != nullptr)
      root = node->right;
  }
  for (int i = 1; i < values.size(); ++i) {
    int dis = abs(values[i] - values[i - 1]);
    if (ret > dis) ret =  dis;
  }
  std::for_each(values.begin(), values.end(), [](int i){printf("%d ", i);});
  printf("\n");
  return ret;
}

void test_min_distance()
{
  std::vector<int> nums = {4, 1, 5, 1, 3};
  TreeNode* root = TreeNode::BuildTree(nums);
  printf("Min distance is %d\n", min_distance(root)); 
  TreeNode::Destroy(root);
}

ListNode* merge_list(ListNode* l1, ListNode* l2)
{
  if (l1 == nullptr) return l2;
  else if (l2 == nullptr) return l1;
  ListNode* head = nullptr;
  if (l1->val < l2->val) {
    l1->next = merge_list(l1->next, l2);
    head = l1;
  } else {
    l2->next = merge_list(l1, l2->next);
    head = l2;
  }
  return head;
}

void test_merge_list()
{
  ListNode* l1 = ListNode::BuildList({1,3,5,7,9});
  ListNode* l2 = ListNode::BuildList({2,4,6,8,10});
  ListNode* node = merge_list(l1, l2);
  ListNode* head = node;
  ListNode::PrintList(head);
  ListNode::Destroy(head);
}

void _mark(int id, std::unordered_set<int>& to_delete, const std::vector<std::pair<int,int>>& set)
{
  to_delete.insert(id);
  for (auto &item : set) {
    if (item.first == id) _mark(item.second, to_delete, set);
  }
}

void delete_bookmark(int id, std::vector<std::pair<int,int>>& set)
{
  std::unordered_set<int> to_delete;
  _mark(id, to_delete, set);
  for (auto item = set.begin(); item != set.end();) {
    if (to_delete.find(item->second) != to_delete.end()) 
      item = set.erase(item);
    else 
      ++item;
  }
}

void test_delete_bookmark()
{
  std::vector<std::pair<int, int>> set = {{5, 6}, {0, 1}, {1, 2}, {3, 4}, {2, 3}, {2, 5}};  
  delete_bookmark(2, set);
  for_each(set.begin(), set.end(), [](const std::pair<int,int> p){printf("[%d, %d]", p.first, p.second); });
  printf("\n");
}

void _travel(TreeNode* root, std::unordered_set<TreeNode*>& set)
{
  if (root == nullptr) return;
  set.insert(root);
  _travel(root->left, set);
  _travel(root->right, set);
}

void _find_err(TreeNode* root, const std::unordered_set<TreeNode*>& set, TreeNode** target)
{
  if (root == nullptr) return;
  if (root->left == nullptr || root->right == nullptr) {
    bool flag = false;
    if (root->left && set.find(root->left) != set.end()) {
      flag = true; 
    } else if (root->right && set.find(root->right) != set.end()) {
      flag = true;
    }
    if (flag) {
      *target = root;
      return;
    }
  }
  _find_err(root->left, set, target);
  _find_err(root->right, set, target);
}

TreeNode* search_err_node(TreeNode* root)
{
  if (root == nullptr) return root;
  std::unordered_set<TreeNode*> set;
  _travel(root, set);
  TreeNode* target = nullptr;
  _find_err(root, set, &target);
  return target;
}

void test_search_err_node()
{
  TreeNode* root = TreeNode::BuildTree({1, 2, 3, 4, 5});
  root->left->left->right = root->right;
  printf("Node %d is err!\n", search_err_node(root)->val);
}

int number_of_1(int n)
{
  if (n <= 0) return 0;
  int first = n, len = 1;
  while (first >= 10) {
    first = first / 10;
    len++;
  }
  if (len == 1 && first == 0) 
    return 0;
  if (len == 1 && first >= 1) 
    return 1;
  int pow_len_m2 = pow(10, len - 2);
  int num_first_digit = pow_len_m2 * 10;
  if (first == 1) {
    num_first_digit = n % num_first_digit + 1;
  }
  int num_other_digit = first * (len - 1) * pow_len_m2;
  return num_first_digit + num_other_digit + number_of_1(n % (pow_len_m2 * 10)); 
}

void test_number_of_1(int n)
{
  printf("%d\n", number_of_1(n)); 
}

int GetUglyNumber_Solution(int index) {
    std::vector<long long> ugly_nums(index, 0);
    ugly_nums[0] = 1;
    int index_of_ugly2 = 0;
    int index_of_ugly3 = 0;
    int index_of_ugly5 = 0;
    for (int i = 1; i < index; ++i) {
        while (ugly_nums[index_of_ugly2] * 2 <= ugly_nums[i-1]) {
            index_of_ugly2++;
        }
        while (ugly_nums[index_of_ugly3] * 3 <= ugly_nums[i-1]) {
            index_of_ugly3++;
        }
        while (ugly_nums[index_of_ugly5] * 5 <= ugly_nums[i-1]) {
            index_of_ugly5++;
        }
        ugly_nums[i] = std::min({ugly_nums[index_of_ugly2] * 2, ugly_nums[index_of_ugly3] * 3, ugly_nums[index_of_ugly5] * 5});
    }
    return ugly_nums[index - 1];
}

void test_GetUglyNumber_Solution(int i) {
  printf("%d\n", GetUglyNumber_Solution(i));
}

void test_min_stack()
{
    MinStack<int> s;
    s.push(1);
    s.push(2);
    s.push(1);
    s.push(3);
    s.push(1);
    s.top();
    s.pop();
    s.pop();
    s.pop();
}

ListNode* deleteDuplication(ListNode* pHead)
{
    if (pHead == nullptr)
        return nullptr;
    ListNode *curNode = pHead, *preNode = nullptr, *nextNode = curNode->next;
    while (nextNode != nullptr) {
        bool need_delete = false;
        while (nextNode != nullptr && curNode->val == nextNode->val) {
            need_delete = true;
            ListNode *tmp = nextNode;
            nextNode = nextNode->next;
            delete tmp;
        }
        if (need_delete) {
            if (preNode != nullptr)
                preNode->next = nextNode;
            else 
                pHead = nextNode;
            delete curNode;
            curNode = nullptr;
        } else {
            preNode = curNode;
        }
        if (nextNode == nullptr)
            break;
        curNode = nextNode;
        nextNode = nextNode->next;
    }
    return pHead;
}

void test_deleteDuplication()
{
  ListNode *head = ListNode::BuildList({1,1,1});
  head = deleteDuplication(head);
  ListNode::PrintList(head);
  ListNode::Destroy(head);


  head = ListNode::BuildList({1,2,3,4,5});
  head = deleteDuplication(head);
  ListNode::PrintList(head);
  ListNode::Destroy(head);

  head = ListNode::BuildList({1,1,2,3,3,4,5,5});
  head = deleteDuplication(head);
  ListNode::PrintList(head);
  ListNode::Destroy(head);
}

std::vector<int> recover_cards(std::vector<int> cards_on_table)
{
	if (cards_on_table.size() == 0)
		return {};
	std::vector<int> cards_in_hand;
	while (!cards_on_table.empty()) {
		int cur = cards_on_table.back();
		cards_on_table.pop_back();
		if (cards_in_hand.size() > 1) {
			int back = cards_in_hand.back();
			cards_in_hand.pop_back();
			cards_in_hand.insert(cards_in_hand.begin(), back);
		}
		cards_in_hand.insert(cards_in_hand.begin(), cur);
	}
	return cards_in_hand;
}

void test_recover_cards()
{
	std::vector<int> res = recover_cards({1,3,2});
	for_each(res.begin(), res.end(), [](int i){printf("%d ", i);});
	printf("\n");
	
	res = recover_cards({1,3,2,4});
	for_each(res.begin(), res.end(), [](int i){printf("%d ", i);});
	printf("\n");
}

std::vector<std::string> split(std::string str) {
  if (str.size() == 0)
    return {};
  std::vector<std::string> output;
  std::stack<int> stk1, stk2;
  std::list<std::pair<int, int>> ranges;
  for (int i = 0; i < str.size(); ++i) {
    if (str[i] == '\'') {
      if (stk1.empty()) {
        stk1.push(i);
      } else {
        ranges.push_back({stk1.top(), i}); 
        stk1.pop();
        if (!stk2.empty()) 
          stk2.pop();
      }
    }
    if (str[i] == '\"') {
      if (stk2.empty()) {
        stk2.push(i);
      } else {
        ranges.push_back({stk2.top(), i}); 
        stk2.pop();
        if (!stk1.empty()) 
          stk1.pop();
      }
    }
  }
  std::pair<int, int> range = ranges.front();
  ranges.pop_front();
  int j = 0;
  for (int i = 0; i < str.size();) {
    if (i == range.first) {
      i = range.second + 1;
      if (!ranges.empty()) {
        range = ranges.front();
        ranges.pop_front();
      }
      continue;
    } 
    if (str[i] == ' ') {
      output.emplace_back(str.begin() + j, str.begin() + i);
      j = i + 1;
    }
    i++;
  }
  output.emplace_back(str.begin() + j, str.end());
  return output;
}
