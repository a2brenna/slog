#include "slog.h"

namespace slog{

Priority GLOBAL_PRIORITY = kLogCrit;

int Log_Buffer::_sync(){
    if (!_buffer.empty() && (_priority <= GLOBAL_PRIORITY)) {
        _sink->write(_buffer);
    }
    _buffer = _header;
    return 0;
}

int Log_Buffer::sync(){
    std::lock_guard<std::mutex> l(_lock);
    return _sync();
}

int Log_Buffer::overflow(int c){
    std::lock_guard<std::mutex> l(_lock);
    if (c != EOF) {
        _buffer += static_cast<char>(c);
    } else {
        _sync();
    }
    return c;
}

Log_Buffer::Log_Buffer(std::shared_ptr<Log_Sink> sink, const Priority &priority, const std::string &header):
    _priority(priority),
    _header(header)
{
    _sink = sink;
    _buffer = _header;
}

}
