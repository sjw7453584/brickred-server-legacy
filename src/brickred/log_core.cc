#include <brickred/log_core.h>

#include <cstdarg>
#include <cstdio>
#include <vector>

#include <brickred/log_sink.h>

namespace brickred {

#define LOG_BUFFER_SIZE 4096

class Logger {
public:
    typedef LogCore::LogFormatter LogFormatter;
    typedef std::vector<LogSink *> LogSinkVector;
    typedef std::vector<int> LogLevelVector;

    explicit Logger(LogFormatter formatter, int level_filter);
    ~Logger();

    bool addSink(LogSink *sink, int level_filter);
    void log(int level, const char *filename, int line,
             const char *function, const char *format, va_list args);
    void log(int level, const char *buffer, size_t size);
    void setLevelFilter(int level_filter) { level_filter_ = level_filter; }

private:
    LogFormatter formatter_;
    int level_filter_;
    LogSinkVector sinks_;
    LogLevelVector sink_level_filters_;
};

///////////////////////////////////////////////////////////////////////////////
Logger::Logger(LogFormatter formatter, int level_filter) :
    formatter_(formatter), level_filter_(level_filter)
{
}

Logger::~Logger()
{
    for (size_t i = 0; i < sinks_.size(); ++i) {
        delete sinks_[i];
    }
}

bool Logger::addSink(LogSink *sink, int level_filter)
{
    sinks_.reserve(sinks_.size() + 1);
    sink_level_filters_.reserve(sink_level_filters_.size() + 1);

    sinks_.push_back(sink);
    sink_level_filters_.push_back(level_filter);

    return true;
}

void Logger::log(int level, const char *filename, int line,
                 const char *function, const char *format, va_list args)
{
    if (level < level_filter_) {
        return;
    }

    char buffer[LOG_BUFFER_SIZE];
    size_t count = 0;
    bool buffer_ready = false;

    for (size_t i = 0; i < sinks_.size(); ++i) {
        if (level < sink_level_filters_[i]) {
            continue;
        }

        // lazy format
        if (!buffer_ready) {
            if (NULL == formatter_) {
                count = ::vsnprintf(buffer, sizeof(buffer), format, args);
            } else {
                count = formatter_(buffer, sizeof(buffer),
                                   level, filename, line, function,
                                   format, args);
            }
            buffer_ready = true;
        }

        sinks_[i]->log(buffer, count);
    }
}

void Logger::log(int level, const char *buffer, size_t size)
{
    if (level < level_filter_) {
        return;
    }

    for (size_t i = 0; i < sinks_.size(); ++i) {
        if (level < sink_level_filters_[i]) {
            continue;
        }

        sinks_[i]->log(buffer, size);
    }
}

///////////////////////////////////////////////////////////////////////////////
class LogCore::Impl {
public:
    typedef LogCore::LogFormatter LogFormatter;
    typedef std::vector<Logger *> LoggerVector;

    Impl();
    ~Impl();

    void setMaxLoggerCount(int count);

    bool registerLogger(int logger_id, LogFormatter formatter,
                        int level_filter);
    void removeLogger(int logger_id);
    bool addSink(int logger_id, LogSink *sink, int level_filter);

    void log(int logger_id, int level,
             const char *filename, int line, const char *function,
             const char *format, va_list args);
    void log(int logger_id, int level,
             const char *buffer, size_t size);

    void setLevelFilter(int logger_id, int level_filter);

private:
    LoggerVector loggers_;
};

///////////////////////////////////////////////////////////////////////////////
BRICKRED_SINGLETON2_IMPL(LogCore)

LogCore::Impl::Impl()
{
}

LogCore::Impl::~Impl()
{
    for (size_t i = 0; i < loggers_.size(); ++i) {
        delete loggers_[i];
    }
}

void LogCore::Impl::setMaxLoggerCount(int count)
{
    if (count < 0) {
        return;
    }

    if (count < (int)loggers_.size()) {
        for (size_t i = count; i < loggers_.size(); ++i) {
            delete loggers_[i];
        }
    }
    loggers_.resize(count, NULL);
}

bool LogCore::Impl::registerLogger(int logger_id, LogFormatter formatter,
                                   int level_filter)
{
    if (logger_id < 0 || logger_id >= (int)loggers_.size()) {
        return false;
    }
    if (loggers_[logger_id] != NULL) {
        return false;
    }

    loggers_[logger_id] = new Logger(formatter, level_filter);

    return true;
}

void LogCore::Impl::removeLogger(int logger_id)
{
    if (logger_id < 0 || logger_id >= (int)loggers_.size()) {
        return;
    }
    if (NULL == loggers_[logger_id]) {
        return;
    }

    delete loggers_[logger_id];
    loggers_[logger_id] = NULL;
}

bool LogCore::Impl::addSink(int logger_id, LogSink *sink, int level_filter)
{
    if (logger_id < 0 || logger_id >= (int)loggers_.size()) {
        return false;
    }
    if (NULL == loggers_[logger_id]) {
        return false;
    }

    return loggers_[logger_id]->addSink(sink, level_filter);
}

void LogCore::Impl::log(int logger_id, int level, const char *filename,
                        int line, const char *function,
                        const char *format, va_list args)
{
    if (logger_id < 0 || logger_id >= (int)loggers_.size()) {
        return;
    }
    if (NULL == loggers_[logger_id]) {
        return;
    }

    loggers_[logger_id]->log(level, filename, line, function, format, args);
}

void LogCore::Impl::log(int logger_id, int level,
                        const char *buffer, size_t size)
{
    if (logger_id < 0 || logger_id >= (int)loggers_.size()) {
        return;
    }
    if (NULL == loggers_[logger_id]) {
        return;
    }

    loggers_[logger_id]->log(level, buffer, size);
}

void LogCore::Impl::setLevelFilter(int logger_id, int level_filter)
{
    if (logger_id < 0 || logger_id >= (int)loggers_.size()) {
        return;
    }
    if (NULL == loggers_[logger_id]) {
        return;
    }

    loggers_[logger_id]->setLevelFilter(level_filter);
}

///////////////////////////////////////////////////////////////////////////////
LogCore::LogCore() :
    pimpl_(new Impl())
{
    setMaxLoggerCount();
}

LogCore::~LogCore()
{
}

void LogCore::setMaxLoggerCount(int count)
{
    pimpl_->setMaxLoggerCount(count);
}

bool LogCore::registerLogger(int logger_id, LogFormatter formatter,
                             int level_filter)
{
    return pimpl_->registerLogger(logger_id, formatter, level_filter);
}

void LogCore::removeLogger(int logger_id)
{
    pimpl_->removeLogger(logger_id);
}

bool LogCore::addSink(int logger_id, LogSink *sink, int level_filter)
{
    return pimpl_->addSink(logger_id, sink, level_filter);
}

void LogCore::log(int logger_id, int level,
                  const char *filename, int line, const char *function,
                  const char *format, ...)
{
    va_list args;
    va_start(args, format);
    pimpl_->log(logger_id, level, filename, line, function, format, args);
    va_end(args);
}

void LogCore::log(int logger_id, int level,
                  const char *buffer, size_t size)
{
    pimpl_->log(logger_id, level, buffer, size);
}

} // namespace brickred
