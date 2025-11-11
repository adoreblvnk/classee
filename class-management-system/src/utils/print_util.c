#include "../../include/utils/print_util.h"

void printMenu() {
    printf("Class Management System (CMS) Initialized.\n");
    printf("---------------------\n");
    printf("CMS Commands:\n");
    printf("  OPEN                - Open the database file.\n");
    printf("  SHOW ALL            - Display all student records.\n");
    printf("  SHOW SUMMARY ALL    - Display summary statistics for all courses.\n");
    printf("  SHOW SUMMARY [course_name] - Display summary statistics. Optionally filter by course.\n");
    printf("  SHOW LOG            - Display the command log.\n");
    printf("  SHOW JOURNAL        - Display the journal of changes.\n");
    printf("  INSERT              - Insert a new student record.\n"); // TODO: combine it with joseph's insert feature.
    printf("  QUERY               - Search for a student record.\n"); // TODO: add a query feature.
    printf("  UPDATE              - Update an existing student record. \n"); // TODO: add an update feature.
    printf("  DELETE              - Delete an existing student record. \n"); //TODO: add a delete feature.
    printf("  SAVE                - Save the current database to file.\n");
    printf("  RESET <change_id>   - Reset the database to a previous state based on change ID.\n");
    printf("  HELP                - Show this help message.\n");
    printf("  EXIT                - Exit the program.\n");
    printf("\n");
    printf("---------------------\n");
    printf("Type open to load the database.\n");
}