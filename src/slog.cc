#include "slog.h"

namespace slog{

void Initialize_Logging(const std::string& ident, const int& facility){
    openlog(ident.c_str(), LOG_PID, facility);
}

Syslog::Syslog(const LogPriority& priority) {
    priority_ = priority;
    header_ = "";
}

Syslog::Syslog(const LogPriority& priority, const std::string &header) {
    priority_ = priority;
    header_ = header;
}

int Syslog::sync() {
    if (buffer_.length()) {
        syslog(priority_, "%s%s", header_.c_str(), buffer_.c_str());
        buffer_.erase();
    }
    return 0;
}

int Syslog::overflow(int c) {
    if (c != EOF) {
        buffer_ += static_cast<char>(c);
    } else {
        sync();
    }
    return c;
}

File::File(std::shared_ptr<std::pair<std::ofstream, std::mutex>> out){
    out_ = out;
    header_ = "";
}

File::File(std::shared_ptr<std::pair<std::ofstream, std::mutex>> out, const std::string &header){
    out_ = out;
    header_ = header;
}

int File::sync() {
    if (buffer_.length()) {
        //syslog(priority_, "%s%s", header_.c_str(), buffer_.c_str());
        {
            std::unique_lock<std::mutex> l(out_->second);
            out_->first << "["
                << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()
                << "] "
                << buffer_;
            out_->first.flush();
        }
        buffer_.erase();
    }
    return 0;
}

int File::overflow(int c) {
    if (c != EOF) {
        buffer_ += static_cast<char>(c);
    } else {
        sync();
    }
    return c;
}

}
