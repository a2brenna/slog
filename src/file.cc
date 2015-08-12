#include "file.h"
#include <chrono>

namespace slog{

    File::File(std::shared_ptr<std::pair<std::ofstream, std::mutex>> backend){
        _backend = backend;
    }

    void File::write(const std::string &msg){
        std::unique_lock<std::mutex> l(_backend->second);
        if(_timestamp){
            char time[255];
            const auto current_time = std::chrono::high_resolution_clock::now();
            const std::time_t t = std::chrono::high_resolution_clock::to_time_t(current_time);
            //The following is not threadsafe due to std::localtime's use of some global storage
            //I don't care to fix this at the moment
            std::strftime(time, 255, "%b %d %H:%M:%S", std::localtime(&t));

            _backend->first << time;
        }
        _backend->first << msg;
        _backend->first.flush();
    }

    void File::enable_timestamp(){
        _timestamp = true;
    }

    void File::disable_timestamp(){
        _timestamp = false;
    }

}
