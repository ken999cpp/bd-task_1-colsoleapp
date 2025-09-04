#include "schedule.h"
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <cctype>

using std::getline, std::find, std::runtime_error, std::cout, std::toupper, std::transform;

const vector<string> Schedule::subjects = {"DATABASES", "DISCRETE MATHEMATICS",
    "SPECIAL CHAPTERS OF HIGHER MATHEMATICS", "METHODS OF MATHEMATICAL ANALYSIS",
    "ARTIFICIAL INTELLIGENCE AND DATABASE SYSTEMS", "MATHEMATICAL ANALYSIS",
    "HIGHER ALGEBRA", "DIFFERENTIAL GEOMETRY", "PHILOSOPHY"};

vector<string> Schedule::split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    std::istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

bool Schedule::isValidSchedule(string& sch, char delimiter) {
    vector<string> subj = split(sch, delimiter);
    for (auto& s : subj) {
        s.erase(0, s.find_first_not_of(" \t\n\r"));
        s.erase(s.find_last_not_of(" \t\n\r") + 1);
        string tmp = s;
        transform(tmp.begin(), tmp.end(), tmp.begin(),
                  [](char c){ return toupper(c); });
        if (find(subjects.begin(), subjects.end(), tmp) == subjects.end()) {
            return false;
        }
    }
    return true;
}

bool Schedule::isValid(const string& date) {
    if (date.size() != CORRECT_DATE_SIZE) return false;
    string day = date.substr(0, DAY_OR_MONTH_SIZE);
    string month = date.substr(3, DAY_OR_MONTH_SIZE);
    string year = date.substr(6, YEAR_SIZE);
    int day_to_int = stoi(day);
    int year_to_int = stoi(year);
    int month_to_int = stoi(month);
    bool is_leap_year = (year_to_int % 4 == 0 && (year_to_int % 100 != 0 || year_to_int % 400 == 0));
    if ((day[0] == '0' && day[1] == '0') || (day_to_int > MAX_DAY)) return false;
    if ((month[0] == '0' && month[1] == '0') || (month_to_int > MAX_MONTH)) return false;
    switch (month_to_int) {
        case JANUARY:
            if (day_to_int > JAN_MAR_MAY_JULY_AUG_OCT_DEC_MAX_DAY) return false;
            break;
        case FEBRUARY:
            if (is_leap_year && day_to_int > FEB_MAX_DAY) return false;
            else if (!is_leap_year && day_to_int > FEB_MAX_DAY - 1) return false;
            break;
        case MARCH:
            if (day_to_int > JAN_MAR_MAY_JULY_AUG_OCT_DEC_MAX_DAY) return false;
            break;
        case APRIL:
            if (day_to_int > APR_JUNE_SEPT_NOV_MAX_DAY) return false;
            break;
        case MAY:
            if (day_to_int > JAN_MAR_MAY_JULY_AUG_OCT_DEC_MAX_DAY) return false;
            break;
        case JUNE:
            if (day_to_int > APR_JUNE_SEPT_NOV_MAX_DAY) return false;
            break;
        case JULY:
            if (day_to_int > JAN_MAR_MAY_JULY_AUG_OCT_DEC_MAX_DAY) return false;
            break;
        case AUGUST:
            if (day_to_int > JAN_MAR_MAY_JULY_AUG_OCT_DEC_MAX_DAY) return false;
            break;
        case SEPTEMBER:
            if (day_to_int > APR_JUNE_SEPT_NOV_MAX_DAY) return false;
            break;
        case OCTOBER:
            if (day_to_int > JAN_MAR_MAY_JULY_AUG_OCT_DEC_MAX_DAY) return false;
            break;
        case NOVEMBER:
            if (day_to_int > APR_JUNE_SEPT_NOV_MAX_DAY) return false;
            break;
        case DECEMBER:
            if (day_to_int > JAN_MAR_MAY_JULY_AUG_OCT_DEC_MAX_DAY) return false;
            break;
    }
    return true;
}

void Schedule::push(pair<string, string>& data, char delimiter) {
    if (!isValid(data.first)) {
        cout << "Invalid date format\n";
        return;
    }
    if (!isValidSchedule(data.second, delimiter)) {
        cout << "No such object in the list\n";
        return;
    }
    if (!collection.is_open()) {
        cout << "No file selected. Please use 'file <filename>' command first.\n";
        return;
    }
    vector<string> subjects = split(data.second, delimiter);
    string formattedSchedule;
    for (size_t i = 0; i < subjects.size(); ++i) {
        string subject = subjects[i];
        subject.erase(0, subject.find_first_not_of(" \t"));
        subject.erase(subject.find_last_not_of(" \t") + 1);
        transform(subject.begin(), subject.end(), subject.begin(),
                  [](char c){ return toupper(c); });
        formattedSchedule += subject;
        if (i < subjects.size() - 1) {
            formattedSchedule += ", ";
        }
    }
    auto it = container.find(data.first);
    if (it != container.end()) {
        it->second += ", " + formattedSchedule;
    }
    else container.insert({data.first, formattedSchedule});
    collection.close();
    collection.open(collection_name, std::ios::out | std::ios::trunc);
    for (const auto& l : container) {
        collection << l.first << " : " << l.second << '\n';
    }
    collection.flush();
}

