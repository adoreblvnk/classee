#ifndef SUMMARY_H
#define SUMMARY_H

#include "database.h"

struct SummaryResults {
    const StudentRecord *topper;
    const StudentRecord *lower;
    float average;
    int count;
    char course_name[100];
};

void displaysummary(const StudentRecord *root, const char *filter);
struct SummaryResults getSummaryResults(const StudentRecord *root);
struct SummaryResults getcourseSummaryResults(const StudentRecord *root, const char *course_name);
int isValidCourse(const StudentRecord *root, const char *course_name);
void traverseForCourseSummary(const StudentRecord *node, const char *course_name,
                             const StudentRecord **topper, const StudentRecord **lower,
                             float *totalMarks, int *count);

#endif