#include "TrafficAdvisory.h"
#include <climits>

TrafficAdvisory::TrafficAdvisory(bool whetherOutput){
    _whetherOutput=whetherOutput;
    randomGraph(3, 1);
}

TrafficAdvisory::TrafficAdvisory(int n, int e, bool whetherOutput){
    _whetherOutput=whetherOutput;
    randomGraph(n, e);
}

TrafficAdvisory::TrafficAdvisory(const string &filename, bool whetherOutput){
    _whetherOutput=whetherOutput;
    graphFromFile(filename);
}

TrafficAdvisory::TrafficAdvisory(const string &file1, const string &file2, const string &file3, bool whetherOutput){
    _whetherOutput=whetherOutput;
    graphFromFile(file1, file2, file3);
}

TrafficAdvisory::TrafficAdvisory(const GraphAdjacencyList<string, int> &other, bool whetherOutput){
    _graph=other;
    _whetherOutput=whetherOutput;
}

int TrafficAdvisory::dijkstra(const string &source, const string &destination, const string &passBy){
    vector<Result_Label_PathLength> result=dijkstraHelper(source, passBy, false);
    for(const auto &rlp : result){
        if(rlp.label==destination){ return rlp.pathLength; }
    }
    return INT_MAX;
}

vector<Result_Label_PathLength> TrafficAdvisory::dijkstraHelper(const string &source, const string &passBy, bool specificLength){
    unordered_map<string, bool> vis;
    unordered_map<string, int> dist;
    for(const string &label : _graph.getLabels()){
        vis[label]=false; 
        dist[label]=INT_MAX;
    }
    using pis=std::pair<int, string>;
    priority_queue<pis, vector<pis>, std::greater<pis>> pq;

    dist[source]=0;
    pq.push({0, source});
    while(!pq.empty()){
        pis top=pq.top(); pq.pop();
        string cur=top.second;
        int d=top.first;
        if(vis[cur]) {continue;}
        vis[cur]=true;
        for(const string &nxt : _graph.getadjLabels(cur)){
            int weight = specificLength ? 1 : _graph.getWeight(cur, nxt);
            if(!passBy.empty() && nxt==passBy) {continue;}
            if(dist[nxt]>dist[cur]+weight){
                dist[nxt]=dist[cur]+weight;
                pq.push({dist[nxt], nxt});
            }
        }
    }

    vector<Result_Label_PathLength> result;
    for(const auto &p : dist){
        result.push_back({p.first, p.second});
    }
    return result;
}

vector<Result_Label_PathLength> TrafficAdvisory::numVertices_from(const string &source){
    // 用dijkstra算法，把图中所有边的长度都当作1，然后求出source到各个点的最短路径长度
    return dijkstraHelper(source, "", true);
}

int TrafficAdvisory::floyd(const string &source, const string &destination, const string &passBy){
    unordered_map<string, unordered_map<string, int>> dist;
    set<string> labels=_graph.getLabels();

    // 初始化
    for(const string &from : labels){
        for(const string &to : labels){
            if(from==to){
                dist[from][to]=0;
            }else if(_graph.isVertex(from) && _graph.isVertex(to) && _graph.getWeight(from, to)!=INT_MAX){
                dist[from][to]=_graph.getWeight(from, to);
            }else {
                dist[from][to]=INT_MAX;
            }
        }
    }

    for(const string &k : labels){
        if(!passBy.empty() && k==passBy) {continue;}
        for(const string &i : labels){
            for(const string &j : labels){
                if(dist[i][k]!=INT_MAX && dist[k][j]!=INT_MAX && dist[i][j]>dist[i][k]+dist[k][j]){
                    dist[i][j]=dist[i][k]+dist[k][j];
                }
            }
        }
    }

    return dist[source][destination];
}

vector<Result_Path_Distance> TrafficAdvisory::allNonRepetitionPaths(const string &source, const string &destination, int maxNodes, string filename){
    maxNodes=10;
    Result_Path_Distance rpd;
    rpd.path.push_back(source);
    rpd.dist=0;
    vector<Result_Path_Distance> allPaths;
    unordered_map<string, bool> vis;
    for(const string &label : _graph.getLabels()) {vis[label]=false;}

    vis[source]=true;
    dfsHelper(source, destination, rpd, allPaths, vis, 1, maxNodes);
    std::sort(allPaths.begin(), allPaths.end(), [&](const Result_Path_Distance &a, const Result_Path_Distance &b){
        return a.dist<b.dist;
    });

    if(!filename.empty()){
        std::ofstream file(filename);
        if(!file.is_open()){
            cerr << filename << "文件打开失败！\n";
            exit(EXIT_FAILURE);
        }
        for(const auto &rpd : allPaths){
            file << "路径：";
            for (const string& s : rpd.path) { file << s << " "; }
            file << "\n" << "距离：" << rpd.dist << "\n";
        }
        file.close();
    }

    return allPaths;
}

