#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include "quadtree.h"
#include <chrono>
#include <ctime>  

int main() {
    
    std::ifstream file("expanded_homicide_data_fixed.csv");
    auto start_construction = std::chrono::system_clock::now();
    // std::ifstream file("expanded_homicide_data_cluster.csv");
    if (!file) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::string line;
    QuadTree qt(Rectangle(0, 0, 180, 90), 4);

    bool isHeader = true;

    
    while (std::getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }

        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        double lat, lon;
        int age;
        std::string race;

        try {
            lat = std::stod(tokens[9]);
            lon = std::stod(tokens[10]);
            age = std::stoi(tokens[5]);
            race = tokens[4]; 

            
            if (lat < -90 || lat > 90 || lon < -180 || lon > 180) {
                throw std::out_of_range("Latitude or Longitude out of valid range");
            }

        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid data in line: " << line << " - stod failed on lat or lon with value lat: " << tokens[8] << " lon: " << tokens[9] << std::endl;
            continue;
        } catch (const std::out_of_range& e) {
            std::cerr << "Out of range value in line: " << line << " - " << e.what() << std::endl;
            continue;
        }

        qt.insert(Point(lon, lat, age, race));
    }

    file.close();
    auto end_construction = std::chrono::system_clock::now();


    
    std::ifstream query_file("query_quad.csv");
    if (!query_file) {
        std::cerr << "Error opening query file." << std::endl;
        return 1;
    }

    auto start_query = std::chrono::system_clock::now();
    std::ofstream result_file("query_results_quad.csv");
    result_file << "x1,y1,x2,y2,attribute,operation,result\n";

    while (std::getline(query_file, line)) {
        std::stringstream ss(line);
        double x1, y1, x2, y2;
        std::string attribute, operation;
        ss >> x1;
        ss.ignore();
        ss >> y1;
        ss.ignore();
        ss >> x2;
        ss.ignore();
        ss >> y2;
        ss.ignore();
        std::getline(ss, attribute, ',');
        std::getline(ss, operation);

        double centerX = (x1 + x2) / 2;
        double centerY = (y1 + y2) / 2;
        double width = std::abs(x2 - x1);
        double height = std::abs(y2 - y1);
        double res = 0;

        Rectangle range(centerX, centerY, width / 2, height / 2);

        double result = 0;
        if (attribute == "x") {
            if (operation == "average") {
                result = qt.averageXInRange(range);
            } else if (operation == "minimum") {
                result = qt.minXInRange(range);
            } else if (operation == "maximum") {
                result = qt.maxXInRange(range);
            } else {
                std::cerr << "Invalid operation for x attribute." << std::endl;
                continue;
            }
        }
        else if (attribute == "race") { 
            if (operation == "average") {
                std::string mostCommonRace = qt.mostCommonRaceInRange(range);
            if (mostCommonRace == "Others") {
                res = 1;
            } else if (mostCommonRace == "White") {
                res = 2;
            } else if (mostCommonRace == "Black") {
                res = 3;
            } else if (mostCommonRace == "Hispanic") {
                res = 4;
            } else if (mostCommonRace == "Unknown") {
                res = 5;
            }
            else if (mostCommonRace == "Asian"){
                res = 6;
            }
            else{
                res = 0;
            }
                result_file << x1 << "," << y1 << "," << x2 << "," << y2 << "," << attribute << "," << operation << "," << res << "\n";
                continue;
            } else {
                std::cerr << "Invalid operation for race attribute." << std::endl;
                continue;
            }
        }
        else if (attribute == "age") { 
            if (operation == "average") {
                result = qt.averageAgeInRange(range);
            } else if (operation == "minimum") {
                result = qt.minAgeInRange(range);
            } else if (operation == "maximum") {
                result = qt.maxAgeInRange(range);
            } else {
                std::cerr << "Invalid operation for age attribute." << std::endl;
                continue;
            }
        }  else if (attribute == "y") { 
            if (operation == "average") {
                result = qt.averageYInRange(range);
            } else if (operation == "minimum") {
                result = qt.minYInRange(range);
            } else if (operation == "maximum") {
                result = qt.maxYInRange(range);
            } else {
                std::cerr << "Invalid operation for y attribute." << std::endl;
                continue;
            }
        } else {
            std::cerr << "Invalid attribute." << std::endl;
            continue;
        }

        result_file << x1 << "," << y1 << "," << x2 << "," << y2 << "," << attribute << "," << operation << "," << result << "\n";
    }
     
    query_file.close();
    auto end_query = std::chrono::system_clock::now();
    std::chrono::duration<double> query_time = end_query - start_query;
    std::cout << "Query execution time: " << query_time.count() << " seconds." << std::endl;

    
    std::chrono::duration<double> construction_time = end_construction - start_construction;
    std::cout << "Quadtree construction time: " << construction_time.count() << " seconds." << std::endl;

    result_file.close();


    return 0;
}