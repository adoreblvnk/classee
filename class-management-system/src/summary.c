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
    
    // Get topper and lower information
    struct SummaryResults topperResults = showTopper(head);
    
    // Get average information  
    struct SummaryResults avgResults = showAverageMark(head);
    
    printf("Topper: %s (ID: %d) with Mark: %.2f\n", 
           topperResults.topper->name, topperResults.topper->id, topperResults.topper->mark);
    printf("Lowest Scorer: %s (ID: %d) with Mark: %.2f\n", 
           topperResults.lower->name, topperResults.lower->id, topperResults.lower->mark);
    printf("Average Mark: %.2f\n", avgResults.average);
    printf("Total Students: %d\n", avgResults.count);
}

struct SummaryResults showTopper(const StudentRecord *head) {
    struct SummaryResults results;
    
    if (!head) {
        printf("No records available to determine the topper.\n");
        results.topper = NULL;
        results.lower = NULL;
        return results;
    }

    const StudentRecord *topper = head;
    const StudentRecord *lower = head; 
    
    for (const StudentRecord *current = head->next; current != NULL; current = current->next) {
        if (current->mark > topper->mark) {
            topper = current;
        }
        if (current->mark < lower->mark) {
            lower = current;
        }
    }
    
    results.topper = topper;
    results.lower = lower;
    return results;
}

struct SummaryResults showAverageMark(const StudentRecord *head) {
    struct SummaryResults results;
    
    if (!head) {
        printf("No records available to calculate average mark.\n");
        results.average = 0.0;
        results.count = 0;
        return results;
    }
    float totalMarks = 0.0;
    int count = 0;
    for (const StudentRecord *current = head; current != NULL; current = current->next) {
        totalMarks += current->mark;
        count++;
    }
    results.average = totalMarks / count;
    results.count = count;
    return results;
}