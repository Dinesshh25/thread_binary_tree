#include "threadbinary.h"


void tree_init(ThreadedBinaryTree *tree){
    tree->header = (ThreadNode*)malloc(sizeof(ThreadNode));
    tree->header->left = NULL;  // Akan di-set ke root saat insert pertama
    tree->header->right = tree->header; // Right thread ke dirinya sendiri
    tree->header->leftThread = 0; // Belum ada root, jadi bukan thread
    tree->header->rightThread = 1; // Right thread ke dirinya sendiri
    tree->root = NULL;
}

void tree_clear(ThreadedBinaryTree *tree){
    while (!tree_isEmpty(tree)) {
        tree_remove(tree, tree->root->data); // Hapus node root secara berulang
    }
}

/* Insert nilai ke dalam threaded BST */
void tree_insert(ThreadedBinaryTree *tree, int value){
    ThreadNode *newNode = (ThreadNode*)malloc(sizeof(ThreadNode));
    newNode->data = value;
    newNode->leftThread = 1; // Awalnya thread
    newNode->rightThread = 1; // Awalnya thread

    if (tree->root == NULL) {
        // Insert pertama kali, jadi set root dan header
        tree->root = newNode;
        tree->header->left = newNode; // Header left ke root
        newNode->left = tree->header; // Thread ke header
        newNode->right = tree->header; // Thread ke header
    } else {
        ThreadNode *current = tree->root;
        ThreadNode *parent = NULL;

        while (current != NULL) {
            parent = current;
            if (value < current->data) {
                if (current->leftThread == 0) {
                    current = current->left; // Lanjut ke child kiri
                } else {
                    break; // Ketemu posisi insert
                }
            } else {
                if (current->rightThread == 0) {
                    current = current->right; // Lanjut ke child kanan
                } else {
                    break; // Ketemu posisi insert
                }
            }
        }

        if (value < parent->data) {
            parent->leftThread = 0; // Bukan thread lagi
            parent->left = newNode; // Set child kiri ke node baru
            newNode->left = parent; // Thread ke parent sebagai predecessor
            newNode->right = parent->right; // Thread ke successor yang lama
            parent->rightThread = 1; // Parent sekarang punya thread kanan baru
            parent->right = newNode; // Set thread kanan ke node baru
        } else {
            parent->rightThread = 0; // Bukan thread lagi
            parent->right = newNode; // Set child kanan ke node baru
            newNode->right = parent; // Thread ke parent sebagai successor
            newNode->left = parent->left; // Thread ke predecessor yang lama
            parent->leftThread = 1; // Parent sekarang punya thread kiri baru
            parent->left = newNode; // Set thread kiri ke node baru
        }
    }
}

boolean tree_search(const ThreadedBinaryTree *tree, int value){
    ThreadNode *current = tree->root;

    while (current != NULL) {
        if (value == current->data) {
            return true; // Ditemukan
        } else if (value < current->data) {
            if (current->leftThread == 0) {
                current = current->left; // Lanjut ke child kiri
            } else {
                break; // Tidak ditemukan
            }
        } else {
            if (current->rightThread == 0) {
                current = current->right; // Lanjut ke child kanan
            } else {
                break; // Tidak ditemukan
            }
        }
    }
    return false; // Tidak ditemukan
}

boolean tree_isEmpty(const ThreadedBinaryTree *tree){
    return tree->root == NULL;
}

void tree_inorder(const ThreadedBinaryTree *tree){
    ThreadNode *current = tree->header->left; // Mulai dari root

}

void tree_remove(ThreadedBinaryTree *tree, int value){
    ThreadNode *current = tree->root;
    ThreadNode *parent = NULL;
    
    while (tree_search(tree, value)) {
        parent = current;
        if (value < current->data) {
            if (current->leftThread == 0) {
                current = current->left; // Lanjut ke child kiri
            } else {
                return; // Tidak ditemukan
            }
        } else {
            if (current->rightThread == 0) {
                current = current->right; // Lanjut ke child kanan
            } else {
                return; // Tidak ditemukan
            }
        }
    }
}

void tree_preorder(const ThreadedBinaryTree *tree){
    ThreadNode *current = tree->header->left; // Mulai dari root

        while (current != tree->header) {
            printf("%d ", current->data); // Cetak data
    
            if (current->leftThread == 0) {
                current = current->left; // Lanjut ke child kiri
            } else if (current->rightThread == 0) {
                current = current->right; // Lanjut ke child kanan
            } else {
                // Kembali ke parent melalui thread
                while (current != tree->header && current->rightThread == 1) {
                    current = current->right; // Lanjut ke successor melalui thread
                }
                if (current != tree->header) {
                    current = current->right; // Lanjut ke child kanan setelah thread
                }
            }
        }
}

void tree_display(const ThreadedBinaryTree *tree){
    ThreadNode *current = tree->header->left; // Mulai dari root

    while (current != tree->header) {
        printf("Node: %d, LeftThread: %d, RightThread: %d\n", current->data, current->leftThread, current->rightThread);
        
        if (current->leftThread == 0) {
            current = current->left; // Lanjut ke child kiri
        } else if (current->rightThread == 0) {
            current = current->right; // Lanjut ke child kanan
        } else {
            // Kembali ke parent melalui thread
            while (current != tree->header && current->rightThread == 1) {
                current = current->right; // Lanjut ke successor melalui thread
            }
            if (current != tree->header) {
                current = current->right; // Lanjut ke child kanan setelah thread
            }
        }
    }

}