#include "syslog.h"

bool syslog_initialized = false;

namespace slog{

    Syslog::Syslog(const Priority &priority, const std::string &ident, const int &facility):
        _priority(priority),
        _ident(ident),
        _facility(facility)
    {
        //TODO some sort of atomic magic?
        if(!syslog_initialized){
            openlog(_ident.c_str(), LOG_PID, _facility);
            syslog_initialized = true;
        }
    }

    void Syslog::write(const std::string &msg){
        syslog(_priority, "%s", msg.c_str());
    }

}
