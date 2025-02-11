#include <stdio.h> // Allows us to use input/output functions
#include <stdlib.h> // Provides functions for memory allocation, process control
#include <string.h> // Enables string manipulation functions like strcmp(), strcpy(), 

#define MAX_BOOKS 100
#define FILENAME "books.txt"

typedef struct {
      char isbn[20];
      char title[100];
      char author[100];
      int year;
} Book;

void addBook(Book books[], int * count ){ // Function: Adds a new book to the books array.
  if(*count >= MAX_BOOKS){
      printf("Library is full!\n");
      return;
  }
  printf("Enter ISBN");
  scanf("%s" , books[*count].isbn);
  printf("Enter Title:");
  getchar(); //Clear newline character 
  fgets(books[*count].title, 100 , stdin); //Reads the book title
  books[*count].title[strcspn(books[*count].title, "\n")] = '\0';
  printf ("Enter Author:");
  fgets(books[*count].author, 100 , stdin);
  books[*count].author[strcspn(books[*count].author, "\n")] = '\0';
  printf("Enter Year");
  scanf("%d" , &books[*count].year);
  (*count)++; //Increments the total book count after adding a new book.
  printf("Book added succcessfully! \n");
}

void displayBooks(const Book books[], int count){
    for(int i = 0 ; i < count; i++){
         printf("ISBN: %s, Title: %s, Author: %s, Year: %d\n", 
               books[i].isbn, books[i].title, books[i].author, books[i].year);
    }
}

int searchBookByISBN(const Book books[], int count, const char *isbn) {
    for (int i = 0; i < count; i++) {
        if (strcmp(books[i].isbn, isbn) == 0) { //Compares two strings (ISBNs in this case). Returns 0 if they are equal.
            return i;
        }
    }
    return -1;
}

void updateBook(Book books[], int count, const char *isbn) { //Searches for the book by ISBN and updates its details if found.
    int index = searchBookByISBN(books, count, isbn);
    if (index != -1) {
        printf("Enter new Title: ");
        getchar();
        fgets(books[index].title, 100, stdin);
        books[index].title[strcspn(books[index].title, "\n")] = '\0';
        printf("Enter new Author: ");
        fgets(books[index].author, 100, stdin);
        books[index].author[strcspn(books[index].author, "\n")] = '\0';
        printf("Enter new Year: ");
        scanf("%d", &books[index].year);
        printf("Book updated successfully!\n");
    } else {
        printf("Book not found.\n");
    }
}

void deleteBook(Book books[], int *count, const char *isbn) {
    int index = searchBookByISBN(books, *count, isbn);
    if (index != -1) {
        for (int i = index; i < *count - 1; i++) {
            books[i] = books[i + 1];
        }
        (*count)--;
        printf("Book deleted successfully!\n");
    } else {
        printf("Book not found.\n");
    }
}

void saveToFile(const Book books[], int count, const char *filename) {
    FILE *file = fopen(filename, "w"); // Opens a file to write ("w" mode).
    if (!file) {
        printf("Unable to open file!\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s,%s,%d\n", books[i].isbn, books[i].title, books[i].author, books[i].year);//Writes book details to the file in CSV format.
    }
    fclose(file); //Closes the file.
    printf("Books saved successfully!\n");
}

void loadFromFile(Book books[], int *count, const char *filename) { //Reads data from the file and stores it back into the books array.
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("No previous data found. Starting fresh.\n");
        return;
    }
    *count = 0;
    while (fscanf(file, "%[^,],%[^,],%[^,],%d\n", 
                  books[*count].isbn, books[*count].title, books[*count].author, &books[*count].year) == 4) {
        (*count)++;
    }
    fclose(file);
    printf("Books loaded successfully!\n");
}

int main() { //Initializes variables and loads any previously saved data.
    Book books[MAX_BOOKS];
    int count = 0;
    int choice;
    char isbn[20];

    loadFromFile(books, &count, FILENAME);

    do {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n2. Display Books\n3. Search Book by ISBN\n4. Update Book\n5. Delete Book\n6. Save to File\n7. Load from File\n8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(books, &count); break;
            case 2: displayBooks(books, count); break;
            case 3: 
                printf("Enter ISBN to search: ");
                scanf("%s", isbn);
                int index = searchBookByISBN(books, count, isbn);
                if (index != -1) {
                    printf("Book Found: %s, %s, %s, %d\n", books[index].isbn, books[index].title, books[index].author, books[index].year);
                } else {
                    printf("Book not found.\n");
                }
                break;
            case 4: 
                printf("Enter ISBN to update: ");
                scanf("%s", isbn);
                updateBook(books, count, isbn);
                break;
            case 5: 
                printf("Enter ISBN to delete: ");
                scanf("%s", isbn);
                deleteBook(books, &count, isbn);
                break;
            case 6: saveToFile(books, count, FILENAME); break;
            case 7: loadFromFile(books, &count, FILENAME); break;
            case 8: printf("Exiting program.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 8); //loop: Keeps showing the menu until the user chooses to exit (choice == 8).

    return 0;
}
