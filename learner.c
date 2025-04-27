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

#define ISSUE_FILE "issues.txt"
#define COMMENT_FILE "comments.txt"


#define MAX_PASSWORD_LENGTH 50
#define MAX_USERNAME_LENGTH 50

//function prototype
void manage_issues(const char *username);
void get_mentor_ip(char user[MAX_USERNAME_LENGTH], char ip[20]);
void mentor_chat();
void view_mentors();

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
    system("git pull origin chat");
    //system("cls");
    char username[50], password[50], ip[20];
    FILE *fp = fopen(LEARNER_FILE, "a+");
    signUp:
    printf("Choose a username: ");
    scanf("%s", username);
    //validate if username already exists
    char line[512], u[MAX_USERNAME_LENGTH], p[MAX_PASSWORD_LENGTH];
    rewind(fp);
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^\n]", u, p);
        if (strcmp(u, username) == 0) {
            printf("\nUsername already exists.\n\n");
            goto signUp;
        }
    }
    printf("Choose a password: ");
    getPassword(password);

    if (fp) {
        fprintf(fp, "%s,%s\n", username, password);
        fclose(fp);

        system("git commit -m \"Update learners.txt\" learners.txt");
        system("git push origin chat");
        // system("cls");
        printf("Signup successful! Please log in.\n");
    } else {
        printf("Failed to register.\n");
    }
}

void submit_issue(const char *username) {
    system("git pull origin chat");
    system("cls");
    char course[100], issue[256], ip[20];
    set_color(LIGHT_AQUA);
    printf("Enter course title: ");
    scanf(" %[^\n]", course);
    printf("Describe your issue: ");
    scanf(" %[^\n]", issue);
    printf("Enter your IP address: ");
    set_color(BRIGHT_WHITE);
    scanf(" %[^\n]", ip);
    
    FILE *fp = fopen(ISSUE_FILE, "a+");
    if (fp) {
        char lines[512], user[MAX_USERNAME_LENGTH], readCourse[100], readIssue[256], readIp[20];
        int usernameCount = 1, count;
        while (fgets(lines, sizeof(lines), fp) != NULL){
            sscanf(lines, "%d,%[^,],%[^,],%[^,],%[^\n]", &count, user, readCourse, readIssue, readIp);
            if (strcmp(user, username) == 0){
                usernameCount = count + 1;
            }
        }
        fprintf(fp, "%d,%s,%s,%s,%s\n", usernameCount, username, course, issue, ip);       //ip address needs to be changed
        fclose(fp);
        // updating file to server
        system("git commit -m \"Update issues.txt\" issues.txt");
        system("git push origin chat");
        system("cls");
        set_color(LIGHT_GREEN);
        printf("\nIssue submitted!\n");
    } else {
        set_color(LIGHT_RED);
        printf("\nFailed to submit issue.\n");
    }
    set_color(BRIGHT_WHITE);
}

void view_comments(const char *username) {
    FILE *fp = fopen(COMMENT_FILE, "r");
    if (!fp) {
        set_color(LIGHT_RED);
        printf("No comments found.\n");
        set_color(BRIGHT_WHITE);
        return;
    }

    char line[512], u[50], course[100], comment[256], mentorUsername[MAX_USERNAME_LENGTH];
    int found = 0, id;
    set_color(LIGHT_AQUA);
    printf("\n=== Comments for You ===\n\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d,%[^,],%[^,],%[^,],%[^\n]", &id, u, course, comment, mentorUsername);
        if (strcmp(u, username) == 0) {
            printf("Course: ");
            set_color(BRIGHT_WHITE);
            printf(" %s\n", course);
            set_color(LIGHT_AQUA);
            printf("Comment: ");
            set_color(BRIGHT_WHITE);
            printf("%s\n", comment);
            set_color(LIGHT_AQUA);
            printf("From: ");
            set_color(GRAY);
            printf("%s\n\n", mentorUsername);
            found = 1;
            set_color(LIGHT_AQUA);
        }
    }
    if (!found) {
        set_color(LIGHT_RED);
        printf("No mentor comments found for your submissions.\n");
    }
    fclose(fp);
    set_color(BRIGHT_WHITE);
}

void mentor_chat(){
    char ip[20], user[MAX_USERNAME_LENGTH];
    
    
    //view mentor list according to Course
    view_mentors();
    //select mentor to chat with
    printf("Enter Mentor username (0 to exit): ");
    fgets(user, sizeof(user), stdin);
    user[strcspn(user, "\n")] = '\0';   //removes \n

    if (strcmp(user, "0") == 0){
        printf("Exiting...\n");
        return;
    }

    char courses[100], users[MAX_USERNAME_LENGTH], passwords[MAX_PASSWORD_LENGTH], lines[512], tempIp[20];
    FILE* fp = fopen(MENTOR_FILE, "r");
    if (fp) {
        while (fgets(lines, sizeof(lines), fp) != NULL) {
            sscanf(lines, "%[^,],%[^,],%[^,],%[^\n]", users, passwords, courses, tempIp);
            if (strcmp(user, users) == 0){
                fclose(fp);
                get_mentor_ip(user, ip);
                if (!(7 <= strlen(ip) && strlen(ip) <= 15)) {
                    system("cls");
                    printf("Mentor IP cannot be retrieved.\nExiting network chat...\n\n");
                    return;
                }

                char str[512];
                char sentence[]="gcc -std=gnu11 -Wall -o chatwinver.exe chatwinver.c udp3winver.c -lws2_32";
                system(sentence);
                system("cls");
                printf("\nConnecting with mentor: %s...\n", user);
                sprintf(str, ".\\chatwinver.exe %s", ip);      //needs mentor IP to establish chat
                system(str);
                return;
            }
        }
        fclose(fp);
        system("cls");
        printf("Invalid username\nExiting network chat...\n\n");
        return;

    }
    else {
        printf("Failed to access files...\n");
    }
    
}

