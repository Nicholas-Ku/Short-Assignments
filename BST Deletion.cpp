#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class binary_search_tree
{
public:
    binary_search_tree() {
        size_ = 0;
        root_ = nullptr;
    }

    binary_search_tree(vector<int> input) {
        size_ = 0;
        root_ = nullptr;

        for (int i = 0; i < input.size(); i++) {
            insert(input[i]);
        }
    }

    ~binary_search_tree()
    {
        post_order_recursive_clear(root_);
    }

    void insert(int value) {
        if (root_ == nullptr) {
            root_ = new Node(value);
            size_++;
            return;
        }
        Node* current = root_;
        for (;;) {
            // Let's not deal with duplicates for our simple BST
            if (value < current->node_value_) {
                if (current->left_ != nullptr) {
                    current = current->left_;
                }
                else {
                    current->left_ = new Node(value);
                    size_++;
                    return;
                }
            }
            else if (value > current->node_value_) {
                if (current->right_ != nullptr) {
                    current = current->right_;
                }
                else {
                    current->right_ = new Node(value);
                    size_++;
                    return;
                }
            }
        }
    }

    void level_order_print() {
        if (root_ == nullptr) return;
        
        Node* traversal_pointer = root_;
        queue<Node*> queue_of_node_pointers;
        queue_of_node_pointers.push(traversal_pointer);

        while (!queue_of_node_pointers.empty()) {
            int int_of_node = (*queue_of_node_pointers.front()).node_value_;
            cout << int_of_node << " ";
            traversal_pointer = queue_of_node_pointers.front();
            queue_of_node_pointers.pop();
            if (traversal_pointer->left_ != nullptr) queue_of_node_pointers.push(traversal_pointer->left_);
            if (traversal_pointer->right_ != nullptr) queue_of_node_pointers.push(traversal_pointer->right_);
        }
    }

    void public_delete(int value) {
        private_delete(value);
    }


private:
    struct Node {
        Node(int value) {
            node_value_ = value;
            right_ = left_ = nullptr;
        }

        Node(int value, Node* left_child, Node* right_child) {
            node_value_ = value;
            left_ = left_child;
            right_ = right_child;
        }

        int node_value_;
        Node* left_;
        Node* right_;
    };

    Node* root_;
    int size_;
    void post_order_recursive_clear(Node* root_) {
        if (root_ == nullptr) return;
        post_order_recursive_clear(root_->left_);
        post_order_recursive_clear(root_->right_);
        delete root_;
        size_--;
    }

    void private_delete(int value) {
        // Step 1: Searching for the value
        Node* parent = nullptr;
        Node* current = root_;

        while (current != nullptr) {
            if (value == current->node_value_) break;
            parent = current;
            if (value < current->node_value_) {
                current = current->left_;
            }
            else if (value > current->node_value_) {
                current = current->right_;
            }
        }

        if (current == nullptr) return; // Didn't find the node to delete

        // Step 2: Consider 3 different cases
        // 
        // Step 2, Case 1: Target Node is a Leaf
        if (current->left_ == nullptr && current->right_ == nullptr) {
            // Step 2, Case 1, Part A) Target Node is a Leaf and NOT the root
            if (parent != nullptr) {
                // We need to sever either the left or right parent link to child
                if (parent->left_ == current) parent->left_ = nullptr;
                else parent->right_ = nullptr;
            }
            // Step 2, Case 1, Part B) Target Node is a Leaf AND the Root
            else {
                root_ = nullptr;
            }
            delete current;
            size_--;
            return;
        }

        // Step 2, Case 2: Target has One Child
        // If it passed the first if statement, that means that it must have at least one child
        // Let's filter out the case where they have two children
        else if (current->left_ == nullptr || current->right_ == nullptr) {
            // We need to check if it has a left or right child
            // If it doesn't have a left child, then it must be the right child and vice versa
            Node* successor;
            if (current->left_ == nullptr) successor = current->right_;
            else successor = current->left_;

            // Step 2, Case 2, Part A) Target has One Child and NOT the root
            if (parent != nullptr) {
                // We need to check again if this was the parent's left or right child
                if (parent->left_ == current) {
                    parent->left_ = successor;
                }
                else {
                    parent->right_ = successor;
                }
            }

            // Step 2, Case 2, Part B) Target has One Chld and IS the root
            else {
                root_ = successor;
            }
            delete current;
            size_--;
            return;
        }

        // Step 2, Case 3: Our Target has two children
        else {
            // Either replace the node with one of the two following:
            // 1) Current node's left subtree's largest valued node
            // 2) Current node's right subtree's smallest valued node
            // Let's use the approach where we find the current nodes left subtree's largest-valued node
            // We can find this node by first going left and then going right until we hit a nullptr
            Node* successor = current->left_;
            Node* successor_parent = current;
            while (successor->right_ != nullptr) {
                successor_parent = successor;
                successor = successor->right_;
            }

            // The successor will either have 0 children or 1 left child
            // It cannot have a right child because we kept traversing to the right as much as possible
            // First we replace the current node's value with the successor's node value
            // Then delete the succcessor in a similar way as case 1 and case 2 from before
            // Notably we don't need to check if the deleted node is a root node though
            current->node_value_ = successor->node_value_;

            // Step 2, Case 3, Part A: Successor has 0 Children
            if (successor->left_ == nullptr && successor->right_ == nullptr) {
                if (successor_parent->left_ == successor) successor_parent->left_ = nullptr;
                else successor_parent->right_ = nullptr;
            }

            // Step 2, Case 3, Part B: Sucessor has 1 Child
            // From the way we found our successor, the child must be from the left of the successor
            else {
                Node* successor_left_child = successor->left_;

                // Need to determine if successor came from left or right parent
                if (successor_parent->left_ == successor) {
                    successor_parent->left_ = successor_left_child;
                }
                else {
                    successor_parent->right_ = successor_left_child;
                }
            }

            delete successor;
            size_--;
            return;
        }

    }
};

