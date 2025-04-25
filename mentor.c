#include "getPassword.h"
#include "setColor.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <conio.h>
#pragma comment(lib, "ws2_32.lib")


#define ISSUE_FILE "issues.txt"
#define COMMENT_FILE "comments.txt"
#define CHAT_LOG_FILE "chat_history.txt"
#define MENTOR_FILE "mentors.txt"

#define MAX_PASSWORD_LENGTH 50
#define MAX_USERNAME_LENGTH 50

int validate_user(const char *username, const char *password, char *mentor_course) {
    FILE *fp = fopen(MENTOR_FILE, "r");
    if (!fp) return 0;
    char line[150], u[50], p[50];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^,],%[^,]", u, p, mentor_course);
        if (strcmp(u, username) == 0 && strcmp(p, password) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void signup_user() {
    char username[50], password[50], course[100], ip[20];
    FILE *fp = fopen(MENTOR_FILE, "a");
    printf("Choose a username: ");
    scanf("%s", username);
    printf("Choose a password: ");
    getPassword(password);
    printf("Enter your IP address: ");
    scanf("%s", ip);
    printf("Which course will you mentor?: ");
    scanf("%s", course);
    if (fp) {
        fprintf(fp, "%s,%s,%s,%s\n", username, password, course, ip);
        printf("Signup successful! Please log in.\n");
    } else {
        printf("Failed to register.\n");
    }
    fclose(fp);
}

void view_issues(const char *mentor_course) {
    FILE *fp = fopen(ISSUE_FILE, "r");
    if (!fp) {
        printf("No issues found.\n");
        return;
    }

    char line[512], username[50], course[100], issue[256], ip[20];
    int count = 1;
    char course_upper[100];
    strcpy(course_upper, mentor_course);
    for (int j = 0; course_upper[j]; j++) {
        course_upper[j] = toupper(course_upper[j]);
    }
    //needs changes for IP
    printf("\n=== LEARNER ISSUES FOR COURSE: %s ===\n", course_upper);
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ",");
        if (token) strcpy(username, token);
        token = strtok(NULL, ",");
        if (token) strcpy(course, token);
        token = strtok(NULL, ",");
        if (token) strcpy(issue, token);
        token = strtok(NULL, ",");
        if (token) strcpy(ip, token);
        ip[strcspn(ip, "\n")] = '\0';

        char course_upper[100];
        strcpy(course_upper, course);
        for (int j = 0; course_upper[j]; j++) course_upper[j] = toupper(course_upper[j]);
        if (strcmp(course, mentor_course) != 0) continue;
        printf("\nIssue ID: %d\nLearner: %s\nCourse: %s\nIssue: %s\nIP: %s\n", count, username, course_upper, issue, ip);
        count++;
    }
    fclose(fp);
    set_color(15);
}

void add_comment(const char *mentor_course) {
    view_issues(mentor_course);
    int selected;
    printf("\nSelect an Issue ID to comment on: ");
    scanf("%d", &selected);
    getchar();

    FILE *fp = fopen(ISSUE_FILE, "r");
    FILE *cfp = fopen(COMMENT_FILE, "a");
    if (!fp || !cfp) {
        printf("Failed to access files.\n");
        return;
    }

    char line[512], username[50], course[100], issue[256], ip[20];
    int count = 1;
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ",");
        if (token) strcpy(username, token);
        token = strtok(NULL, ",");
        if (token) strcpy(course, token);
        token = strtok(NULL, ",");
        if (token) strcpy(issue, token);
        token = strtok(NULL, ",");
        if (token) strcpy(ip, token);
        ip[strcspn(ip, "\n")] = 0;

        if (_stricmp(course, mentor_course) != 0) continue;
        if (count == selected) {
            char comment[256];
            set_color(11);
            printf("Enter your comment: ");
            set_color(15);
            fgets(comment, 256, stdin);
            comment[strcspn(comment, "\n")] = 0;
            fprintf(cfp, "%s,%s,%s\n", username, course, comment);
            set_color(10);
            printf("Comment added successfully!\n");
            break;
        }
        count++;
    }
    fclose(fp);
    fclose(cfp);
    set_color(15);
}

void get_learner_ip(char user[MAX_USERNAME_LENGTH], char ip[20]) {
    char courses[100], users[MAX_USERNAME_LENGTH], issues[256], lines[512];
    FILE* fp = fopen(ISSUE_FILE, "r");
    if (fp) {
        while (fgets(lines, sizeof(lines), fp) != NULL) {
            sscanf(lines, "%[^,],%[^,],%[^,],%s", users, courses, issues, ip);
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

void mentor_entry() {
    char username[50], password[50], mentor_course[100];
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    getPassword(password);

    if (!validate_user(username, password, mentor_course)) {
        printf("Invalid credentials or not a mentor.\n");
        return;
    }

    int choice;
    do {
        set_color(14);
        printf("\n=== Mentor Menu ===\n");
        set_color(15);
        printf("1. View Learner Issues\n");
        printf("2. Comment on an Issue\n");
        printf("3. Chat with Learner\n");
        printf("4. Return to Main Menu\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: view_issues(mentor_course); break;
            case 2: add_comment(mentor_course); break;
            case 3: {
                //select learner like in manage_issues()
                char user[MAX_USERNAME_LENGTH], ip[20];
                view_issues(mentor_course);
                printf("\nEnter the learner username you want to chat with: ");
                scanf("%s", user);

                get_learner_ip(user, ip);       //get learner ip for network chat

                char str[512];
                char sentence[]="gcc -std=gnu11 -Wall -o chatwinver.exe chatwinver.c udp3winver.c -lws2_32";
                system(sentence);
                sprintf(str, ".\\chatwinver.exe %s", ip); 
                system(str);
                break;
            }
            case 4: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 4);
}

int main() {
    int choice;
    while (1) {
        printf("\n=== Mentor Access ===\n");
        printf("1. Sign Up\n");
        printf("2. Log In\n");
        printf("3. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            signup_user();
        } else if (choice == 2) {
            mentor_entry();
        } else if (choice == 3) {
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }
    return 0;
}