#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include <string>
#include <vector>

struct Cube {
    std::string name;
    std::string shift;
    std::string scale;
    std::string rotXYZ;
    std::string trans;
    std::string color;
};

struct Configuration {
    std::vector<std::string> plugins;
    std::vector<Cube> cubes;
};

#endif
