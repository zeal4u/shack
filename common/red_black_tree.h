#ifndef _RED_BLACK_TREE_H_
#define _RED_BLACK_TREE_H_

#include "traits.h"

#include <utility>
#include <iostream>
#include <assert.h>
#include <memory>

template<class Key, class Val>
class RedBlackTree {
  // TODO supply user defined comparator
 public:
  typedef typename TypeTraits<Key>::ValueType KeyValueType;
  typedef typename TypeTraits<Key>::Pointer KeyPointer;
  typedef typename TypeTraits<Key>::Reference KeyReference;

  typedef typename TypeTraits<Val>::ValueType ValueValueType;
  typedef typename TypeTraits<Val>::Pointer ValuePointer;
  typedef typename TypeTraits<Val>::Reference ValueReference;

  typedef std::pair<KeyValueType, ValueValueType> DataType;

  typedef enum {
    RED = 0,
    BLACK = 1
  } ColorType;

  struct __TreeNode {
    DataType data_;
    ColorType color_;
    __TreeNode *parent_;
    __TreeNode *left_;
    __TreeNode *right_;

    __TreeNode()
        : data_(),
          color_(RED),
          parent_(nullptr),
          left_(nullptr),
          right_(nullptr) {}

    __TreeNode(DataType data)
        : data_(data),
          color_(RED),
          parent_(nullptr),
          left_(nullptr),
          right_(nullptr) {}
  };

  class Iterator {
   public:
    Iterator(__TreeNode *header) : current_(header->left_) {}

    bool Next() {
      if (current_)
        value_ = current_->data_;
      if (!current_)
        return false;
      if (current_->right_) {
        __TreeNode *node = current_->right_;
        while (node && node->left_) {
          node = node->left_;
        }
        current_ = node;
        return true;
      }

      if (current_->parent_ && current_->parent_->left_ == current_) {
        current_ = current_->parent_;
        return true;
      }

      __TreeNode *parent = current_->parent_, *grand;
      if (parent)
        grand = parent->parent_;
      else
        current_ = parent;

      while (parent && grand && grand->right_ == parent) {
        parent = grand;
        grand = grand->parent_;
      }
      current_ = grand;
      return true;
    }

    DataType Value() {
      return value_;
    }

   private:
    __TreeNode *current_;
    DataType value_;
  };

  RedBlackTree() : header_() {}

  ~RedBlackTree() {
    destroy_node(header_.parent_);
  }

  std::shared_ptr<Iterator> GetIterator() {
    return std::make_shared<Iterator>(&header_);
  }

  void Insert(DataType data) {
    __TreeNode *new_node = new __TreeNode(data);
    insert_by_key(&(header_.parent_), new_node);
    adjust_tree(new_node);
    update_root();
    update_most_left();
    update_most_right();
  }

  void View(int type=1) {
    __TreeNode *root = header_.parent_;
    if (type == 1)
      pre_order_print(root);
    else if (type == 2)
      in_order_print(root);
    std::cout << std::endl;
  }

  bool Get(const KeyValueType &key, ValueReference value) {
    __TreeNode *root = header_.parent_;
    __TreeNode *result = search(root, key);
    if (result) {
      value = result->data_.second;
    }
    return result != nullptr;
  }

  void Delete(const KeyValueType &key) {
    __TreeNode *target_node = search(header_.parent_, key);
    if (!target_node)
      return;

    __TreeNode *to_delete_node = min(target_node->right_);
    if (!to_delete_node) {
      to_delete_node = max(target_node->left_);
    }
    if (to_delete_node) {
      swap(to_delete_node->data_, target_node->data_);
      delete_node_with_one_child(to_delete_node);
    } else {
      //replace_node(target_node, nullptr);
      delete_node_with_one_child(target_node);
    }
    update_root();
    update_most_left();
    update_most_right();
  }

  DataType Min() {
    return header_.left_->data_;
  }

  DataType Max() {
    return header_.right_->data_;
  }

  bool Valid() {
    return verify_color(header_.parent_) != -1;
  }

 private:
  // header_.parent points to root
  // header_.left_ points to the minimum item
  // header_.right_ points to the maximum  item
  __TreeNode header_;

  void pre_order_print(__TreeNode *root) {
    if (root) {
      std::cout << root->data_.first << (root->color_ == RED ? "R" : "B") <<" ";
      pre_order_print(root->left_);
      pre_order_print(root->right_);
    } else
      std::cout << "# ";
  }

  void in_order_print(__TreeNode *root) {
    if (root) {
      in_order_print(root->left_);
      std::cout << root->data_.first << (root->color_ == RED ? "R" : "B") <<" ";
      in_order_print(root->right_);
    } else
      std::cout << "# ";
  }

  void destroy_node(__TreeNode *node) {
    if (node != nullptr) {
      destroy_node(node->left_);
      destroy_node(node->right_);
      delete node;
    }
  }

  KeyValueType key(__TreeNode *node) const {
    return node->data_.first;
  }

