#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[100];
    char batch[30];
    char membership[10];
    char regDate[15];
    char dob[15];
    char interest[10];
} Student;

Student *db = NULL;
int count = 0;

void loadDatabase() {
    FILE *f = fopen("members.dat", "rb");
    if (!f) return;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    count = size / sizeof(Student);
    db = malloc(size);
    fread(db, sizeof(Student), count, f);
    fclose(f);
}

void saveDatabase() {
    FILE *f = fopen("members.dat", "wb");
    if (!f) return;
    fwrite(db, sizeof(Student), count, f);
    fclose(f);
}

int existsID(int id) {
    for (int i = 0; i < count; i++)
        if (db[i].id == id) return 1;
    return 0;
}

void addStudent(Student s) {
    if (existsID(s.id)) {
        printf("Duplicate ID\n");
        return;
    }
    db = realloc(db, (count + 1) * sizeof(Student));
    db[count] = s;
    count++;
    saveDatabase();
}

void updateStudent(int studentID) {
    for (int i = 0; i < count; i++) {
        if (db[i].id == studentID) {
            printf("Enter new batch: ");
            scanf(" %[^\n]", db[i].batch);
            printf("Enter new membership (IEEE/ACM): ");
            scanf(" %[^\n]", db[i].membership);
            return;
        }
    }
    printf("Record not found\n");
}

void deleteStudent(int studentID) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (db[i].id == studentID) {
            found = 1;
            for (int j = i; j < count - 1; j++)
                db[j] = db[j + 1];
            count--;
            db = realloc(db, count * sizeof(Student));
            break;
        }
    }
    if (!found) printf("Record not found\n");
}

void displayAll() {
    if (count == 0) {
        printf("No records\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        printf("\nID: %d\nName: %s\nBatch: %s\nMembership: %s\nRegDate: %s\nDOB: %s\nInterest: %s\n",
               db[i].id, db[i].name, db[i].batch, db[i].membership,
               db[i].regDate, db[i].dob, db[i].interest);
    }
}

void batchReport() {
    char b[30], m[10];
    printf("Enter batch (CS/SE/Cyber Security/AI): ");
    scanf(" %[^\n]", b);
    printf("Enter membership (IEEE/ACM/Both): ");
    scanf(" %[^\n]", m);
    int found = 0;
    for (int i = 0; i < count; i++) {
        int matchBatch = strcmp(db[i].batch, b) == 0;
        int matchMember = strcmp(m, "Both") == 0 || strcmp(db[i].membership, m) == 0;
        if (matchBatch && matchMember) {
            found = 1;
            printf("\nID: %d\nName: %s\nMembership: %s\n",
                   db[i].id, db[i].name, db[i].membership);
        }
    }
    if (!found) printf("No matching records\n");
}

int main() {
    loadDatabase();
    int choice;

    while (1) {
        printf("\n1. Register Student\n2. Update Student\n3. Delete Student\n4. View All\n5. Batch-wise Report\n6. Exit\nEnter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            Student s;
            printf("Enter ID: ");
            scanf("%d", &s.id);
            printf("Enter Name: ");
            scanf(" %[^\n]", s.name);
            printf("Enter Batch (CS/SE/Cyber Security/AI): ");
            scanf(" %[^\n]", s.batch);
            printf("Enter Membership (IEEE/ACM): ");
            scanf(" %[^\n]", s.membership);
            printf("Enter Registration Date (YYYY-MM-DD): ");
            scanf(" %[^\n]", s.regDate);
            printf("Enter DOB (YYYY-MM-DD): ");
            scanf(" %[^\n]", s.dob);
            printf("Enter Interest (IEEE/ACM/Both): ");
            scanf(" %[^\n]", s.interest);
            addStudent(s);
        }

        else if (choice == 2) {
            int id;
            printf("Enter ID to update: ");
            scanf("%d", &id);
            updateStudent(id);
            saveDatabase();
        }

        else if (choice == 3) {
            int id;
            printf("Enter ID to delete: ");
            scanf("%d", &id);
            deleteStudent(id);
            saveDatabase();
        }

        else if (choice == 4) {
            displayAll();
        }

        else if (choice == 5) {
            batchReport();
        }

        else if (choice == 6) {
            saveDatabase();
            free(db);
            break;
        }

        else printf("Invalid option\n");
    }
    return 0;
}
