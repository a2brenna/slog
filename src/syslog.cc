#include "syslog.h"

namespace slog{

    void Syslog::write(const std::string &msg){
        syslog(_priority, "%s", msg.c_str());
    }

}
