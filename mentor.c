#include "mentor.h"
#include "getPassword.h"
#include "setColor.h"
#include "validateInput.h"
#include "safeGitPush.h"

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

//function prototypes
void get_learner_ip(char [], char []);
void learner_chat();

int validate_mentor(const char *username, const char *password, char *mentor_course) {
    system("git pull origin liveDB");
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

void signup_mentor() {
    system("git pull origin liveDB");
    //system("cls");

    char username[50], password[50], course[100], ip[20];
    FILE *fp = fopen(MENTOR_FILE, "a+");
    signUp:
    printf("Choose a username: ");
    fgets(username,sizeof(username),stdin);
    username[strcspn(username, "\n")] = '\0';
    //validate if username already exists
    char line[512], u[MAX_USERNAME_LENGTH], p[MAX_PASSWORD_LENGTH];
    rewind(fp);
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^\n]", u, p);
        if (strcmp(u, username) == 0) {
            system("cls");
            printf("\nUsername already exists.\n\n");
            goto signUp;
        }
    }
    printf("Choose a password: ");
    getPassword(password);
    printf("Enter your IP address: ");
    fgets(ip,sizeof(ip),stdin);
    ip[strcspn(ip, "\n")] = '\0';
    printf("Which course will you mentor?: ");
    fgets(course,sizeof(course),stdin);
    course[strcspn(course, "\n")] = '\0';
    if (fp) {
        fprintf(fp, "%s,%s,%s,%s\n", username, password, course, ip);
        printf("Signup successful! Please log in.\n");
    } else {
        printf("Failed to register.\n");
    }
    fclose(fp);
    system("git commit -m \"Update mentors.txt\" mentors.txt");
    if (!safeGitPush()) {
        remove(MENTOR_FILE);
        system("git commit -am \"removing mentors.txt\"");
        system("git pull origin liveDB");
        system("git add mentors.txt");
        system("git commit -am \"Resolving merge conflict\"");
        //system("cls");
        return;
    }
}

void view_issues(const char *mentor_course) {
    system("git pull origin liveDB");
    //system("cls");
    FILE *fp = fopen(ISSUE_FILE, "r");
    if (!fp) {
        printf("No issues found.\n");
        return;
    }

    char line[512], username[50], course[100], issue[256], ip[20], id[3];
    int count = 1;
    char course_upper[100];
    // course upper for display
    strcpy(course_upper, mentor_course);
    for (int j = 0; course_upper[j]; j++) {
        course_upper[j] = toupper(course_upper[j]);
    }

    printf("\n=== LEARNER ISSUES FOR COURSE: %s ===\n", course_upper);
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^\n]", id, username, course, issue, ip);

        if (_stricmp(course, mentor_course) != 0) continue;
        printf("\nIssue ID: %d\nLearner: %s\nCourse: %s\nIssue: %s\nIP: %s\n", count, username, course_upper, issue, ip);
        count++;
    }
    fclose(fp);
    set_color(15);
}

void add_comment(const char *mentor_course, const char* mentorUsername) {
    system("git pull origin liveDB");
    //system("cls");

    view_issues(mentor_course);
    int selected;
    printf("\n=== Select an Issue ID to comment on ===\n\n ");
    printf("Enter your choice: ");
    selected = getValidatedInteger();

    FILE *fp = fopen(ISSUE_FILE, "r");
    FILE *cfp = fopen(COMMENT_FILE, "a");
    if (!fp || !cfp) {
        printf("Failed to access files.\n");
        return;
    }

    char line[512], username[50], course[100], issue[256], ip[20];
    int count = 1, id;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d,%[^,],%[^,],%[^,],%[^\n]", &id, username, course, issue, ip);

        if (_stricmp(course, mentor_course) != 0) continue;
        if (count == selected) {
            char comment[256];
            set_color(11);
            printf("Enter your comment: ");
            set_color(15);
            fgets(comment, 256, stdin);
            comment[strcspn(comment, "\n")] = '\0';

            for (int i = 0; course[i] != '\0'; i++){
                course[i] = toupper(course[i]);
            }
            fprintf(cfp, "%d,%s,%s,%s,%s\n", id, username, course, comment, mentorUsername);
            set_color(10);
            printf("Comment added successfully!\n");
            break;
        }
        count++;
    }
    if (count != selected){
        system("cls");
        printf("\nInvalid choice\n\n");
    }
    fclose(fp);
    fclose(cfp);
    system("git commit -m \"Update comments.txt\" comments.txt");
    if (!safeGitPush()) {
        remove(COMMENT_FILE);
        system("git commit -am \"removing comments.txt\"");
        system("git pull origin liveDB");
        system("git add comments.txt");
        system("git commit -am \"Resolving merge conflict\"");
        //system("cls");
        return;
    }
    //system("cls");
    set_color(BRIGHT_WHITE);
}

