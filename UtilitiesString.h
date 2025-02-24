#ifndef __UTILITIESSTRING_H__
#define __UTILITIESSTRING_H__

#include <iostream>
#include <string>
#include <stdlib.h>
using std::string;

// 去除字符串前后的空格
static std::string trim_blank_space(const std::string& str) {
    std::string::size_type pos = str.find_first_not_of(' ');
    if (pos == std::string::npos) {
        return str;
    }

    std::string::size_type pos2 = str.find_last_not_of(' ');
    if (pos2 != std::string::npos) {
        return str.substr(pos, pos2 - pos + 1);
    }

    return str.substr(pos);
}

// 对字符串前后空格裁剪
static std::string stringfunction(const std::string& str) {
    return trim_blank_space(str);
}

// 将字符串转换成整数然后转换成 bool 值
static bool boolfunction(const std::string& str) {
    return static_cast<bool>(std::stoi(str));
}

// 将字符串转换成整数
static int intfunction(const std::string& str) {
    return std::stoi(str);
}


#endif // __UTILITIESSTRING_H__