string Schedule::at(const string& date) {
    collection.close();
    collection.open(collection_name, std::ios::in);
    string line;
    while (getline(collection, line)) {
        auto pos = line.find(':');
        if (pos != string::npos) {
            string tmp_date = line.substr(0, pos);
            tmp_date.erase(0, tmp_date.find_first_not_of(" \t"));
            tmp_date.erase(tmp_date.find_last_not_of(" \t") + 1);
            if (tmp_date == date) {
                string value = line.substr(pos + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                return value;
            }
        }
    }
    return "";
}

void Schedule::replace(const string& date, string& newSchedule, char delimiter) {
    if (!isValid(date)) {
        cout << "Invalid date format\n";
        return;
    }
    if (!isValidSchedule(newSchedule, delimiter)) {
        cout << "No such object in the list\n";
        return;
    }
    if (!collection.is_open()) {
        cout << "No file selected. Please use 'file <filename>' command first.\n";
        return;
    }
    container[date] = newSchedule;
    string fileSchedule = container[date];
    vector<string> subjects = split(fileSchedule, delimiter);
    string formattedSchedule;
    for (size_t i = 0; i < subjects.size(); ++i) {
        string subject = subjects[i];
        subject.erase(0, subject.find_first_not_of(" \t"));
        subject.erase(subject.find_last_not_of(" \t") + 1);
        transform(subject.begin(), subject.end(), subject.begin(),
                  [](char c){ return toupper(c); });
        formattedSchedule += subject;
        if (i < subjects.size() - 1) formattedSchedule += ", ";
    }
    cout << "Updating container: " << date << " : " << container[date] << '\n';
    vector<pair<string, string>> lines;
    collection.clear();
    collection.seekg(0);
    string line;
    while (getline(collection, line)) {
        auto pos = line.find(':');
        if (pos != string::npos) {
            string tmp_date = line.substr(0, pos);
            tmp_date.erase(0, tmp_date.find_first_not_of(" \t"));
            tmp_date.erase(tmp_date.find_last_not_of(" \t") + 1);
            if (tmp_date != date) {
                string schedule = line.substr(pos + 2);
                schedule.erase(0, schedule.find_first_not_of(" \t"));
                lines.emplace_back(tmp_date, schedule);
            }
        }
    }
    lines.emplace_back(date, formattedSchedule);
    collection.close();
    collection.open(collection_name, std::ios::in | std::ios::out | std::ios::trunc);
    if (!collection.is_open()) {
        cout << "Failed to reopen file " << collection_name << '\n';
        return;
    }
    cout << "Writing to file:\n";
    for (const auto& l : lines) {
        collection << l.first << " : " << l.second << '\n';
        cout << "Wrote: " << l.first << " : " << l.second << '\n';
        if (!collection.good()) {
            cout << "Error writing to file " << collection_name << '\n';
            return;
        }
    }
    collection.flush();
}

void Schedule::erase(const string& date) {
    auto it = container.find(date);
    if (it != container.end()) {
        container.erase(it);
        collection.close();
        collection.open(collection_name, std::ios::out | std::ios::trunc);
        for (const auto& l : container) collection << l.first << " : " << l.second << '\n';
        collection.flush();
        cout << "Schedule for " << date << " has been erased\n";
    } 
    else cout << "There is no schedule on this day\n";
}

void Schedule::choose_file(const string& filename) {
    collection.open(filename, std::ios::in | std::ios::out | std::ios::app);
    if (!collection.is_open()) {
        cout << "Could not open the file " + filename << '\n';
        return;
    }
    cout << "File has been successfully opened\n";
    collection_name = filename;
    container.clear();
    collection.close();
    collection.open(collection_name, std::ios::in);
    string line;
    while (getline(collection, line)) {
        auto pos = line.find(':');
        if (pos != string::npos) {
            string tmp_date = line.substr(0, pos);
            tmp_date.erase(0, tmp_date.find_first_not_of(" \t"));
            tmp_date.erase(tmp_date.find_last_not_of(" \t") + 1);
            string schedule = line.substr(pos + 1);
            schedule.erase(0, schedule.find_first_not_of(" \t"));
            container[tmp_date] = schedule;
        }
    }
    collection.close();
    collection.open(collection_name, std::ios::in | std::ios::out | std::ios::app);
}

void Schedule::view_subjects() {
    for (int i = 0; i < subjects.size(); ++i) {
        if (i == subjects.size() - 1) cout << subjects[i] << '\n';
        else cout << subjects[i] << ", ";
    }

}

