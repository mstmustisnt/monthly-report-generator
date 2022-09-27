#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include "logger.h"

using namespace std;

// TODO: using smth like Map in C++ to quickly pick by key, not loop every time
struct user_data
{
    string full_name;
    string date;
    int hours;
};

int FindIndexByNameAndDate(const vector<user_data>& users, const string& full_name, const string& date) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i].full_name == full_name && users[i].date == date) {
            return i;
        }
    }
    return -1;
}

vector<string> Split(const string& s, char delim) {
  stringstream ss(s);
  string item;
  vector<string> elems;
  while (getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

string PrepareDate(const string& raw_date) {
    const char * months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    vector<string> date_row = Split(raw_date, '-');
    const int month_num = stoi(date_row[1]);
    string year = date_row[0];
    string date(months[month_num - 1]);
    date.append(" ");
    date.append(year);

    return date;
}

vector<user_data> PrepareMonthlyReport(const string& file_name)
{
    vector<user_data> users;
    string line;
    // Open an existing file
    ifstream input(file_name);
    int line_num = 0;

    while (getline(input, line))
    {
        line_num += 1;
        if (line_num == 1) {
            continue;
        }

        const vector<string> row = Split(line, ';');
        // Name;email;department;position;project;task;date;logged hours
        user_data user;
        const string date = PrepareDate(row[6]);

        const int index = FindIndexByNameAndDate(users, row[0], date);

        if (index != -1) {
            // user record for this month and year already exists
            users[index].hours = stoi(row[7]) + user.hours;
        } else {
            user.full_name = row[0];
            user.date = date;
            user.hours = stoi(row[7]);
            users.push_back(user);
        }
    }

    return users;
}

void LogReport(const vector<user_data>& users, const string& out_file_name)
{
    Logger logger;
    logger.SetLevel(info);
    // if we want to write to file
    // ofstream out_file;
    // out_file.open(out_file_name);
    for (int i = 0; i < users.size(); i++) {
        user_data user = users[i];
        logger.Info(user.full_name + ";" + user.date + ";" + to_string(user.hours));
        // optionally if it was meant to write to file
        // out_file << user.full_name << ";" << user.date << ";" << user.hours << "\n";
    }
     //out_file.close();
}

int main()
{
    const string source_file_name = "report.csv";
    const string out_file_name = "monthly_report.csv";
    const vector<user_data> users = PrepareMonthlyReport(source_file_name);
    LogReport(users, out_file_name);
    return 0;
}
