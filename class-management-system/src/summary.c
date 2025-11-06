
#include "../include/summary.h"
#include <stdio.h>
#include <stdlib.h>
#include "../include/cms.h"
#include "../include/utils.h"

void getAll(const StudentRecord *head) {
    if (!head) {
        printf("No records to display.\n");
        return;
    }
    
    // Get summary results
    struct SummaryResults results = getSummaryResults(head);
    
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

struct SummaryResults getSummaryResults(const StudentRecord *head) {
    struct SummaryResults results;
    
    // Initialize with safe defaults
    results.topper = NULL;
    results.lower = NULL;
    results.average = 0.0;
    results.count = 0;
    
    if (!head) {
        printf("No records available to show summary.\n");
        return results;
    }

    const StudentRecord *current = head;
    const StudentRecord *topper = head;
    const StudentRecord *lower = head; 
    float totalMarks = 0.0;
    int count = 0;
    
    // Traverse all nodes including head
    while (current != NULL) {
        totalMarks += current->mark;
        count++;
        
        if (current->mark > topper->mark) {
            topper = current;
        }
        if (current->mark < lower->mark) {
            lower = current;
        }
        
        current = current->next;
    }
    
    results.average = (count > 0) ? totalMarks / count : 0.0;
    results.count = count;
    results.topper = topper;
    results.lower = lower;
    
    return results;
}