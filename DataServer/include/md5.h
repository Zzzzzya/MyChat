#pragma once

#include <openssl/md5.h>

#include <iomanip>
#include <sstream>
#include <string>

std::string md5(const std::string& password, const std::string& hxm) {
    std::string input = password;
    input = input + hxm;
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)input.c_str(), input.length(), (unsigned char*)&digest);

    std::stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
        ss << std::hex << std::setw(2) << std::setfill('0')
           << (unsigned int)digest[i];

    return ss.str();
}