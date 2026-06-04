#include "threadbinary.h"

/* Helper: find inorder predecessor of a given node (used internally) */
static ThreadNode* inorder_predecessor(ThreadNode *node) {
    if (node->leftThread == 1)
        return node->left;
    ThreadNode *curr = node->left;
    while (curr->rightThread == 0)
        curr = curr->right;
    return curr;
}

/* Helper: find inorder successor of a given node (used internally) */
static ThreadNode* inorder_successor(ThreadNode *node) {
    if (node->rightThread == 1)
        return node->right;
    ThreadNode *curr = node->right;
    while (curr->leftThread == 0)
        curr = curr->left;
    return curr;
}

void tree_init(ThreadedBinaryTree *tree) {
    tree->header = (ThreadNode*)malloc(sizeof(ThreadNode));
    tree->header->left = tree->header;   /* left thread to itself (empty) */
    tree->header->right = tree->header;  /* right thread to itself */
    tree->header->leftThread = 1;        /* left is a thread */
    tree->header->rightThread = 1;       /* right is a thread */
    tree->root = NULL;
}

/* Recursive postorder deletion helper (does not free header) */
static void delete_subtree(ThreadNode *node) {
    if (node == NULL) return;
    if (node->leftThread == 0) delete_subtree(node->left);
    if (node->rightThread == 0) delete_subtree(node->right);
    free(node);
}
void tree_clear(ThreadedBinaryTree *tree) {
    if (tree->root != NULL) {
        delete_subtree(tree->root);
        tree->root = NULL;
    }
    /* Reset header to empty state */
    tree->header->left = tree->header;
    tree->header->leftThread = 1;
    tree->header->right = tree->header;
    tree->header->rightThread = 1;
}

boolean tree_isEmpty(const ThreadedBinaryTree *tree) {
    return tree->root == NULL;
}

boolean tree_search(const ThreadedBinaryTree *tree, int value) {
    ThreadNode *curr = tree->root;
    while (curr != NULL) {
        if (value == curr->data)
            return true;
        else if (value < curr->data) {
            if (curr->leftThread == 0)
                curr = curr->left;
            else
                break;
        } else {
            if (curr->rightThread == 0)
                curr = curr->right;
            else
                break;
        }
    }
    return false;
}

void tree_insert(ThreadedBinaryTree *tree, int value) {
    ThreadNode *newNode = (ThreadNode*)malloc(sizeof(ThreadNode));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    newNode->data = value;
    newNode->leftThread = 1;
    newNode->rightThread = 1;

    if (tree->root == NULL) {
        /* Empty tree: attach as root */
        tree->root = newNode;
        newNode->left = tree->header;
        newNode->right = tree->header;
        tree->header->left = newNode;
        tree->header->leftThread = 0;  /* header's left now points to root (child) */
        return;
    }

    /* Find insertion point */
    ThreadNode *curr = tree->root;
    ThreadNode *parent = NULL;
    int direction = 0; /* 0 = left, 1 = right */

    while (1) {
        if (value < curr->data) {
            if (curr->leftThread == 0) {
                parent = curr;
                curr = curr->left;
                direction = 0;
            } else {
                parent = curr;
                direction = 0;
                break;
            }
        } else if (value > curr->data) {
            if (curr->rightThread == 0) {
                parent = curr;
                curr = curr->right;
                direction = 1;
            } else {
                parent = curr;
                direction = 1;
                break;
            }
        } else {
            /* Duplicate value: free newNode and return (or handle as you wish) */
            free(newNode);
            return;
        }
    }

    if (direction == 0) {  /* Insert as left child */
        newNode->left = parent->left;     /* thread to inorder predecessor */
        newNode->right = parent;          /* thread to parent (successor) */
        parent->left = newNode;
        parent->leftThread = 0;

        /* Update the predecessor's right thread to point to newNode */
        ThreadNode *pred = newNode->left;
        pred->right = newNode;
        pred->rightThread = 1;  /* still a thread, now points to newNode */
    } else {  /* Insert as right child */
        newNode->right = parent->right;   /* thread to inorder successor */
        newNode->left = parent;           /* thread to parent (predecessor) */
        parent->right = newNode;
        parent->rightThread = 0;

        /* Update the successor's left thread to point to newNode */
        ThreadNode *succ = newNode->right;
        succ->left = newNode;
        succ->leftThread = 1;
    }
}

