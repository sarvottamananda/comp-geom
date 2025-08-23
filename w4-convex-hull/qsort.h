#ifndef QSORT_H
#define QSORT_H

#include "data.h"

typedef struct point qs_type;

int qs_cmp(qs_type, qs_type);
void qs_sort(qs_type[], int, int);

#endif
