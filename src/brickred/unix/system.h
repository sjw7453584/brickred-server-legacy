#ifndef BRICKRED_UNIX_SYSTEM_H
#define BRICKRED_UNIX_SYSTEM_H

namespace brickred {
namespace os {

bool daemon(bool change_dir = false, bool close_stdio = false);
bool createPidFile(const char *file);

typedef void (*SignalHandler)(int);
SignalHandler signal(int signum, SignalHandler sighandler);

} // namespace os
} // namespace brickred

#endif
