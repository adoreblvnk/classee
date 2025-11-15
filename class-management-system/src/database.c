#include "../include/database.h"
#include "../include/config.h"
#include "../include/utils/str_utils.h"
#include <time.h>

// forward declarations for recursive helpers (only used in this file)
static void showInOrder(const StudentRecord *root);
static void saveInOrder(const StudentRecord *root, FILE *file);
static void printRecord(const StudentRecord *record);
static StudentRecord *findMin(StudentRecord *node);

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

// find the node with the minimum id in a subtree (for deletion)
static StudentRecord *findMin(StudentRecord *node) {
  while (node && node->left != NULL) {
    node = node->left;
  }
  return node;
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
  while (i < record_count && fscanf(file, "%d\t%[^\t]\t%[^\t]\t%f\n", &records[i].id,
                                    records[i].name, records[i].programme, &records[i].mark) == 4) {
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

// insert new record to bst (sorted by id)
// O(log n) avg case. O(n) worst case if bst degenerates to a linked list
void insertRecord(StudentRecord **root, int id, const char *name, const char *programme,
                  float mark) {
  if (*root == NULL) {
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
    newNode->left = newNode->right = NULL;
    *root = newNode;
    return;
  }

  if (id < (*root)->id) {
    insertRecord(&(*root)->left, id, name, programme, mark);
  } else if (id > (*root)->id) {
    insertRecord(&(*root)->right, id, name, programme, mark);
  }
}

// search for a record by id
// O(log n) avg case, O(n) worst case
StudentRecord *queryRecord(const StudentRecord *root, int id) {
  if (root == NULL || root->id == id) { return (StudentRecord *)root; }
  if (id < root->id) { return queryRecord(root->left, id); }
  return queryRecord(root->right, id);
}

// update an existing record's fields
// O(log n) avg case, O(n) worst case
void updateRecord(StudentRecord *root, int id, const char *name, const char *programme,
                  float mark) {
  StudentRecord *record = queryRecord(root, id);
  if (record) {
    if (name) {
      strncpy(record->name, name, MAX_NAME_LEN - 1);
      record->name[MAX_NAME_LEN - 1] = '\0';
    }
    if (programme) {
      strncpy(record->programme, programme, MAX_PROG_LEN - 1);
      record->programme[MAX_PROG_LEN - 1] = '\0';
    }
    if (mark >= 0) { record->mark = mark; }
  }
}

// delete a record from the bst by id
// O(log n) avg case, O(n) worst case
StudentRecord *deleteRecord(StudentRecord **root, int id) {
  if (*root == NULL) return *root;

  if (id < (*root)->id) {
    (*root)->left = deleteRecord(&(*root)->left, id);
  } else if (id > (*root)->id) {
    (*root)->right = deleteRecord(&(*root)->right, id);
  } else {
    // node with only one child or no child
    if ((*root)->left == NULL) {
      StudentRecord *temp = (*root)->right;
      free(*root);
      return temp;
    } else if ((*root)->right == NULL) {
      StudentRecord *temp = (*root)->left;
      free(*root);
      return temp;
    }

    // node with two children: get the inorder successor (smallest in the right subtree)
    StudentRecord *temp = findMin((*root)->right);
    (*root)->id = temp->id;
    strcpy((*root)->name, temp->name);
    strcpy((*root)->programme, temp->programme);
    (*root)->mark = temp->mark;
    (*root)->right = deleteRecord(&(*root)->right, temp->id); // delete the inorder successor
  }
  return *root;
}

// comparison functions for qsort
static int compare_by_id_asc(const void *a, const void *b) {
  StudentRecord *rec_a = *(StudentRecord **)a;
  StudentRecord *rec_b = *(StudentRecord **)b;
  return rec_a->id - rec_b->id;
}

static int compare_by_id_desc(const void *a, const void *b) { return compare_by_id_asc(b, a); }

static int compare_by_mark_asc(const void *a, const void *b) {
  StudentRecord *rec_a = *(StudentRecord **)a;
  StudentRecord *rec_b = *(StudentRecord **)b;
  if (rec_a->mark < rec_b->mark) return -1;
  if (rec_a->mark > rec_b->mark) return 1;
  return 0;
}

static int compare_by_mark_desc(const void *a, const void *b) { return compare_by_mark_asc(b, a); }

// helper to populate array from bst
static void populate_array(const StudentRecord *root, StudentRecord **array, int *index) {
  if (root == NULL) return;
  populate_array(root->left, array, index);
  array[(*index)++] = (StudentRecord *)root;
  populate_array(root->right, array, index);
}

// get count of nodes in bst
static int count_nodes(const StudentRecord *root) {
  if (root == NULL) return 0;
  return 1 + count_nodes(root->left) + count_nodes(root->right);
}

// shows all records, sorted by a specific field
void showAllSorted(const StudentRecord *root, const char *sort_by, const char *sort_order) {
  if (!root) {
    printf("No records to display.\n");
    return;
  }

  int count = count_nodes(root);
  StudentRecord **records_array = malloc(count * sizeof(StudentRecord *));
  if (!records_array) {
    perror("failed to allocate memory for sorting");
    return;
  }

  int index = 0;
  populate_array(root, records_array, &index);

  int (*comparator)(const void *, const void *) = NULL;

  if (util_strcasecmp(sort_by, "ID") == 0) {
    comparator =
        (util_strcasecmp(sort_order, "DESC") == 0) ? compare_by_id_desc : compare_by_id_asc;
  } else if (util_strcasecmp(sort_by, "MARK") == 0) {
    comparator =
        (util_strcasecmp(sort_order, "DESC") == 0) ? compare_by_mark_desc : compare_by_mark_asc;
  } else {
    printf("CMS: Invalid sort key. Use 'ID' or 'MARK'.\n");
    free(records_array);
    return;
  }

  qsort(records_array, count, sizeof(StudentRecord *), comparator);

  printf("CMS: Here are all the records found in the table \"StudentRecords\".\n");
  printf("%-8s %-20s %-25s %-5s\n", "ID", "Name", "Programme", "Mark");
  for (int i = 0; i < count; i++) {
    printRecord(records_array[i]);
  }
  free(records_array);
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
