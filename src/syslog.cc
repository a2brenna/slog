#include "syslog.h"

namespace slog{

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

	Syslog::Syslog(const Priority &priority):
            _priority(priority)
	{
		if(!_syslog_initialized){
			throw Backend_Uninitialized();
		}
	}


    void Syslog::write(const std::string &msg){
        syslog(_priority, "%s", msg.c_str());
    }

}
