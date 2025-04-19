
#include <stdio.h>

// Function declarations (Assuming these functions exist in your codebase)
void runServerProgram();
void runLearnerProgram();
void runMentorProgram();

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
                runServerProgram();
                break;
            case 2:
                runLearnerProgram();
                break;
            case 3:
                runMentorProgram();
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
