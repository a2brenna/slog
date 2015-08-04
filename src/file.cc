#include "file.h"

namespace slog{

    File::File(std::shared_ptr<std::pair<std::ofstream, std::mutex>> backend){
        _backend = backend;
    }

    void File::write(const std::string &msg){
        std::unique_lock<std::mutex> l(_backend->second);
        _backend->first << msg;
        _backend->first.flush();
    }

}
