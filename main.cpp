#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <sstream>
#include "json.hpp"

using json = nlohmann::json;

int main() {

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d.%m.%Y");
    auto today = oss.str();

    // read a JSON file
    std::ifstream ifs("costs.json");
    json j = json::parse(ifs);

    // add an object (using an initializer list of pairs)
//    j["11.10.2021"] = 7.70;
//    j["12.10.2021"] = 3.55;
//    j["13.10.2021"] = 12.95;

    // read data
    double tdyValue;
    try {
        if (j[today] == nullptr) {
            throw 404;
        } else {
            for (auto& el : j[today].items())
            {
                tdyValue = el.value(); // read combined total of today's entries
            }
        }
    }
    catch (...) {
        tdyValue = 0.00; // if there was no entry today set to 0.00
    }

    double cost;
    std::cout << "Enter the cost: \n";
    std::cin >> cost;
    double value = tdyValue + cost;

    value = std::ceil(value * 100.0) / 100.0;
    j[today] = value;

    // write prettified JSON to the file
    std::ofstream o("costs.json");
    o << std::setw(4) << j << std::endl;

    return 0;
}