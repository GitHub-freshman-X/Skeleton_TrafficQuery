#ifndef __UTILITIESSTRING_H__
#define __UTILITIESSTRING_H__

#include <string>
#include <stdlib.h>
using std::string;

// 裁剪字符串前后的空格
static string trim_blank_space(const string &str){
    string::size_type pos=str.find_first_not_of(' ');

    // 若找不到空格，string::npos是一个常数，表示size_t的最大值，用于表示不存在的位置
    if(pos==string::npos){
        return str;
    }

    string::size_type pos2=str.find_last_not_of(' ');
    if(pos2!=string::npos){
        return str.substr(pos, pos2-pos+1);
    }

    return str.substr(pos);
}
//――――――――――――――――
//版权声明：本文为CSDN博主「MangoGZH 」的原创文章，遵循CC 4.0 BY - SA版权协议，转载请附上原文出处链接及本声明。
//原文链接：https ://blog.csdn.net/weixin_43793960/article/details/106193050

// 对字符串前后空格裁剪
static string stringfunction(char *str){
    return trim_blank_space(str);
}

// 将字符串转换成整数然后转换成bool值
static bool boolfunction(char *str){
    return bool(std::atoi(str));
}

// 将字符串转换成整数
static int intfunction(char *str){
    return std::atoi(str);
}

#endif // __UTILITIESSTRING_H__