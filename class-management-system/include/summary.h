#ifndef SUMMARY_H
#define SUMMARY_H

#include "types.h"

struct SummaryResults {
    const StudentRecord *topper;
    const StudentRecord *lower;
    float average;
    int count;
};

void getAll(const StudentRecord *head);
struct SummaryResults getSummaryResults(const StudentRecord *head);

#endif