void tree_remove(ThreadedBinaryTree *tree, int value) {
    if (tree_isEmpty(tree)) return;

    /* Find node to delete and its parent */
    ThreadNode *curr = tree->root;
    ThreadNode *parent = tree->header;
    int direction = 0;  /* 0 = left, 1 = right (relative to parent) */

    while (curr != NULL && curr->data != value) {
        parent = curr;
        if (value < curr->data) {
            if (curr->leftThread == 0) {
                curr = curr->left;
                direction = 0;
            } else {
                return;  /* not found */
            }
        } else {
            if (curr->rightThread == 0) {
                curr = curr->right;
                direction = 1;
            } else {
                return;
            }
        }
    }
    if (curr == NULL) return;

    /* Case 1: Node has two children */
    if (curr->leftThread == 0 && curr->rightThread == 0) {
        /* Find predecessor (largest in left subtree) */
        ThreadNode *pred = curr->left;
        ThreadNode *predParent = curr;
        while (pred->rightThread == 0) {
            predParent = pred;
            pred = pred->right;
        }
        /* Replace data */
        curr->data = pred->data;
        /* Now delete the predecessor (which has at most one child) */
        curr = pred;
        parent = predParent;
        /* Determine direction of curr relative to parent */
        if (parent->left == curr)
            direction = 0;
        else
            direction = 1;
        /* Fall through to single/zero child case */
    }

    /* Case 2 & 3: Node has 0 or 1 child */
    ThreadNode *child = NULL;
    if (curr->leftThread == 0 && curr->rightThread == 1) {
        /* Only left child */
        child = curr->left;
    } else if (curr->leftThread == 1 && curr->rightThread == 0) {
        /* Only right child */
        child = curr->right;
    } else {
        /* No children */
        child = NULL;
    }

    /* Link parent to child (if any) and adjust threads */
    if (parent == tree->header) {
        /* Deleting root */
        tree->root = child;
        if (child != NULL) {
            /* Update header's left pointer */
            tree->header->left = child;
            tree->header->leftThread = 0;
            /* Find leftmost node and set its left thread to header */
            ThreadNode *leftmost = child;
            while (leftmost->leftThread == 0)
                leftmost = leftmost->left;
            leftmost->left = tree->header;
            leftmost->leftThread = 1;
            /* Find rightmost node and set its right thread to header */
            ThreadNode *rightmost = child;
            while (rightmost->rightThread == 0)
                rightmost = rightmost->right;
            rightmost->right = tree->header;
            rightmost->rightThread = 1;
        } else {
            /* Tree becomes empty */
            tree->header->left = tree->header;
            tree->header->leftThread = 1;
        }
    } else {
        /* Not root */
        if (direction == 0) {
            parent->left = child;
            parent->leftThread = (child == NULL) ? 1 : 0;
        } else {
            parent->right = child;
            parent->rightThread = (child == NULL) ? 1 : 0;
        }

        /* If child exists, we must fix its inorder predecessor/successor threads */
        if (child != NULL) {
            /* Find inorder predecessor of child (the node that should have its right thread pointing to child) */
            ThreadNode *pred = inorder_predecessor(child);
            pred->right = child;
            pred->rightThread = 1;

            /* Find inorder successor of child */
            ThreadNode *succ = inorder_successor(child);
            succ->left = child;
            succ->leftThread = 1;
        } else {
            /* No child: the parent's thread pointers are already set,
               but we need to update the neighbour's thread that used to point to curr */
            /* For left removal: the predecessor (which is parent->left before deletion)
               should now point to parent as its successor? Actually, if curr had no child,
               its predecessor's right thread pointed to curr, and successor's left thread pointed to curr.
               We need to redirect those to each other. */
            ThreadNode *pred = inorder_predecessor(curr);
            ThreadNode *succ = inorder_successor(curr);
            if (pred != tree->header) {
                pred->right = succ;
                pred->rightThread = 1;
            }
            if (succ != tree->header) {
                succ->left = pred;
                succ->leftThread = 1;
            }
        }
    }

    free(curr);
}

void tree_inorder(const ThreadedBinaryTree *tree) {
    if (tree_isEmpty(tree)) return;
    ThreadNode *curr = tree->root;
    /* Go to leftmost node */
    while (curr->leftThread == 0)
        curr = curr->left;
    while (curr != tree->header) {
        printf("%d ", curr->data);
        if (curr->rightThread == 1)
            curr = curr->right;
        else {
            curr = curr->right;
            while (curr->leftThread == 0)
                curr = curr->left;
        }
    }
}

void tree_preorder(const ThreadedBinaryTree *tree) {
    if (tree_isEmpty(tree)) return;
    ThreadNode *curr = tree->root;
    while (curr != tree->header) {
        printf("%d ", curr->data);
        if (curr->leftThread == 0) {
            curr = curr->left;
        } else if (curr->rightThread == 0) {
            curr = curr->right;
        } else {
            /* Move up via right threads until we find a node with a right child */
            while (curr->rightThread == 1 && curr->right != tree->header) {
                curr = curr->right;
            }
            if (curr->right != tree->header) {
                curr = curr->right;
            } else {
                break;
            }
        }
    }
}

/* Recursive postorder helper (uses child links only, ignores threads) */
static void postorder_helper(ThreadNode *node) {
    if (node == NULL) return;
    if (node->leftThread == 0) postorder_helper(node->left);
    if (node->rightThread == 0) postorder_helper(node->right);
    printf("%d ", node->data);
}

void tree_postorder(const ThreadedBinaryTree *tree) {
    if (tree_isEmpty(tree)) return;
    postorder_helper(tree->root);
}

void tree_display(const ThreadedBinaryTree *tree) {
    if (tree_isEmpty(tree)) {
        printf("Tree kosong.\n");
        printf("Header: left->%s, right->header (self)\n",
               tree->header->left == tree->header ? "header" : "?");
        return;
    }

    printf("Struktur Threaded Binary Tree:\n");
    printf("Header -> left: %s, right: header (self)\n",
           tree->header->leftThread == 0 ? "root (child)" : "thread");
    printf("Root: %d\n", tree->root->data);

    /* Traverse inorder and show details */
    ThreadNode *curr = tree->root;
    while (curr->leftThread == 0)
        curr = curr->left;
    while (curr != tree->header) {
        printf("Node %d: left -> ", curr->data);
        if (curr->leftThread == 1) {
            if (curr->left == tree->header)
                printf("header");
            else
                printf("%d (thread)", curr->left->data);
        } else {
            printf("%d (child)", curr->left->data);
        }
        printf(", right -> ");
        if (curr->rightThread == 1) {
            if (curr->right == tree->header)
                printf("header");
            else
                printf("%d (thread)", curr->right->data);
        } else {
            printf("%d (child)", curr->right->data);
        }
        printf("\n");

        if (curr->rightThread == 1)
            curr = curr->right;
        else {
            curr = curr->right;
            while (curr->leftThread == 0)
                curr = curr->left;
        }
    }
}
