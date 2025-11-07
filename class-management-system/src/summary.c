#include "../include/summary.h"
#include "../include/cms.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

// forward declaration for recursive helper
void traverseForSummary(const StudentRecord *node, const StudentRecord **topper,
                        const StudentRecord **lower, float *totalMarks, int *count);

void traverseForSummary(const StudentRecord *node, const StudentRecord **topper,
                        const StudentRecord **lower, float *totalMarks, int *count) {
    if (node == NULL) { return; }
    
    *totalMarks += node->mark;
    (*count)++;
    
    if (node->mark > (*topper)->mark) { *topper = node; }
    if (node->mark < (*lower)->mark) { *lower = node; }
    
    traverseForSummary(node->left, topper, lower, totalMarks, count);
    traverseForSummary(node->right, topper, lower, totalMarks, count);
}

void getAll(const StudentRecord *root) {
    if (!root) {
        printf("No records to display.\n");
        return;
    }
    
    // Get summary results
    struct SummaryResults results = getSummaryResults(root);
    
    if (results.count == 0) {
        printf("No valid records found.\n");
        return;
    }
    
    // Fixed printf parameter order - name comes before ID
    printf("Topper: %s (ID: %d) with Mark: %.2f\n", 
           results.topper->name, results.topper->id, results.topper->mark);
    printf("Lowest Scorer: %s (ID: %d) with Mark: %.2f\n", 
           results.lower->name, results.lower->id, results.lower->mark);
    printf("Average Mark: %.2f\n", results.average);
    printf("Total Students: %d\n", results.count);
}

struct SummaryResults getSummaryResults(const StudentRecord *root) {
    struct SummaryResults results;
    
    // Initialize with safe defaults
    results.topper = NULL;
    results.lower = NULL;
    results.average = 0.0;
    results.count = 0;
    
    if (!root) {
        printf("No records available to show summary.\n");
        return results;
    }

    const StudentRecord *topper = root;
    const StudentRecord *lower = root;
    float totalMarks = 0.0;
    int count = 0;
    
    traverseForSummary(root, &topper, &lower, &totalMarks, &count);
    
    results.average = (count > 0) ? totalMarks / count : 0.0;
    results.count = count;
    results.topper = topper;
    results.lower = lower;
    
    return results;
}
