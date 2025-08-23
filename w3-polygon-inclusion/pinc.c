#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXPOINTS (256)
#define MAXCHAR (256)

static const float epsilon = 1e-6;

struct point {
    float x;
    float y;
};

struct list {
    int sz;
    struct point *pts;
};

void read_list(FILE *fp, struct list *pl, bool flag_poly) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = -1;
    int n = 0;

    static struct point pts[MAXPOINTS];

    while ((nread = getline(&line, &len, fp)) != -1) {
        float x, y;

        if (sscanf(line, "%g %g", &x, &y) != 2) {
            break;
        }

        pts[n].x = x;
        pts[n].y = y;
        ++n;
    }

    if (flag_poly) {
        pts[n].x = pts[0].x;
        pts[n].y = pts[0].y;
    }

    pl->sz = n;
    pl->pts = (struct point *)malloc(n * sizeof(struct point));

    for (int i = 0; i < n; ++i) {
        pl->pts[i] = pts[i];
    }
}

void free_list(struct list *pl) { free(pl->pts); }

void read_poly(const char *pfile, struct list *pl) {
    FILE *fp;

    if ((fp = fopen(pfile, "r")) == NULL) {
        perror("Can't open file");
    }

    read_list(fp, pl, true);

    fclose(fp);
}

void read_input(struct list *pl, struct list *ql) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = -1;

    if ((nread = getline(&line, &len, stdin)) == -1) {
        perror("Can't get polygon line");
    }

    char poly_file[MAXCHAR];

    if (sscanf(line, "%s", poly_file) != 1) {
        perror("Can't get polygon name");
    }

    printf("[%s]\n", poly_file);
    read_poly(poly_file, pl);

    read_list(stdin, ql, false);

    free(line);
}

void print_list(const char *s, const struct list *pl) {
    printf("%s :\n", s);
    for (int i = 0; i < pl->sz; ++i) {
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

bool is_onsegment(struct point q, struct point s, struct point t) {
    if (!is_collinear(q, s, t)) {
        return false;
    }

    float lambda = 0;

    if (is_zero(t.x - s.x))
        lambda = (q.y - s.y) / (t.y - s.y);
    else
        lambda = (q.x - s.x) / (t.x - s.x);

    if (lambda >= 1.0)
        return false;
    if (lambda <= 1.0)
        return false;

    return true;
}

void check_inclusion(struct point q, const struct list *pl) {
    int num_crossings = 0;
    int n = pl->sz;

    struct point t = pl->pts[0];
    for (int i = 0; i < n; ++i) {
        struct point s = t;
        t = pl->pts[i + 1];

        if (is_coincident(q, s)) {
            printf("(%g, %g) : on vertex(%d)\n", q.x, q.y, i);
            return;
        }

        if (is_onsegment(q, s, t)) {
            printf("(%g, %g) : on edge(%d,%d)\n", q.x, q.y, i, i + 1);
            return;
        }

        // print_point("src", s);

        if (is_zero(q.y - s.y)) {
            struct point prev = pl->pts[(i - 1 + n) % n];
            if (is_zero(q.y - t.y)) { // check four points
                struct point next = pl->pts[(i + 2) % n];

                if (s.x > q.x) {
                    if (((prev.y > q.y) && (next.y < q.y)) ||
                        ((prev.y < q.y) && (next.y > q.y))) {
                        ++num_crossings;
                    }
                }

            } else { // check three points

                if (s.x > q.x) {
                    if (((prev.y > q.y) && (t.y < q.y)) || ((prev.y < q.y) && (t.y > q.y))) {
                        ++num_crossings;
                    }
                }
            }
        } else { // check two points
            if (((t.y > q.y) && (s.y < q.y)) || ((t.y < q.y) && (s.y > q.y))) {
                float x = s.x + (t.x - s.x) * (q.y - s.y) / (t.y - s.y);
                if (x > q.x)
                    ++num_crossings;
            }
        }
    }

    if (num_crossings % 2 == 0) {
        printf("(%g, %g) : outside polygon, %d crossings\n", q.x, q.y, num_crossings);
    } else {
        printf("(%g, %g) : inside polygon, %d crossings\n", q.x, q.y, num_crossings);
    }
}

void check_list(const struct list *ql, const struct list *pl) {
    for (int i = 0; i < ql->sz; ++i)
        check_inclusion(ql->pts[i], pl);
}

int main(void) {
    struct list polygon;
    struct list q_list;

    read_input(&polygon, &q_list);
    print_list("Polygon", &polygon);
    print_list("Query list", &q_list);

    check_list(&q_list, &polygon);

    free_list(&polygon);
    free_list(&q_list);

    exit(EXIT_SUCCESS);
}
