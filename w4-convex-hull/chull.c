#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "data.h"
#include "qsort.h"
#include "stack.h"

#define MAXPOINTS (256)
#define MAXCHAR (256)

static const float epsilon = 1e-6;

void read_list(struct lst_points *pl) {
    char line[MAXCHAR];
    int n = 0;

    static struct point pts[MAXPOINTS];

    while (fgets(line, sizeof(line), stdin) != NULL) {
        float x, y;

        if (sscanf(line, "%g %g", &x, &y) != 2) {
            break;
        }

        pts[n].x = x;
        pts[n].y = y;
        n++;
    }

    pl->cnt = n;
    pl->pts = (struct point *)malloc(n * sizeof(struct point));

    for (int i = 0; i < n; i++) {
        pl->pts[i] = pts[i];
    }
}

void free_list(struct lst_points *pl) { free(pl->pts); }

void print_list(const char *s, const struct lst_points *pl) {
    printf("%s, %d points :\n", s, pl->cnt);
    for (int i = 0; i < pl->cnt; i++) {
        printf("%g %g\n", pl->pts[i].x, pl->pts[i].y);
    }
    putchar('\n');
}

void print_point(const char *s, const struct point p) { printf("%s: %g %g\n", s, p.x, p.y); }

bool is_zero(float x) { return fabsf(x) < epsilon; }

bool is_coincident(struct point p1, struct point p2) {
    return is_zero(p2.x - p1.x) && is_zero(p2.y - p1.y);
}

bool is_collinear(struct point p1, struct point p2, struct point p3) {
    return is_zero((p2.x * p3.y - p2.y * p3.x) + (p3.x * p1.y - p3.y * p1.x) +
                   (p1.x * p2.y - p1.y * p2.x));
}

bool is_leftturn(struct point p1, struct point p2, struct point p3) {
    return (p2.x * p3.y - p2.y * p3.x) + (p3.x * p1.y - p3.y * p1.x) +
               (p1.x * p2.y - p1.y * p2.x) >
           0;
}

bool is_rightturn(struct point p1, struct point p2, struct point p3) {
    return ((p2.x * p3.y - p2.y * p3.x) + (p3.x * p1.y - p3.y * p1.x) +
            (p1.x * p2.y - p1.y * p2.x)) < 0;
}
void sort_list(struct lst_points *pl) { qs_sort(pl->pts, 0, pl->cnt - 1); }

void remove_dups(struct lst_points *pl) {
    if (pl->cnt < 2)
        return;

    int new_cnt = 1;

    struct point cur = pl->pts[0];
    for (int i = 1; i < pl->cnt; i++) {
        if (!is_coincident(pl->pts[i], cur)) {
            new_cnt++;
            cur = pl->pts[i];
        }
    }

    struct point *new_pts = (struct point *)malloc(new_cnt * sizeof(struct point));

    int j = 0;
    cur = pl->pts[0];
    new_pts[j++] = cur;

    for (int i = 1; i < pl->cnt; i++) {
        if (!is_coincident(pl->pts[i], cur)) {
            cur = pl->pts[i];
            new_pts[j++] = cur;
        }
    }

    free_list(pl);
    pl->pts = new_pts;
    pl->cnt = new_cnt;
}

void compute_hull_chain(struct lst_points *pl, struct lst_points *chain, bool flag_islower) {
    Stack s = s_init();
    struct point prev;
    struct point cur;

    s = s_push(s, pl->pts[0]);
    prev = pl->pts[1];

    int cnt_s = 2;

    for (int i = 2; i < pl->cnt; i++) {
        cur = pl->pts[i];

        if (is_collinear(s_top(s), prev, cur)) {
            prev = cur;
            continue;
        }

        if (flag_islower) {
            while (is_collinear(s_top(s), prev, cur) || is_rightturn(s_top(s), prev, cur)) {
                prev = s_top(s);
                s = s_pop(s);
                --cnt_s;
                if (cnt_s < 2)
                    break;
            };
        } else {
            while (is_collinear(s_top(s), prev, cur) || is_leftturn(s_top(s), prev, cur)) {
                prev = s_top(s);
                s = s_pop(s);
                --cnt_s;
                if (cnt_s < 2)
                    break;
            };
        }

        s = s_push(s, prev);
        prev = cur;
        cnt_s++;
    }

    chain->cnt = cnt_s;

    chain->pts = (struct point *)malloc(cnt_s * sizeof(struct point));

    int i = 0;

    chain->pts[i++] = cur;

    while (!s_isempty(s)) {
        chain->pts[i++] = s_top(s);
        s = s_pop(s);
    }

    // print_list("Chain", chain);
}

void compute_hull(struct lst_points *pl, struct lst_points *hull) {
    sort_list(pl);
    remove_dups(pl);

    // print_list("sorted uniq point list", pl);

    if (pl->cnt < 2) {
        printf("Two few points (%d)\n", pl->cnt);
    }

    struct lst_points lh;
    struct lst_points uh;

    compute_hull_chain(pl, &lh, true);
    compute_hull_chain(pl, &uh, false);

    hull->cnt = uh.cnt + lh.cnt - 2;
    hull->pts = (struct point *)malloc(hull->cnt * sizeof(struct point));

    int j = 0;
    for (int i = 0; i < lh.cnt; i++) {
        hull->pts[j++] = lh.pts[lh.cnt - i - 1];
    }
    for (int i = 1; i < uh.cnt - 1; i++) {
        hull->pts[j++] = uh.pts[i];
    }

    free_list(&lh);
    free_list(&uh);
}

int main(void) {
    struct lst_points pts;
    struct lst_points hull;

    read_list(&pts);
    print_list("input", &pts);

    compute_hull(&pts, &hull);
    print_list("convex hull", &hull);

    free_list(&pts);
    free_list(&hull);

    exit(EXIT_SUCCESS);
}
