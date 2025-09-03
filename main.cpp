#include <iostream>
#include <string>
#include "schedule.h"
#include "algorithm"

using namespace std;

int main() {
    Schedule sch;
    string command, date, schedule, filename;
    char delimiter = ',';

    cout << "Welcome to the schedule management program!\n";
    cout << "Available commands:\n";
    cout << "  add <date> <schedule> - add a schedule for a date\n";
    cout << "  view <date> - view the schedule for the date\n";
    cout << "  replace <date> <new_schedule> - replace the schedule for the date\n";
    cout << "  erase <date> - delete the schedule on the date\n";
    cout << "  file <filename> - select a file to store the schedule\n";
    cout << "  delim <char> - change the delimiter for schedules (comma on default)\n";
    cout << "  help - help with the functionality of the program\n";
    cout << "  subjects - viewing subjects\n";
    cout << "  exit - exit the program\n\n";

    while (true) {
        cout << "Enter a command: ";
        getline(cin, command);
        vector<string> tokens = Schedule::split(command, ' ');
        if (tokens.empty()) {
            cout << "An empty command, try again\n";
            continue;
        }
        string cmd = tokens[0];
        transform(cmd.begin(), cmd.end(), cmd.begin(), 
          [](char c){ return toupper(c); });
        if (cmd == "HELP" && tokens.size() == 1) {
            cout << "Available commands:\n";
            cout << "  add <date> <schedule> - add a schedule for a date\n";
            cout << "  view <date> - view the schedule for the date\n";
            cout << "  replace <date> <new_schedule> - replace the schedule for the date\n";
            cout << "  erase <date> - delete the schedule on the date\n";
            cout << "  file <filename> - select a file to store the schedule\n";
            cout << "  delim <char> - change the delimiter for schedules\n";
            cout << "  help - help with the functionality of the program\n";
            cout << "  exit - exit the program\n\n";
        }
        else if (cmd == "SUBJECTS") sch.view_subjects();
        else if (cmd == "EXIT") {
            cout << "The program is completed\n";
            break;
        }
        else if (cmd == "FILE" && tokens.size() == 2) {
            filename = tokens[1];
            sch.choose_file(filename);
        }
        else if (cmd == "DELIM" && tokens.size() == 2) {
            if (tokens[1].size() == 1) {
                delimiter = tokens[1][0];
                cout << "Delimiter changed to '" << delimiter << "'\n";
            }
            else cout << "Delimiter must be a single character\n";
        }
        else if (cmd == "ADD" && tokens.size() >= 3) {
            date = tokens[1];
            schedule = command.substr(command.find(tokens[2]));
            schedule.erase(0, schedule.find_first_not_of(" \t"));
            schedule.erase(schedule.find_last_not_of(" \t") + 1);

            pair<string, string> data = {date, schedule};
            cout << "Adding schedule: " << schedule << '\n';
            sch.push(data, delimiter);
        }
        else if (cmd == "VIEW" && tokens.size() == 2) {
            date = tokens[1];
            string result = sch.at(date);
            if (result.empty()) cout << "There is no schedule for the specified date\n";
            else cout << "Schedule for " << date << ": " << result << "\n";
        }
        else if (cmd == "REPLACE" && tokens.size() >= 3) {
            date = tokens[1];
            schedule = command.substr(command.find(tokens[2]));
            schedule.erase(0, schedule.find_first_not_of(" \t"));
            schedule.erase(schedule.find_last_not_of(" \t") + 1);

            sch.replace(date, schedule, delimiter);
        }
        else if (cmd == "ERASE" && tokens.size() == 2) {
            date = tokens[1];
            sch.erase(date);
        }
        else {
            cout << "Incorrect command or format, try again\n";
        }
    }
    return 0;
}