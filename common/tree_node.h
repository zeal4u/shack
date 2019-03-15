//
// Created by zeal4u on 2018/9/20.
//

#ifndef LEETCODE_TREENODE_H
#define LEETCODE_TREENODE_H

#include <stdint.h>
#include <vector>

#define null INT32_MIN

class TreeNode {
 public:
  int val;
  TreeNode* left;
  TreeNode* right;
  TreeNode(int x):val(x),left(nullptr),right(nullptr){}
  static TreeNode* BuildTree(const std::vector<int> nums);
  static void Destroy(TreeNode* root);

};

#endif //LEETCODE_TREENODE_H
