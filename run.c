#include <stdio.h>
#include <stdlib.h>

int main(){
    system("gcc -o main main.c learner.c mentor.c server.c setColor.c getPassword.c validateInput.c -lws2_32");
    system(".\\main.exe");
}