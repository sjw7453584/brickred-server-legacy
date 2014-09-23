#include <cstdio>

#include <brickred/codec/base64.h>

using namespace brickred;

int main(int argc, char *argv[])
{
    FILE *fp = NULL;

    if (1 == argc) {
        fp = stdin;
    } else if (2 == argc) {
        fp = ::fopen(argv[1], "rb");
        if (NULL == fp) {
            ::fprintf(stderr, "can not open file %s\n", argv[1]);
            return -1;
        }
    } else {
        ::fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return -1;
    }

    char buffer[300];

    for (;;) {
        size_t count = ::fread(buffer, 1, sizeof(buffer), fp);
        if (count > 0) {
            ::printf("%s", codec::base64Encode(buffer, count).c_str());
        }
        if (count < sizeof(buffer)) {
            break;
        }
    }

    ::fclose(fp);

    return 0;
}
