#ifndef __GRAPH_H_
#define __GRAPH_H_

#include <set>
#include <limits>
using std::set;

const int MaxWeight = INT_MAX;  // 最大权值，用来表示边不存在

template<class V, class W>  // V: 顶点标签类型，W: 权值类型
class Graph{
protected:

public:
    Graph() {}

    /*
    * 结论：当基类可能被继承且会通过基类指针删除子类对象时，应使用虚析构
    * 1、保证子类对象的析构函数被正确调用
    * 2、支持多态销毁，当基类指针指向子类对象时，确保子类的析构函数被正确调用
    * 
    * 纯虚析构使用场景：
    * 1、纯虚函数要求子类必须实现；当子类中有动态申请的资源时，子类就必须自己实现析构函数，
    * 将父类析构写成纯虚析构，可以强制要求子类实现析构
    * 2、将自己这个类变成一个纯虚类，确保不能实例化对象
    */
    virtual ~Graph() {}

    // 得到图中所有顶点标签集合
    virtual set<V> getLabels() const = 0;

    // 返回边(v1, v2)的权值，如果边不存，返回MaxWeight
    virtual W getWeight(const V& v1, const V& v2) const = 0;

    // 判断图中有没有标签为vertex的顶点
    virtual bool isVertex(const V& vertex) const = 0;

    // 插入一个标签为vertex的顶点
    virtual bool insertVertex(const V& vertex) = 0;

    // 插入边(v1, v2)，权值为weight
    virtual bool insertEdge(const V& v1, const V& v2, const W& weight) = 0;

    // 置图为空图
    virtual void clearGraph() = 0;
};

#endif // __GRAPH_H_