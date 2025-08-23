#ifndef DATA_H
#define DATA_H

struct point {
    float x;
    float y;
};

struct lst_points {
    int cnt;
    struct point *pts;
};

extern int pt_cmp(struct point, struct point);
extern void pt_print(struct point);

#endif
