#include <algorithm>
#include <iostream>

#include "common.h"
#include "io.h"
#include "search.h"

namespace args {

class Options {
  public:
    char *pfile;
    char *sfile;
};

char *get_option(char **begin, char **end, const std::string &option) {
    char **itr = std::find(begin, end, option);
    if (itr != end && ++itr != end) {
        return *itr;
    }
    return 0;
}

bool has_option(char **begin, char **end, const std::string &option) {
    return std::find(begin, end, option) != end;
}

void parse_args(int argc, char *argv[], Options &options) {
    using std::cerr;
    using std::cout;

    if (has_option(argv, argv + argc, "-h")) {

        // Usage
        cout << "Usage: " << argv[0]
             << " -p PFILE -s SFILE\n"
                "Program to check if points in file SFILE are inside polygon in PFILE\n"
                "Options:\n"
                "  -p PFILE\tPolygon file\n"
                "  -s SFILE\tQuery points file\n";
        exit(0);
    }

    char *p_fname = get_option(argv, argv + argc, "-p");
    char *s_fname = get_option(argv, argv + argc, "-s");

    if (p_fname && s_fname) {
        options.pfile = p_fname;
        options.sfile = s_fname;
    } else {
        cerr << "No valid polygon and query points given.\n";
        exit(1);
    }
}

} // namespace args

int main(int argc, char *argv[]) {
    args::Options options;

    args::parse_args(argc, argv, options);
    //std::cout << options.pfile << ' ' << options.sfile << std::endl;

    PointList cp;
    PointList set;

    read_input(options.pfile, options.sfile, cp, set);

    PointList uc, lc;

    pre_process(cp, uc, lc);

    search(set, uc, lc);

    exit(0);
}
