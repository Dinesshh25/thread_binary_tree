#include "threadbinary.h"

int main(){
    int menu, value;
    ThreadedBinaryTree tree;
    tree_init(&tree);

    do {
        printf("\nMenu:\n");
        printf("1. Insert\n");
        printf("2. Search\n");
        printf("3. Remove\n");
        printf("4. Inorder Traversal\n");
        printf("5. Preorder Traversal\n");
        printf("6. Display Tree\n");
        printf("0. Exit\n");
        printf("Pilih menu: ");
        scanf("%d", &menu);

        switch (menu) {
            case 1:
                printf("Masukkan nilai untuk insert: ");
                scanf("%d", &value);
                tree_insert(&tree, value);
                break;
            case 2:
                printf("Masukkan nilai untuk search: ");
                scanf("%d", &value);
                if (tree_search(&tree, value)) {
                    printf("Nilai %d ditemukan dalam tree.\n", value);
                } else {
                    printf("Nilai %d tidak ditemukan dalam tree.\n", value);
                }
                break;
            case 3:
                printf("Masukkan nilai untuk remove: ");
                scanf("%d", &value);
                tree_remove(&tree, value);
                break;
            case 4:
                printf("Inorder Traversal: ");
                tree_inorder(&tree);
                printf("\n");
                break;
            case 5:
                printf("Preorder Traversal: ");
                tree_preorder(&tree);
                printf("\n");
                break;
            case 6:
                printf("Struktur Tree:\n");
                tree_display(&tree);
                break;
            case 0:
                tree_clear(&tree); // Bersihkan memori sebelum keluar
                printf("Keluar dari program.\n");
                break;
            default:
                printf("Menu tidak valid. Silakan pilih lagi.\n");
        }
    } while (menu != 0);
}