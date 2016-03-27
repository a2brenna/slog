#ifndef __HGUTIL_LOG_H__
#define __HGUTIL_LOG_H__

#include <sstream>
#include <fstream>
#include <memory>
#include <mutex>
#include <syslog.h>

//SEE: http://stackoverflow.com/a/4457138

namespace slog{

typedef std::basic_streambuf<char, std::char_traits<char>> Char_Stream;

enum Priority {
    kLogEmerg   = LOG_EMERG,   // system is unusable
    kLogAlert   = LOG_ALERT,   // action must be taken immediately
    kLogCrit    = LOG_CRIT,    // critical conditions
    kLogErr     = LOG_ERR,     // error conditions
    kLogWarning = LOG_WARNING, // warning conditions
    kLogNotice  = LOG_NOTICE,  // normal, but significant, condition
    kLogInfo    = LOG_INFO,    // informational message
    kLogDebug   = LOG_DEBUG    // debug-level message
};

extern Priority GLOBAL_PRIORITY;

class Backend_Uninitialized {};

class Log_Sink{

    public:
        virtual void write(const std::string &msg) = 0;

};

class Log_Buffer : public Char_Stream {

    private:
        std::mutex _lock;
        std::string _buffer;
        std::shared_ptr<Log_Sink> _sink;
        const Priority _priority;
        const std::string _header;
        int _sync();

    protected:
        int sync();
        int overflow(int c);

    public:
        Log_Buffer(std::shared_ptr<Log_Sink> sink, const Priority &priority, const std::string &header);

};

class Log : public std::basic_ostringstream<char> {
    private:
        Log_Buffer _backend;

    public:
        explicit Log(std::shared_ptr<Log_Sink> sink, const Priority &priority, const std::string &header) : _backend(sink, priority, header){
            (static_cast<std::ostream*>(this))->rdbuf(&_backend);
        }
};

}

#endif
