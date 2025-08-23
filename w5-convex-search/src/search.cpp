#include <iostream>
#include <cmath>

#include "common.h"
#include "io.h"

const float epsilon = 1e-9;

bool is_lesser(const Point p, const Point q) { 
    return (p.x < q.x) || ((fabsf(p.x - q.x)<epsilon) && (p.y < q.y)); 
}

bool is_greater(const Point p, const Point q) { 
    return (p.x > q.x) || ((fabsf(p.x - q.x)<epsilon) && (p.y > q.y)); 
}

void get_chains(const PointList cp, PointList &lchain, PointList &uchain) {
    auto cur = cp.begin();
    auto prev = cur++;

    Point b, e, bb;

    while (is_lesser(*prev, *cur)) {
        prev = cur++; 
        if (cur == cp.end()) 
            cur = cp.begin();
    }
    while (!is_lesser(*prev, *cur)) {
        prev = cur++; 
        if (cur == cp.end()) 
            cur = cp.begin();
    }

    b = *prev;
    bool vert = false;
    while (is_lesser(*prev, *cur)) {
        lchain.push_back(*prev);
        if ((prev->x - cur->x) < epsilon) vert = true;
        else vert = false;
        prev = cur++; 
        if (cur == cp.end()) 
            cur = cp.begin();
    }
    if (!vert) lchain.push_back(*prev);

    e = *prev;
    while (!is_lesser(*prev, *cur)) {
        uchain.push_back(*prev);
        if ((prev->x - cur->x) < epsilon) vert = true;
        else vert = false;
        prev = cur++; 
        if (cur == cp.end()) 
            cur = cp.begin();
    }
    if (!vert) uchain.push_back(*prev);
    bb = *prev;


    //print_point("First point", b);
    //print_point("Last point", e);
    //print_point("Repeat first point", b);

    print_list("Uchain", uchain);
    print_list("Lchain", lchain);
}

void pre_process(const PointList &cp, PointList &uchain, PointList &lchain) {

    if (cp.size() < 3) {
        using std::cout;

        cout << "Polygon too small.\n";
        exit(1);
    }

    get_chains(cp, lchain, uchain);
}

enum class Incidence {below, on, above};

void print_inc(const char * cap, Incidence rel) {
    if (rel == Incidence::below) std::cout << cap << ":bl";
    if (rel == Incidence::on) std::cout << cap << ":on";
    if (rel == Incidence::above) std::cout << cap << ":ab";
}

Incidence point_incidence(Point q, Point p) {
    //print_point("checkin against point ", p);
    if (q.y > p.y + epsilon) return Incidence::above;
    if (q.y < p.y - epsilon) return Incidence::below;
    return Incidence::on;
}

Incidence line_incidence(Point q, Point s, Point t) {
    //print_point("checkin against line s", s);
    //print_point("checkin against line t", t);

    if (((q.x < s.x) && (q.x < t.x)) ||  ((q.x > s.x) && (q.x > t.x))) {
        std::cerr << "Program error in line_incidence.\n";
        //print_point("Query", q);
        //print_point("from", s);
        //print_point("to", t);
        exit(1);
    }

    if (fabsf(s.x - t.x) < epsilon) {
        if (( q.y > s.y + epsilon) && (q.y > t.y + epsilon)) return Incidence::above;
        else if (( q.y < s.y - epsilon) && (q.y < t.y - epsilon)) return Incidence::below;
        else return Incidence::on;
    }

    float m = (t.y - s.y) / (t.x - s.x);
    float c = s.y - m * s.x;

    float yy = m * q.x + c;

    //std::cout << yy << ' ' << q.y << '\n';

    if (fabsf(q.y - yy) < epsilon) return Incidence::on; 
    else if (q.y > yy) return Incidence::above;
    else return Incidence::below;
}

Incidence binary_search(const Point q, const PointList &chain, bool asc) {
    int lo = 0;
    int hi = chain.size() - 1;

    while (hi > lo + 1) {
        //std::cout <<"Rng : "<< lo << ' ' << hi << '\n';
        int mid = (hi + lo) / 2;
        if (fabsf(q.x - chain[mid].x) < epsilon) {
           //std::cout << "fabsf " << fabsf(q.x - chain[mid].x) << ' ' << q.x << ' '<< chain[mid].x << '\n'; 
           return point_incidence(q, chain[mid]);
        } else if ((asc &&(q.x < chain[mid].x)) || (!asc && (q.x > chain[mid].x))) {
            hi = mid;
        } else {
            lo = mid;
        }
    }

    if (hi == lo) {
        return point_incidence(q, chain[lo]);
    }

    if (hi == lo + 1) {
        return line_incidence(q, chain[lo], chain[hi]);
    }

    std::cerr <<"Should not reach here.\n";
    exit (1);
}

void search(const PointList &sl, const PointList &uc, const PointList &lc) {

    using std::cout;
    using std::cerr;

    for (auto &q: sl) {
        if ((q.x < lc[0].x) || (q.x > uc[0].x)) {
            cout << q.x << ' ' << q.y << ": Outside\n";
            continue;
        }

        bool onend = false;
        Point b, e;
        
        if (fabsf(q.x - lc[0].x)<epsilon) {
            b = lc[0];
            e = uc[uc.size() -1 ];

            onend = true;
        }
        if (fabsf(q.x - uc[0].x)<epsilon) {
            b = uc[0];
            e = lc[lc.size() -1 ];
            onend = true;
        }

        if (onend) {
            Incidence inc;
            if (fabsf(b.x - e.x)<epsilon) {
                inc = line_incidence(q, b, e);
            } else {
                inc = point_incidence(q, b);
            }
            if (inc == Incidence::on) {
                cout << q.x << ' ' << q.y << ": On boundary\n";
            } else {
                cout << q.x << ' ' << q.y << ": Outside\n";
            }
            continue;
        }

        //cout << q.x << ' ' << q.y << ' ';
        auto uinc = binary_search(q, uc, false);
        //print_inc(" Uinc ", uinc);
        //cout << '\n';
        auto linc = binary_search(q, lc, true);
        //print_inc(" Linc ", linc);
        //cout << '\n';


        if ((uinc == Incidence:: above) ||  (linc == Incidence:: below)) {
            cout << q.x << ' ' << q.y << ": Outside\n";
        } else if ((uinc == Incidence:: on) || (linc == Incidence::on)) {
            cout << q.x << ' ' << q.y << ": On boundary\n";
        } else {
            cout << q.x << ' ' << q.y << ": Inside\n";
        }
    }
}
