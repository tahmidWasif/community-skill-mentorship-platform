
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <conio.h>
#pragma comment(lib, "ws2_32.lib")

#define CHAT_LOG_FILE "chat_history.txt"

void manage_issues(const char *username);
#define ISSUE_FILE "issues.txt"
#define COMMENT_FILE "comments.txt"

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

int validate_user(const char *username, const char *password) {
    FILE *fp = fopen("users.txt", "r");
    if (!fp) return 0;
    char line[150], u[50], p[50];
    int role;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^,],%d", u, p, &role);
        if (strcmp(u, username) == 0 && strcmp(p, password) == 0 && role == 2) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void signup_user() {
    char username[50], password[50];
    FILE *fp = fopen("users.txt", "a");
    printf("Choose a username: ");
    scanf("%s", username);
    printf("Choose a password: ");
    getPassword(password);
    if (fp) {
        fprintf(fp, "%s,%s,2\n", username, password);
        fclose(fp);
        printf("Signup successful! Please log in.\n");
    } else {
        printf("Failed to register.\n");
    }
}

void submit_issue(const char *username) {
    char course[100], issue[256];
    set_color(11);
    printf("Enter course title: ");
    set_color(15);
    scanf(" %[^\n]", course);
    set_color(11);
    printf("Describe your issue: ");
    set_color(15);
    scanf(" %[^\n]", issue);

    FILE *fp = fopen(ISSUE_FILE, "a");
    if (fp) {
        fprintf(fp, "%s,%s,%s,127.0.0.1\n", username, course, issue);
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

void udp_chat(const char *mentor_ip) {
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
    self.sin_port = htons(8889);
    self.sin_addr.s_addr = INADDR_ANY;
    bind(s, (struct sockaddr *)&self, sizeof(self));

    memset(&peer, 0, sizeof(peer));
    peer.sin_family = AF_INET;
    peer.sin_port = htons(8888);
    peer.sin_addr.s_addr = inet_addr(mentor_ip);

    printf("[ UDP Chat - Type 'quit' to leave ]\n");

    while (1) {
        printf("You: ");
        fgets(message, 512, stdin);
        sendto(s, message, strlen(message), 0, (struct sockaddr *)&peer, slen);
        if (log) fprintf(log, "Learner: %s", message);
        if (strncmp(message, "quit", 4) == 0) break;

        int recv_len = recvfrom(s, buf, 512, 0, (struct sockaddr *)&peer, &slen);
        buf[recv_len] = '\0';
        printf("Mentor: %s\n", buf);
        if (log) fprintf(log, "Mentor: %s\n", buf);
    }

    if (log) fclose(log);
    closesocket(s);
    WSACleanup();
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
            case 1: submit_issue(username); break;
            case 2: view_comments(username); break;
            case 3: {
                char str[512];
                char sentence[]="gcc -std=gnu11 -Wall -o chatwinver.exe chatwinver.c udp3winver.c -lws2_32";
                system(sentence);
                sprintf(str, ".\\chatwinver.exe %s", "127.0.0.1");
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