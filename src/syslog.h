#ifndef __SLOG_SYSLOG_H__
#define __SLOG_SYSLOG_H__

#include "slog.h"

bool _syslog_initialized = false;

namespace slog{

class Bad_Facility {};

void initialize_syslog(const std::string &ident, const int &facility);

class Syslog : public Log_Sink {

    private:
        const Priority _priority;

    public:
        void write(const std::string &msg);
        Syslog(const Priority &priority);

};

}

#endif
