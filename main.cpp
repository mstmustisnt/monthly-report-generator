#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <map>
#include <exception>
#include "logger.h"

using namespace std;

class ReportConverter
{
    private:
        Logger m_logger;
        struct UserData
        {
            string fullName;
            string date;
            int hours = 0;
        };
        map<string, UserData> m_usersMap;

        vector<string> Split(const string& s, char delim)
        {
          stringstream ss(s);
          string item;
          vector<string> elems;
          while (getline(ss, item, delim)) {
            elems.push_back(item);
          }
          return elems;
        }

        string PrepareDate(const string& raw_date)
        {
            const char * MONTHS[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
            vector<string> date_row = Split(raw_date, '-');
            const int month_num = stoi(date_row[1]);
            string year = date_row[0];
            string date = MONTHS[month_num - 1];
            date.append(" ");
            date.append(year);

            return date;
        }
    public:
        ReportConverter(Logger logger): m_logger(logger)
        {
        }

        void ConvertDailyToMonthly(const string& file_name)
        {
            string line;
            // Open an existing file
            ifstream input(file_name);
            int line_num = 0;
            // reading header
            getline(input, line);

            while (getline(input, line))
            {
                const vector<string> row = Split(line, ';');
                // Name;email;department;position;project;task;date;logged hours
                const string date = PrepareDate(row[6]);
                const string key = row[0] + date;
                UserData& user = m_usersMap[key];

                if (user.hours != 0)
                {
                    // user record for this month and year already exists
                    user.hours += stoi(row[7]);
                }
                else
                {
                    user.fullName = row[0];
                    user.date = date;
                    user.hours = stoi(row[7]);
                }
            }
        }

        void LogReport(const string& outFileName)
        {
            for (auto const& [key, user] : m_usersMap) {
                m_logger.Info(user.fullName + ";" + user.date + ";" + to_string(user.hours));
            }
        }
};

int main(int argc, char **argv)
{
    const char* logDestination = argv[1] ? argv[1] : "console";
    const string sourceFileName = "report.csv";
    const string outFileName = "logs.txt";
    ofstream outFileStream;
    const bool logToFile = string(logDestination).compare(string("file")) == 0;
    const bool logToConsole = string(logDestination).compare(string("console")) == 0;

    if (!(logToFile || logToConsole))
    {
        throw invalid_argument("Unsupported logging destination " + string(logDestination));
    }

    auto logFunction = [&logToFile, &outFileStream] (const string& message)
    {
        logToFile
            ? outFileStream << message << "\n"
            : cout << message << endl;
    };

    Logger logger(logFunction);

    if (logToFile)
    {
        outFileStream.open(outFileName);
    }

    ReportConverter reportConverter (logger);
    reportConverter.ConvertDailyToMonthly(sourceFileName);
    reportConverter.LogReport(outFileName);

    if (logToFile)
    {
        cout << "Check logs in ./" << outFileName << endl;
        outFileStream.close();
    };

    return 0;
}
