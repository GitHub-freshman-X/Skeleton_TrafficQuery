#ifndef __TRAFFICADVISORYCHECK_H__
#define __TRAFFICADVISORYCHECK_H__

#include <iostream>
#include <algorithm>

#include "TrafficAdvisory.h"
using std::cout;
using std::endl;
using std::cerr;

class TrafficAdvisoryCheck{
public:
    TrafficAdvisoryCheck(TrafficAdvisory &other);

    /**
     * 检查源点到终点的五个路径功能，如果第一和第二个参数为空，则检查所有顶点间的五个路径功能。
	 * 第三个参数控制是否输出详细信息。
    */
    bool checkAll(string source="", string destination="", bool whetherOutput=false);

    /**
     * filename1是路径文件,filename2是路径距离文件。判断filename1文件的路径是否可行，如果可行是否和filename2中的距离相等。
     * 如果有路径不可行，或者距离不匹配，则返回false。否则返回true。
    */
    bool checkFiles(const string &filename1, const string &filename2);

private:
    TrafficAdvisory solver;
};

#endif // __TRAFFICADVISORYCHECK_H__