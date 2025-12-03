#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Employee
{
    int id;
    char *name;
    char *designation;
    float salary;
};

void displayEmployees(struct Employee *e, int n)
{
    printf("\nID\tName\t\tDesignation\tSalary\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t%-10s\t%-12s\t%.2f\n", e[i].id, e[i].name, e[i].designation, e[i].salary);
    }
}
void findHighestSalary(struct Employee *e, int n)
{
    int index = 0;
    for (int i = 1; i < n; i++)
    {
        if (e[i].salary > e[index].salary)
            index = i;
    }
    printf("\nHighest Salary Employee:\n");
    printf("ID: %d\nName: %s\nDesignation: %s\nSalary: %.2f\n",
           e[index].id, e[index].name, e[index].designation, e[index].salary);
}
void searchEmployee(struct Employee *e, int n)
{
    int choice, searchID;
    char searchName[50];
    printf("\n1. Search by ID\n2. Search by Name\nEnter choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("Enter ID: ");
        scanf("%d", &searchID);
        for (int i = 0; i < n; i++)
        {
            if (e[i].id == searchID)
            {
                printf("\nEmployee Found:\n");
                printf("ID: %d\nName: %s\nDesignation: %s\nSalary: %.2f\n",
                       e[i].id, e[i].name, e[i].designation, e[i].salary);
                return;
            }
        }
        printf("Employee not found\n");
    }
    else if (choice == 2)
    {
        printf("Enter Name: ");
        scanf("%s", searchName);
        for (int i = 0; i < n; i++)
        {
            if (strcmp(e[i].name, searchName) == 0)
            {
                printf("\nEmployee Found:\n");
                printf("ID: %d\nName: %s\nDesignation: %s\nSalary: %.2f\n",
                       e[i].id, e[i].name, e[i].designation, e[i].salary);
                return;
            }
        }
        printf("Employee not found\n");
    }
}

void giveBonus(struct Employee *e, int n, float threshold)
{
    for (int i = 0; i < n; i++)
    {
        if (e[i].salary < threshold)
            e[i].salary *= 1.10;
    }
}

int main()
{
    int n;
    printf("Enter number of employees: ");
    scanf("%d", &n);

    struct Employee *emp = malloc(n * sizeof(struct Employee));

    for (int i = 0; i < n; i++)
    {
        char tempName[50], tempDes[50];

        printf("\nEnter Employee %d Data:\n", i + 1);
        printf("ID: ");
        scanf("%d", &emp[i].id);

        printf("Name: ");
        scanf("%s", tempName);
        emp[i].name = malloc(strlen(tempName) + 1);
        strcpy(emp[i].name, tempName);

        printf("Designation: ");
        scanf("%s", tempDes);
        emp[i].designation = malloc(strlen(tempDes) + 1);
        strcpy(emp[i].designation, tempDes);

        printf("Salary: ");
        scanf("%f", &emp[i].salary);
    }

    displayEmployees(emp, n);
    findHighestSalary(emp, n);
    searchEmployee(emp, n);

    giveBonus(emp, n, 50000);
    printf("\nAfter Bonus:\n");
    displayEmployees(emp, n);

    for (int i = 0; i < n; i++)
    {
        free(emp[i].name);
        free(emp[i].designation);
    }
    free(emp);

    return 0;
}
