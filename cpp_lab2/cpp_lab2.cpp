#include <iostream>
#include <vector>
#include <algorithm>
#include <list>


template <typename T>
class BinaryTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        int size = 1;

        Node(T value) : data(value), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void insert(Node*& node, T value) {
        if (node == nullptr) {
            node = new Node(value);
        }
        else if (value < node->data) {
            insert(node->left, value);
            node->size++;
        }
        else {
            insert(node->right, value);
            node->size++;
        }
    }

    bool search(Node* node, T value) {
        if (node == nullptr) {
            return false;
        }
        if (node->data == value) {
            return true;
        }
        else if (value < node->data) {
            return search(node->left, value);
        }
        else {
            return search(node->right, value);
        }
    }

    void inOrder(Node* node) {
        if (node != nullptr) {
            inOrder(node->left);
            std::cout << node->data << " ";
            inOrder(node->right);
        }
    }

    void clear(Node* node) {
        if (node != nullptr) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    Node* copy(Node* node) {
        if (node == nullptr) {
            return nullptr;
        }
        Node* newNode = new Node(node->data);
        newNode->left = copy(node->left);
        newNode->right = copy(node->right);
        return newNode;
    }

public:
    BinaryTree() : root(nullptr) {}

    bool operator<(const BinaryTree<T>& another) const {
        return (this->root ? this->root->size : 0) < (another.root ? another.root->size : 0);
    }

    bool operator>(const BinaryTree<T>& another) const {
        return (this->root ? this->root->size : 0) > (another.root ? another.root->size : 0);
    }

    bool operator==(const BinaryTree<T>& another) const {
        return (this->root ? this->root->size : 0) == (another.root ? another.root->size : 0);
    }


    BinaryTree(const BinaryTree& other) {
        std::cout << "Copy Constructor" << std::endl;
        root = copy(other.root);
    }

    BinaryTree(BinaryTree&& other) noexcept : root(other.root) {
        std::cout << "Move Constructor" << std::endl;
        other.root = nullptr; 
    }

    BinaryTree& operator=(const BinaryTree& other) {
        std::cout << "Assignment Operator" << std::endl;
        if (this != &other) {
            clear(root);
            root = copy(other.root);
        }
        return *this;
    }

    BinaryTree& operator=(BinaryTree&& other) noexcept {
        std::cout << "Move Assignment Operator" << std::endl;
        if (this != &other) {
            clear(root); 
            root = other.root; 
            other.root = nullptr; 
        }
        return *this;
    }

    void insert(T value) {
        std::cout << "insert" << std::endl;
        insert(root, value);
    }

    bool search(T value) {
        std::cout << "search" << std::endl;
        return search(root, value);
    }

    void inOrder() {
        std::cout << "inOrder" << std::endl;
        inOrder(root);
        std::cout << std::endl; 
    }

    friend std::ostream& operator<<(std::ostream& os, const BinaryTree& tree) {
        tree.print(os, tree.root);
        return os;
    }

    void print(std::ostream& os, Node* node) const {
        if (node != nullptr) {
            print(os, node->left);
            os << node->data << " ";
            print(os, node->right);
        }
    }

    ~BinaryTree() {
        std::cout << "Destructor" << std::endl;
        clear(root);
    }
};

int main() {
    BinaryTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(2);
    tree.insert(4);

    std::cout << "In-order traversal: ";
    tree.inOrder();

    int valueToSearch = 4;
    if (tree.search(valueToSearch)) {
        std::cout << valueToSearch << " found in the tree." << std::endl;
    }
    else {
        std::cout << valueToSearch << " not found in the tree." << std::endl;
    }

    BinaryTree<int> treeCopy = tree;
    std::cout << "Copied tree in-order traversal: " << treeCopy << std::endl;

    BinaryTree<int> anotherTree;
    anotherTree.insert(10);
    anotherTree = tree; 
    std::cout << "Assigned tree in-order traversal: " << anotherTree << std::endl;

    tree.insert(15);

    BinaryTree<int> treeMove = std::move(BinaryTree<int>(tree));
    std::cout << "Move tree in-order traversal: " << treeMove << std::endl;

    tree.insert(45);

    BinaryTree<int> treeMoveAs;
    treeMoveAs = std::move(BinaryTree<int>(tree));
    std::cout << "Move tree with operator in-order traversal: " << treeMoveAs << std::endl;

    return 0;
}