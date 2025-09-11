#include <BFF/Logger.hpp>
#include <ctime>
#include <iostream>
#include <cstring> // for strerror
#include <cerrno>  // for errno
#include <iomanip>
#include <sstream>

namespace BFF{

    std::string getCompactTimestamp() {
        time_t timestamp;
        time(&timestamp);
        
        struct tm* timeinfo = localtime(&timestamp);
        
        std::stringstream ss;
        ss << std::put_time(timeinfo, "%Y-%m-%d_%H:%M:%S");
        return ss.str();
    }

    Logger::Logger(){
        file.open("log.txt", std::ios::trunc);
        file.close();
        file.open("log.txt", std::ios::app);
    }

    Logger::~Logger(){
        if(file.is_open())
            file.close();
    }

    void Logger::write_message(const std::string &message){
        if(file.is_open()){
            file << getCompactTimestamp() << " " << message << std::endl;
        }
    }

    void Logger::log(const std::string &message){
        write_message("LOG " + message);
    }

    void Logger::error(const std::string &message){
        write_message("ERROR " + message);
    }
}