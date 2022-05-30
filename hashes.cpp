#include "hashes.hpp"

static const int K_READ_BUF_SIZE{ 1024 * 16 };

std::optional<std::string> CalcSha256ForFile(const std::string & filename)
{
    SHA256_CTX context;
    if(!SHA256_Init(&context))
    {
        return std::nullopt;
    }

    // Read file and update calculated SHA
    char buf[K_READ_BUF_SIZE];
    std::ifstream file(filename, std::ifstream::binary);
    while (file.good())
    {
        file.read(buf, sizeof(buf));
        if(!SHA256_Update(&context, buf, file.gcount()))
        {
            return std::nullopt;
        }
    }

    // Get Final SHA
    unsigned char result[SHA256_DIGEST_LENGTH];
    if(!SHA256_Final(result, &context))
    {
        return std::nullopt;
    }

    // Transform byte-array to string
    std::stringstream shastr;
    shastr << std::hex << std::setfill('0');
    for (const auto &byte: result)
    {
        shastr << std::setw(2) << (int)byte;
    }
    return shastr.str();
}
std::optional<std::string> CalcSha256ForString(const std::string & str){

    SHA256_CTX context;
    if(!SHA256_Init(&context))
        return std::nullopt;

    // Read file and update calculated SHA
    char buf[K_READ_BUF_SIZE];
    if(!SHA256_Update(&context, buf, str.size()))
        return std::nullopt;

    // Get Final SHA
    unsigned char result[SHA256_DIGEST_LENGTH];
    if(!SHA256_Final(result, &context))
        return std::nullopt;

    // Transform byte-array to string
    std::stringstream shastr;
    shastr << std::hex << std::setfill('0');
    for (const auto &byte: result)
    {
        shastr << std::setw(2) << (int)byte;
    }
    return shastr.str();
}