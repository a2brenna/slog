#ifndef __SLOG_SYSLOG_H__
#define __SLOG_SYSLOG_H__

#include "slog.h"

bool _syslog_initialized = false;

namespace slog{

class Bad_Facility {};

void initialize_syslog(const std::string &ident, const int &facility){
    //TODO: check facility
    if(ident.empty()){
        openlog(nullptr, LOG_PID, facility);
    }
    else{
        openlog(ident.c_str(), LOG_PID, facility);
    }

    _syslog_initialized = true;
}

class Syslog : public Log_Sink {

    private:
        const Priority _priority;

    public:
        void write(const std::string &msg);
        Syslog(const Priority &priority):
            _priority(priority)
        {
            if(!_syslog_initialized){
                throw Backend_Uninitialized();
            }
        }

};

}

#endif