void learner_chat(const char* mentor_course){
    system("git pull origin liveDB");
    char ip[20], user[MAX_USERNAME_LENGTH];
    
    //view mentor list according to Course
    view_issues(mentor_course);
    //select mentor to chat with
    printf("\nEnter learner username (0 to exit): ");
    fgets(user, sizeof(user), stdin);
    user[strcspn(user, "\n")] = '\0';   //removes \n

    if (strcmp(user, "0") == 0){
        printf("Exiting...\n");
        return;
    }

    char courses[100], users[MAX_USERNAME_LENGTH], issues[256], lines[512], tempIp[20], garbage[3];
    FILE* fp = fopen(ISSUE_FILE, "r");
    if (fp) {
        while (fgets(lines, sizeof(lines), fp) != NULL) {
            sscanf(lines, "%[^,],%[^,],%[^,],%[^,],%[^\n]", garbage, users, courses, issues, tempIp);
            if (strcmp(user, users) == 0){
                fclose(fp);
                get_learner_ip(user, ip);
                if (!(7 <= strlen(ip) && strlen(ip) <= 15)) {
                    system("cls");
                    printf("Learner IP cannot be retrieved.\nExiting network chat...\n\n");
                    return;
                }

                char str[512];
                char sentence[]="gcc -std=gnu11 -Wall -o chatwinver.exe chatwinver.c udp3winver.c -lws2_32";
                system(sentence);
                system("cls");
                printf("\nConnecting with learner: %s...\n", user);
                sprintf(str, ".\\chatwinver.exe %s", ip);      //needs learner IP to establish chat
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


void get_learner_ip(char user[MAX_USERNAME_LENGTH], char ip[20]) {
    system("git pull origin liveDB");
    char courses[100], users[MAX_USERNAME_LENGTH], issues[256], lines[512], tempIp[20], garbage[3], lastUser[MAX_USERNAME_LENGTH];
    FILE* fp = fopen(ISSUE_FILE, "r");
    if (fp) {
        while (fgets(lines, sizeof(lines), fp)) {
            sscanf(lines, "%[^,],%[^,],%[^,],%[^,],%[^\n]", garbage, users, courses, issues, tempIp);
            if (strcmp(user, users) == 0){
                strcpy(ip, tempIp);
                strcpy(lastUser, users);
            }
        }
        fclose(fp);
        if (strcmp(user, lastUser) == 0) return;
        system("cls");
        printf("Invalid username\n\n");
        return;

    }
    else {
        printf("Failed to access files...\n\n");
    }

}

void mentor_entry() {
    system("git pull origin liveDB");
    //system("cls");

    char username[50], password[50], mentor_course[100];
    printf("Username: ");
    fgets(username,sizeof(username),stdin);
    username[strcspn(username, "\n")] = '\0';
    printf("Password: ");
    getPassword(password);

    if (!validate_mentor(username, password, mentor_course)) {
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
        printf("Enter your choice: ");
        choice = getValidatedInteger();

        switch (choice) {
            case 1: system("cls"); view_issues(mentor_course); break;
            case 2: system("cls"); add_comment(mentor_course, username); break;
            case 3: system("cls"); learner_chat(mentor_course); break;
            case 4: 
                printf("\nReturning to main menu"); 
                for (int i = 0; i < 3; i++){
                    Sleep(300);
                    printf(".");
                }
                Sleep(300);
                system("cls");
                break;
            default: 
                system("cls"); 
                printf("Invalid choice.\n\n");
        }
    } while (choice != 4);
}

int mainMentor() {
    system("git pull origin liveDB");
    int choice;
    while (1) {
        printf("\n=== Mentor Access ===\n");
        printf("1. Sign Up\n");
        printf("2. Log In\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        choice = getValidatedInteger();

        if (choice == 1) {
            signup_mentor();
        } else if (choice == 2) {
            mentor_entry();
        } else if (choice == 3) {
            printf("Exiting");
            for (int i = 0; i < 3; i++){
                Sleep(300);
                printf(".");
            }
            Sleep(300);
            system("cls");
            break;
        } else {
            system("cls");
            printf("Invalid choice.\n");
        }
    }
    return 0;
}

