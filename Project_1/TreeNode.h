/*
 * Class used to define a single node within
 * a search tree.
 */
#ifndef PROJECT_1_TREENODE_H
#define PROJECT_1_TREENODE_H

class TreeNode {
public:
    TreeNode(int state, int path_cost, TreeNode *parent);

    // Getter functions
    [[nodiscard]] int get_state() const;
    [[nodiscard]] int get_depth() const;
    [[nodiscard]] int get_path_cost() const;
    [[nodiscard]] TreeNode *get_parent();

private:
    int _state; // Integer representing state of node
    int _depth; // Depth of node in tree
    int _path_cost; // Cost to get to node from origin
    TreeNode *_parent; // Parent of node
};


#endif //PROJECT_1_TREENODE_H
