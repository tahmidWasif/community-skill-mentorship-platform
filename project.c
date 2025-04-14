#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

//defining constants
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 20

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

//function prototypes
void getPassword(char password[MAX_PASSWORD_LENGTH]);
void signUp();
void login();
void learnerMenu();
void mentorMenu();

int main() {
    int userInput;

    while (1) {
        mainMenu();
        printf("Enter your choice: ");
        scanf("%d", &userInput);
        getchar();

        switch (userInput) {
            case 1:
                signUp();
                break;
            case 2:
                //login()
                break;
            case 3:
                exit(0);
            default:
                printf("\nPlease enter a valid choice.\n");
                break;
        }

    }

    return 0;
}

void getPassword(char password[MAX_PASSWORD_LENGTH]) {
    for (int i = 0; i < MAX_PASSWORD_LENGTH - 1; i++) {
        if (i < 0) {
            i = 0;
        }
        char inputChar = getch();

        if (inputChar == 13){       //If the user presses Enter, it exits the loop
            password[i] = '\0';     //Ends the password with null character
            break;
        }
        if (inputChar == 8) {       //removes * from the terminal when backspace is entered
            putch('\b');
            putch(' ');
            putch('\b');
            i--;

            if (i >= 0){
                password[i] = '\0';
                i--;            //i is decremented so that it points to the index where '\0' is at after the for loop increments i
                continue;
            }

        }
        password[i] = inputChar;
        putch('*');
    }
    printf("\n");
}

void signUp(){
    char username[MAX_USERNAME_LENGTH], password[MAX_PASSWORD_LENGTH], checkPassword[MAX_PASSWORD_LENGTH];
    printf("\nCreate a username: ");
    fgets(username, sizeof(username), stdin);  //removing the newline character

    //creating password
    while (1) {
        printf("Create a password: ");
        getPassword(password);
        printf("Retype the new password: ");
        getPassword(checkPassword);

        if (!strcmp(password, checkPassword)) {
            break;
        }
        else {
            printf("Password does not match. Try again.\n");
        }
    }
}