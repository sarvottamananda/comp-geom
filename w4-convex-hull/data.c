#include <stdio.h>

#include "data.h"
#include "qsort.h"

int pt_cmp(qs_type p, qs_type q) {
    if (p.x < q.x)
        return -1;
    else if (p.x > q.x)
        return +1;
    else if (p.y < q.y)
        return -1;
    else if (p.y > q.y)
        return +1;
    else
        return 0;
}

void pt_print(qs_type p) { printf("(%g, %g)", p.x, p.y); }
