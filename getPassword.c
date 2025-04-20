#include "getPassword.h"

#include <conio.h>
#include <stdio.h>

#define MAX_PASSWORD_LENGTH 50
#define MAX_USERNAME_LENGTH 50

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
