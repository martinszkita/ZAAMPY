#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include <string>
#include <vector>

struct Cube {
    std::string name; // obowiązkowe pole
    std::string shift = "0 0 0";
    std::string scale = "1 1 1";
    std::string rotXYZ = "0 0 0";
    std::string trans_m = "0 0 0";
    std::string color = "128 128 128";
};

struct Configuration {
    std::vector<std::string> plugins;
    std::vector<Cube> cubes;
};

#endif
