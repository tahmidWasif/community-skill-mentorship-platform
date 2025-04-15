#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

//defining constants
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 20

//structs
typedef struct Learner {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} learner;

typedef struct Mentor {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} mentor;

//main menu function
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
        char passwordChar = getch();

        if (passwordChar == 13){       //If the user presses Enter, it exits the loop
            password[i] = '\0';     //Ends the password with null character
            break;
        }
        if (passwordChar == 8) {       //removes * from the terminal when backspace is entered
            putch('\b');
            putch(' ');
            putch('\b');
            i--;

            if (i >= 0){
                password[i] = '\0';
                i--;            //i is decremented so that it points to the index where '\0' is at after i is incremented by the for loop
                continue;
            }

        }
        password[i] = passwordChar;
        putch('*');
    }
    printf("\n");
}

void signUp(){
    char username[MAX_USERNAME_LENGTH], password[MAX_PASSWORD_LENGTH], checkPassword[MAX_PASSWORD_LENGTH];
    int userChoice, valid = 0;
    
    while (!valid){
        printf("\nWhat type of account do you want to create?\n");
        printf("[1] Learner\n");
        printf("[2] Mentor\n");
        printf("***************************************\n");
        printf("Enter your choice: ");
        scanf("%d", &userChoice);
        getchar();      //consumes the leftover newline character

        if (userChoice == 1 || userChoice == 2){
            valid = 1;
        }
        else {
            printf("Please enter a valid input (1 or 2).\n");
        }
    }
    printf("\nCreate a username: ");
    fgets(username, sizeof(username), stdin);  
    username[strlen(username) - 1] = '\0';           //removing the newline character

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
            printf("Passwords do not match. Try again.\n");
        }
    }
    
    //creating the account of said type
    if (userChoice == 1){
        learner* learnerUser;
        learnerUser = (learner *)malloc(sizeof(learner));
        strcpy(learnerUser->username, username);
        strcpy(learnerUser->password, password);
    }
    else {
        mentor* mentorUser;
        mentorUser = (mentor *)malloc(sizeof(mentor));
        strcpy(mentorUser->username, username);
        strcpy(mentorUser->password, password);
    }
    
}