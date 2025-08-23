#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "common.h"
#include "io.h"

void read_list(char *fname, PointList &list) {
    using namespace std;

    ifstream file(fname);
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            //cout << line << endl;

            istringstream is(line);

            Point p;

            is >> p.x >> p.y;
            list.push_back(p);
        }
        file.close();
    } else {
        cerr << "Unable to open file!" << endl;
        exit(1);
    }
}

void print_point(const char *caption, const Point &p) {
    using std::cout;
    if (caption == nullptr) {
        cout << p.x << ' ' << p.y << '\n';
    } else {
        cout << caption << " : " << p.x << ' ' << p.y << '\n';
    }
}

void print_list(const char *caption, const PointList &list) {
    using std::cout;

    cout << caption << ":\n";

    for (auto &p : list) {
        print_point((char *)nullptr, p);
    }
}

/**
 * @brief Read polygon file and set of points file
 *
 * Reads the polygon input file and the points input file.
 * We assume that the polygon is convex.
 *
 * The format of the files is as follows.
 * Each line contains a point, which is represented by two reals separated by a blank.
 * The input is terminated by a blank line.
 *
 * We assume that input is valid.
 * We do not validate the input.
 *
 * @param polygon_fname: name of the file containing a polygon
 * @param set_fname: name of the file containing a set of points
 *
 * @param poly: storage for polygon
 * @param pts: storage for query points
 *
 * @return nothing.
 */
void read_input(char *poly_fname, char *pts_fname, PointList &poly, PointList &pts) {
    read_list(poly_fname, poly);

    //print_list("Polygon", poly);

    read_list(pts_fname, pts);
    //print_list("Points", pts);
}
