#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mainMenu(){
    printf("\nCommunity Skill Mentorship Platform\n");
    printf("***************************************\n");
    printf("Main Menu\n");
    printf("***************************************\n");
    printf("[1] Sign up\n");
    printf("[2] Log in\n");
    printf("[3] Exit\n");
    printf("***************************************\n");
}

int main() {
    int userInput;

    while (1) {
        mainMenu();
        printf("Enter your choice: ");
        scanf("%d", &userInput);

        switch (userInput) {
            case 1:
                system("cls");
                //signUp()
                break;
            case 2:
                system("cls");
                //login()
                break;
            case 3:
                system("cls");
                exit(0);
            default:
                system("cls");
                printf("\nPlease enter a valid choice.\n");
                break;
        }

    }

    return 0;
}