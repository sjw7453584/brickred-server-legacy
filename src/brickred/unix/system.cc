#include <brickred/unix/system.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdio>

namespace brickred {
namespace os {

bool daemon(bool change_dir, bool close_stdio)
{
    pid_t pid = ::fork();
    if (-1 == pid) {
        return false;
    } else if (pid != 0) {
        _exit(0);
    }

    if (::setsid() == -1) {
        return false;
    }

    umask(0);

    if (change_dir) {
        if (::chdir("/") != 0) {
            return false;
        }
    }

    if (close_stdio) {
        int fd = ::open("/dev/null", O_RDWR);
        if (-1 == fd) {
            return false;
        }

        if (::dup2(fd, STDIN_FILENO) == -1) {
            return false;
        }
        if (::dup2(fd, STDOUT_FILENO) == -1) {
            return false;
        }
        if (::dup2(fd, STDERR_FILENO) == -1) {
            return false;
        }

        if (fd > STDERR_FILENO) {
            ::close(fd);
        }
    }

    return true;
}

bool createPidFile(const char *file)
{
    FILE *fp = ::fopen(file, "w");
    if (NULL == fp) {
        return false;
    }

    ::fprintf(fp, "%d\n", ::getpid());
    ::fclose(fp);

    return true;
}

} // namespace os
} // namespace brickred
