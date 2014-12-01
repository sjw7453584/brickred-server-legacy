#ifndef BRICKRED_LOG_CORE_H
#define BRICKRED_LOG_CORE_H

#include <cstddef>
#include <cstdarg>

#include <brickred/class_util.h>
#include <brickred/unique_ptr.h>

namespace brickred {

class LogSink;

class LogCore {
public:
    typedef size_t (*LogFormatter)(
        char *buffer, size_t buffer_size, int level,
        const char *filename, int line, const char *function,
        const char *format, va_list args
    );

    // call by LogCore constructor, default max logger count is 1
    // if you need more loggers, you should call this function
    // before call registerLogger()
    void setMaxLoggerCount(int count = 1);

    // logger_id must be in [0, max_logger_count)
    bool registerLogger(int logger_id, LogFormatter formatter = NULL,
                        int level_filter = -1);
    void removeLogger(int logger_id);
    bool addSink(int logger_id, LogSink *sink, int level_filter = -1);

    // log with formatter
    void log(int logger_id, int level,
             const char *filename, int line, const char *function,
             const char *format, ...);
    // raw log
    void log(int logger_id, int level,
             const char *buffer, size_t size);

    // change logger level filter
    void setLevelFilter(int logger_id, int level_filter);

private:
    BRICKRED_PRECREATED_SINGLETON(LogCore)
    
    class Impl;
    UniquePtr<Impl> pimpl_;
};

} // namespace brickred

#endif
