#ifndef __SLOG_SYSLOG_H__
#define __SLOG_SYSLOG_H__

#include "slog.h"

namespace slog{

class Syslog : public Log_Sink {

    private:
        const Priority _priority;
        const std::string _ident;
        const int _facility;

    public:
        void write(const std::string &msg);
        Syslog(const Priority &priority, const std::string &ident, const int &facility);

};

}

#endif
