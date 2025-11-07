#include "../include/cms.h"
#include "../include/utils.h"

// core logic

// open db file & load records to linked list
void openDatabase(StudentRecord **head, const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("CMS: The database file \"%s\" could not be opened. A new one will be created upon "
           "saving.\n",
           filename);
    return;
  }

  // free existing list before loading new stuff
  freeList(*head);
  *head = NULL;

  char line[256];
  // skip the 5 header lines (db name, authors, blank line, table name, col headers)
  for (int i = 0; i < 5; i++) {
    fgets(line, sizeof(line), file);
  }

  int id;
  char name[MAX_NAME_LEN], programme[MAX_PROG_LEN];
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
  if (!head) {
    printf("No records to display.\n");
  } else {
    for (const StudentRecord *current = head; current != NULL; current = current->next) {
      printRecord(current);
    }
  }
}

// insert new record to linked list (sorted by id)
void insertRecord(StudentRecord **head, int id, const char *name, const char *programme,
                  float mark) {
  // NOTE: check if record already exists

  StudentRecord *newNode = (StudentRecord *)malloc(sizeof(StudentRecord));
  // if malloc returns NULL, then malloc failed
  if (!newNode) {
    perror("Failed to allocate memory for new record");
    return;
  }
  newNode->id = id;
  // safe copy, if we don't add \0, then it's not a valid string & will keep copying forever!
  strncpy(newNode->name, name, MAX_NAME_LEN - 1);
  newNode->name[MAX_NAME_LEN - 1] = '\0';
  strncpy(newNode->programme, programme, MAX_PROG_LEN - 1);
  newNode->programme[MAX_PROG_LEN - 1] = '\0';
  newNode->mark = mark;

  // insert new node in correct pos
  // if linked list is empty or new id is smaller than head id, then make this id the new head
  if (*head == NULL || (*head)->id > id) {
    newNode->next = *head;
    *head = newNode;
  } else {
    StudentRecord *current = *head;
    // skip to next node if we're not at last node & new id is greater than curr id of StudentRecord
    while (current->next != NULL && current->next->id < id) {
      current = current->next;
    }
    // insert new node after current (current -> new -> next)
    newNode->next = current->next;
    current->next = newNode;
  }
}

// save all records from linked list to db
void saveDatabase(const StudentRecord *head, const char *filename) {
  char header[5][256]; // 2D array is like a matrix
  int header_line_no = 0;

  // read the headers into header[5][256] which is 5 lines
  FILE *file = fopen(filename, "r");
  if (file) {
    while (header_line_no < 5 && fgets(header[header_line_no], 256, file)) {
      header_line_no++;
    }
    fclose(file);
  }
  // if header lines is not 5 then exit with error
  if (header_line_no != 5) {
    printf("CMS: The database file \"%s\" is not a valid database file.\n", filename);
    return;
  }

  // NOTE: write mode erases curr file contents!
  file = fopen(filename, "w");
  if (!file) {
    perror("CMS: Error opening file for saving");
    return;
  }

  // write the preserved header back to file
  for (int i = 0; i < 5; i++) {
    fputs(header[i], file);
  }

  // write the student records for each node from linked list
  for (const StudentRecord *current = head; current != NULL; current = current->next) {
    fprintf(file, "%d\t%s\t%s\t%.1f\n", current->id, current->name, current->programme,
            current->mark);
  }

  fclose(file);
  printf("CMS: The database file \"%s\" is successfully saved.\n", filename);
}
