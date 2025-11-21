#indef SORTING_H
#define SORTING_H

#include "types.h"

typedef enum {
    SORT_BY_ID,
    SORT_BY_MARK
} SortOrder;

void SortRecords(StudentRecord **array, int count, SortField field, SortOrder order);
void flattenTree(const StudentRecord *rootm StudentRecord ***arr, int *size);

#endif