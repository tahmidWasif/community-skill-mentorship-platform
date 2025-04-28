#include "safeGitPush.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int safeGitPush() {
    char cmd[128];
    int push_failed, status;

    // build: git push origin liveDB 2>&1
    snprintf(cmd, sizeof(cmd), "git push origin liveDB 2>&1");

    do {
        push_failed = 0;
        FILE *fp = popen(cmd, "r");
        if (!fp) return -1;

        char line[512];
        while (fgets(line, sizeof(line), fp)) {
            fputs(line, stdout);
            if (strstr(line, "non-fast-forward") ||
                strstr(line, "rejected") ||
                strstr(line, "etch first") ||
                strstr(line, "git pull")) {
                push_failed = 1;
            }
        }
        status = pclose(fp);
        // If push error (and it was due to updates), pull and retry:
        if (push_failed || status != 0) {
            printf("\nUpdating database failed. Try again.\n\n");
            return 0;
        }
    } while (push_failed || status != 0);

    return 1;
}
