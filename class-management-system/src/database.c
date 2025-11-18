#include "../include/database.h"
#include "../include/config.h"
#include <time.h>
#include <stdbool.h>

// forward declarations for recursive helpers (only used in this file)
static void showInOrder(const StudentRecord *root);
static void saveInOrder(const StudentRecord *root, FILE *file);
static void printRecord(const StudentRecord *record);

// helper for showall
static void showInOrder(const StudentRecord *root) {
  if (root != NULL) {
    showInOrder(root->left);
    printRecord(root);
    showInOrder(root->right);
  }
}

// helper for savedb
static void saveInOrder(const StudentRecord *root, FILE *file) {
  if (root != NULL) {
    saveInOrder(root->left, file);
    fprintf(file, "%d\t%s\t%s\t%.1f\n", root->id, root->name, root->programme, root->mark);
    saveInOrder(root->right, file);
  }
}

// print single student record, formatted
static void printRecord(const StudentRecord *record) {
  if (record) {
    printf("%-8d %-20s %-25s %-5.1f\n", record->id, record->name, record->programme, record->mark);
  }
}

// shuffles records array in place. This prevents BST degenerating into a linked list
static void fisher_yates_shuffle(StudentRecord records[], int n) {
  if (n > 1) {
    for (int i = n - 1; i > 0; i--) {
      int j = rand() % (i + 1); // get random index from 0 to i
      StudentRecord temp = records[i];
      records[i] = records[j];
      records[j] = temp;
    }
  }
}

// free all mem used by bst
void freeTree(StudentRecord *root) {
  if (root == NULL) { return; }
  freeTree(root->left);
  freeTree(root->right);
  free(root);
}

// core logic

// open db file & load records to bst
void openDatabase(StudentRecord **root, const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    // if file doesn't exist, create it with a default header
    FILE *new_file = fopen(filename, "w");
    if (new_file) {
      fprintf(new_file, "Database Name: INF1002-CMS\n");
      fprintf(new_file, "Authors: \n\n");
      fprintf(new_file, "Table Name: StudentRecords\n");
      fprintf(new_file, "ID\tName\tProgramme\tMark\n");
      fclose(new_file);
      printf("CMS: The database file \"%s\" was not found. A new one has been created.\n",
             filename);
    } else {
      perror("CMS: Error creating database file");
    }
    return;
  }

  // free existing tree before loading new stuff
  freeTree(*root);
  *root = NULL;

  char line[256];
  // skip the 5 header lines & check for read errors
  for (int i = 0; i < 5; i++) {
    if (fgets(line, sizeof(line), file) == NULL) {
      printf("CMS: Error reading header from database file or file is malformed.\n");
      fclose(file);
      return;
    }
  }

  // create a temp array to shuffle records before inserting to BST
  long start_pos = ftell(file); // not 0 cah we read header lines
  int record_count = 0;
  while (fgets(line, sizeof(line), file)) {
    record_count++;
  }

  if (record_count == 0) {
    fclose(file);
    printf("CMS: The database file \"%s\" is open, but contains no records.\n", filename);
    return;
  }

  // allocate memory for temp array
  StudentRecord *records = (StudentRecord *)malloc(record_count * sizeof(StudentRecord));
  if (!records) {
    fclose(file);
    perror("failed to allocate memory for records");
    return;
  }

  fseek(file, start_pos, SEEK_SET); // reset file ptr

  // parse file & insert to bst, %[^\t] is a scanset that reads until tab
  // NOTE: must be separated by tabs!
  int i = 0;
  while (i < record_count && fscanf(file, "%d\t%[^\t]\t%[^\t]\t%f\n", &records[i].id, records[i].name, records[i].programme, &records[i].mark) == 4) {
    i++;
  }
  fclose(file);

  // shuffle array to balance BST during initial insertion
#if SHUFFLE
  srand(time(NULL));
  fisher_yates_shuffle(records, record_count);
#endif

  // insert records from shuffled array
  for (i = 0; i < record_count; i++) {
    insertRecord(root, records[i].id, records[i].name, records[i].programme, records[i].mark);
  }

  free(records); // free temp arr
  printf("CMS: The database file \"%s\" is successfully opened.\n", filename);
}

// display all records in bst (in-order traversal)
// O(n). must visit every node once for traversal
void showAll(const StudentRecord *root) {
  printf("CMS: Here are all the records found in the table \"StudentRecords\".\n");
  printf("%-8s %-20s %-25s %-5s\n", "ID", "Name", "Programme", "Mark");
  if (!root) {
    printf("No records to display.\n");
  } else {
    showInOrder(root);
  }
}

StudentRecord* findStudent(const StudentRecord *root, int id) {
    if (root == NULL) return NULL; // empty or reached end of root
    if (root -> id == id) return (StudentRecord*) root;  // current root -> id == id

    // start searching left
    StudentRecord *left = findStudent(root->left, id); // recursively find until root -> id == id for left node
    if (left != NULL) return left;

    return findStudent(root->right, id); // recursively find until root -> id == id for right node
}

void queryStudent(const StudentRecord *root, int id) {
    const StudentRecord* student = findStudent(root, id);
    if(student){
        printRecord(student);
        return;
    }
    printf("Student %d is not found in the database.\n", id);
}

bool studentExist(const StudentRecord *root, int id){
    if (root == NULL) return false;
    if (root->id == id) return true;
    // recursively searches the left and right of each node to find id
    return studentExist(root->left, id) || studentExist(root->right, id);
}

// insert new record to bst (sorted by id)
// O(log n) avg case. O(n) worst case if bst degenerates to a linked list (eg every new id is lower
// than root id)
void insertRecord(StudentRecord **root, int id, const char *name, const char *programme,
                  float mark) {
  // NOTE: check if record already exists
  if (*root == NULL) {
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
    newNode->left = newNode->right = NULL;
    *root = newNode;
    return;
  }

  // insert left if id < root->id, right if id > root->id
  if (id < (*root)->id) {
    insertRecord(&(*root)->left, id, name, programme, mark);
  } else if (id > (*root)->id) {
    insertRecord(&(*root)->right, id, name, programme, mark);
  }
  // else, id already exists, do nothing
}

// save all records from bst to db
// O(n). must visit every node once to write to file
void saveDatabase(const StudentRecord *root, const char *filename) {
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

  saveInOrder(root, file);

  fclose(file);
  printf("CMS: The database file \"%s\" is successfully saved.\n", filename);
}