void TrafficAdvisory::dfsHelper(
    const string &cur, 
    const string &destination,
    Result_Path_Distance &rpd,
    vector<Result_Path_Distance> &allPaths,
    unordered_map<string, bool> &vis, 
    int cnt, 
    int maxNodes){
        if(cnt>maxNodes) {return;}
        if(cur==destination){
            rpd.path.push_back(cur);
            allPaths.push_back(rpd);
            rpd.path.pop_back();
            return;
        }

        for(const string &to : _graph.getadjLabels(cur)){
            if(vis.find(to)!=vis.end() && vis[to]) {continue;}
            int weight=_graph.getWeight(cur, to);
            rpd.path.push_back(to);
            rpd.dist += weight;
            vis[to]=true;
            dfsHelper(to, destination, rpd, allPaths, vis, cnt+1, maxNodes);
            rpd.path.pop_back();
            rpd.dist -= weight;
            vis[to]=false;
        }
}

int TrafficAdvisory::kthShortestPath(const string &source, const string &destination, int k){
    vector<Result_Path_Distance> allPaths=allNonRepetitionPaths(source, destination, 5);
    using rpd=Result_Path_Distance;
    priority_queue<rpd, vector<rpd>, std::greater<rpd>> pq;
    for(const rpd &r : allPaths) {pq.push(r);}
    for(int i=0; i<std::min(k, (int)allPaths.size()); ++i){
        if(i==k-1) {return pq.top().dist;}
        pq.pop();
    }
    return INT_MAX;
}

void TrafficAdvisory::randomGraph(int n, int e){
    using T = string;
    using E = int;

    // 产生随机的字符串数组：图的顶点标签
    T *arr = makeUniqueRandomArray<T>(n, randomString);

    // 矩阵Matrix记录边，以防止生成重复边
    vector<vector<E>> Matrix;
    Matrix.resize(n, vector<int>(n,0));

    for(int i=0; i<n; ++i){
        _graph.insertVertex(arr[i]);
    }

    for(int i=0; i<e; ++i){
        int source = dist(gen)%n;
        int destination = dist(gen)%n;
        E weight = dist(gen)%100;

        // 防止生成自环或重复边
        while(source==destination || Matrix[source][destination]!=0){
            source = dist(gen)%n;
            destination = dist(gen)%n;
        }
        Matrix[source][destination]=1;
        Matrix[destination][source]=1;

        // 将边的权值设置为随机整数
        _graph.insertEdge(arr[source], arr[destination], weight);
        _graph.insertEdge(arr[destination], arr[source], weight);
    }

    delete[] arr;
}

void TrafficAdvisory::graphFromFile(const string &filename){
    vector<vector<string>> lines=readCSV(filename, stringfunction);
    vector<vector<string>>::iterator it;

    it=lines.begin();
    for(string name : *it){
        _graph.insertVertex(name);
    }

    for(it++; it!=lines.end(); ++it){
        vector<string>::iterator it2;
        it2 = (*it).begin();
        string source = *it2;
        string destination = *(++it2);
        _graph.insertEdge(source, destination, 1);  // 文件中无权值信息，默认为1
        _graph.insertEdge(destination, source, 1);
    }
}

void TrafficAdvisory::graphFromFile(const string &file1, const string &file2, const string &file3){
    vector<vector<string>> lines1=readCSV<string>(file1, stringfunction);
    vector<vector<int>> lines2=readCSV<int>(file2, intfunction);
    vector<vector<bool>> lines3=readCSV<bool>(file3, boolfunction);

    vector<vector<string>>::iterator it1=lines1.begin();
    for(const string &name : *it1){
        _graph.insertVertex(name);
    }
    for(int i=0; i<lines3.size(); ++i){
        for(int j=0; j<lines3[i].size(); ++j){
            if(lines3[i][j]){
                string source=lines1[0][i];
                string destination=lines1[0][j];
                if(i<j){
                    _graph.insertEdge(source, destination, lines2[j][i]);
                    _graph.insertEdge(destination, source, lines2[j][i]);
                }
            }
        }
    }
}
