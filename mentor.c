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


void set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int validate_user(const char *username, const char *password, char *mentor_course) {
    FILE *fp = fopen("users.txt", "r");
    if (!fp) return 0;
    char line[150], u[50], p[50];
    int role;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^,],%d,%[^\n]", u, p, &role, mentor_course);
        if (strcmp(u, username) == 0 && strcmp(p, password) == 0 && role == 1) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void signup_user() {
    char username[50], password[50], course[100];
    FILE *fp = fopen("users.txt", "a");
    printf("Choose a username: ");
    scanf("%[^\n]", username);
    printf("Choose a password: ");
    getPassword(password);
    printf("Enter your IP address: ");
    printf("Which course will you mentor?: ");
    scanf(" %[^\n]", course);
    if (fp) {
        fprintf(fp, "%s,%s,1,%s\n", username, password, course);
        fclose(fp);
        printf("Signup successful! Please log in.\n");
    } else {
        printf("Failed to register.\n");
    }
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
        ip[strcspn(ip, "\n")] = 0;

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

void udp_chat(const char *learner_ip) {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in self, peer;
    int slen = sizeof(peer);
    char buf[512], message[512];
    FILE *log = fopen(CHAT_LOG_FILE, "a");

    WSAStartup(MAKEWORD(2, 2), &wsa);
    s = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&self, 0, sizeof(self));
    self.sin_family = AF_INET;
    self.sin_port = htons(8888);
    self.sin_addr.s_addr = INADDR_ANY;
    bind(s, (struct sockaddr *)&self, sizeof(self));

    memset(&peer, 0, sizeof(peer));
    peer.sin_family = AF_INET;
    peer.sin_port = htons(8889);
    peer.sin_addr.s_addr = inet_addr(learner_ip);

    printf("[ UDP Chat - Type 'quit' to leave ]\n");

    while (1) {
        printf("You: ");
        fgets(message, 512, stdin);
        sendto(s, message, strlen(message), 0, (struct sockaddr *)&peer, slen);
        if (log) fprintf(log, "Mentor: %s", message);
        if (strncmp(message, "quit", 4) == 0) break;

        int recv_len = recvfrom(s, buf, 512, 0, (struct sockaddr *)&peer, &slen);
        buf[recv_len] = '\0';
        printf("Learner: %s\n", buf);
        if (log) fprintf(log, "Learner: %s\n", buf);
    }

    if (log) fclose(log);
    closesocket(s);
    WSACleanup();
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
                int user;
                view_issues(mentor_course);
                printf("Enter the learner ID you want to chat with: ");
                scanf("%d", &user);
                char str[512];
                char sentence[]="gcc -std=gnu11 -Wall -o chatwinver.exe chatwinver.c udp3winver.c -lws2_32";
                system(sentence);
                sprintf(str, ".\\chatwinver.exe %s", ""); //needs learner IP
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