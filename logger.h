enum LogLevel { error, warning, info };

class Logger
{
    private:
        LogLevel m_level;
    public:
        void SetLevel(LogLevel newLevel)
        {
            m_level = newLevel;
        }
        void Error(std::string message)
        {
             if (m_level >= error)
             {
                std::cout << "[ERROR]: " << message << std::endl;
             }
        }
        void Warning(std::string message)
        {
            if (m_level >= warning)
            {
                std::cout << "[WARNING]: " << message << std::endl;
            }
        }
        void Info(std::string message)
        {
            if (m_level >= info)
            {
                std::cout << "[INFO]: " << message << std::endl;
            }
        }
};
