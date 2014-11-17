#include <brickred/log_core.h>

#include <cstdarg>
#include <cstdio>
#include <vector>

#include <brickred/log_sink.h>

namespace brickred {

#define MAX_LOGGER_COUNT 8
#define LOG_BUFFER_SIZE 4096

class Logger {
public:
    typedef LogCore::LogFormatter LogFormatter;
    typedef std::vector<LogSink *> LogSinkVector;
    typedef std::vector<int> LogLevelVector;

    explicit Logger(LogFormatter formatter);
    ~Logger();

    bool addSink(LogSink *sink, int level_filter);
    void log(int level, const char *filename, int line,
             const char *function, const char *format, va_list args);
    void log(int level, const char *buffer, size_t size);

private:
    LogFormatter formatter_;
    LogSinkVector sinks_;
    LogLevelVector sink_level_filters_;
};

///////////////////////////////////////////////////////////////////////////////
Logger::Logger(LogFormatter formatter) : formatter_(formatter)
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

    bool registerLogger(int logger_id, LogFormatter formatter);
    void removeLogger(int logger_id);
    bool addSink(int logger_id, LogSink *sink, int level_filter);
    void log(int logger_id, int level,
             const char *filename, int line, const char *function,
             const char *format, va_list args);
    void log(int logger_id, int level,
             const char *buffer, size_t size);

private:
    LoggerVector loggers_;
};

///////////////////////////////////////////////////////////////////////////////
BRICKRED_SINGLETON2_IMPL(LogCore)

LogCore::Impl::Impl() :
    loggers_(MAX_LOGGER_COUNT, NULL)
{
}

LogCore::Impl::~Impl()
{
    for (size_t i = 0; i < loggers_.size(); ++i) {
        if (loggers_[i] != NULL) {
            delete loggers_[i];
        }
    }
}

bool LogCore::Impl::registerLogger(int logger_id, LogFormatter formatter)
{
    if (logger_id < 0 || logger_id >= MAX_LOGGER_COUNT) {
        return false;
    }
    if (loggers_[logger_id] != NULL) {
        return false;
    }

    loggers_[logger_id] = new Logger(formatter);

    return true;
}

void LogCore::Impl::removeLogger(int logger_id)
{
    if (logger_id < 0 || logger_id >= MAX_LOGGER_COUNT) {
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
    if (logger_id < 0 || logger_id >= MAX_LOGGER_COUNT) {
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
    if (logger_id < 0 || logger_id >= MAX_LOGGER_COUNT) {
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
    if (logger_id < 0 || logger_id >= MAX_LOGGER_COUNT) {
        return;
    }
    if (NULL == loggers_[logger_id]) {
        return;
    }

    loggers_[logger_id]->log(level, buffer, size);
}

///////////////////////////////////////////////////////////////////////////////
LogCore::LogCore() :
    pimpl_(new Impl())
{
}

LogCore::~LogCore()
{
}

bool LogCore::registerLogger(int logger_id, LogFormatter formatter)
{
    return pimpl_->registerLogger(logger_id, formatter);
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
