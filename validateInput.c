#include "validateInput.h"
#include <stdio.h>
#include <string.h>

int getValidatedInteger() {
    char buffer[100];
    int value;
    char extra;

    while (1) { 
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error. Try again.\n");
            continue;
        }

        // Try to parse one integer and check if there's extra junk after it
        if (sscanf(buffer, "%d %c", &value, &extra) == 1) {
            return value;
        } else {
            return -1;
        }
    }
}
