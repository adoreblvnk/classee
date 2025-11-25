#include "../include/database.h"
#include "../include/config.h"
#include <stdbool.h>
#include <time.h>

// file scope globals for sorting
static SortField g_sortField;
static SortOrder g_sortOrder;

// Helper Funcs

// print single student record, formatted
static void printRecord(const StudentRecord *record) {
  if (record) {
    printf("%-8d %-20s %-25s %-5.1f\n", record->id, record->name, record->programme, record->mark);
  }
}

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

// find the SMALLEST id in left node
StudentRecord *findMin(StudentRecord *root) {
  while (root->left != NULL)
    root = root->left;
  return root;
}

// Public CRUD Core Logic

StudentRecord *studentExist(const StudentRecord *root, int id) {
  const StudentRecord *current = root;
  while (current != NULL) {
    if (id == current->id) {
      return (StudentRecord *)current; // found
    } else if (id < current->id) {
      current = current->left; // go left
    } else {
      current = current->right; // go right
    }
  }
  return NULL; // not found
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

void queryStudent(const StudentRecord *root, int id) {
  const StudentRecord *student = studentExist(root, id);
  if (student) {
    printf("classee: The record with ID=%d is found in the data table.\n", id);
    printf("%-8s %-20s %-25s %-5s\n", "ID", "Name", "Programme", "Mark");
    printRecord(student);
    return;
  }
  printf("classee: The record with ID=%d does not exist.\n", id);
}

// display all records in bst (in-order traversal)
// O(n). must visit every node once for traversal
void showAll(const StudentRecord *root) {
  printf("classee: Here are all the records found in the table \"StudentRecords\".\n");
  printf("%-8s %-20s %-25s %-5s\n", "ID", "Name", "Programme", "Mark");
  if (!root) {
    printf("No records to display.\n");
  } else {
    showInOrder(root);
  }
}

void updateRecord(StudentRecord *root, const char *name, const char *programme, const float mark) {
  strncpy(root->name, name, sizeof(root->name) - 1);
  root->name[sizeof(root->name) - 1] = '\0';

  strncpy(root->programme, programme, sizeof(root->programme) - 1);
  root->programme[sizeof(root->programme) - 1] = '\0';

  root->mark = mark;
  return;
}

void deleteRecord(StudentRecord **root, int id) {
  if (*root == NULL)
    return;                    // first validation
  else if (id < (*root)->id) { // recursively find left tree until found
    deleteRecord(&((*root)->left), id);
  } else if (id > (*root)->id) { // recursively find right tree until found
    deleteRecord(&((*root)->right), id);
  } else { // found
    // 3 cases
    // case 1: no child node
    if ((*root)->left == NULL && (*root)->right == NULL) {
      free((*root));
      (*root) = NULL;
    }

    // case 2: if only one child node
    else if ((*root)->left == NULL) {
      StudentRecord *p_temp = (*root); // the target node
      (*root) = (*root)->right;        // right child will be the right child of the tree
      free(p_temp);
    } else if ((*root)->right == NULL) {
      StudentRecord *p_temp = (*root); // the target node
      (*root) = (*root)->left;         // left child will be the left child of the tree
      free(p_temp);
    }

    // case 3: if node has 2 child.
    // find smallest in RIGHT node
    else {
      StudentRecord *p_temp = findMin((*root)->right);
      // copy all data from successor
      (*root)->id = p_temp->id;
      strcpy((*root)->name, p_temp->name);
      strcpy((*root)->programme, p_temp->programme);
      (*root)->mark = p_temp->mark;

      // recursively deletes the duplicated id node
      deleteRecord(&((*root)->right), p_temp->id);
    }
  }
  return;
}

// Public Database File I/O & Memory Management

// free all mem used by bst
void freeTree(StudentRecord *root) {
  if (root == NULL) { return; }
  freeTree(root->left);
  freeTree(root->right);
  free(root);
}

// open db file & load records to bst
void openDatabase(StudentRecord **root, const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    // if file doesn't exist, create it with a default header
    FILE *new_file = fopen(filename, "w");
    if (new_file) {
      fprintf(new_file, "Database Name: classee\n");
      fprintf(new_file, "Authors: \n\n");
      fprintf(new_file, "Table Name: StudentRecords\n");
      fprintf(new_file, "ID\tName\tProgramme\tMark\n");
      fclose(new_file);
      printf("classee: The database file \"%s\" was not found. A new one has been created.\n",
             filename);
    } else {
      perror("classee: Error creating database file");
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
      printf("classee: Error reading header from database file or file is malformed.\n");
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
    printf("classee: The database file \"%s\" is open, but contains no records.\n", filename);
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
  printf("classee: The database file \"%s\" is successfully opened.\n", filename);
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
    printf("classee: The database file \"%s\" is not a valid database file.\n", filename);
    return;
  }

  // NOTE: write mode erases curr file contents!
  file = fopen(filename, "w");
  if (!file) {
    perror("classee: Error opening file for saving");
    return;
  }

  // write the preserved header back to file
  for (int i = 0; i < 5; i++) {
    fputs(header[i], file);
  }

  saveInOrder(root, file);

  fclose(file);
  printf("classee: The database file \"%s\" is successfully saved.\n", filename);
}

// Public Sorting Implementation

// Count total nodes in BST
static int countNodes(const StudentRecord *node) {
  if (!node) { return 0; }
  return 1 + countNodes(node->left) + countNodes(node->right);
}

// in-order traversal to fill array
static void flattenHelper(const StudentRecord *node, StudentRecord *arr, int *index) {
  if (!node) { return; }
  flattenHelper(node->left, arr, index);
  arr[*index] = *node; // copies entire struct
  (*index)++;
  flattenHelper(node->right, arr, index);
}

/** 
 * converts the bst into a linear array.  
 * 
 * performs an in-order traversal to flatten all nodes of the tree
 * into a dynamically allocated array.
 * 
 * @param *root Pointer to root of the StudentRecord bst
 * @param *size Output parameter to store the amount of nodes
 * 
 * @return Pointer to a newly allocated array containing all records, or NULL
 * if the tree is empty or malloc fails.
*/
StudentRecord *flattenTreeToArray(const StudentRecord *root, int *size) {
  if (!size) return NULL;

  if (!root) {
    *size = 0;
    return NULL;
  }

  int count = countNodes(root);
  *size = count;

  StudentRecord *arr = (StudentRecord *)malloc(count * sizeof(StudentRecord));
  if (!arr) {
    perror("classee: Failed to allocate memory for sorting");
    *size = 0;
    return NULL;
  }

  int index = 0;
  flattenHelper(root, arr, &index);
  return arr;
}

// freeing of array due to allocated via malloc
void freeRecordArray(StudentRecord *arr) { free(arr); }


/**
 * Comparison function used by qSort to sort StudentRecord entries.
 * 
 * Sorting behavior is determined by 2 field:
 *    g_sortField: SORT_BY_ID or SORT_BY_MARK
 *    g_sortOrder: ORDER_ASC or OREDER_DESC
 * 
 * Compares two StudentRecord structs by the selected field and returns:
 *    negative value if a < b
 *    zero if equal
 *    positive value if a > b
 * 
 * @param *a Pointer to first element (StudentRecord*) for comparison
 * @param *b Pointer to second element (StudentRecord*) for compariosn
 * 
 * @return Comparison result
 */
static int compareRecords(const void *a, const void *b) {
  const StudentRecord *x = (const StudentRecord *)a;
  const StudentRecord *y = (const StudentRecord *)b;
  int cmp = 0;

  if (g_sortField == SORT_BY_ID) {
    if (x->id < y->id)
      cmp = -1;
    else if (x->id > y->id)
      cmp = 1;
    else
      cmp = 0;
  } else if (g_sortField == SORT_BY_MARK) {
    if (x->mark < y->mark)
      cmp = -1;
    else if (x->mark > y->mark)
      cmp = 1;
    else
      cmp = 0;
  }

  if (g_sortOrder == ORDER_DESC) cmp = -cmp;

  return cmp;
}

//sorts an array of StudentRecord struct using qSort
void sortRecords(StudentRecord *arr, int size, SortField field, SortOrder order) {
  if (!arr || size <= 1) return;
  g_sortField = field;
  g_sortOrder = order;
  qsort(arr, size, sizeof(StudentRecord), compareRecords);
}

void printSortedRecords(const StudentRecord *arr, int size) {
  printf("classee: Here are all the records found in the table \"StudentRecords\" (sorted).\n");
  printf("%-8s %-20s %-25s %-5s\n", "ID", "Name", "Programme", "Mark");

  if (!arr || size <= 0) {
    printf("No records to display.\n");
    return;
  }

  for (int i = 0; i < size; i++) {
    printf("%-8d %-20s %-25s %-5.1f\n", arr[i].id, arr[i].name, arr[i].programme, arr[i].mark);
  }
}
