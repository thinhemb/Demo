



#ifndef config_hpp
#define config_hpp

#include <string>
#include <chrono>
#include <fstream>
#include <ostream>


typedef struct param_event {
    int stream_id;
    std::string path_video;
    int object_id;
    std::string time_stamp;
    int camera_id;
    int video_id;
}param_event;

typedef struct param_camera{
    int camera_id;
    std::string type;
    std::string name_camera;
    std::string created_time;
    std::string updated_time;
    std::string path_image_camera;
}param_camera;





#endif /* config_hpp */