#ifndef SUMMARY_H
#define SUMMARY_H

#include "../include/cms.h"

// Define the summary results structure
struct SummaryResults {
    const StudentRecord *topper;
    const StudentRecord *lower;
    float average;
    int count;
};

// Function declarations
void getAll(const StudentRecord *head);
struct SummaryResults showTopper(const StudentRecord *head);
struct SummaryResults showAverageMark(const StudentRecord *head);

#endif