#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int id;
    int popularity;
    int occupied;
    int rank;
} BOOK;

int findbook(BOOK shelf[], int capacity, int id) {
    for (int i = 0; i < capacity; i++) {
        if (shelf[i].occupied && shelf[i].id == id)
            return i;
    }
    return -1;
}

int emptychk(BOOK shelf[], int capacity) {
    for (int i = 0; i < capacity; i++) {
        if (!shelf[i].occupied)
            return i;
    }
    return -1;
}

int main() {
    int capacity, q;

    printf("Enter capacity and number of operations: ");
    scanf("%d %d", &capacity, &q);
    BOOK *shelf = (BOOK *)malloc(capacity * sizeof(BOOK));
    if (shelf == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    for (int i = 0; i < capacity; i++) {
        shelf[i].occupied = 0;
        shelf[i].rank = 0;
    }

    int rankcount = 1;

    while (q--) {

        char in[10];
        printf("Write ADD or ACCESS: ");
        scanf("%s", in);

        if (strcmp(in, "add") == 0 || strcmp(in, "ADD") == 0) {
            int x, y;
            printf("Enter id and popularity: ");
            scanf("%d %d", &x, &y);

            int ind = findbook(shelf, capacity, x);

            if (ind != -1) {
                shelf[ind].popularity = y;
                shelf[ind].rank = rankcount++;
            }
            else {
                int empty = emptychk(shelf, capacity);
                if (empty != -1) {
                    shelf[empty].id = x;
                    shelf[empty].popularity = y;
                    shelf[empty].rank = rankcount++;
                    shelf[empty].occupied = 1;
                }
                else {
                    int least = 0;
                    for (int i = 1; i < capacity; i++) {
                        if (shelf[i].rank < shelf[least].rank)
                            least = i;
                    }
                    shelf[least].id = x;
                    shelf[least].popularity = y;
                    shelf[least].rank = rankcount++;
                    shelf[least].occupied = 1;
                }
            }
        }

        else if (strcmp(in, "access") == 0 || strcmp(in, "ACCESS") == 0) {
            int x;
            printf("Enter id to access: ");
            scanf("%d", &x);

            int ind = findbook(shelf, capacity, x);
            if (ind == -1) {
                printf("-1\n");
            } else {
                printf("Popularity = %d\n", shelf[ind].popularity);
                shelf[ind].rank = rankcount++;
            }
        }

        else {
            printf("Invalid operation!\n");
        }
    }

    free(shelf);  
    return 0;
}
