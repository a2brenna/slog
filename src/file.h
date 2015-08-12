#ifndef __SLOG_FILE_H__
#define __SLOG_FILE_H__

#include "slog.h"
#include <memory>
#include <fstream>
#include <mutex>

namespace slog{

class File : public Log_Sink{

    private:
        bool _timestamp = false;
        std::shared_ptr<std::pair<std::ofstream, std::mutex>> _backend;

    public:
        File(std::shared_ptr<std::pair<std::ofstream, std::mutex>> backend);
        void write(const std::string &msg);
        void enable_timestamp();
        void disable_timestamp();

};

}


#endif