void get_mentor_ip(char user[MAX_USERNAME_LENGTH], char ip[20]) {
    char courses[100], users[MAX_USERNAME_LENGTH], passwords[MAX_PASSWORD_LENGTH], lines[512], tempIp[20];
    FILE* fp = fopen(MENTOR_FILE, "r");
    if (fp) {
        while (fgets(lines, sizeof(lines), fp) != NULL) {
            sscanf(lines, "%[^,],%[^,],%[^,],%[^\n]", users, passwords, courses, tempIp);
            if (strcmp(user, users) == 0){
                strcpy(ip, tempIp);
                fclose(fp);
                return;
            }
        }
        fclose(fp);
        system("cls");
        printf("Invalid username\n\n");
        return;

    }
    else {
        printf("Failed to access files...\n");
    }

}

void view_mentors() {
    // system("cls");
    FILE* fp = fopen(MENTOR_FILE, "r");
    char line[512], user[MAX_USERNAME_LENGTH], garbage[MAX_PASSWORD_LENGTH], course[100], garbage2[20];
    int count = 1;
    set_color(5);
    printf("\n=== Mentor List ===\n\n");
    set_color(15);
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^,],%[^,],%[^\n]", user, garbage, course, garbage2);
        printf("%d.\nMentor username: %s\nCourse: %s\n\n", count++, user, course);
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
        printf("2. View Mentor List\n");
        printf("3. View Mentor Comments\n");
        printf("4. Chat with Mentor\n");
        printf("5. View/Delete My Issues\n");
        printf("6. Return to Learner Access Menu\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: system("cls"); submit_issue(username); break;
            case 2: system("cls"); view_mentors(); break;
            case 3: system("cls"); view_comments(username); break;
            case 4: system("cls"); mentor_chat(); break;
            case 5: system("cls"); manage_issues(username); break;
            case 6: system("cls"); printf("Returning to learner access menu...\n"); break;
            default:
            int c;
            while ((c = getchar()) != '\n' && c != EOF);    //consumes leftover characters if there are any
            // system("cls");
            printf("Invalid choice.\n\n");
            break;
        }

    } while (choice != 6);
}


void manage_issues(const char *username) {
    system("git pull origin chat");
    // system("cls");
    FILE *fp = fopen(ISSUE_FILE, "r");
    if (!fp) {
        printf("No issues found.\n");
        return;
    }

    char lines[100][512];
    char users[100][50], courses[100][100], issues[100][256], ips[100][50];
    int count = 0, id[100];

    while (fgets(lines[count], sizeof(lines[count]), fp)) {
        sscanf(lines[count], "%d,%[^,],%[^,],%[^,],%s", &id[count], users[count], courses[count], issues[count], ips[count]);
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

    if (choice > 0 && choice <= count) {
        FILE* wfp = fopen("temp_issues.txt", "w");
        int deleteId;
        for (int i = 0; i < count; i++) {
            if (i != choice - 1) {
                fprintf(wfp, "%s", lines[i]);
            }
            else {
                deleteId = id[i];
                printf("Comment ID: %d\n", deleteId);
            }
        }
        FILE* cfp = fopen(COMMENT_FILE, "r");
        char commentLine[512], commentUser[MAX_USERNAME_LENGTH], mentorUser[MAX_USERNAME_LENGTH], commentCourse[100], comment[512];
        int commentId;
        FILE* temp_cfp = fopen("temp_comments.txt", "w");
        
        while (fgets(commentLine, sizeof(commentLine), cfp)){
            sscanf(commentLine, "%d,%[^,],%[^,],%[^,],%[^\n]", &commentId, commentUser, commentCourse, comment, mentorUser);
            if (strcmp(commentUser, username) == 0 && commentId == deleteId) {
                continue;
            }
            fprintf(temp_cfp, "%s", commentLine);
            
        }
        fclose(wfp);
        fclose(cfp);
        fclose(temp_cfp);
        remove(COMMENT_FILE);
        rename("temp_comments.txt", COMMENT_FILE);
        remove(ISSUE_FILE);
        rename("temp_issues.txt", ISSUE_FILE);
        system("git commit -m \"Update issues.txt\" issues.txt");
        system("git commit -m \"Update comments.txt\" comments.txt");
        system("git push origin chat");
        // system("cls");
        printf("Issue deleted.\n");
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);    //consumes leftover characters if there are any
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

        if (choice == 1) {
            signup_user();
        } else if (choice == 2) {
            learner_entry();
        } else if (choice == 3) {
            printf("Exiting...\n");
            break;
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);    //consumes leftover characters if there are any
            system("cls");
            printf("Invalid choice.\n");
        }
    }
    return 0;
}