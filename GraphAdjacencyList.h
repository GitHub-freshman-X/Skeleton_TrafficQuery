#ifndef __GRAPHADJACENCYLIST_H__
#define __GRAPHADJACENCYLIST_H__

#include <set>
#include <unordered_map>
#include "Graph.h"

using std::set;
using std::unordered_map;

template<class V, class W>
class GraphAdjacencyList : public Graph<V, W>{
public:
    GraphAdjacencyList();

    ~GraphAdjacencyList();

    set<V> getLabels() const;

    set<V> getadjLabels(const V& v) const;

    W getWeight(const V& v1, const V& v2) const;

    bool isVertex(const V& vertex) const;

    bool insertVertex(const V& vertex);

    bool insertEdge(const V& v1, const V& v2, const W& weight);

    int size() const { return _adjList.size(); }

    void clearGraph();

private:
    unordered_map<V, unordered_map<V, W>> _adjList;
};

// 构造、析构
template<class V, class W>
GraphAdjacencyList<V, W>::GraphAdjacencyList(){
    _adjList = unordered_map<V, unordered_map<V, W>>();
}
template<class V, class W>
GraphAdjacencyList<V, W>::~GraphAdjacencyList(){
    clearGraph();
}

// 返回图中顶点标签集合
template<class V, class W>
set<V> GraphAdjacencyList<V, W>::getLabels() const {
    set<V> labels;
    for(const auto& p : _adjList){
        labels.insert(p.first);
    }
    return labels;
}

// 返回与顶点v邻接的顶点标签集合
template<class V, class W>
set<V> GraphAdjacencyList<V, W>::getadjLabels(const V& v) const {
    set<V> adjLabels;
    auto it = _adjList.find(v);
    if(it != _adjList.end()){
        for(const auto& p : it->second){
            adjLabels.insert(p.first);
        }
    }
    return adjLabels;
}

// 返回标签为v1和v2的边的权值。不存在则返回MaxWeight
template<class V, class W>
W GraphAdjacencyList<V, W>::getWeight(const V& v1, const V& v2) const {
    auto it1 = _adjList.find(v1);
    if(it1 != _adjList.end()){
        auto it2 = it1->second.find(v2);
        if(it2 != it1->second.end()){
            return it2->second;
        }
    }
    return MaxWeight;
}

// 判断顶点vertex是否是图中的顶点
template<class V, class W>
bool GraphAdjacencyList<V, W>::isVertex(const V& vertex) const {
    return (_adjList.find(vertex) != _adjList.end());
}

// 插入标签为vertex的顶点
template<class V, class W>
bool GraphAdjacencyList<V, W>::insertVertex(const V& vertex){
    if(_adjList.find(vertex) == _adjList.end()){
        _adjList[vertex] = unordered_map<V, W>();
        return true;
    }
    return false;
}

// 插入边(v1, v2)，权值为weight
template<class V, class W>
bool GraphAdjacencyList<V, W>::insertEdge(const V& v1, const V& v2, const W& weight){
    // 首先检查v1和v2是否在图中，如果不在，先插入这两个点
    if(_adjList.find(v1) == _adjList.end()){
        insertVertex(v1);
    }
    if(_adjList.find(v2) == _adjList.end()){
        insertVertex(v2);
    }
    _adjList[v1][v2] = weight;
    return true;
}

// 清空图
template<class V, class W>
void GraphAdjacencyList<V, W>::clearGraph(){
    for(auto& p : _adjList){
        p.second.clear();
    }
    _adjList.clear();
}

#endif // __GRAPHADJACENCYLIST_H__