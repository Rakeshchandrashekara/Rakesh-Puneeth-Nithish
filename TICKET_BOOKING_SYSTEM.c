#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#define MAX_SEATS 100

struct Movie {
    char title[100];
    int duration;
    int year;
    struct Movie* next; 
};

struct Theater {
    char name[100];
    struct Theater* left;
    struct Theater* right;
};

struct SeatBooking {
    int seatNumber;
    char customerName[100];
};

struct PendingSeatQueue {
    struct SeatBooking data[MAX_SEATS];
    int front, rear, size;
};

void initializeQueue(struct PendingSeatQueue *queue) {
    queue->front = queue->rear = -1;
    queue->size = 0;
}

struct Movie* createMovie(const char* title, int year) {
    struct Movie* newMovie = (struct Movie*)malloc(sizeof(struct Movie));
    if (newMovie == NULL) {
        perror("Memory allocation error");
        exit(1);
    }

    strncpy(newMovie->title, title, sizeof(newMovie->title));
    newMovie->year = year;
    newMovie->next = NULL;
    return newMovie;
}

void insertMovie(struct Movie** head, const char* title, int year) {
    struct Movie* newMovie = createMovie(title, year);
    newMovie->next = *head;
    *head = newMovie;
}

void deleteMovie(struct Movie** head, const char* title) {
    struct Movie* current = *head;
    struct Movie* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->title, title) == 0) {
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("Movie deleted successfully: %s\n", title);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Movie not found: %s\n", title);
}

void displayMovies(struct Movie* head) {
    struct Movie* current = head;
    printf("List of Movies:\n");
    while (current != NULL) {
        printf("%s (%d)\n", current->title, current->year);
        current = current->next;
    }
}

void addMovie(struct Movie** head) {
    char title[100];
    int year;
    printf("Enter movie title: ");
    scanf("%s", title);
    
    struct Movie* existingMovie = *head;
    while (existingMovie != NULL) {
        if (strcmp(existingMovie->title, title) == 0) {
            printf("Movie already exists.\n");
            return;
        }
        existingMovie = existingMovie->next;
    }

    printf("Enter movie year: ");
    scanf("%d", &year);
    insertMovie(head, title, year);
    printf("Movie added successfully.\n");
}

