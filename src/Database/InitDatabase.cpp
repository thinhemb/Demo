
#include <string>
#include <sqlite3.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <sys/types.h>

#include "../include/config.hpp"
#include "InitDatabase.hpp"


static bool exists_test (const std::string& path_mem);
static bool create_table(std::string path_mem);
static bool check_table(sqlite3* DB, std::string nameTB);

static bool exists_test (const std::string& path_mem) 
{
  struct stat buffer;   
  return (stat (path_mem.c_str(), &buffer) == 0); 
}

int init_database(const char* path_memory, std::string nameDB){
    std::string path = path_memory;
    std::string path_mem = path + nameDB;
    int exist = exists_test(path_mem);
    if (exist == 0){
        while (!exists_test(path_mem))
            std::ofstream outfile (path_mem.c_str());
    }
    else{
        std::cout <<nameDB <<" existed" << std::endl;
    }
    if (create_table(path_mem)) return 1;
    else return 0;
}


static bool create_table(std::string path_mem)
{
    std::cout << "Create table" << std::endl;
    sqlite3* DB;
    int exit;
    bool ret;
    exit = sqlite3_open(path_mem.c_str(), &DB);
    if(exit) 
    {
        std::cout << "Can't open database, please check your database";
        ret = false;
    } else 
    {
        std::cout << "Opened database successfully" << std::endl;
        if (check_table(DB,"Camera") && check_table(DB,"Event"))
        {
            ret = true;
        }
        else
            ret = false;
    }
    sqlite3_close(DB);    ///
    return ret;
}


static bool check_table(sqlite3* DB,std::string nameTB) 
{
    sqlite3_stmt *stmt;
    int exist;
    std::string sql = "select count(type) from sqlite_master where type='table' and name='"+ nameTB+"';";
    int rc = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) 
    {
        return false;
    } else {
        while (sqlite3_step(stmt) != SQLITE_DONE) 
        {
            exist = sqlite3_column_int(stmt, 0);
        }
        bool Flags=true;
        if (exist == 0) 
        {   
            if( nameTB== "Camera")
            {
                std::string sql = "CREATE TABLE "+ nameTB+" ("
                              "id	INTEGER NOT NULL,"
                              "camera_id INTEGER NOT NULL,"
                              "type TEXT NOT NULL,"
                              "name_camera TEXT NOT NULL,"
                              "created_time	TEXT NOT NULL,"
                              "updated_time	TEXT NOT NULL,"
                              "path_image_camera TEXT NOT NULL,"
                              "PRIMARY KEY(id AUTOINCREMENT));";
                char *messaggeError;
                int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
                if (exit != SQLITE_OK) {
                    std::cout << "Error: Cant Not Create Table "<<nameTB << std::endl;
                    sqlite3_free(messaggeError);
                    Flags= false;
                } else {
                    std::cout <<nameTB << " table created Successfully" << std::endl;
                    
                }
            }
            if (nameTB== "Event")
            {
                std::string sql = "CREATE TABLE "+ nameTB+" ("
                              "id	INTEGER NOT NULL,"
                              "stream_id INTEGER NOT NULL,"
                              "path_video	TEXT NOT NULL,"
                              "object_id	INTEGER NOT NULL,"
                              "time_stamp	INTEGER NOT NULL,"
                              "camera_id	INTEGER NOT NULL,"
                              "video_id	TEXT NOT NULL,"
                              "PRIMARY KEY(id AUTOINCREMENT));";
                char *messaggeError;
                int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
                if (exit != SQLITE_OK) 
                {
                    std::cout << "Error: Cant Not Create Table "<<nameTB << std::endl;
                    sqlite3_free(messaggeError);
                    Flags= false;
                } else 
                {
                    std::cout <<nameTB << " table created Successfully" << std::endl;
                }
            }

            
            
        } 
        else 
        {
            std::cout << nameTB <<" table existed";
            return true;
        }
        return Flags;//true or false
    }
}