int main()
{
    vector<int> tree_to_insert = { 25, 10, 30, 5, 20, 35, 15 };
    binary_search_tree test_1(tree_to_insert);
    test_1.level_order_print();
    cout << endl << endl;

    cout << "Testing Case 1, Part A : Deleting a leaf that is not the root" << endl;
    binary_search_tree test_2(tree_to_insert);
    test_2.public_delete(15);
    test_2.level_order_print();
    cout << endl << endl;

    cout << "Testing Case 1, Part B : Deleting a leaf that is the root" << endl;
    vector<int> one_root = { 3 };
    binary_search_tree test_3(one_root);
    test_3.public_delete(3);
    test_3.level_order_print();
    cout << endl << endl;

    cout << "Testing Case 2, Part A: Deleting a Node with one child and Node is not the root." << endl;
    vector<int> right_branch_tree = { 5, 10 };
    binary_search_tree test_4(right_branch_tree);
    test_4.public_delete(10);
    test_4.level_order_print();
    cout << endl;
    vector<int> left_branch_tree = { 8, 3 };
    binary_search_tree test_5(left_branch_tree);
    test_5.public_delete(3);
    test_5.level_order_print();
    cout << endl << endl;

    cout << "Testing Case 2, Part B: Deleting a node with one child and Node is the root. " << endl;
    binary_search_tree test_6(right_branch_tree);
    test_6.public_delete(5);
    test_6.level_order_print();
    cout << endl;
    binary_search_tree test_7(left_branch_tree);
    test_7.public_delete(8);
    test_7.level_order_print();
    cout << endl << endl;

    cout << "Testing Case 3, Part A: Sucessor has no children." << endl;
    binary_search_tree test_8(tree_to_insert);
    test_8.public_delete(10);
    test_8.level_order_print();
    cout << endl;

    cout << "Testing Case 3, Part B: Sucessor has one child." << endl;
    binary_search_tree test_9(tree_to_insert);
    // Root(25) should be replaced by 20
    // 20 has one left child that is 15, it should now be attached to 10
    test_9.public_delete(25);
    test_9.level_order_print();
    cout << endl << endl;
}
