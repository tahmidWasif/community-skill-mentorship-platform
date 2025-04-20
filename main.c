#include <stdio.h>

// Importing menu functions from other .c files
#include "server.c"
#include "learner.c"
#include "mentor.c"

int main() {
    int mainChoice;

    while(1) {
        printf("\n=== Program Selection Menu ===\n");
        printf("1. Server Program\n");
        printf("2. Learner Program\n");
        printf("3. Mentor Program\n");
        printf("4. Exit\n");
        printf("Choice: ");
        scanf("%d", &mainChoice);

        switch(mainChoice) {
            case 1:
                mainServer();
                break;
            case 2:
                mainLearner();
                break;
            case 3:
                mainMentor();
                break;
            case 4:
                printf("Exiting the entire program.\n");
                return 0;
            default:
                printf("Invalid choice. Please select again.\n");
        }
    }
    return 0;
}
