#include "../include/cms.h"
#include "../include/utils.h"

// --- core ops ---

// open db file & load records to linked list
void openDatabase(StudentRecord **head, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("CMS: The database file \"%s\" could not be opened. A new one will be created upon saving.\n", filename);
        return;
    }

    // free existing list before loading new data
    freeList(*head);
    *head = NULL;

    char line[256];
    // skip all 5 header lines, including blank one
    fgets(line, sizeof(line), file); // db name
    fgets(line, sizeof(line), file); // authors
    fgets(line, sizeof(line), file); // blank line
    fgets(line, sizeof(line), file); // table name
    fgets(line, sizeof(line), file); // col headers

    int id;
    char name[MAX_NAME_LEN];
    char programme[MAX_PROG_LEN];
    float mark;

    // parse file, %[^\t] is a scanset that reads until tab
	// NOTE: must be separated by tabs!
    while (fscanf(file, "%d\t%[^\t]\t%[^\t]\t%f\n", &id, name, programme, &mark) == 4) {
        insertRecord(head, id, name, programme, mark);
    }

    fclose(file);
    printf("CMS: The database file \"%s\" is successfully opened.\n", filename);
}

// display all records in linked list
void showAll(const StudentRecord *head) {
    printf("CMS: Here are all the records found in the table \"StudentRecords\".\n");
    printf("%-8s %-20s %-25s %-5s\n", "ID", "Name", "Programme", "Mark");
    printf("------------------------------------------------------------------\n");
    if (!head) {
        printf("No records to display.\n");
    } else {
        for (const StudentRecord *current = head; current != NULL; current = current->next) {
            printRecord(current);
        }
    }
}

// insert new record to linked list (sorted by id)
void insertRecord(StudentRecord **head, int id, const char *name, const char *programme, float mark) {
	// NOTE: check if record already exists

    StudentRecord *newNode = (StudentRecord *)malloc(sizeof(StudentRecord));
    if (!newNode) {
        perror("Failed to allocate memory for new record");
        return;
    }
    newNode->id = id;
    strncpy(newNode->name, name, MAX_NAME_LEN - 1);
    newNode->name[MAX_NAME_LEN - 1] = '\0';
    strncpy(newNode->programme, programme, MAX_PROG_LEN - 1);
    newNode->programme[MAX_PROG_LEN - 1] = '\0';
    newNode->mark = mark;

    // insert in sorted order (by id)
    if (*head == NULL || (*head)->id > id) {
        newNode->next = *head;
        *head = newNode;
    } else {
        StudentRecord *current = *head;
        while (current->next != NULL && current->next->id < id) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

// save all records from linked list to db
void saveDatabase(const StudentRecord *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("CMS: Error opening file for saving");
        return;
    }

    // format ensures file can be read back correctly
    fprintf(file, "Database Name: Sample-CMS\n");
    fprintf(file, "Authors: Your Name Here\n\n");
    fprintf(file, "Table Name: StudentRecords\n");
    fprintf(file, "ID\tName\tProgramme\tMark\n");

    for (const StudentRecord *current = head; current != NULL; current = current->next) {
        fprintf(file, "%d\t%s\t%s\t%.1f\n", current->id, current->name, current->programme, current->mark);
    }

    fclose(file);
    printf("CMS: The database file \"%s\" is successfully saved.\n", filename);
}
