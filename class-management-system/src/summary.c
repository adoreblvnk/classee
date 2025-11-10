#include "../include/summary.h"
#include "../include/database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  // Add this for tolower()

// Helper function for case-insensitive comparison
void toLowerString(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Function to validate if course exists in records
int isValidCourse(const StudentRecord *root, const char *course_name) {
    if (!root || !course_name) return 0;
    
    // Convert input course name to lowercase for comparison
    char lower_course[100];
    strcpy(lower_course, course_name);
    toLowerString(lower_course);
    
    // Check current node
    char node_course[100];
    strcpy(node_course, root->programme);
    toLowerString(node_course);
    
    if (strcmp(node_course, lower_course) == 0) {
        return 1;
    }
    else if (strcmp(lower_course, "all") == 0) {
        return 1; // "all" is always valid
    }
    
    // Recursively check left and right subtrees
    return isValidCourse(root->left, course_name) || 
           isValidCourse(root->right, course_name);
}

// Modified course summary function with proper filtering
struct SummaryResults getcourseSummaryResults(const StudentRecord *root, const char *course_name) {
    struct SummaryResults results;
    results.topper = NULL;
    results.lower = NULL;
    results.average = 0.0;
    results.count = 0;
    results.course_name[0] = '\0';

    
    if (!root || !course_name) return results;
    
    float totalMarks = 0.0;
    int count = 0;
    
    // Traverse and only include students from the specified course
    traverseForCourseSummary(root, course_name, &results.topper, &results.lower, &totalMarks, &count);
    strncpy(results.course_name, course_name, sizeof(results.course_name) - 1);
    results.average = (count > 0) ? totalMarks / count : 0.0;
    results.count = count;
    
    return results;
}

// New traversal function that filters by course
void traverseForCourseSummary(const StudentRecord *node, const char *course_name,
                             const StudentRecord **topper, const StudentRecord **lower,
                             float *totalMarks, int *count) {
    if (node == NULL || course_name == NULL) return;
    
    // Convert both strings to lowercase for case-insensitive comparison
    char node_course[100];
    char target_course[100];
    
    strcpy(node_course, node->programme);
    strcpy(target_course, course_name);
    
    toLowerString(node_course);
    toLowerString(target_course);
    
    // Only process students in the specified course
    if (strcmp(node_course, target_course) == 0) {
        *totalMarks += node->mark;
        (*count)++;
        
        // Initialize topper and lower if this is the first valid student
        if (*topper == NULL) {
            *topper = node;
            *lower = node;
        } else {
            if (node->mark > (*topper)->mark) *topper = node;
            if (node->mark < (*lower)->mark) *lower = node;
        }
    }
    
    // Continue traversal
    traverseForCourseSummary(node->left, course_name, topper, lower, totalMarks, count);
    traverseForCourseSummary(node->right, course_name, topper, lower, totalMarks, count);
}

// Rest of your functions remain the same...
void displaysummary(const StudentRecord *root, const char *filter) {
    if (!root) {
        printf("No records to display.\n");
        return;
    }
    
    if (strcmp(filter, "all") == 0) {
        printf("Displaying summary for all courses:\n");
        struct SummaryResults results = getSummaryResults(root);
    
        if (results.count == 0) {
            printf("No valid records found.\n");
            return;
        }
    
        printf("Topper: %s (ID: %d) with Mark: %.2f\n", 
            results.topper->name, results.topper->id, results.topper->mark);
        printf("Lowest Scorer: %s (ID: %d) with Mark: %.2f\n", 
            results.lower->name, results.lower->id, results.lower->mark);
        printf("Average Mark: %.2f\n", results.average);
        printf("Total Students: %d\n", results.count);
    } 
    else {
        // Check if course exists before processing
        if (!isValidCourse(root, filter)) {
            printf("Error: Course '%s' not found in records.\n", filter);
            return;
        }
        struct SummaryResults results = getcourseSummaryResults(root, filter);
        if (results.count == 0) {
            printf("No students found in course '%s'.\n", filter);
            return;
        }
        printf("Displaying summary for course: %s\n", results.course_name);
        printf("Topper: %s (ID: %d) with Mark: %.2f\n", 
            results.topper->name, results.topper->id, results.topper->mark);
        printf("Lowest Scorer: %s (ID: %d) with Mark: %.2f\n", 
            results.lower->name, results.lower->id, results.lower->mark);
        printf("Average Mark: %.2f\n", results.average);
        printf("Total Students: %d\n", results.count);
    }
}

void traverseForSummary(const StudentRecord *node, const StudentRecord **topper,
                        const StudentRecord **lower, float *totalMarks, int *count) {
    if (node == NULL) return;
    
    *totalMarks += node->mark;
    (*count)++;
    
    if (*topper == NULL) {
        *topper = node;
        *lower = node;
    } else {
        if (node->mark > (*topper)->mark) *topper = node;
        if (node->mark < (*lower)->mark) *lower = node;
    }
    
    traverseForSummary(node->left, topper, lower, totalMarks, count);
    traverseForSummary(node->right, topper, lower, totalMarks, count);
}

struct SummaryResults getSummaryResults(const StudentRecord *root) {
    struct SummaryResults results;
    results.topper = NULL;
    results.lower = NULL;
    results.average = 0.0;
    results.count = 0;
    results.course_name[0] = '\0';
    
    if (!root) {
        printf("No records available to show summary.\n");
        return results;
    }

    float totalMarks = 0.0;
    int count = 0;
    
    traverseForSummary(root, &results.topper, &results.lower, &totalMarks, &count);
    
    results.average = (count > 0) ? totalMarks / count : 0.0;
    results.count = count;
    
    return results;
}