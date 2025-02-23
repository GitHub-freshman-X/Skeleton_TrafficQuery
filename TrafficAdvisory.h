#ifndef __TRAFFICADVISORY_H__
#define __TRAFFICADVISORY_H__

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>

#include "GraphAdjacencyList.h"
#include "UtilitiesRandom.h"
#include "UtilitiesString.h"

using std::string;
using std::vector;
using std::ifstream;
using std::cerr;
using std::stringstream;
using std::priority_queue;

// 操作类型1：表示顶点B标签、A标签到B顶点的最小路径距离
struct Result_Label_PathLength{
    string label;
    int pathLength;
    bool operator==(const Result_Label_PathLength &other) const {
        return (label.compare(other.label)==0) && (pathLength==other.pathLength);
    }
    bool operator<(const Result_Label_PathLength &other) const {
        return (label.compare(other.label)<0) || (label.compare(other.label)==0 && pathLength<other.pathLength);
    }
};

// 操作类型2：表示路径、路径距离
struct Result_Path_Distance{
    vector<string> path;
    int dist;
    bool operator<(const Result_Path_Distance &other) const {
        return dist<other.dist;
    }
    bool operator>(const Result_Path_Distance &other) const {
        return dist>other.dist;
    }
};

class TrafficAdvisory{
public:
    // 默认构造：顶点数是3、边数是1的随机图
    TrafficAdvisory(bool whetherOutput=false);
    TrafficAdvisory(int n, int e, bool whetherOutput=false);

    // 根据filename创建一个TrafficAdvisory对象
    TrafficAdvisory(const string &filename, bool whetherOutput=false);

    // 通过三个文件构造对象
    TrafficAdvisory(const string&, const string&, const string&, bool whetherOutput=false);

    // 通过另一个GraphAdjacencyList对象构造对象
    TrafficAdvisory(const GraphAdjacencyList<string, int> &other, bool whetherOutput=false);

    ~TrafficAdvisory() {_graph.clearGraph();}

public:
    TrafficAdvisory& operator=(const TrafficAdvisory &other){
        this->_graph=other._graph;
        this->_whetherOutput=other._whetherOutput;
        return *this;
    }

public:
    set<string> getLabels() {return _graph.getLabels();}

    /**
     * 使用Dijkstra求标签source到destination绕过passBy（默认值是空串）的最短路径，返回距离值。
     * 如果顶点不存在或不能到达，返回INT_MAX。
    */
    int dijkstra(const string&, const string&, const string&);
    vector<Result_Label_PathLength> dijkstraHelper(const string&, const string&, bool specificLength=false);

    /**
     * 返回全国其它各个省会城市到source的最少边数，返回复合每个终点标签与路径长度（边数）的向量。
     * 其中，如果源点和终点是同一个顶点，路径长度是0。如果两顶点不能到达，路径长度是INT_MAX。
    */
    vector<Result_Label_PathLength> numVertices_from(const string& source);

    /**
     * 使用Floyd求标签source到destination绕过passBy（默认值是空串）的最短路径，返回距离值。
     * 如果顶点不存在或不能到达，返回INT_MAX。
    */
    int floyd(const string&, const string&, const string&);

    /**
     * 求标签source到destination所有不重复的可行路径，路径上所有顶点不超过maximum_nodes个（默认值是INT_MAX）， 
     * 利用有效排序对所有可行的路径方案依据总长度进行排序输出到文件（如果第4个参数为空，则输出到屏幕，否则输出到其指定文件），
     * 返回可行的复合路径顶点标签序列与距离的向量（源点和终点是同一个顶点，返回0）
    */
    vector<Result_Path_Distance> allNonRepetitionPaths(const string&, const string&, int, string filename="");
    void dfsHelper(const string&, const string&, Result_Path_Distance &, 
        vector<Result_Path_Distance> &, unordered_map<string, bool> &vis, int cnt, int maxNodes);

    /**
    * 直接求解两个城市间的前第 K 短的路径，返回这条路径的距离。
    * 如果顶点不存在或第K条最短路径不存在，返回INT_MAX。
    */
    int kthShortestPath(const string& source, const string &destination, int k);

private:
    GraphAdjacencyList<string, int> _graph;
    bool _whetherOutput=false;

    // 创建一个顶点数是n、边数是e的图
    void randomGraph(int n, int e);

    /**
     * 读取整个CSV文件并返回向量，每个内部列表表示CSV的一行，其中每个逗号分隔的值作为一个条目
     * 假设CSV文件除了作为分隔符的逗号之外没有其他分隔符
     * 如果文件名无效，则退出
     * 
     * 逗号分隔值(Comma-Separated Values, CSV)有时也成为字符分隔值，因为分割字符也可以不是逗号
     * 文件以纯文本形式存储表格数据
    */
    template<class T>
    vector<vector<T>> readCSV(const string &filename, T(*function)(char*));

    // 从一个文件filename创建图对象g
    void graphFromFile(const string &filename);

	/**
     * 从名为 filename1、filename2、filename3 文件创建一个 图对象：从文件创建图对象g。
	 * filename1：城市名
	 * filename2：城市间是否相邻
	 * filename3：城市间距离
	*/
    void graphFromFile(const string&, const string&, const string&);
};

template<class T>
vector<vector<T>> TrafficAdvisory::readCSV(const string &filename, T(*function)(char*)){
    vector<vector<T>> records;

    ifstream in(filename);
    if(!in.is_open()){
        cerr<<filename<<"文件不能打开！\n";
        exit(EXIT_FAILURE);
    }

    string line;
    while(getline(in, line)){
        stringstream ss(line);
        string cell;
        vector<T> row;
        while(getline(ss, cell, ',')){
            row.push_back(function(cell.c_str()));
        }
        records.push_back(row);
    }

    return records;
}

#endif // __TRAFFICADVISORY_H__