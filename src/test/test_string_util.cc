#include <iostream>
#include <string>
#include <vector>
#include <ext/hash_map>

#include <brickred/string_util.h>

using namespace brickred;

int main(void)
{
    // split
    char str[] = ";1;2;3;";
    std::vector<std::string> result;

    string_util::split(str, ";", &result);

    for (size_t i = 0; i < result.size(); ++i) {
        std::cout << result[i] << std::endl;
    }

    // replace
    std::cout << string_util::replace("1;1;1", "1", "2", 1) << std::endl;
    std::cout << string_util::replace("1;1;1", "1", "2", 2) << std::endl;
    std::cout << string_util::replace("1;1;1", "1", "2") << std::endl;

    // string hash function
    __gnu_cxx::hash_map<std::string,
                        std::string,
                        string_util::Hash> m;
    m["111"] = "222";
    std::cout << m["111"] << std::endl;

    return 0;
}
