#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>
#include <limits>
#include <chrono>


struct Record {
    std::string uid;
    int reported_date;
    std::string victim_last;
    std::string victim_first;
    std::string victim_race;
    int victim_age;
    std::string victim_sex;
    std::string city;
    std::string state;
    double lat;
    double lon;
    std::string disposition;
};


std::vector<Record> loadData(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Record> data;
    std::string line;


    std::getline(file, line);


    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Record rec;
        std::string temp;

        std::getline(ss, rec.uid, ',');
        ss >> rec.reported_date;
        ss.ignore(1, ',');
        std::getline(ss, rec.victim_last, ',');
        std::getline(ss, rec.victim_first, ',');
        std::getline(ss, rec.victim_race, ',');
        ss >> rec.victim_age;
        ss.ignore(1, ',');
        std::getline(ss, rec.victim_sex, ',');
        std::getline(ss, rec.city, ',');
        std::getline(ss, rec.state, ',');
        ss >> rec.lat;
        ss.ignore(1, ',');
        ss >> rec.lon;
        ss.ignore(1, ',');
        std::getline(ss, rec.disposition, ',');

        data.push_back(rec);
    }

    return data;
}


void calculateStatsAndWriteToCSV(double x1, double y1, double x2, double y2, const std::string& field, const std::string& operation, const std::vector<Record>& data, std::ofstream& outfile) {
    double min_lon = std::min(x1, x2);
    double max_lon = std::max(x1, x2);
    double min_lat = std::min(y1, y2);
    double max_lat = std::max(y1, y2);

    std::vector<Record> filtered_data;
    for (const auto& rec : data) {
        if (rec.lon >= min_lon && rec.lon <= max_lon && rec.lat >= min_lat && rec.lat <= max_lat) {
            filtered_data.push_back(rec);
        }
    }

    std::string result;

    if (filtered_data.empty()) {
        result = "No data available";
        outfile << x1 << "," << y1 << "," << x2 << "," << y2 << "," << field << "," << operation << "," << result << "\n";
        return;
    }

    if(field == "x"){/* To add implementation */}
    else if(field == "y"){/* To add implementation */}
    else if (field == "age") {
        if (operation == "average") {
            double total_age = 0.0;
            for (const auto& rec : filtered_data) {
                
                total_age += rec.victim_age;
            }
            double avg_age = total_age / filtered_data.size();
            result = std::to_string(avg_age);
        } else if (operation == "max") {
            int max_age = std::numeric_limits<int>::min();
            for (const auto& rec : filtered_data) {
                max_age = std::max(max_age, rec.victim_age);
            }
            result = std::to_string(max_age);
        } else if (operation == "min") {
            int min_age = std::numeric_limits<int>::max();
            for (const auto& rec : filtered_data) {
                min_age = std::min(min_age, rec.victim_age);
            }
            result = std::to_string(min_age);
        }
    }

    
    if (field == "race" && operation == "average") {
        std::map<std::string, int> race_distribution;
        for (const auto& rec : filtered_data) {
            race_distribution[rec.victim_race]++;
        }

        
        std::string most_common_race;
        int max_count = 0;
        for (const auto& race : race_distribution) {
            if (race.second > max_count) {
                max_count = race.second;
                most_common_race = race.first;
            }
        }

        if (most_common_race == "Others") {
                result = "1";
            } else if (most_common_race == "White") {
                result = "2";
            } else if (most_common_race == "Black") {
                result = "3";
            } else if (most_common_race == "Hispanic") {
                result = "4";
            } else if (most_common_race == "Unknown") {
                result = "5";
            }
            else if (most_common_race == "Asian"){
                result = "6";
            }
            else{
                result = "0";
            }

        
    }


    outfile << x1 << "," << y1 << "," << x2 << "," << y2 << "," << field << "," << operation << "," << result << "\n";
}


std::vector<std::tuple<double, double, double, double, std::string, std::string>> loadQueries(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::tuple<double, double, double, double, std::string, std::string>> queries;
    std::string line;

    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        double x1, y1, x2, y2;
        std::string field, operation;

        
        ss >> x1;
        ss.ignore(1, ',');
        ss >> y1;
        ss.ignore(1, ',');
        ss >> x2;
        ss.ignore(1, ',');
        ss >> y2;
        ss.ignore(1, ',');
        std::getline(ss, field, ',');
        std::getline(ss, operation, ',');

        queries.push_back(std::make_tuple(x1, y1, x2, y2, field, operation));
    }

    return queries;
}

int main() {
    auto start_preprocessing = std::chrono::system_clock::now();
    std::string dataset_filename = "expanded_homicide_data_cluster.csv";
    std::vector<Record> data = loadData(dataset_filename);
    auto end_preprocessing = std::chrono::system_clock::now();
    auto start = std::chrono::system_clock::now();
    std::string query_filename = "query_uniform.csv";
    std::vector<std::tuple<double, double, double, double, std::string, std::string>> queries = loadQueries(query_filename);
    std::ofstream outfile("query_uniform_results.csv");
    outfile << "x1,y1,x2,y2,field,operation,result\n"; 

    
    for (const auto& query : queries) {
        double x1, y1, x2, y2;
        std::string field, operation;

        std::tie(x1, y1, x2, y2, field, operation) = query;

        calculateStatsAndWriteToCSV(x1, y1, x2, y2, field, operation, data, outfile);
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::chrono::duration<double> elapsed_seconds_preprocessing = end_preprocessing - start_preprocessing;
    std::cout  << "Query execution elapsed time: " << elapsed_seconds.count() << "s"
               << std::endl;
    std::cout  << "Preprocessing elapsed time: " << elapsed_seconds_preprocessing.count() << "s"
    << std::endl;
    outfile.close();

    return 0;
}
