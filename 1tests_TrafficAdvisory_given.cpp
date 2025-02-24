#include <algorithm>
#include <string>
#include <vector>

#include "GraphAdjacencyList.h"
#include "catch.hpp"
#include "catchmain.cpp"
#include "TrafficAdvisory.h"
#include "TrafficAdvisoryCheck.h"

using std::string;
using std::sort;
using std::vector;

TEST_CASE("TrafficAdvisory::1 triangle", "[part = 0][weight = 3]"){
    GraphAdjacencyList<string, int> g;
    g.insertEdge("A", "B", 1);
    g.insertEdge("A", "C", 1);
    g.insertEdge("A", "D", 1);
    g.insertEdge("C", "D", 1);
    
    TrafficAdvisory solver(g, true);
    SECTION("numVertices_from"){
        vector<Result_Label_PathLength> expect_A={
            {"A", 0},
            {"B", 1},
            {"C", 1},
            {"D", 1}
        };
        sort(expect_A.begin(), expect_A.end());
        vector<Result_Label_PathLength> actual_A=solver.numVertices_from("A");
        sort(actual_A.begin(), actual_A.end());
        REQUIRE(actual_A==expect_A);
    }

    SECTION("Dijkstra ans Floyd"){
        REQUIRE(1 == solver.dijkstra("A", "B"));
        REQUIRE(1 == solver.floyd("A", "B"));
    }

    SECTION("allNonRepetitionPaths"){
        REQUIRE(1 == (solver.allNonRepetitionPaths("A", "B")).size());
    }

    SECTION("kthShortestPath"){
        REQUIRE(1 == solver.kthShortestPath("A", "B", 1));
        REQUIRE(INT_MAX == solver.kthShortestPath("A", "B", 2));
    }

    TrafficAdvisoryCheck c(solver);
    REQUIRE(true == c.checkAll());
    g.clearGraph();
}

TEST_CASE("TrafficAdvisory::2 disinsertEdge 1", "[part=0][weight=6]"){
    GraphAdjacencyList<string, int> g;
    g.insertEdge("A", "B", 1);
    g.insertEdge("C", "D", 1);
    TrafficAdvisory solver(g, true);
    
    SECTION("numVertices_from"){
        vector<Result_Label_PathLength> expect_A={
            { "A",0 },
            { "B",1 },
            { "C",INT_MAX },
            { "D",INT_MAX } };
        std::sort(expect_A.begin(), expect_A.end());
        vector<Result_Label_PathLength>actual_A = solver.numVertices_from("A");
        std::sort(actual_A.begin(), actual_A.end());
        REQUIRE(expect_A == actual_A);
    }

    SECTION("Dijkstra and Floyd"){
        REQUIRE(1 == solver.dijkstra("A", "B"));
        REQUIRE(1 == solver.floyd("A", "B")); 
        REQUIRE(INT_MAX == solver.dijkstra("A", "C"));
        REQUIRE(INT_MAX == solver.floyd("A", "C"));
        REQUIRE(INT_MAX == solver.dijkstra("A", "D"));
        REQUIRE(INT_MAX == solver.floyd("A", "D"));
    }

    SECTION("allNonRepetitionPaths"){
        REQUIRE(1 == (solver.allNonRepetitionPaths("A", "B")).size());//只有1条
        REQUIRE(0 == (solver.allNonRepetitionPaths("A", "C")).size());
        REQUIRE(0 == (solver.allNonRepetitionPaths("A", "D")).size());
    }

    SECTION("kthShortestPath"){
        REQUIRE(1 == solver.kthShortestPath("A", "B", 1));
        REQUIRE(INT_MAX == solver.kthShortestPath("A", "B", 2));
        REQUIRE(INT_MAX == solver.kthShortestPath("A", "C", 1));
        REQUIRE(INT_MAX == solver.kthShortestPath("A", "D", 1));
    }

    TrafficAdvisoryCheck c(solver);
    REQUIRE(true == c.checkAll());
    g.clearGraph();
}

TEST_CASE("TrafficAdvisory::3 disinsertEdge 2", "[part=0][weight=1]"){
    GraphAdjacencyList<string, int> g;
    g.insertEdge("A", "B", 1);
    g.insertEdge("C", "D", 1);
    g.insertEdge("E", "D", 1);
    g.insertEdge("E", "C", 1);

    TrafficAdvisory solver(g, true);
    SECTION("allNonRepetitionPaths"){
        REQUIRE(0 == (solver.allNonRepetitionPaths("A", "D")).size());
    }
    TrafficAdvisoryCheck c(solver);
    REQUIRE(true == c.checkAll());
    g.clearGraph();
}

TEST_CASE("TrafficAdvisory::4 Dijkstra and Floyd three parameters", "[part=0][weight=1]"){
    GraphAdjacencyList<string, int> g;
    g.insertEdge("A", "B", 1);
    g.insertEdge("A", "C", 3);
    g.insertEdge("B", "C", 1);
    TrafficAdvisory solver(g, true);
    REQUIRE(3 == solver.dijkstra("A", "C", "B"));
    REQUIRE(3 == solver.floyd("A", "C", "B"));
    g.clearGraph();
}

