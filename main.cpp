#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <sstream>
#include <sys/stat.h>
#include <string>
#include "json.hpp"

using json = nlohmann::json;

bool does_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

int main() {

    // check if json-file exists
    if (!does_file_exist("cost-logs.json")) {
        json j;
        std::ofstream o("cost-logs.json");
        o << std::setw(4) << j << std::endl;
    }

    // get today's date as a string
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d.%m.%Y");
    auto today = oss.str();

    // read a JSON file
    std::ifstream ifs("cost-logs.json");
    json j = json::parse(ifs);

    // read today's entry-data
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

    // calculate new cost for today's entry
    double cost;
    std::cout << "Enter the cost: \n";
    std::cin >> cost;
    double todayCost = tdyValue + cost;

    // rounding today's cost and writing it to the json-object
    todayCost = std::round(todayCost * 20.0) / 20.0; // round to 0.05
    j["entries"][today] = todayCost;

    // calculate and round total cost of all entries
    double total = 0.00;
    for (auto it : j["entries"])
    {
        for (auto& el : it.items())
        {
            double elVal = el.value();
            total = total + elVal;
        }
    }
    total = std::round(total * 20.0) / 20.0; // round to 0.05
    j["total"] = total;

    // write prettified JSON to file
    std::ofstream o("cost-logs.json");
    o << std::setw(4) << j << std::endl;

    return 0;
}