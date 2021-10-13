#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include "json.hpp"

using json = nlohmann::json;

int main() {

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d.%m.%Y");
    auto date = oss.str();
//    std::cout << date << std::endl;

    // read a JSON file
    std::ifstream ifs("pretty.json");
    json j = json::parse(ifs);

    // add an object (using an initializer list of pairs)
//    j["11.10.2021"] = 7.70;
//    j["12.10.2021"] = 3.55;
//    j["13.10.2021"] = 12.95;

    // read data
    double tdyValue;
    for (auto& el : j[date].items())
    {
        tdyValue = el.value();
    }

    j["13.10.2021"] = tdyValue + 3.00;

    // write prettified JSON to another file
    std::ofstream o("pretty.json");
    o << std::setw(4) << j << std::endl;

    return 0;
}