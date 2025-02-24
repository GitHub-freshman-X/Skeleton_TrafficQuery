#include "TrafficAdvisoryCheck.h"

TrafficAdvisoryCheck::TrafficAdvisoryCheck(TrafficAdvisory &other)
    : solver(other)
{}

bool TrafficAdvisoryCheck::checkAll(string source, string destination, bool whetherOutput){
    set<string> vertexSet=solver._graph.getLabels();
    int length=vertexSet.size();
    set<string> V;

    // 这里进行判断，如果src和dest都空，则V为所有顶点，否则V为src和dest
    if(source=="" && destination==""){
        V=vertexSet;
    }else if(source!="" && destination!=""){
        if(solver._graph.isVertex(source) && solver._graph.isVertex(destination)){
            V.insert(source);
            V.insert(destination);
        }else{
            cerr << "Source or destination is not in the graph." << endl;
            exit(EXIT_FAILURE);
        }
    }else{
        cerr<<"Source and destination must be both empty or both not empty."<<endl;
        exit(EXIT_FAILURE);
    }

    for(const string &s : V){
        // 验证源点到其他各顶点边数检查
        vector<Result_Label_PathLength> expect_numVertices;
        for(const string &d : V){
            Result_Label_PathLength tmp;
            tmp.label=d;
            tmp.pathLength=solver.dijkstra(s, d);
            expect_numVertices.push_back(tmp);
        }
        vector<Result_Label_PathLength> actual_numVertices=solver.numVertices_from(s);
        std::sort(expect_numVertices.begin(), expect_numVertices.end());
        std::sort(actual_numVertices.begin(), actual_numVertices.end());
        if(actual_numVertices != expect_numVertices) {return false;}

        // 验证路径操作
        for(const string &d : V){
            vector<Result_Path_Distance> allRoutes;
            if(whetherOutput){
                allRoutes=solver.allNonRepetitionPaths(s, d, length, "./output/" + s + "-" + d + std::to_string(length) + ".txt");
            }else{
                allRoutes=solver.allNonRepetitionPaths(s, d, length);
            }

            int numAllRoutes=allRoutes.size();
            // 最短路径检查
            int dijkstraRoute, floydRoute;
            dijkstraRoute=solver.dijkstra(s, d);
            floydRoute=solver.floyd(s, d);
            if(dijkstraRoute!=floydRoute) {return false;}
            if(allRoutes.size()==0){
                if(dijkstraRoute!=INT_MAX) {
                    cout << s << " - " << d << "最短路径出错:不存在？存在" << endl;
                    return false;
                }
            }else if(dijkstraRoute!=allRoutes[0].dist){
                cout << s << " - " << d << "最短路径出错:" << dijkstraRoute << " " << allRoutes[0].dist << endl;
                return false;
            }

            // 绕过某个点的最短路径
            bool found=false;
            for(const string &p : vertexSet){   // p：要绕过的点
                if(p==s || p==d) {continue;}
                dijkstraRoute=solver.dijkstra(s, d, p);
                floydRoute=solver.floyd(s, d, p);
                if(dijkstraRoute!=floydRoute) {return false;}
                if(allRoutes.size()==0){
                    if(dijkstraRoute!=INT_MAX) {
                        cout << s << " - " << d << "绕过" << p << "最短路径出错:不存在？存在" << endl;
                        return false;
                    }
                    else{
                        for(int i=0; i<numAllRoutes; ++i){
                            auto it=std::find(allRoutes[i].path.begin(), allRoutes[i].path.end(), p);
                            if(it==allRoutes[i].path.end()){
                                if(dijkstraRoute != allRoutes[i].dist){
                                    cout << s << " - " << d << "绕过" << p << "最短路径出错" << endl;
                                    return false;
                                }else {found=true;}
                                break;
                            }
                        }
                        if(found==false && dijkstraRoute!=INT_MAX){
                            cout << s << " - " << d << "绕过" << p << "最短路径出错" << endl;
                            return false;
                        }
                    }
                }
            }

            // 所有路径与第k条路径检查
            for(int k=1; k<=numAllRoutes; ++k){
                int kthRoute=solver.kthShortestPath(s, d, k);
                if(allRoutes.size()==0){
                    if(kthRoute!=INT_MAX) {
                        cout << s << " - " << d << "第" << k << "短路径出错:不存在？存在" << endl;
                        return false;
                    }
                }else if(allRoutes[k-1].dist != kthRoute){
                    cout << s << " - " << d << "第" << k << "短路径出错" << endl;
                    return false;
                }
                if(kthRoute == INT_MAX) {break;}
            }
            if(solver.kthShortestPath(s, d, numAllRoutes+1) != INT_MAX) {return false;}
        }
    }

    return true;
}

bool TrafficAdvisoryCheck::checkFiles(const string &filename1, const string &filename2){
    vector<vector<string>> lines1=solver.readCSV<string>(filename1, stringfunction);
    vector<vector<int>> lines2=solver.readCSV<int>(filename2, intfunction);
    for(int i=0; i<lines1.size(); ++i){
        int sum=0;
        for(int j=0; j<lines1[i].size()-1; ++j){
            if(solver._graph.getWeight(lines1[i][j], lines1[i][j+1]) != INT_MAX){
                sum += solver._graph.getWeight(lines1[i][j], lines1[i][j+1]);
            }else{
                cout<<lines1[i][j]<<"-"<<lines1[i][j+1]<<"不相邻，出错！"<<endl;
                return false;
            }
        }
        int expect=lines2[i][0];
        if(sum!=expect){
            cout<<lines1[i][0]<<"-"<<lines1[i][lines1[i].size()-1]<<"距离不匹配，出错！"<<endl;
            return false;
        }
    }
    return true;
}