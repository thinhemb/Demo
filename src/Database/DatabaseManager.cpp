#include "DatabaseManager.hpp"
#include <sys/statvfs.h>
#include <dirent.h>
#include <sys/stat.h>

void DatabaseManager::connectDB()
{
    this->rc=sqlite3_open(this->nameDB.c_str(),&db);
    if(this->rc)
    {
        std::cout<<"Can't open database, please check your database" << std::endl;
    }
    else
    {
        this->connected=true;
        std::cout << "Opened database successfully" << std::endl;
    }
}

DatabaseManager::DatabaseManager(std::string nameDB)
{
    this->nameDB;
    this->connected=false;
    sleep(1);
    connectDB();
}
DatabaseManager::~DatabaseManager()
{
    sqlite3_close(db);
}


int DatabaseManager::convertTimeStamp (std::string time){
    std::tm t{};
    time.erase(time.find("+"));
    time.replace(time.find("T"),1," ");
    std::istringstream ss(time);
    ss >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
        throw std::runtime_error{"failed to parse time string"};
        return 0;
    }
    else
    {
        return std::mktime(&t);
    }
}

bool DatabaseManager::insert_event(param_event event)
{
    std::string sql ="INSERT into Event (stream_id, path_video , object_id , time_stamp, camera_id,video_id) "
            "values ('"+ std::to_string(event.stream_id) +"', '" + event.path_video+"', '"+ std::to_string(event.object_id)+"', '"+std::to_string(convertTimeStamp(event.time_stamp))+"', '"+std::to_string(event.camera_id)+"', '"+std::to_string(event.video_id)+ "');";
    std::cout<<"SQL: "<<sql<<std::endl;

    char *zErrMsg = 0;
    this->rc = sqlite3_exec(this->db, sql.c_str(), NULL, 0, &zErrMsg);
    if (this->rc != SQLITE_OK) {
        std::cout << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
        return false;
    } else { 
        return true;
    }     
}

bool DatabaseManager::insert_camera(param_camera camera)
{
    std::string sql="INSERT into Camera (int camera_id,type,name_camera,created_time,updated_time,path_image_camera)"
        "values ('"+std::to_string(camera.camera_id)+"','"+ camera.type+"','"+camera.name_camera+"','"+std::to_string(convertTimeStamp(camera.created_time)) +"','"+std::to_string(convertTimeStamp(camera.updated_time))+"','"+camera.path_image_camera+"');";

    std::cout<<"SQL: "<<sql<<std::endl;

    char *zErrMsg = 0;
    this->rc = sqlite3_exec(this->db, sql.c_str(), NULL, 0, &zErrMsg);
    if (this->rc != SQLITE_OK) {
        std::cout << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
        return false;
    } else { 
        return true;
    } 
}

bool DatabaseManager::getTypeCam(std::string type, json& data)
{
    bool result;
    sqlite3_stmt *stmt;
    std::string sql= "SELECT * FROM Camera WHERE type = '" +  type + "';";

    int error = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (error != SQLITE_OK) {
        std::cout << "Error not get information camera" << std::endl;
        result=false;
    } 
    else {
        while (sqlite3_step(stmt) != SQLITE_DONE) 
        {
            json j_data;
            j_data["id"] = sqlite3_column_int(stmt,0);
            j_data["camera_id"] = (const char*) (sqlite3_column_text(stmt,1));
            j_data["type"] = (const char*) (sqlite3_column_text(stmt,2));
            j_data["name_camera"] = (const char*) (sqlite3_column_text(stmt,3));
            j_data["created_time"] = (const char*) (sqlite3_column_text(stmt,4));
            j_data["updated_time"] = (const char *) sqlite3_column_text(stmt,5);
            j_data["path_image_camera"] = (const char *) sqlite3_column_text(stmt,6);
            data.push_back(j_data);
        }
    }
    return true;
}
bool DatabaseManager::get_Info_Cam(json& data)
{
    bool result;
    sqlite3_stmt *stmt;
    std::string sql= "SELECT * FROM Camera;";

    int error = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (error != SQLITE_OK) {
        std::cout << "Error not get information camera" << std::endl;
        result=false;
    } 
    else {
        while (sqlite3_step(stmt) != SQLITE_DONE) 
        {
            json j_data;
            j_data["id"] = sqlite3_column_int(stmt,0);
            j_data["camera_id"] = (const char*) (sqlite3_column_text(stmt,1));
            j_data["type"] = (const char*) (sqlite3_column_text(stmt,2));
            j_data["name_camera"] = (const char*) (sqlite3_column_text(stmt,3));
            j_data["created_time"] = (const char*) (sqlite3_column_text(stmt,4));
            j_data["updated_time"] = (const char *) sqlite3_column_text(stmt,5);
            j_data["path_image_camera"] = (const char *) sqlite3_column_text(stmt,6);
            data.push_back(j_data);
        }
    }
    return true;
}

bool DatabaseManager::getEvent(json& data)
{
    sqlite3_stmt *stmt;
    std::string sql = "SELECT * FROM Event;";
    int error = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (error != SQLITE_OK) {
        return false;
    } else
    {
        while (sqlite3_step(stmt) != SQLITE_DONE) {
            json j_data;
            j_data["id"] = sqlite3_column_int(stmt,0);
            j_data["stream_id"] = (const char*) (sqlite3_column_text(stmt,1));
            j_data["path_video"] = (const char*) (sqlite3_column_text(stmt,2));
            j_data["object_id"] = (const char*) (sqlite3_column_text(stmt,3));
            j_data["time_stamp"] = (const char*) (sqlite3_column_text(stmt,4));
            j_data["camera_id"] = (const char *) sqlite3_column_text(stmt,5);
            j_data["video_id"] = (const char *) sqlite3_column_text(stmt,6);
            
            data.push_back(j_data);
        }
        sqlite3_finalize(stmt);
        return true;
    }

}