TEST_CASE("TrafficAdvisory::5 input 1", "[part=0][weight=5]"){
    TrafficAdvisory solver("./input/party1.txt", true);
    SECTION("allNonRepetitionPaths"){
        REQUIRE(2 == (solver.allNonRepetitionPaths("Michelle", "Brian")).size());
    }
    TrafficAdvisoryCheck c(solver);
    REQUIRE(true == c.checkAll());
}

TEST_CASE("TrafficAdvisory::6 input 2", "[part=0][weight=1]"){
    TrafficAdvisory solver(".\\input\\party2.txt", true);
    SECTION("allNonRepetitionPaths") {
        REQUIRE(1 == (solver.allNonRepetitionPaths("Louis Tomlinson", "Niall Horan")).size());
    }
    TrafficAdvisoryCheck c(solver);
    REQUIRE(true == c.checkAll());
}

TEST_CASE("TrafficAdvisory::7 input 2::full connected", "[part=0][weight=4]"){
    TrafficAdvisory solver("./input/party5.txt", true);
    SECTION("allNonRepetitionPaths"){
        REQUIRE(5 == (solver.allNonRepetitionPaths("Michelle", "Brian")).size());
    }

    SECTION("Dijkstra and Floyd"){
        REQUIRE(1 == solver.dijkstra("Michelle", "Brian"));
        REQUIRE(1 == solver.floyd("Michelle", "Brian"));
    }

    SECTION("kthShortestPath"){
        REQUIRE(2 == solver.kthShortestPath("Michelle", "Brian", 2));
        REQUIRE(1 == solver.kthShortestPath("Michelle", "Brian", 1));
        REQUIRE(2 == solver.kthShortestPath("Michelle", "Brian", 3));
        REQUIRE(3 == solver.kthShortestPath("Michelle", "Brian", 4));
        REQUIRE(3 == solver.kthShortestPath("Michelle", "Brian", 5));
        REQUIRE(INT_MAX == solver.kthShortestPath("Michelle", "Brian", 6));
    }

    TrafficAdvisoryCheck c(solver);
    REQUIRE(true == c.checkAll());
}

TEST_CASE("TrafficAdvisory::8 input 3", "[part=0][weight=1]"){
    TrafficAdvisory solver("./input/party3.txt", true);
    SECTION("allNonRepetitionPaths"){
        REQUIRE(5 == (solver.allNonRepetitionPaths("Eggplant", "Fig")).size());
    }
    TrafficAdvisoryCheck c(solver);
    REQUIRE(true == c.checkAll());
}

TEST_CASE("TrafficAdvisory::9 input 4", "[part=0][weight=1]"){
    TrafficAdvisory solver("./input/party4.txt", true);
    SECTION("allNonRepetitionPaths"){
        REQUIRE(0 == (solver.allNonRepetitionPaths("Louis Tomlinson", "Fig")).size());
    }
    TrafficAdvisoryCheck c(solver);
    REQUIRE(true == c.checkAll());
}

TEST_CASE("TrafficAdvisory::10 numVertices_from", "[part=0][weight=1]"){
    TrafficAdvisory solver("./input/3.csv", "./input/2.csv", "./input/1.csv", true);
    vector<Result_Label_PathLength> expect_Wuhan={
        {"北京",3},
        {"上海",3},
        {"天津",3},
        {"重庆",1},
        {"哈尔滨",3},
        {"长春",3},
        {"沈阳",3},
        {"呼和浩特",2},
        {"石家庄",2},
        {"太原",2},
        {"济南",2},
        {"郑州",1},
        {"西安",1},
        {"兰州",2},
        {"银川",2},
        {"西宁",3},
        {"乌鲁木齐",3},
        {"合肥",1},
        {"南京",2},
        {"杭州",2},
        {"长沙",1},
        {"南昌",1},
        {"武汉",0},
        {"成都",2},
        {"贵阳",2},
        {"福州",2},
        {"台北",3},
        {"广州",2},
        {"海口",3},
        {"南宁",2},
        {"昆明",3},
        {"拉萨",3},
        {"香港",3},
        {"澳门",3} };
    vector<Result_Label_PathLength> actual_Wuhan=solver.numVertices_from("武汉");
    sort(expect_Wuhan.begin(), expect_Wuhan.end());
    sort(actual_Wuhan.begin(), actual_Wuhan.end());
    REQUIRE(expect_Wuhan==actual_Wuhan);
}

TEST_CASE("TrafficAdvisory::11 allNonRepetitionPaths 2", "[part=0][weight=2]"){
    TrafficAdvisory solver("./input/3.csv", "./input/2.csv", "./input/1.csv", true);
    REQUIRE(1963 == (solver.allNonRepetitionPaths("北京", "武汉", 10, ".\\output\\Beijing-Wuhan10.txt")).size());
    REQUIRE(1963 == (solver.allNonRepetitionPaths("武汉", "北京", 10, ".\\output\\Wuhan-Beijing10.txt")).size());

    REQUIRE(2681 == (solver.allNonRepetitionPaths("重庆", "长沙", 10, ".\\output\\Chongqing-Changsha10.txt")).size());//806
    REQUIRE(2681 == (solver.allNonRepetitionPaths("长沙", "重庆", 10, ".\\output\\Changsha-Chongqing10.txt")).size());//806
}

