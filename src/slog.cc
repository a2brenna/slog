#include "slog.h"

namespace slog{

Priority GLOBAL_PRIORITY = kLogCrit;

int Log_Buffer::sync(){
    if (!_buffer.empty() && (_priority <= GLOBAL_PRIORITY)) {
        _sink->write(_buffer);
    }
    _buffer = _header;
    return 0;
}

int Log_Buffer::overflow(int c){
    if (c != EOF) {
        _buffer += static_cast<char>(c);
    } else {
        sync();
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
