enum LogLevel { error, warning, info };
enum LogDestination { file, console };

using namespace std;
using LogFunction = function<void(const string&)>;

class Logger
{
    private:
        LogLevel m_level;
        LogFunction m_logFunction;
        LogDestination m_logDestination;
    public:
        Logger(const LogFunction &logFunction): m_logFunction(logFunction)
        {
        }

        void SetLevel(LogLevel newLevel)
        {
            m_level = newLevel;
        }

        void SetLogDestination(LogDestination logDestination)
        {
            m_logDestination = logDestination;
        }

        void Error(string message)
        {
             if (m_level >= error)
             {
                m_logFunction("[ERROR]: " + message);
             }
        }

        void Warning(string message)
        {
            if (m_level >= warning)
            {
                m_logFunction("[WARNING]: " + message);
            }
        }

        void Info(string message)
        {
            if (m_level >= info)
            {
                m_logFunction("[INFO]: " + message);
            }
        }
};
