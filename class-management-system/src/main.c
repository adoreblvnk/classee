#include "../include/command_parser.h"
#include "../include/utils.h"

int main() {
    StudentRecord *head = NULL;
    char input[256];
    const char *db_filename = "data/P1_1-CMS.txt"; // per project spec

    printf("Class Management System (CMS) Initialized.\n");
    printf("Type 'OPEN' to load the database or 'EXIT' to quit.\n");

    while (1) {
        printf("P1_1: ");
        if (!fgets(input, sizeof(input), stdin)) break; // handle eof

        // exit condition
        if (strncasecmp(input, "EXIT", 4) == 0) {
            break;
        }
        
        processCommand(&head, input, db_filename);
    }

    freeList(head); // free mem before exit
    printf("CMS: Shutting down. All data has been cleared from memory.\n");
    return 0;
}
