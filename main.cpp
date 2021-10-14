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
        if (j["entries"][today] == nullptr) {
            throw 404;
        } else {
            for (auto& el : j["entries"][today].items())
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
    double todayCost = tdyValue + cost;

    todayCost = std::ceil(todayCost * 20.0) / 20.0; // round to 0.05
    j["entries"][today] = todayCost;

    // calculate total
    double total = 0.00;
    for (auto it : j["entries"])
    {
        for (auto& el : it.items())
        {
            double elVal = el.value();
            total = total + elVal;
        }
    }
    total = std::round(total * 100.0) / 100.0;
//    std::cout << total << '\n';
    j["total"] = total;

    // write prettified JSON to file
    std::ofstream o("costs.json");
    o << std::setw(4) << j << std::endl;

    return 0;
}