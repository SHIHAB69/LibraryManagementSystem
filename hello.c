#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 100

typedef struct {
    int id;
    char title[100];
    char author[50];
    int quantity;
    int isIssued; 
    float price;  
} Book;

typedef struct {
    int userId;
    char userName[50];
    int bookId;
} IssueRecord;

Book library[MAX_BOOKS];
IssueRecord issuedBooks[MAX_BOOKS];
int bookCount = 0;
int issueCount = 0;


int adminLogin();
void addBook();
void viewBooks();
void viewAvailableBooks();
void issueBook();
void returnBook();
void buyBook();
void sellBook();

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
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("6. Buy Books (Add Stock)\n");
        printf("7. Sell Books\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(); break;
            case 2: viewBooks(); break;
            case 3: viewAvailableBooks(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 6: buyBook(); break;
            case 7: sellBook(); break;
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

   
    if (strcmp(adminId, "admin") == 0 && strcmp(password, "admin203") == 0) {
        return 1; 
    } else {
        return 0; 
    }
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
    printf("Enter Price per Book: ");
    scanf("%f", &library[bookCount].price);
    library[bookCount].isIssued = 0;

    printf("Book added successfully!\n");
    bookCount++;
}

void viewBooks() {
    if (bookCount == 0) {
        printf("No books available in the library.\n");
        return;
    }

    printf("\n--- List of All Books ---\n");
    for (int i = 0; i < bookCount; i++) {
        printf("ID: %d, Title: %s, Author: %s, Quantity: %d, Price: $%.2f, Status: %s\n",
               library[i].id,
               library[i].title,
               library[i].author,
               library[i].quantity,
               library[i].price,
               library[i].isIssued ? "Issued" : "Available");
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
                   library[i].id,
                   library[i].title,
                   library[i].author,
                   library[i].quantity,
                   library[i].price);
            availableCount++;
        }
    }

    if (availableCount == 0) {
        printf("No books are currently available.\n");
    }
}

void issueBook() {
    int bookId, userId;
    char userName[50];

    printf("Enter Book ID to issue: ");
    scanf("%d", &bookId);

    for (int i = 0; i < bookCount; i++) {
        if (library[i].id == bookId) {
            if (library[i].quantity <= 0) {
                printf("Book is out of stock!\n");
                return;
            }

            if (library[i].isIssued) {
                printf("Book is already issued!\n");
                return;
            }

            printf("Enter User ID: ");
            scanf("%d", &userId);
            printf("Enter User Name: ");
            getchar(); 
            fgets(userName, 50, stdin);
            strtok(userName, "\n");

           
            issuedBooks[issueCount].userId = userId;
            strcpy(issuedBooks[issueCount].userName, userName);
            issuedBooks[issueCount].bookId = bookId;
            issueCount++;

            library[i].isIssued = 1;
            library[i].quantity--;
            printf("Book issued successfully to %s (User ID: %d)!\n", userName, userId);
            return;
        }
    }
    printf("Book ID not found!\n");
}

void returnBook() {
    int bookId;
    printf("Enter Book ID to return: ");
    scanf("%d", &bookId);

    for (int i = 0; i < bookCount; i++) {
        if (library[i].id == bookId) {
            if (!library[i].isIssued) {
                printf("Book is already available in the library.\n");
                return;
            }

            library[i].isIssued = 0;
            library[i].quantity++;
            printf("Book returned successfully!\n");
            return;
        }
    }
    printf("Book ID not found!\n");
}

void buyBook() {
    int bookId, quantity;
    printf("Enter Book ID to restock: ");
    scanf("%d", &bookId);

    for (int i = 0; i < bookCount; i++) {
        if (library[i].id == bookId) {
            printf("Enter quantity to buy: ");
            scanf("%d", &quantity);

            library[i].quantity += quantity;
            printf("Successfully restocked %d copies of '%s'.\n", quantity, library[i].title);
            return;
        }
    }
    printf("Book ID not found!\n");
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
            printf("Invoice:\n");
            printf("Book: %s\n", library[i].title);
            printf("Quantity: %d\n", quantity);
            printf("Price per Book: $%.2f\n", library[i].price);
            printf("Total Price: $%.2f\n", totalPrice);

            printf("Books sold successfully!\n");
            return;
        }
    }
    printf("Book ID not found!\n");
}
