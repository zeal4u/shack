#include "tree_node.h"

#include <vector>
#include <deque>

TreeNode* TreeNode::BuildTree(const std::vector<int> nums)
{
  TreeNode *root = new TreeNode(nums[0]);
  std::deque<TreeNode*> parents;
  parents.push_back(root);
  std::deque<TreeNode*> children;
  TreeNode * parent;
  bool left = true;
  for (int i = 1; i < nums.size(); i++) {
    if (left) {
      parent = parents.front();
      parents.pop_front();
    }
    if (nums[i] != null) {
      TreeNode *new_node = new TreeNode(nums[i]);
      if (left)
          parent->left = new_node;
      else
          parent->right = new_node;
      children.push_back(new_node);
    }
    left = !left;
    if (left && parents.empty()) {
      parents = children;
      children.clear();
    }
  }
  return root;
}
void TreeNode::Destroy(TreeNode* root) 
{
  if (root != nullptr) {
    Destroy(root->left);
    Destroy(root->right);
    delete root;
    root = nullptr;
  }
}

