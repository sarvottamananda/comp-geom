#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "qsort.h"

int qs_cmp(qs_type x, qs_type y) { return pt_cmp(x, y); }

int partition(qs_type a[], int lo, int hi) {
    bool flag = false;

    int r = lo + rand() % (hi - lo + 1);

    qs_type pivot = a[r];
    a[r] = a[lo];
    a[lo] = pivot;

    int i = lo;
    int j = hi;

    while (i < j) {
        if (flag) {
            while (qs_cmp(a[i], pivot) < 0)
                i++;
            flag = false;
            a[j--] = a[i];
            a[i] = pivot;
        } else {
            while (qs_cmp(a[j], pivot) > 0)
                --j;
            flag = true;
            a[i++] = a[j];
            a[j] = pivot;
        }
    }

    if (flag) {
        a[j] = pivot;
        return j;
    } else {
        a[i] = pivot;
        return i;
    }
}

void qs_sort(qs_type a[], int lo, int hi) {
    if (lo < hi) {
        int mid = partition(a, lo, hi);

        qs_sort(a, lo, mid - 1);
        qs_sort(a, mid + 1, hi);
    }
}
