#pragma once
#include <string>
#include <fstream>

namespace BFF{
    class Logger {
        std::ofstream file;

        public:
        Logger();
        ~Logger();
        
        void write_message(const std::string &message);
        void log(const std::string &message);
        void error(const std::string &message);
    };
}