  ValueValueType value(__TreeNode *node) const {
    return node->data_.second;
  }

  __TreeNode* parent(__TreeNode *node) const {
    if (node == nullptr)
      return nullptr;
    return node->parent_;
  }

  __TreeNode* grandparent(__TreeNode *node) const {
    __TreeNode *p = parent(node);
    if (p != nullptr)
      return p->parent_;
    return nullptr;
  }

  __TreeNode* sibling(__TreeNode *node) const {
    __TreeNode *p = parent(node);
    if (p != nullptr) {
      if (p->left_ = node)
        return p->right_;
      else
        return p->left_;
    }
    return nullptr;
  }

  __TreeNode *uncle(__TreeNode *node) const {
    __TreeNode *p = parent(node);
    if (p != nullptr) {
      __TreeNode *g = parent(p);
      if (g != nullptr) {
        if (g->left_ == p)
          return g->right_;
        else
          return g->left_;
      }
    }
    return nullptr;
  }

  void update_root() {
    __TreeNode *root = header_.parent_;
    while (root && root->parent_) {
      root = root->parent_;
    }
    header_.parent_ = root;
  }

  void update_most_left() {
    __TreeNode *root = header_.parent_;
    while (root && root->left_) {
      root = root->left_;
    }
    header_.left_ = root;
  }

  void update_most_right() {
    __TreeNode *root = header_.parent_;
    while (root && root->right_) {
      root = root->right_;
    }
    header_.right_ = root;
  }

  __TreeNode* rotate_left(__TreeNode *node) {
    __TreeNode *node_parent = parent(node);
    __TreeNode *node_right = node->right_;
    if (node_right == nullptr)
      return nullptr;
    node->right_ = node_right->left_;
    if (node->right_)
      node->right_->parent_ = node;
    node_right->left_ = node;
    node->parent_ = node_right;
    if (node_parent) {
      if(node_parent->left_ == node) {
        node_parent->left_ = node_right;
      } else {
        node_parent->right_ = node_right;
      }
    }
    node_right->parent_ = node_parent;
    return node_right;
  }

  __TreeNode* rotate_right(__TreeNode *node) {
    __TreeNode *node_parent = parent(node);
    __TreeNode *node_left = node->left_;
    if (node_left == nullptr)
      return nullptr;
    node->left_ = node_left->right_;
    if (node->left_)
      node->left_->parent_ = node;
    node_left->right_ = node;
    node->parent_ = node_left;
    if (node_parent) {
      if (node_parent->left_ == node) {
        node_parent->left_ = node_left;
      } else {
        node_parent->right_ = node_left;
      }
    }
    node_left->parent_ = node_parent;
    return node_left;
  }

  __TreeNode* insert_by_key(__TreeNode **root, __TreeNode *new_node) {
    if (*root != nullptr) {
      if(key(*root) > key(new_node)) { // TODO can be replace by comparer
        if ((*root)->left_)
          return insert_by_key(&(*root)->left_, new_node);
        else
          (*root)->left_ = new_node;
      } else {
        if ((*root)->right_)
          return insert_by_key(&(*root)->right_, new_node);
        else
          (*root)->right_ = new_node;
      }
      new_node->parent_ = *root;
    } else {
      *root = new_node;
    }
    return new_node;
  }

  __TreeNode* adjust_tree(__TreeNode *node) {
    if (parent(node) == nullptr)
      adjust_case1(node);
    else if (parent(node)->color_ == BLACK)
      adjust_case2(node);
    else if (uncle(node) != nullptr && uncle(node)->color_ == RED)
      adjust_case3(node);
    else
      adjust_case4(node);
    return node;
  }

  void adjust_case1(__TreeNode *node) {
    if (node != nullptr && parent(node) == nullptr)
      node->color_ = BLACK;
  }

  void adjust_case2(__TreeNode *node) {
    return;
  }

  void adjust_case3(__TreeNode *node) {
    if (parent(node)) {
      parent(node)->color_ = BLACK;
    }
    if(uncle(node)) {
      uncle(node)->color_ = BLACK;
    }
    __TreeNode *g = grandparent(node);
    if (g != nullptr) {
      g->color_ = RED;
      adjust_tree(g);
    }
  }

  void adjust_case4(__TreeNode *node) {
    __TreeNode *p = parent(node);
    __TreeNode *u = uncle(node);
    __TreeNode *g = grandparent(node);
    if ((u == nullptr || u->color_ == BLACK) &&
        p && p->right_ == node &&
        g && g->left_ == p) {
      rotate_left(p);
      node = node->left_;
    } else if ((u == nullptr || u->color_ == BLACK) &&
        p && p->left_ == node &&
        g && g->right_ == p) {
      rotate_right(p);
      node = node->right_;
    }

    p = parent(node);
    g = grandparent(node);
    if (node == p->left_)
      rotate_right(g);
    else
      rotate_left(g);
    p->color_ = BLACK;
    g->color_ = RED;
  }

