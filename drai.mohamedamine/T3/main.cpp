#include <fstream>
#include <iostream>

#include "polygon.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: missing filename argument\n";
        return 1;
    }
    std::ifstream infile(argv[1]);
    if (!infile) {
        std::cerr << "Error: could not open file\n";
        return 1;
    }
    std::vector<Polygon> polygons;
    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty()) continue;
        Polygon poly;
        if (parse_polygon(line, poly) && poly.points.size() >= 3) {
            polygons.push_back(std::move(poly));
        }
    }
    process_commands(polygons);
    return 0;
}
