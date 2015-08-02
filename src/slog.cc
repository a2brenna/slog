#include "slog.h"

namespace slog{

void Initialize_Logging(const std::string& ident, const int& facility){
    openlog(ident.c_str(), LOG_PID, facility);
}

Syslog::Syslog(const Priority& priority) {
    _priority = priority;
    _header = "";
}

Syslog::Syslog(const Priority& priority, const std::string &header) {
    _priority = priority;
    _header = header;
}

int Syslog::sync() {
    if (_buffer.length()) {
        syslog(_priority, "%s%s", _header.c_str(), _buffer.c_str());
        _buffer.erase();
    }
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

File::File(std::shared_ptr<std::pair<std::ofstream, std::mutex>> out){
    _out = out;
    _header = "";
}

File::File(std::shared_ptr<std::pair<std::ofstream, std::mutex>> out, const std::string &header){
    _out = out;
    _header = header;
}

int File::sync() {
    if (_buffer.length()) {
        {
            std::unique_lock<std::mutex> l(_out->second);
            _out->first << "["
                << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()
                << "] "
                << _buffer;
            _out->first.flush();
        }
        _buffer.erase();
    }
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
