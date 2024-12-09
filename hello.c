#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 100
#define MAX_SALES 100

typedef struct {
    int id;
    char title[100];
    char author[50];
    int quantity;
    float costPrice;
    float price;
} Book;

typedef struct {
    int bookId;
    char title[100];
    int quantitySold;
    float totalPrice;
    float profit;
} SaleRecord;

Book library[MAX_BOOKS];
SaleRecord sales[MAX_SALES];
int bookCount = 0;
int salesCount = 0;

int adminLogin();
void addBook();
void viewBooks();
void viewAvailableBooks();
void searchBook();
void sellBook();
void viewSalesDetails();
void viewTotalProfit();

int main() {
    int choice;

    printf("\n--- Welcome to the Library Management System ---\n");

    if (!adminLogin()) {
        printf("Invalid login. Exiting program.\n");
        return 0;
    }

    printf("Login successful! Welcome, Admin.\n");

    while (1) {
        printf("\n--- Library Management Menu ---\n");
        printf("1. Add Book\n");
        printf("2. View All Books\n");
        printf("3. View Available Books\n");
        printf("4. Search Book\n");
        printf("5. Sell Books\n");
        printf("6. View Sales Details\n");
        printf("7. View Total Profit\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(); break;
            case 2: viewBooks(); break;
            case 3: viewAvailableBooks(); break;
            case 4: searchBook(); break;
            case 5: sellBook(); break;
            case 6: viewSalesDetails(); break;
            case 7: viewTotalProfit(); break;
            case 8: exit(0);
            default: printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

int adminLogin() {
    char adminId[20], password[20];
    printf("\n--- Admin Login ---\n");
    printf("Enter Admin ID: ");
    scanf("%s", adminId);
    printf("Enter Password: ");
    scanf("%s", password);
    return (strcmp(adminId, "admin") == 0 && strcmp(password, "admin203") == 0);
}

void addBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("Library is full! Cannot add more books.\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d", &library[bookCount].id);
    printf("Enter Book Title: ");
    getchar();
    fgets(library[bookCount].title, 100, stdin);
    strtok(library[bookCount].title, "\n");
    printf("Enter Author Name: ");
    fgets(library[bookCount].author, 50, stdin);
    strtok(library[bookCount].author, "\n");
    printf("Enter Quantity: ");
    scanf("%d", &library[bookCount].quantity);
    printf("Enter Cost Price per Book: ");
    scanf("%f", &library[bookCount].costPrice);
    printf("Enter Selling Price per Book: ");
    scanf("%f", &library[bookCount].price);

    bookCount++;
    printf("Book added successfully!\n");
}

void viewBooks() {
    if (bookCount == 0) {
        printf("No books available in the library.\n");
        return;
    }

    printf("\n--- List of All Books ---\n");
    for (int i = 0; i < bookCount; i++) {
        printf("ID: %d, Title: %s, Author: %s, Quantity: %d, Price: $%.2f\n",
               library[i].id, library[i].title, library[i].author, library[i].quantity, library[i].price);
    }
}

void viewAvailableBooks() {
    int availableCount = 0;

    if (bookCount == 0) {
        printf("No books available in the library.\n");
        return;
    }

    printf("\n--- List of Available Books ---\n");
    for (int i = 0; i < bookCount; i++) {
        if (library[i].quantity > 0) {
            printf("ID: %d, Title: %s, Author: %s, Quantity: %d, Price: $%.2f\n",
                   library[i].id, library[i].title, library[i].author, library[i].quantity, library[i].price);
            availableCount++;
        }
    }

    if (availableCount == 0) {
        printf("No books are currently available.\n");
    }
}

void searchBook() {
    int choice, bookId, found = 0;
    char title[100];

    printf("\n--- Search Book ---\n");
    printf("1. Search by Title\n");
    printf("2. Search by ID\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter Book Title: ");
        getchar();
        fgets(title, 100, stdin);
        strtok(title, "\n");

        for (int i = 0; i < bookCount; i++) {
            if (strstr(library[i].title, title)) {
                printf("ID: %d, Title: %s, Author: %s, Quantity: %d, Price: $%.2f\n",
                       library[i].id, library[i].title, library[i].author, library[i].quantity, library[i].price);
                found = 1;
            }
        }
    } else if (choice == 2) {
        printf("Enter Book ID: ");
        scanf("%d", &bookId);

        for (int i = 0; i < bookCount; i++) {
            if (library[i].id == bookId) {
                printf("ID: %d, Title: %s, Author: %s, Quantity: %d, Price: $%.2f\n",
                       library[i].id, library[i].title, library[i].author, library[i].quantity, library[i].price);
                found = 1;
            }
        }
    } else {
        printf("Invalid choice! Please try again.\n");
        return;
    }

    if (!found) {
        printf("Book not found.\n");
    }
}

void sellBook() {
    int bookId, quantity;

    printf("Enter Book ID to sell: ");
    scanf("%d", &bookId);

    for (int i = 0; i < bookCount; i++) {
        if (library[i].id == bookId) {
            printf("Enter quantity to sell: ");
            scanf("%d", &quantity);

            if (library[i].quantity < quantity) {
                printf("Not enough stock available!\n");
                return;
            }

            library[i].quantity -= quantity;
            float totalPrice = quantity * library[i].price;
            float profit = quantity * (library[i].price - library[i].costPrice);

            sales[salesCount].bookId = bookId;
            strcpy(sales[salesCount].title, library[i].title);
            sales[salesCount].quantitySold = quantity;
            sales[salesCount].totalPrice = totalPrice;
            sales[salesCount].profit = profit;
            salesCount++;

            printf("Book sold successfully!\n");
            printf("Total Price: $%.2f, Profit: $%.2f\n", totalPrice, profit);
            return;
        }
    }

    printf("Book ID not found!\n");
}

void viewSalesDetails() {
    if (salesCount == 0) {
        printf("No sales have been recorded.\n");
        return;
    }

    printf("\n--- Sales Details ---\n");
    for (int i = 0; i < salesCount; i++) {
        printf("Book ID: %d, Title: %s, Quantity Sold: %d, Total Price: $%.2f, Profit: $%.2f\n",
               sales[i].bookId, sales[i].title, sales[i].quantitySold, sales[i].totalPrice, sales[i].profit);
    }
}

void viewTotalProfit() {
    float totalProfit = 0;

    for (int i = 0; i < salesCount; i++) {
        totalProfit += sales[i].profit;
    }

    printf("\n--- Total Profit ---\n");
    printf("Total Profit from Sales: $%.2f\n", totalProfit);
}