struct Movie* searchMovie(struct Movie* head, const char* title) {
    struct Movie* current = head;
    while (current != NULL) {
        if (strcmp(current->title, title) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void searchMovieOption(struct Movie* head) {
    char title[100];
    printf("Enter the title of the movie you want to search for: ");
    scanf("%s", title);
    struct Movie* foundMovie = searchMovie(head, title);

    if (foundMovie != NULL) {
        printf("Movie found: %s (%d)\n", foundMovie->title, foundMovie->year);
    } else {
        printf("Movie not found: %s\n", title);
    }
}

struct Theater* createTheater(const char* name) {
    struct Theater* newTheater = (struct Theater*)malloc(sizeof(struct Theater));
    if (newTheater == NULL) {
        perror("Memory allocation error");
        exit(1);
    }
    strncpy(newTheater->name, name, sizeof(newTheater->name));
    newTheater->left = newTheater->right = NULL;
    return newTheater;
}

struct Theater* insertTheater(struct Theater* root, const char* name) {
    if (root == NULL) {
        return createTheater(name);
    }
    if (strcmp(name, root->name) < 0) {
        root->left = insertTheater(root->left, name);
    } else if (strcmp(name, root->name) > 0) {
        root->right = insertTheater(root->right, name);
    }
 return root;
}

struct Theater* searchTheater(struct Theater* root, const char* name) {
    if (root == NULL || strcmp(name, root->name) == 0) {
        return root;
    }
    if (strcmp(name, root->name) < 0) {
        return searchTheater(root->left, name);
    } else {
        return searchTheater(root->right, name);
    }
}

void inOrderTraversal(struct Theater* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("%s\n", root->name);
        inOrderTraversal(root->right);
    }
}

int isQueueEmpty(struct PendingSeatQueue *queue) {
    return queue->size == 0;
}

int isQueueFull(struct PendingSeatQueue *queue) {
    return queue->size == MAX_SEATS;
}

void enqueue(struct PendingSeatQueue *queue, int seatNumber, const char *customerName) {
    if (!isQueueFull(queue)) {
        if (queue->rear == MAX_SEATS - 1) {
            queue->rear = -1; 
        }
        struct SeatBooking newBooking;
        newBooking.seatNumber = seatNumber;
        snprintf(newBooking.customerName, sizeof(newBooking.customerName), "%s", customerName);
        queue->data[++(queue->rear)] = newBooking;
        queue->size++;
    } else {
        printf("Queue is full; cannot add more seat bookings.\n");
    }
}

struct SeatBooking dequeue(struct PendingSeatQueue *queue) {
    struct SeatBooking emptyBooking = { -1, "" }; 
    if (!isQueueEmpty(queue)) {
        if (queue->front == MAX_SEATS - 1) {
            queue->front = -1; 
        }
        return queue->data[++(queue->front)];
    } else {
        printf("Queue is empty; no seat bookings to remove.\n");
        return emptyBooking;
    }
}
int main() {
    int choice;
    int seatNumber;  
    struct Movie* movieList = NULL;
    struct Theater* theaterTree = NULL;
    int totalSeats = 10; 
    int seatsFilled  = 0; 
    int seatsPending  =  0; 
    do {
        char strings[5][50] = {"Add movie", "Delete movie", "Display movies","Search movie","Exit"};
    for (int i = 0; i < 5; ++i) {
        printf("%d. %s\n", i + 1, strings[i]);
        }
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                addMovie(&movieList);
                break;
            case 2: {
                char titleToDelete[100];
                printf("Enter the title of the movie to delete: ");
                scanf("%s", titleToDelete);
                deleteMovie(&movieList, titleToDelete);
                break;
            }
            case 3:
                displayMovies(movieList);
                break;
            case 4:
                searchMovieOption(movieList);
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    
    while (movieList != NULL) {
        struct Movie* temp = movieList;
        movieList = movieList->next;
        free(temp);
    }
  
    int numTheaters;
    printf("Enter the number of theaters: ");
    scanf("%d", &numTheaters);

    for (int i = 0; i < numTheaters; ++i) {
        char theaterName[100];
        printf("Enter the name of Theater %d: ", i + 1);
        scanf("%s", theaterName);
        theaterTree = insertTheater(theaterTree, theaterName);
    }

    
    printf("\nTheaters (Sorted by Name):\n");
    inOrderTraversal(theaterTree);
    char searchName[100];
    printf("\nEnter the name of the theater you want to search for: ");
    scanf("%s", searchName);
    struct Theater* foundTheater = searchTheater(theaterTree, searchName);
    if (foundTheater != NULL) {
        printf("Theater found: %s\n", foundTheater->name);
    } else {
        printf("Theater not found: %s\n", searchName);
    }
    
struct PendingSeatQueue pendingSeats;
    initializeQueue(&pendingSeats);
    do {
        char strings[4][50] = {"BOOK TICKET", " CANCEL THE TICKET", "SEATS AVAILABLE","Exit"};

    for (int i = 0; i < 4; ++i) {
        printf("%d. %s\n", i + 1, strings[i]);
        }
     
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (seatsFilled < totalSeats) {
                    char customerName[100];
                    printf("Enter customer name: ");
                    scanf("%s", customerName);
                    enqueue(&pendingSeats, seatNumber++, customerName);
                    seatsFilled++;
                    }
                     else {
                    printf("All seats are filled; cannot add more seat bookings.\n");
                }
                break;
            case 2: {
    struct SeatBooking removedBooking = dequeue(&pendingSeats);

    if (removedBooking.seatNumber != -1) {
        seatsPending++;
        seatsFilled--;  

        printf("Removed seat booking for seat %d (Customer: %s)\n", removedBooking.seatNumber, removedBooking.customerName);
    }
    break;
}
        case 3:
                printf("Total Seats: %d\nSeats Filled: %d\nSeats Pending: %d\n", totalSeats, seatsFilled, totalSeats-seatsFilled);
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);
    return 0;
}