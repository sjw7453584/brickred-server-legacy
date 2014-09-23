#ifndef BRICKRED_UNIX_SYSTEM_H
#define BRICKRED_UNIX_SYSTEM_H

namespace brickred {
namespace os {

bool daemon(bool change_dir = false, bool close_stdio = false);
bool createPidFile(const char *file);

} // namespace os
} // namespace brickred

#endif
