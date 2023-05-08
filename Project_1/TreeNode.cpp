/*
 * Source file for the TreeNode class
 */

#include "TreeNode.h"

TreeNode::TreeNode(int state, int path_cost, TreeNode *parent) {
    this->_state = state;
    if (parent != nullptr)
        this->_depth = parent->get_depth() + 1;
    else
        this->_depth = 0;
    this->_path_cost = path_cost;
    this->_parent = parent;
}

int TreeNode::get_state() const {
    return this->_state;
}

int TreeNode::get_depth() const {
    return this->_depth;
}

int TreeNode::get_path_cost() const {
    return this->_path_cost;
}

TreeNode *TreeNode::get_parent() {
    return this->_parent;
}
