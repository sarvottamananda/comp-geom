#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
        i++;
    }

    num_pts = i;

    free(line);
}

void print_input(void) {
    printf("Input :\n");

    for (int i = 0; i < num_pts; i++) {
        printf("%g %g\n", pts[i].x, pts[i].y);
    }
}

void check_collinearity(void) {

    if (num_pts < 2) {
        printf("Too few points, num_pts = %d\n", num_pts);
        return;
    }

    struct point p1, p2, cur;
    int i = 1;

    p1 = pts[0];

    while (true) {
        cur = pts[i++];
        if (fabsf(cur.x - p1.x) + fabsf(cur.y - p1.y) > EPSILON) {
            break;
        }

        if (i == num_pts) {
            printf("Coincident\n");
            return;
        }
    }

    p2 = cur;
    i++;

    while (true) {
        cur = pts[i++];
        if (fabsf((p2.x - p1.x) * (cur.y - p1.y) - (p2.y - p1.y) * (cur.x - p1.x)) > EPSILON) {
            break;
        }

        if (i == num_pts) {
            printf("Collinear\n");
            return;
        }
    }

    printf("Non-collinear\n");
    return;
}

int main(void) {
    read_input();
    print_input();

    check_collinearity();

    exit(EXIT_SUCCESS);
}
