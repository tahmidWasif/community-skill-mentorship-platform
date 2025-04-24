#include "getPassword.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define ISSUE_FILE "issues.txt"
#define COMMENT_FILE "comments.txt"
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "password"

#define MAX_PASSWORD_LENGTH 50
#define MAX_USERNAME_LENGTH 50

void view_all_issues() {
    FILE *fp = fopen(ISSUE_FILE, "r");
    if (!fp) {
        printf("No issues submitted.\n");
        return;
    }
    char line[512];
    printf("\n=== Submitted Issues ===\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}

void view_all_comments() {
    FILE *fp = fopen(COMMENT_FILE, "r");
    if (!fp) {
        printf("No comments found.\n");
        return;
    }
    char line[512];
    printf("\n=== Mentor Comments ===\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}

void server_menu() {
    int choice;
    do {
        printf("\n=== Server Menu ===\n");
        printf("1. View All Issues\n");
        printf("2. View All Comments\n");
        printf("3. Return to Main Menu\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: view_all_issues(); break;
            case 2: view_all_comments(); break;
            case 3: printf("Returning to main menu...\n"); break;
            default: printf("Invalid option.\n");
        }
    } while (choice != 3);
}

int admin_login() {
    char username[50], password[50];
    printf("Admin Username: ");
    scanf("%s", username);
    printf("Admin Password: ");
    getPassword(password);

    if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
        printf("Admin login successful!\n");
        return 1;
    } else {
        printf("Incorrect admin credentials.\n");
        return 0;
    }
}

int main() {
    int choice;
    do {
        printf("\n=== Main Menu ===\n");
        printf("1. Admin Login (Server Access)\n");
        printf("2. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                if (admin_login()) {
                    server_menu();
                }
                break;
            case 2:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid option.\n");
        }
    } while (choice != 2);

    return 0;
}