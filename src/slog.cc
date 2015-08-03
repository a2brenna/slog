#include "slog.h"

bool syslog_initialized = false;

namespace slog{

Syslog::Syslog(const Priority& priority, const std::string &ident, const int &facility) {
    if(!syslog_initialized){
        openlog(ident.c_str(), LOG_PID, facility);
        syslog_initialized = true;
    }
    _priority = priority;
    _header = "";
}

Syslog::Syslog(const Priority& priority, const std::string &header, const std::string &ident, const int &facility) {
    if(!syslog_initialized){
        openlog(ident.c_str(), LOG_PID, facility);
        syslog_initialized = true;
    }
    _priority = priority;
    _header = header;
}

int Syslog::sync() {
    if (!_buffer.empty() && (_priority <= GLOBAL_PRIORITY)) {
        syslog(_priority, "%s%s", _header.c_str(), _buffer.c_str());
    }
    _buffer.erase();
    return 0;
}

int Syslog::overflow(int c) {
    if (c != EOF) {
        _buffer += static_cast<char>(c);
    } else {
        sync();
    }
    return c;
}

File::File(const Priority &priority, std::shared_ptr<std::pair<std::ofstream, std::mutex>> out) : _priority(priority){
    _out = out;
    _header = "";
}

File::File(const Priority &priority, const std::string &header, std::shared_ptr<std::pair<std::ofstream, std::mutex>> out) : _priority(priority){
    _out = out;
    _header = header;
}

int File::sync() {
    if (!_buffer.empty() && (_priority <= GLOBAL_PRIORITY)) {
        std::unique_lock<std::mutex> l(_out->second);
        _out->first << "["
            << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()
            << "] "
            << _buffer;
        _out->first.flush();
    }
    _buffer.erase();
    return 0;
}

int File::overflow(int c) {
    if (c != EOF) {
        _buffer += static_cast<char>(c);
    } else {
        sync();
    }
    return c;
}

}
