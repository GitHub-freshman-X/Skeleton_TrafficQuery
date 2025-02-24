#ifndef __UTILITIESRANDOM_H__
#define __UTILITIESRANDOM_H__

#include <chrono>
#include <climits>
#include <random>
#include <algorithm>
#include <set>
#include <string>

using std::string;
using std::set;

const int MaxStringLength=10;
const int ALPHABET_SIZE=26;
const int MaxIntValue=100000;

static std::mt19937 gen(std::random_device{}());
static std::uniform_int_distribution<int> dist(1, INT_MAX);

// 创建大小为N的随机T类型数组
template<class T>
T* makeRandomArray(int N, T(*Function)(int)){
    T *A=new T[N];
    for(int i=0; i<N; ++i){
        A[i]=Function(i);
    }
    unsigned seed=std::chrono::system_clock::now().time_since_epoch().count();
    if(std::is_sorted(A, A+N)){
        std::shuffle(A, A+N, std::default_random_engine(seed));
    }
    return A;
}

// 创建一个大小为N，值不重复的随机字符串数组
template<class T>
T* makeUniqueRandomArray(int N, T(*Function)()){
    T *A=new T[N];
    set<T> uniqueStrings;
    int i=0;

    while(uniqueStrings.size()<N){
        string randomString=Function();
        if(uniqueStrings.find(randomString)==uniqueStrings.end()){
            uniqueStrings.insert(randomString);
            A[i]=randomString;
            ++i;
        }
    }
    return A;
}

// 返回一个长度是随机值length的字符串
static string randomString(){
    string s="";
    int length = dist(gen)%MaxStringLength+1;

    s.resize(length);
    for(int i=0; i<length; ++i){
        s[i] = (char)(dist(gen)%ALPHABET_SIZE+'a');
    }
    return s;
}

// 返回一个值为1~MaxIntValue的随机整数
static int randomInt(){
    return dist(gen)%MaxIntValue+1;
}

// 返回一个值为1~n的随机整数
static int randomInt_n(int n){
    return dist(gen)%n+1;
}

#endif // __UTILITIESRANDOM_H__