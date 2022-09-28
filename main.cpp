#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <map>
#include "logger.h"

using namespace std;

struct UserData
{
    string fullName;
    string date;
    int hours;
};
map<string, UserData> usersMap;
Logger logger;

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

void PrepareMonthlyReport(const string& file_name)
{
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
        const string date = PrepareDate(row[6]);
        const string key = row[0] + date;
        UserData& user = usersMap[key];

        if (user.hours != 0) {
            // user record for this month and year already exists
            user.hours = stoi(row[7]) + user.hours;
        } else {
            user.fullName = row[0];
            user.date = date;
            user.hours = stoi(row[7]);
        }
    }
}

void LogReport(const string& outFileName)
{
    // if we want to write to file
    // ofstream out_file;
    // out_file.open(outFileName);
    for (auto const& [key, user] : usersMap) {
        logger.Info(user.fullName + ";" + user.date + ";" + to_string(user.hours));
        // optionally if it was meant to write to file
        // out_file << user.fullName << ";" << user.date << ";" << user.hours << "\n";
    }
     //out_file.close();
}

int main()
{
    logger.SetLevel(info);
    const string sourceFileName = "report.csv";
    const string outFileName = "monthly_report.csv";
    PrepareMonthlyReport(sourceFileName);
    LogReport(outFileName);
    return 0;
}
