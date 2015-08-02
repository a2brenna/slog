#ifndef __HGUTIL_LOG_H__
#define __HGUTIL_LOG_H__

#include <sstream>
#include <fstream>
#include <memory>
#include <mutex>
#include <syslog.h>

//The following is mostly stolen from http://stackoverflow.com/a/4457138

namespace slog{

typedef std::basic_streambuf<char, std::char_traits<char>> Char_Stream;

void Initialize_Logging(const std::string& ident, const int& facility);

enum LogPriority {
    kLogEmerg   = LOG_EMERG,   // system is unusable
    kLogAlert   = LOG_ALERT,   // action must be taken immediately
    kLogCrit    = LOG_CRIT,    // critical conditions
    kLogErr     = LOG_ERR,     // error conditions
    kLogWarning = LOG_WARNING, // warning conditions
    kLogNotice  = LOG_NOTICE,  // normal, but significant, condition
    kLogInfo    = LOG_INFO,    // informational message
    kLogDebug   = LOG_DEBUG    // debug-level message
};

class Syslog : public Char_Stream {
    private:
        std::string buffer_;
        int priority_;

    protected:
        int sync();
        int overflow(int c);
        std::string header_;

    public:
        explicit Syslog(const LogPriority& priority, const std::string &header);
        explicit Syslog(const LogPriority& priority);
};

class File : public Char_Stream {
    private:
        std::string buffer_;
        std::shared_ptr<std::pair<std::ofstream, std::mutex>> out_;

    protected:
        int sync();
        int overflow(int c);
        std::string header_;

    public:
        explicit File( std::shared_ptr<std::pair<std::ofstream, std::mutex>> out, const std::string &header);
        explicit File( std::shared_ptr<std::pair<std::ofstream, std::mutex>> out);
};

class Log : public std::ostringstream {
    private:
        std::shared_ptr<Char_Stream> _backend;

    public:
        explicit Log(std::shared_ptr<Char_Stream> backend){
            _backend = backend;
            (static_cast<std::ostream*>(this))->rdbuf(_backend.get());
        }

};

}

#endif
