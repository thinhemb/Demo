#ifndef DatabaseManager_hpp
#define DatabaseManager_hpp

#include "../include/config.hpp"
#include <sstream>
#include <ctime>
#include <iomanip>
#include <sqlite3.h> 
#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/json.h"
#include <array>
#include <regex>
#include <math.h>



class DatabaseManager
{
    private:
        sqlite3* db;
        bool connected;
        std::string nameDB;
        int rc;
        std::array<std::string,22> fileLocation;
        void connectDB();
    public:
        DatabaseManager(std::string nameDB);
        ~DatabaseManager();
        virtual int convertTimeStamp (std::string time);
        bool insert_event(param_event event);
        bool insert_camera(param_camera camera);
        virtual bool getTypeCam(std::string type, json& data);
        virtual bool getEvent (json& data);
        virtual bool get_Info_Cam(json& data);

};
#endif