#include <iostream>

#include <brickred/codec/sha1.h>
#include <brickred/codec/base64.h>

using namespace brickred;

int main(void)
{
    std::cout << codec::sha1("111") << std::endl;
    std::cout << codec::base64Encode("111") << std::endl;
    std::cout << codec::base64Encode("11") << std::endl;
    std::cout << codec::base64Encode("1") << std::endl;
    std::cout << codec::base64Decode("MTEx") << std::endl;
    std::cout << codec::base64Decode("MTE=") << std::endl;
    std::cout << codec::base64Decode("MQ==") << std::endl;
}
