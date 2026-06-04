#ifndef THREADBINARY_H
#define THREADBINARY_H

/*
threadbinary.h - Threaded Binary Tree (Double-Threaded BST)
Bahasa C

Threaded Binary Tree adalah variasi binary tree di mana
pointer NULL digantikan dengan thread yang menunjuk
ke inorder predecessor (left thread) atau inorder
successor (right thread), sehingga inorder traversal
dapat dilakukan secara iteratif tanpa stack/rekursi.

Tipe: Double-Threaded Binary Search Tree
- leftThread == 1  -> left  menunjuk ke inorder predecessor
- leftThread == 0  -> left  menunjuk ke child kiri
- rightThread == 1 -> right menunjuk ke inorder successor
- rightThread == 0 -> right menunjuk ke child kanan
 */

typedef struct ThreadNode {
  int data;
  struct ThreadNode *left;
  struct ThreadNode *right;
  int leftThread;  /* 1: left = thread ke inorder predecessor   */
  int rightThread; /* 1: right = thread ke inorder successor    */
} ThreadNode;

typedef struct {
  ThreadNode *root;
  ThreadNode *header;
} ThreadedBinaryTree;

/* Inisialisasi tree kosong */
void tree_init(ThreadedBinaryTree *tree);

/* Dealokasi seluruh node */
void tree_clear(ThreadedBinaryTree *tree);

/* Cek apakah tree kosong. Return 1 jika kosong, 0 jika tidak */
int tree_isEmpty(const ThreadedBinaryTree *tree);

/* Insert nilai ke dalam threaded BST */
void tree_insert(ThreadedBinaryTree *tree, int value);

/* Cari nilai dalam tree. Return 1 jika ditemukan, 0 jika tidak */
int tree_search(const ThreadedBinaryTree *tree, int value);

/* Hapus nilai dari tree */
void tree_remove(ThreadedBinaryTree *tree, int value);

/* Traversal inorder (cetak ke layar) */
void tree_inorder(const ThreadedBinaryTree *tree);

/* Traversal preorder (cetak ke layar) */
void tree_preorder(const ThreadedBinaryTree *tree);

/* Tampilkan struktur tree secara detail */
void tree_display(const ThreadedBinaryTree *tree);

#endif
