#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#define MAXPOINTS (100)

#define EPSILON (0.01)

struct point {
    float x;
    float y;
};

struct point pts[MAXPOINTS];
int num_pts = 0;

void read_input(void) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    float x;
    float y;
    int i;

    i = 0;
    while ((nread = getline(&line, &len, stdin)) != -1) {
        // printf("Retrieved line of length %zd:\n", nread);
        // fwrite(line, nread, 1, stdout);

        if (sscanf(line, "%g %g", &x, &y) != 2) {
            break;
        }

        pts[i].x = x;
        pts[i].y = y;
        ++i;
    }

    num_pts = i;

    free(line);
}

void print_input(void) {
    printf("Input :\n");
    for (int i = 0; i < num_pts; ++i) {
        printf("%g %g\n", pts[i].x, pts[i].y);
    }
    putchar('\n');
}

void print_point(char *s, struct point p) { printf("%s: %g %g\n", s, p.x, p.y); }

bool is_coincident(struct point p1, struct point p2) {
    return (fabsf(p2.x - p1.x) + fabsf(p2.y - p1.y) < EPSILON);
}

bool is_extension(struct point p, struct point n, struct point f) {
    if ((p.x - n.x) * (p.x - n.x) + (p.y - n.y) * (p.y - n.y) >
        (p.x - f.x) * (p.x - f.x) + (p.y - f.y) * (p.y - f.y))
        return false;
    if ((n.x - p.x) * (f.x - p.x) > 0)
        return true;
    if ((n.y - p.y) * (f.y - p.y) > 0)
        return true;
    return false;
}

bool is_collinear(struct point p1, struct point p2, struct point p3) {
    return (fabsf((p2.x * p3.y - p2.y * p3.x) + (p3.x * p1.y - p3.y * p1.x) +
                  (p1.x * p2.y - p1.y * p2.x)) < EPSILON);
}

bool is_left(struct point p1, struct point p2, struct point p3) {
    return (p2.x * p3.y - p2.y * p3.x) + (p3.x * p1.y - p3.y * p1.x) +
               (p1.x * p2.y - p1.y * p2.x) >
           0;
}

float angle(struct point p1, struct point p2, struct point p3) {
    float theta =  atan2f(p3.y - p2.y, p3.x - p3.x) - atan2f(p2.y - p1.y, p2.x - p1.x);
    if (theta < 0) 
        return M_PI + theta;
    else 
        return theta;
}

void check_convex(void) {

    if (num_pts < 2) {
        printf("Too few points, num_pts = %d\n", num_pts);
        return;
    }

    struct point p1, p2, cur;
    int i = 1;

    p1 = pts[0];

    p2 = pts[i];
    while (is_coincident(p1, p2)) {
        ++i;
        if (i == num_pts) {
            printf("Coincident points\n");
            return;
        }
        p2 = pts[i];
    }

    // print_point("p1",p1);

    cur = pts[i];
    while (is_collinear(p1, p2, cur)) {
        // print_point("p2",p2);
        if (is_extension(p1, p2, cur)) {
            p2 = cur;
        }
        ++i;
        if (i == num_pts) {
            printf("Collinear points\n");
            return;
        }
        cur = pts[i];
    }

    double turn = 0;
    turn += angle(p1, p2, cur);

    while (i < num_pts) {

        // print_point("p1",p1);
        // print_point("p2",p2);
        // print_point("p3",cur);
        // putchar('\n');

        if (is_left(p1, p2, cur)) {
            turn += angle(p1, p2, cur);
            if (turn > 2*M_PI + EPSILON) {
                printf("Non convex polygon\n");
                return;
            }
            p1 = p2;
            p2 = cur;
        } else if (is_collinear(p1, p2, cur) && is_extension(p1, p2, cur)) {
            p2 = cur;
        } else if (!is_coincident(p2, cur)) {
            printf("Non convex polygon\n");
            return;
        }
        ++i;
        if (i == num_pts)
            break;
        cur = pts[i];
    }

    cur = pts[0];

    if (is_coincident(p2, cur) || (is_collinear(p1, p2, cur) && is_extension(p1, p2, cur)) ||
        is_left(p1, p2, cur)) {
        printf("Convex polygon\n");
        return;
    }

    printf("Non convex polygon\n");
    return;
}

int main(void) {
    read_input();
    print_input();

    check_convex();

    exit(EXIT_SUCCESS);
}
