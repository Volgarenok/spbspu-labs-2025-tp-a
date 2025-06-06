#include "polygon.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <functional>

void process_rmecho(std::vector<Polygon>& polygons, const Polygon& query) {
    int removed = 0;
    auto it = polygons.begin();
    while (it != polygons.end()) {
        if (it->points.size() == query.points.size() &&
            std::equal(it->points.begin(), it->points.end(),
                      query.points.begin(), query.points.end(),
                      [](const Point& a, const Point& b) {
                          return a.x == b.x && a.y == b.y;
                      })) {
            auto next_it = it + 1;
            while (next_it != polygons.end() &&
                   next_it->points.size() == query.points.size() &&
                   std::equal(next_it->points.begin(), next_it->points.end(),
                             query.points.begin(), query.points.end(),
                             [](const Point& a, const Point& b) {
                                 return a.x == b.x && a.y == b.y;
                             })) {
                ++next_it;
                ++removed;
            }
            if (next_it != it + 1) {
                it = polygons.erase(it + 1, next_it);
                ++removed;
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }
    std::cout << removed << "\n";
}
void process_commands(std::vector<Polygon>& polygons) {
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;
        bool invalid = false;
        bool printDouble = false;
        double dblResult = 0.0;
        int intResult = 0;
        if (cmd == "AREA") {
            std::string arg;
            iss >> arg;
            if (arg == "EVEN") {
                dblResult = std::accumulate(
                    polygons.begin(), polygons.end(), 0.0,
                    [](double acc, const Polygon& p) {
                        return acc + ((p.points.size() % 2 == 0) ? compute_area(p) : 0.0);
                    });
                printDouble = true;
            }
            else if (arg == "ODD") {
                dblResult = std::accumulate(
                    polygons.begin(), polygons.end(), 0.0,
                    [](double acc, const Polygon& p) {
                        return acc + ((p.points.size() % 2 != 0) ? compute_area(p) : 0.0);
                    });
                printDouble = true;
            }
            else if (arg == "MEAN") {
                if (polygons.empty()) {
                    invalid = true;
                } else {
                    double total = std::accumulate(
                        polygons.begin(), polygons.end(), 0.0,
                        [](double acc, const Polygon& p) {
                            return acc + compute_area(p);
                        });
                    dblResult = total / polygons.size();
                    printDouble = true;
                }
            }
            else {
                bool is_num = !arg.empty() && std::all_of(arg.begin(), arg.end(), ::isdigit);
                if (is_num) {
                    int num = std::stoi(arg);
                    if (num < 3) {
                        invalid = true;
                    } else {
                        dblResult = std::accumulate(
                            polygons.begin(), polygons.end(), 0.0,
                            [num](double acc, const Polygon& p) {
                                return acc + (static_cast<int>(p.points.size()) == num ? compute_area(p) : 0.0);
                            });
                        printDouble = true;
                    }
                } else {
                    invalid = true;
                }
            }
        }
        else if (cmd == "MAX") {
            std::string arg;
            iss >> arg;
            if (polygons.empty()) {
                invalid = true;
            } else if (arg == "AREA") {
                auto it = std::max_element(
                    polygons.begin(), polygons.end(),
                    [](const Polygon& a, const Polygon& b) {
                        return compute_area(a) < compute_area(b);
                    });
                dblResult = compute_area(*it);
                printDouble = true;
            }
            else if (arg == "VERTEXES") {
                auto it = std::max_element(
                    polygons.begin(), polygons.end(),
                    [](const Polygon& a, const Polygon& b) {
                        return a.points.size() < b.points.size();
                    });
                intResult = it->points.size();
            }
            else {
                invalid = true;
            }
        }
        else if (cmd == "MIN") {
            std::string arg;
            iss >> arg;
            if (polygons.empty()) {
                invalid = true;
            } else if (arg == "AREA") {
                auto it = std::min_element(
                    polygons.begin(), polygons.end(),
                    [](const Polygon& a, const Polygon& b) {
                        return compute_area(a) < compute_area(b);
                    });
                dblResult = compute_area(*it);
                printDouble = true;
            }
            else if (arg == "VERTEXES") {
                auto it = std::min_element(
                    polygons.begin(), polygons.end(),
                    [](const Polygon& a, const Polygon& b) {
                        return a.points.size() < b.points.size();
                    });
                intResult = it->points.size();
            }
            else {
                invalid = true;
            }
        }
        else if (cmd == "COUNT") {
            std::string arg;
            iss >> arg;
            if (arg == "EVEN") {
                intResult = std::count_if(
                    polygons.begin(), polygons.end(),
                    [](const Polygon& p) {
                        return p.points.size() % 2 == 0;
                    });
            }
            else if (arg == "ODD") {
                intResult = std::count_if(
                    polygons.begin(), polygons.end(),
                    [](const Polygon& p) {
                        return p.points.size() % 2 != 0;
                    });
            }
            else {
                bool is_num = !arg.empty() && std::all_of(arg.begin(), arg.end(), ::isdigit);
                if (is_num) {
                    int num = std::stoi(arg);
                    if (num < 3) {
                        invalid = true;
                    } else {
                        intResult = std::count_if(
                            polygons.begin(), polygons.end(),
                            [num](const Polygon& p) {
                                return static_cast<int>(p.points.size()) == num;
                            });
                    }
                } else {
                    invalid = true;
                }
            }
        }
        else if (cmd == "INTERSECTIONS") {
            std::string rest;
            std::getline(iss, rest);
            if (rest.empty()) {
                invalid = true;
            } else {
                size_t pos = rest.find_first_not_of(' ');
                if (pos != std::string::npos) {
                    rest = rest.substr(pos);
                }
                Polygon query;
                if (!parse_polygon(rest, query) || query.points.size() < 3) {
                    invalid = true;
                } else {
                    intResult = std::count_if(
                        polygons.begin(), polygons.end(),
                        [&query](const Polygon& p) {
                            return polygons_intersect(p, query);
                        });
                }
            }
        }
        else if (cmd == "RMECHO") {
            std::string rest;
            std::getline(iss, rest);
            if (rest.empty()) {
                invalid = true;
            } else {
                size_t pos = rest.find_first_not_of(' ');
                if (pos != std::string::npos) {
                    rest = rest.substr(pos);
                }
                Polygon query;
                if (!parse_polygon(rest, query)) {
                    invalid = true;
                } else {
                    process_rmecho(polygons, query);
                }
            }
            continue;
        }
        else {
            invalid = true;
        }
        if (!invalid && cmd != "INTERSECTIONS" && cmd != "RMECHO") {
            std::string extra;
            if (iss >> extra) {
                invalid = true;
            }
        }
        if (invalid) {
            std::cout << "<INVALID COMMAND>\n";
        }
        else if (printDouble) {
            std::cout << std::fixed << std::setprecision(1) << dblResult << "\n";
        }
        else {
            std::cout << intResult << "\n";
        }
    }
}
