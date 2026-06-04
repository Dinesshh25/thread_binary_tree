#include "threadbinary.h"

void tree_init(ThreadedBinaryTree *tree){
    tree->header = (ThreadNode*)malloc(sizeof(ThreadNode));
    tree->header->left = NULL;
    tree->header->right = tree->header;
    tree->header->leftThread = 0;
    tree->header->rightThread = 1;
    tree->root = NULL;
}

void tree_clear(ThreadedBinaryTree *tree){
    while (!tree_isEmpty(tree)) {
        tree_remove(tree, tree->root->data);
    }
}

boolean tree_isEmpty(const ThreadedBinaryTree *tree){
    return tree->root == NULL;
}

boolean tree_search(const ThreadedBinaryTree *tree, int value){
    ThreadNode *current = tree->root;

    while (current != NULL) {
        if (value == current->data) {
            return true;
        } else if (value < current->data) {
            if (current->leftThread == 0) {
                current = current->left;
            } else {
                break;
            }
        } else {
            if (current->rightThread == 0) {
                current = current->right;
            } else {
                break;
            }
        }
    }
    return false;
}

void tree_insert(ThreadedBinaryTree *tree, int value){
    ThreadNode *newNode = (ThreadNode*)malloc(sizeof(ThreadNode));
    newNode->data = value;
    newNode->leftThread = 1;
    newNode->rightThread = 1;

    if (tree->root == NULL) {
        tree->root = newNode;
        tree->header->left = newNode;
        newNode->left = tree->header;
        newNode->right = tree->header;
    } else {
        ThreadNode *current = tree->root;
        ThreadNode *parent = NULL;

        while (current != NULL) {
            parent = current;
            if (value < current->data) {
                if (current->leftThread == 0) {
                    current = current->left;
                } else {
                    break;
                }
            } else {
                if (current->rightThread == 0) {
                    current = current->right;
                } else {
                    break;
                }
            }
        }

        if (value < parent->data) {
            parent->leftThread = 0;
            parent->left = newNode;
            newNode->left = parent;
            newNode->right = parent->right;
            parent->rightThread = 1;
            parent->right = newNode;
        } else {
            parent->rightThread = 0;
            parent->right = newNode;
            newNode->right = parent;
            newNode->left = parent->left;
            parent->leftThread = 1;
            parent->left = newNode;
        }
    }
}

void tree_remove(ThreadedBinaryTree *tree, int value){
    if (tree_isEmpty(tree)) return;

    ThreadNode *current = tree->root;
    ThreadNode *parent = tree->header;

    while (current != NULL && current->data != value) {
        if (value < current->data) {
            if (current->leftThread == 1) return;
            parent = current;
            current = current->left;
        } else {
            if (current->rightThread == 1) return;
            parent = current;
            current = current->right;
        }
    }

    if (current == NULL) return;

    // Kasus 1: Node memiliki dua child
    if (current->leftThread == 0 && current->rightThread == 0) {
        // Cari predecessor (paling kanan dari subtree kiri)
        ThreadNode *pred = current->left;
        ThreadNode *predParent = current;
        while (pred->rightThread == 0) {
            predParent = pred;
            pred = pred->right;
        }
        current->data = pred->data;
        current = pred;
        parent = predParent;
    }

    // Kasus 2 & 3: Node memiliki 0 atau 1 child
    ThreadNode *child = (current->leftThread == 1) ? current->right : current->left;

    if (current == parent->left) {
        parent->leftThread = 1;
        parent->left = child;
    } else if (current == parent->right) {
        parent->rightThread = 1;
        parent->right = child;
    } else if (current == tree->root) {
        tree->root = child;
        if (child != NULL) {
            tree->header->left = child;
        } else {
            tree->header->left = NULL;
        }
    }

    free(current);
}

void tree_inorder(const ThreadedBinaryTree *tree){
    if (tree_isEmpty(tree)) return;

    ThreadNode *current = tree->root;

    // Cari node paling kiri (first in inorder)
    while (current->leftThread == 0) {
        current = current->left;
    }

    // Traversing menggunakan thread
    while (current != tree->header) {
        printf("%d ", current->data);

        if (current->rightThread == 1) {
            current = current->right;
        } else {
            current = current->right;
            while (current->leftThread == 0) {
                current = current->left;
            }
        }
    }
}

void tree_preorder(const ThreadedBinaryTree *tree){
    if (tree_isEmpty(tree)) return;

    ThreadNode *current = tree->root;

    while (current != tree->header) {
        printf("%d ", current->data);

        if (current->leftThread == 0) {
            current = current->left;
        } else {
            while (current->rightThread == 1 && current->right != tree->header) {
                current = current->right;
            }
            if (current->right != tree->header) {
                current = current->right;
            } else {
                break;
            }
        }
    }
}

void tree_display(const ThreadedBinaryTree *tree){
    if (tree_isEmpty(tree)) {
        printf("Tree kosong.\n");
        return;
    }

    printf("Struktur Threaded Binary Tree:\n");
    printf("Header -> left: %s, right: %s\n",
           tree->header->left ? "node" : "NULL",
           tree->header->right == tree->header ? "header" : "node");

    ThreadNode *current = tree->root;

    // Cari node paling kiri
    while (current->leftThread == 0) {
        current = current->left;
    }

    // Traversing semua node
    while (current != tree->header) {
        printf("Node: %d | Left: %s | Right: %s\n",
               current->data,
               current->leftThread == 1 ? "thread" : "child",
               current->rightThread == 1 ? "thread" : "child");

        if (current->rightThread == 1) {
            current = current->right;
        } else {
            current = current->right;
            while (current->leftThread == 0) {
                current = current->left;
            }
        }
    }
}