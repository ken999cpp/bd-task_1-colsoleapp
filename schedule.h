#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include "defines.h"
#include <fstream>

using std::unordered_map, std::string, std::vector, std::pair, std::stoi, std::fstream;

class Schedule {
private:
    unordered_map<string, string> container;
    static const vector<string> subjects;
    fstream collection;
    string collection_name;
    static bool isValid(const string& date);
    static bool isValidSchedule(string& sch, char delimiter);
public:
    static vector<string> split(const string& str, char delimiter);
    void push(pair<string, string>& data, char delimiter = ',');
    string at(const string& date);
    void replace(const string& date, string& newSchedule, char delimiter);
    void erase(const string& date);
    void choose_file(const string& filename);
    void view_subjects();
};