  int verify_color(__TreeNode *root) {
    if (root) {
      if (root->color_ == RED) {
        if (root->left_ && root->left_->color_ == RED ||
            root->right_ && root->right_->color_ == RED)
          return -1;
      }
    } else {
      return 1;
    }
    int left_black_count = verify_color(root->left_);
    int right_black_count = verify_color(root->right_);
    if (left_black_count != right_black_count ||
        right_black_count == -1) {
      return -1;
    } else {
      return left_black_count + (root->color_ == BLACK ? 1 : 0);
    }
  }

  __TreeNode* search(__TreeNode *root, const KeyValueType &key) {
    if (root) {
      if(root->data_.first == key)
        return root;
      else if (root->data_.first > key){
        search(root->left_, key);
      } else {
        search(root->right_, key);
      }
    } else
      return nullptr;
  }

  __TreeNode* min(__TreeNode *root) {
    if (root) {
      while (root->left_) {
        root = root->left_;
      }
    }
    return root;
  }

  __TreeNode* max(__TreeNode *root) {
    if (root) {
      while (root->right_) {
        root = root->right_;
      }
    }
    return root;
  }

  void replace_node(__TreeNode *node, __TreeNode *child) {
    if (node == nullptr)
      return;
    if (child != nullptr) {
      child->parent_ = parent(node);
    }
    if (parent(node) == nullptr) {
      header_.parent_ = child;
      return;
    }
    if (node == parent(node)->left_) {
      node->parent_->left_ = child;
    } else {
      node->parent_->right_ = child;
    }
  }

  void delete_node_with_one_child(__TreeNode *node) {
    if (node == nullptr)
      assert(false);
    __TreeNode *child = node->right_ ? node->right_ : node->left_;
    __TreeNode *p = parent(node);

    replace_node(node, child);
    if (node->color_ == BLACK) {
      if (child && child->color_ == RED) {
        child->color_ = BLACK;
      } else {
        // complex case for delete node
        delete_node_complex1(p, child);
      }
    }
    delete node;
  }

  // we do nothing if node is new root
  void delete_node_complex1(__TreeNode *p, __TreeNode *node) {
    if (p != nullptr) {
      delete_node_complex2(p, node);
    }
  }

  // node is parent of to_be_deleted node
  void delete_node_complex2(__TreeNode *p, __TreeNode *node) {
    __TreeNode *s = p->left_ != node ? p->left_ : p->right_;
    if (s->color_ == RED) {
      p->color_ = RED;
      s->color_ = BLACK;
      if (node == p->left_) {
        rotate_left(p);
      } else {
        rotate_right(p);
      }
    }
    delete_node_complex3(p, node);
  }

  // node is parent of to_be_deleted node
  void delete_node_complex3(__TreeNode *p, __TreeNode *node) {
    __TreeNode *s = p->left_ != node ? p->left_ : p->right_;
    if (p->color_ == BLACK &&
        s->color_ == BLACK &&
        (s->left_ == nullptr || s->left_->color_ == BLACK) &&
        (s->right_ == nullptr || s->right_->color_ == BLACK)) {
      s->color_ = RED;
      delete_node_complex1(parent(p), p);
    } else
      delete_node_complex4(p, node);
  }

  // node is parent of to_be_deleted node
  void delete_node_complex4(__TreeNode *p, __TreeNode *node) {
    __TreeNode *s = p->left_ != node ? p->left_ : p->right_;
    if (p->color_ == RED &&
        s->color_ == BLACK &&
        (s->left_ == nullptr || s->left_->color_ == BLACK) &&
        (s->right_ == nullptr || s->right_->color_ == BLACK)) {
      s->color_ = RED;
      p->color_ = BLACK;
    } else
      delete_node_complex5(p, node);
  }

  // node is parent of to_be_deleted node
  void delete_node_complex5(__TreeNode *p, __TreeNode *node) {
    __TreeNode *s = p->left_ != node ? p->left_ : p->right_;
    if (s->color_ == BLACK) {
      if (node == p->left_ &&
          (s->right_ == nullptr || s->right_->color_ == BLACK) &&
          (s->left_ != nullptr && s->left_->color_ == RED)) {
        s->color_ = RED;
        s->left_->color_ = BLACK;
        rotate_right(s);
      } else if (node == p->right_ &&
                 (s->left_ == nullptr || s->left_->color_ == BLACK) &&
                 (s->right_ != nullptr && s->right_->color_ == RED)) {
        s->color_ = RED;
        s->right_->color_ = BLACK;
        rotate_left(s);
      }
    }
    delete_node_complex6(p, node);
  }

  void delete_node_complex6(__TreeNode *p, __TreeNode *node) {
    __TreeNode *s = p->left_ != node ? p->left_ : p->right_;
    s->color_ = p->color_;
    p->color_ = BLACK;

    if (node == p->left_) {
      s->right_->color_ = BLACK;
      rotate_left(p);
    } else {
      s->left_->color_ = BLACK;
      rotate_right(p);
    }
  }
};

#endif //_RED_BLACK_TREE_H_
