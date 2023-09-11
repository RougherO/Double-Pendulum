#include <iostream>
#include <optional>
#include <vector>
#include "App/App.hpp"

int main(int argc, char* argv[])
{
    std::vector<std::optional<double>> vals(8);
    /*
     *  Vals will be used to take in following order(1 is upper pendulum 2 is lower):-
     *  0: length1
     *  1: length2
     *  2: mass1
     *  3: mass2
     *  4: start_angle1
     *  5: start_angle2
     *  6: gravity
     *  7: trail enable or disable
     */
    double val {};
    for (int i = 1; i < argc - 1; ++i) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
            case 'L':
            case 'l':
                val = std::stod(argv[i + 1]);
                if (val > 0 && val < 500) {
                    switch (argv[i][2]) {
                    case '1':
                        vals[0].emplace(val);
                        break;
                    case '2':
                        vals[1].emplace(val);
                        break;
                    }
                }
            case 'M':
            case 'm':
                val = std::stod(argv[i + 1]);
                if (val > 0 && val < 200) {
                    switch (argv[i][2]) {
                    case '1':
                        vals[2].emplace(val);
                        break;
                    case '2':
                        vals[3].emplace(val);
                        break;
                    }
                }
            case 'A':
            case 'a':
                val = std::stod(argv[i + 1]);
                switch (argv[i][2]) {
                case '1':
                    vals[4].emplace(val);
                    break;
                case '2':
                    vals[5].emplace(val);
                    break;
                }
            case 'G':
            case 'g':
                val = std::stod(argv[i + 1]);
                if (val > 0 && val < 100) {
                    vals[6].emplace(val);
                }
                break;
            case 'T':
            case 't':
                val = std::stoi(argv[i + 1]);
                if (val >= 0 && val <= 500) {
                    vals[7].emplace(val);
                }
                break;
            }
        }
    }
    App().run(vals);
}