TEST_CASE("TrafficAdvisory::12 Dijkstra and Floyd 2", "[part=0][weight=12]"){
    TrafficAdvisory solver("./input/3.csv", "./input/2.csv", "./input/1.csv", true);
    REQUIRE(1105 == solver.dijkstra("北京", "武汉"));
    REQUIRE(1105 == solver.floyd("武汉", "北京"));
    REQUIRE(1347 == solver.dijkstra("北京", "上海"));
    REQUIRE(1347 == solver.floyd("上海", "北京"));
    REQUIRE(956 == solver.dijkstra("北京", "西安"));
    REQUIRE(956 == solver.floyd("西安", "北京"));
    REQUIRE(904 == solver.dijkstra("上海", "长沙"));
    REQUIRE(904 == solver.floyd("上海", "长沙"));
    REQUIRE(904 == solver.dijkstra("长沙", "上海"));
    REQUIRE(904 == solver.floyd("长沙", "上海"));
    REQUIRE(1025 == solver.dijkstra("上海", "长沙", "南昌"));
    REQUIRE(1025 == solver.floyd("长沙", "上海", "南昌"));
    REQUIRE(670 == solver.dijkstra("长沙", "香港"));
    REQUIRE(670 == solver.floyd("香港", "长沙"));
    REQUIRE(1199 == solver.dijkstra("南昌", "成都"));//这个案例很好，源代码到达终点位置放错！
    REQUIRE(1199 == solver.floyd("成都", "南昌"));
    REQUIRE(971 == solver.dijkstra("武汉", "澳门"));
    REQUIRE(971 == solver.floyd("澳门", "武汉"));
    REQUIRE(1028 == solver.dijkstra("武汉", "成都"));
    REQUIRE(1028 == solver.floyd("成都", "武汉"));
    REQUIRE(2121 == solver.dijkstra("天津", "广州", "武汉"));
    REQUIRE(2121 == solver.floyd("广州", "天津", "武汉"));
    REQUIRE(1388 == solver.dijkstra("武汉", "北京", "郑州"));
    REQUIRE(1388 == solver.floyd("北京", "武汉", "郑州"));
    REQUIRE(INT_MAX == solver.dijkstra("武汉", "北京", "石家庄"));
    REQUIRE(INT_MAX == solver.floyd("北京", "武汉", "石家庄"));
    REQUIRE(1258 == solver.dijkstra("武汉", "成都", "重庆"));
    REQUIRE(1258 == solver.floyd("成都", "武汉", "重庆"));

    solver.allNonRepetitionPaths("北京", "广州", 10, "./output/Beijing-Guangzhou10.txt");
    REQUIRE(2124 == solver.dijkstra("北京", "广州", "武汉"));
    REQUIRE(2124 == solver.floyd("广州", "北京", "武汉")); 

    REQUIRE(2201 == solver.dijkstra("昆明", "北京", "武汉"));
    REQUIRE(2201 == solver.floyd("北京", "昆明", "武汉"));
}

TEST_CASE("TrafficAdvisory::13 kthShortestPath", "[part=0][weight=6]"){
    TrafficAdvisory solver("./input/3.csv", "./input/2.csv", "./input/1.csv", true);
    REQUIRE(1105 == solver.kthShortestPath("武汉", "北京", 1));
    REQUIRE(1105 == solver.kthShortestPath("北京", "武汉", 1));
    REQUIRE(1263 == solver.kthShortestPath("武汉", "北京", 3));
    REQUIRE(1263 == solver.kthShortestPath("北京", "武汉", 3));
    REQUIRE(1517 == solver.kthShortestPath("武汉", "北京", 10));
    REQUIRE(1517 == solver.kthShortestPath("北京", "武汉", 10));
    REQUIRE(2102 == solver.kthShortestPath("武汉", "北京", 50));
    REQUIRE(2102 == solver.kthShortestPath("北京", "武汉", 50));
    // REQUIRE(3399 == solver.kthShortestPath("武汉", "北京", 700));
    // REQUIRE(3399 == solver.kthShortestPath("北京", "武汉", 700));
    REQUIRE(3250 == solver.kthShortestPath("哈尔滨", "长沙", 67));//3265
    REQUIRE(3250 == solver.kthShortestPath("长沙", "哈尔滨", 67));//3265
    REQUIRE(1028 == solver.kthShortestPath("成都", "武汉", 1));
    REQUIRE(1028 == solver.kthShortestPath("武汉", "成都", 1));
    REQUIRE(1712 == solver.kthShortestPath("成都", "武汉", 10));
    REQUIRE(1712 == solver.kthShortestPath("武汉", "成都", 10));
}
