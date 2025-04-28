#include <stdio.h>
#include <windows.h>

// Importing menu functions from other .c files
#include "validateInput.h"
#include "server.h"
#include "learner.h"
#include "mentor.h"

int main() {
    int mainChoice;

    while(1) {
        printf("\n=== Program Selection Menu ===\n\n");
        printf("1. Server Program\n");
        printf("2. Learner Program\n");
        printf("3. Mentor Program\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        mainChoice = getValidatedInteger();     // returns -1 for invalid input

        switch(mainChoice) {
            case 1:
                system("cls");
                mainServer();
                break;
            case 2:
                system("cls");
                mainLearner();
                break;
            case 3:
                system("cls");
                mainMentor();
                break;
            case 4:
                system("cls");
                printf("Exiting the entire program");
                for (int i = 0; i < 3; i++){
                    Sleep(300);
                    printf(".");
                }
                Sleep(300);
                system("cls");
                return 0;
            default:
                printf("Invalid choice. Please select again.\n");
        }
    }
    return 0;
}
