#include "getPassword.h"    // for getPassword() function
#include "setColor.h"       // for set_color() function

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <conio.h>
#pragma comment(lib, "ws2_32.lib")

#define CHAT_LOG_FILE "chat_history.txt"
#define LEARNER_FILE "learners.txt"
#define MENTOR_FILE "mentors.txt"

void manage_issues(const char *username);
#define ISSUE_FILE "issues.txt"
#define COMMENT_FILE "comments.txt"


#define MAX_PASSWORD_LENGTH 50
#define MAX_USERNAME_LENGTH 50

int validate_user(const char *username, const char *password) {     //check if username exists
    FILE *fp = fopen(LEARNER_FILE, "r");
    if (!fp) return 0;
    char line[150], u[50], p[50];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^\n]", u, p);
        if (strcmp(u, username) == 0 && strcmp(p, password) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void signup_user() {
    char username[50], password[50], ip[20];
    FILE *fp = fopen(LEARNER_FILE, "a");
    printf("Choose a username: ");
    scanf("%s", username);
    printf("Choose a password: ");
    getPassword(password);
    printf("Enter IPv4 address: ");
    fgets(ip, sizeof(ip), stdin);
    ip[strlen(ip)-1] = '\0';        //removes newline character

    if (fp) {
        fprintf(fp, "%s,%s\n", username, password);
        fclose(fp);
        printf("Signup successful! Please log in.\n");
    } else {
        printf("Failed to register.\n");
    }
}

void submit_issue(const char *username) {
    char course[100], issue[256], ip[20];
    set_color(11);
    printf("Enter course title: ");
    set_color(15);
    scanf(" %[^\n]", course);
    set_color(11);
    printf("Describe your issue: ");
    set_color(15);
    scanf(" %[^\n]", issue);
    set_color(11);
    printf("Enter your IP address: ");
    set_color(15);
    scanf(" %[^\n]", ip);
    

    FILE *fp = fopen(ISSUE_FILE, "a");
    if (fp) {
        fprintf(fp, "%s,%s,%s,%s\n", username, course, issue, ip);       //ip address needs to be changed
        fclose(fp);
        set_color(10);
        printf("Issue submitted!\n");
    } else {
        set_color(12);
        printf("Failed to submit issue.\n");
    }
    set_color(15);
}

void view_comments(const char *username) {
    FILE *fp = fopen(COMMENT_FILE, "r");
    if (!fp) {
        set_color(12);
        printf("No comments found.\n");
        set_color(15);
        return;
    }

    char line[512], u[50], course[100], comment[256];
    int found = 0;
    printf("\n=== Comments for You ===\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^,],%[^\n]", u, course, comment);
        if (strcmp(u, username) == 0) {
            printf("Course: %s\nComment: %s\n", course, comment);
            found = 1;
        }
    }
    if (!found) {
        set_color(14);
        printf("No mentor comments found for your submissions.\n");
    }
    fclose(fp);
    set_color(15);
}
void get_mentor_ip(char user[MAX_USERNAME_LENGTH], char ip[20]) {
    char courses[100], users[MAX_USERNAME_LENGTH], passwords[MAX_PASSWORD_LENGTH], lines[512];
    FILE* fp = fopen(MENTOR_FILE, "r");
    if (fp) {
        while (fgets(lines, sizeof(lines), fp) != NULL) {
            sscanf(lines, "%[^,],%[^,],%[^,],%s", users, passwords, courses, ip);
            if (strcmp(user, users) == 0){
                return;
            }
        }
        printf("Invalid username\n");
        get_ip(user, ip);

    }
    else {
        printf("Failed to access files...\n");
    }
    fclose(fp);

}

void learner_entry() {
    char username[50], password[50];
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    getPassword(password);

    if (!validate_user(username, password)) {
        printf("Invalid credentials or not a learner.\n");
        return;
    }

    int choice;
    do {
        set_color(3);
        printf("\n=== Learner Menu ===\n");
        set_color(15);
        printf("1. Submit a Help Request\n");
        printf("2. View Mentor Comments\n");
        printf("3. Chat with Mentor\n");
        printf("4. View/Delete My Issues\n");
        printf("5. Return to Learner Access Menu\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: submit_issue(username);
                break;
            case 2: view_comments(username); break;
            case 3: {
                char ip[20], user[MAX_USERNAME_LENGTH];


                get_mentor_ip(user, ip);

                char str[512];
                char sentence[]="gcc -std=gnu11 -Wall -o chatwinver.exe chatwinver.c udp3winver.c -lws2_32";
                system(sentence);
                sprintf(str, ".\\chatwinver.exe %s", ip);      //needs mentor IP
                system(str);
                break;
            }
            case 4: manage_issues(username); break;
            case 5: printf("Returning to learner access menu...\n"); break;
            printf("Returning to learner access menu...\n");
            break;
        }
    } while (choice != 5);
}


void manage_issues(const char *username) {
    FILE *fp = fopen(ISSUE_FILE, "r");
    if (!fp) {
        printf("No issues found.\n");
        return;
    }

    char lines[100][512];
    char users[100][50], courses[100][100], issues[100][256], ips[100][50];
    int count = 0;

    while (fgets(lines[count], sizeof(lines[count]), fp)) {
        sscanf(lines[count], "%[^,],%[^,],%[^,],%s", users[count], courses[count], issues[count], ips[count]);
        if (strcmp(users[count], username) == 0) {
            count++;
        } 
    }
    fclose(fp);

    if (count == 0) {
        printf("You have no previous issues.\n");
        return;
    }

    printf("\n=== Your Reported Issues ===\n");
    for (int i = 0; i < count; i++) {
        printf("%d. [%s] %s (IP: %s)\n", i + 1, courses[i], issues[i], ips[i]);
    }

    int choice;
    printf("Enter the issue number to delete it (or 0 to cancel): ");
    scanf("%d", &choice);
    getchar();

    if (choice > 0 && choice <= count) {
        FILE *wfp = fopen("temp_issues.txt", "w");
        for (int i = 0; i < count; i++) {
            if (i != choice - 1) {
                fprintf(wfp, "%s", lines[i]);
            }
        }
        fclose(wfp);
        remove(ISSUE_FILE);
        rename("temp_issues.txt", ISSUE_FILE);
        printf("Issue deleted.\n");
    } else {
        printf("No issue deleted.\n");
    }
}

int main() {
    int choice;
    while (1) {
        printf("\n=== Learner Access ===\n");
        printf("1. Sign Up\n");
        printf("2. Log In\n");
        printf("3. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            signup_user();
        } else if (choice == 2) {
            learner_entry();
        } else if (choice == 3) {
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }
    return 0;
}