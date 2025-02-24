#ifndef __UTILITIESSPEEDTEST_H__
#define __UTILITIESSPEEDTEST_H__

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <set>
#include <iomanip>

#include "eigen-3.4.0/Eigen/Dense"  //提供拟合功能的开源矩阵运算库
#include "eigen-3.4.0/Eigen/QR"
#include "UtilitiesTime.h"

#define maxN 8
#define maxE 30
#define numTrials 50
#define incN 2
#define incE 1

using std::cout;
using std::endl;
using std::vector;
using std::stringstream;
using std::string;
using std::set;
using std::setw;

//参考O(n+e)程序类
class O_n_add_e{
public:
    O_n_add_e() {}
    void run(int n, int e){
        for (int i = 0; i < n; i++) {
            int x = 1; //循环......
        }
        for (int i = 0; i < 2 * e; i++) {
            int x = 1; //循环......
        }
    }
};

//参考O(n*n)程序类
class O_n_square {
public:
    O_n_square() {};
    void run(int n) {//参考程序
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                int x = 1; //循环......
        }
    }
};

//参考O(n*n*n)程序类
class O_n_cube {
public:
    O_n_cube() {};
    void run(int n) {//参考程序
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                for (int k = 0; k < n; k++) {
                    int x = 1;
                }					 //循环......
        }
    }
};

//参考O(elogn)程序类
class O_elogn {
public:
    O_elogn() {};
    void run(int n, int e) {//参考程序
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < 2 * e; j++) {
                int x = 1; //循环......
            }
            i = i << 1;
        }
    }
};
    
//参考O(n*e)程序类
class O_n_multiply_e {
public:
    O_n_multiply_e() {};
    void run(int n, int e) {//参考程序
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < 2 * e; j++)
                int x = 1; //循环......
        }
    }
};

// 使用Eigen库进行二元三次多项式回归:使用拟合函数f(x1,x2)=c0+c1*x1+c2*x2+c3*x1^2+c4*x1*x2+c5*x2^2+c6*x1^3+c7*x1^2*x2+c8*x1*x2^2+c9*x2^3。
Eigen::VectorXd multivariateCubicPolynomialRegression(const vector<double>& x1, const vector<double>& x2, const vector<double>& y) {
    int dataSize = x1.size();
    Eigen::MatrixXd A(dataSize, 10);  // Ten coefficients for a cubic polynomial
    Eigen::VectorXd b(y.size());

    for (size_t k = 0; k < dataSize; ++k) {
        A(k, 0) = 1.0;
        A(k, 1) = x1[k];
        A(k, 2) = x2[k];
        A(k, 3) = std::pow(x1[k], 2);
        A(k, 4) = x1[k] * x2[k];
        A(k, 5) = std::pow(x2[k], 2);
        A(k, 6) = std::pow(x1[k], 3);
        A(k, 7) = std::pow(x1[k], 2) * x2[k];
        A(k, 8) = x1[k] * std::pow(x2[k], 2);
        A(k, 9) = std::pow(x2[k], 3);

        b(k) = y[k];
    }

    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);// 使用JacobiSVD进行最小二乘法拟合
    Eigen::VectorXd result = svd.solve(b);
    return result;
}

//使用Eigen库计算拟合程度指标SSE、R2
vector<double> calculateSSE_RSquared(const Eigen::VectorXd& coefficients, const std::vector<double>& x1, const std::vector<double>& x2, const std::vector<double>& y) {
    vector<double> returnVec;
    int dataSize = x1.size();
    Eigen::VectorXd predicted(dataSize);
    Eigen::VectorXd yVector = Eigen::Map<const Eigen::VectorXd>(y.data(), y.size());
    double meanY = yVector.mean();

    for (size_t k = 0; k < dataSize; ++k) {
        predicted(k) = coefficients[0] +
            coefficients[1] * x1[k] +
            coefficients[2] * x2[k] +
            coefficients[3] * std::pow(x1[k], 2) +
            coefficients[4] * x1[k] * x2[k] +
            coefficients[5] * std::pow(x2[k], 2) +
            coefficients[6] * std::pow(x1[k], 3) +
            coefficients[7] * std::pow(x1[k], 2) * x2[k] +
            coefficients[8] * x1[k] * std::pow(x2[k], 2) +
            coefficients[9] * std::pow(x2[k], 3);
    }

    Eigen::VectorXd residuals = yVector - predicted;
    double SSE = residuals.squaredNorm();
    double SST = (yVector.array() - meanY).square().sum();  // Corrected line:Now, the SST calculation uses the .array() method to convert yVector to an array, and then the .square().sum() methods for element-wise squaring and summing, which should resolve the error you encountered.
    double RSquared = 1.0 - (SSE / SST);
    returnVec.push_back(SSE);
    returnVec.push_back(RSquared);
    return returnVec;
}

//计算运行时间的平均梯度模，反映算法的时间效率
double calculateAverageGradientMagnitude_10(const Eigen::VectorXd& coefficients, const std::vector<double>& x1, const std::vector<double>& x2) {
    int dataSize = x1.size();
    double sumGradientMagnitude = 0.0;

    for (size_t k = 0; k < dataSize; ++k) {
        // Gradient vector for the cubic polynomial
        Eigen::VectorXd gradient(2);
        gradient << coefficients[1] + 2 * coefficients[3] * x1[k] + coefficients[4] * x2[k] + 3 * coefficients[6] * std::pow(x1[k], 2) + 2 * coefficients[7] * x1[k] * x2[k] + coefficients[8] * std::pow(x2[k], 2),
            coefficients[2] + coefficients[4] * x1[k] + 2 * coefficients[5] * x2[k] + 3 * coefficients[9] * std::pow(x2[k], 2) + 2 * coefficients[8] * x1[k] * x2[k] + coefficients[7] * std::pow(x1[k], 2);

        // Calculate the gradient magnitude
        sumGradientMagnitude += gradient.norm();
    }

    return sumGradientMagnitude / dataSize;
}

template<typename T, typename E, typename TestClass>
class SpeedTest{
public:
    SpeedTest() {}

    //计算随n、e变化的十个算法的执行时间曲线的斜率
    vector<stringstream> run(){
        vector<stringstream> ss_Speed;
        vector<Timer> timer;    // 计时器
        timer.resize(10, Timer());

        vector<double> vertexSequence;  // 顶点数序列
        vector<double> edgeSequence;    // 边数序列
        vector<vector<double>> runtimeSequence;  // 运行时间序列

        for(int i=0; i<10; ++i){
            runtimeSequence.push_back(vector<double>());
        }

        vector<string> algorithmtype;
        algorithmtype.push_back("O(n+e)");
        algorithmtype.push_back("O(n*n)");
        algorithmtype.push_back("O(elogn)");
        algorithmtype.push_back("O(n*e)");
        algorithmtype.push_back("O(n*n*n)");
        algorithmtype.push_back("numVertices_from");
        algorithmtype.push_back("dijkstra");
        algorithmtype.push_back("floyd");
        algorithmtype.push_back("kthShortestPath");
        algorithmtype.push_back("allNonRepetitionPaths");

        for(int n=incN; n<=maxN; n = n+incN){
            for(int e=incE; e<=n*(n-1)/2 && e<=maxE; e = e+incE){
                vertexSequence.push_back(n);
                edgeSequence.push_back(e);

                //五个实验对象
                vector<O_n_add_e> references0;//参考程序对象1
                vector<O_n_square> references1;//参考程序对象2
                vector<O_elogn> references2;//参考程序对象3
                vector<O_n_multiply_e> references3;//参考程序对象4
                vector<O_n_cube> references4;//参考程序对象5
                vector<TestClass> tests;// 被重点关注的程序对象
                std::vector<std::vector<string>> testsLabels;//顶点标签

                //分别构建numTrials个五类参考程序对象、顶点数为n，边数为e的类型是TestClass的对象
                for(int num_tests=0; num_tests<numTrials; ++num_tests){
                    O_n_add_e rObject0;
                    references0.push_back(rObject0);
                    O_n_square rObject1;
                    references1.push_back(rObject1);
                    O_elogn rObject2;
                    references2.push_back(rObject2);
                    O_n_multiply_e rObject3;
                    references3.push_back(rObject3);
                    O_n_cube rObject4;
                    references4.push_back(rObject4);

                    //随机生成一个n个顶点，e条边的TestClass类型对象，第三个参数控制是否输出
                    TestClass tObject(n, e, false);
                    set<T> labelSet=tObject.getLabels();
                    vector<T> labelVector;
                    if(labelSet.size()==0){
                        cout << "===============================================================================\n";
                        cout << "The execution time performance statistics and regression analysis of the TrafficAdvisory class functions cannot be performed. \n";
                        cout << "Because you have not completed operations such as getLabels and insertVertex for the graph. You need to implement these operations first!\n";
                        cout << "===============================================================================\n";
                        return ss_Speed;
                    }else{
                        labelVector=vector<T>(labelSet.begin(), labelSet.end());
                    }
                    testsLabels.push_back(labelVector);
                    tests.push_back(tObject);
                }

                //记录十个程序运行numTrials次的运行时间

                // 运行并计时参考程序0
                timer[0].start();
                for(int num_tests=0; num_tests<numTrials; ++num_tests){
                    references0[num_tests].run(n, e);   // O_n_add_e
                }
                timer[0].stop();

                // 运行并计时参考程序1
                timer[1].start();
                for(int num_tests=0; num_tests<numTrials; ++num_tests){
                    references1[num_tests].run(n);   // O_n_square
                }
                timer[1].stop();

                // 运行并计时参考程序2
                timer[2].start();
                for (int num_tests = 0; num_tests < numTrials; num_tests++){
                    references2[num_tests].run(n, e);   // O_elogn
                }
                timer[2].stop();

                // 运行并计时参考程序3
                timer[3].start();
                for (int num_tests = 0; num_tests < numTrials; num_tests++){
                    references3[num_tests].run(n, e);   // O_n_multiply_e
                }
                timer[3].stop();

                // 运行并计时参考程序4
                timer[4].start();
                for (int num_tests = 0; num_tests < numTrials; num_tests++){
                    references4[num_tests].run(n);  // O_n_cube
                }
                timer[4].stop();

                // 运行并计时实验程序2的算法1
                timer[5].start();
                for (int num_tests = 0; num_tests < numTrials; num_tests++) {//实验numTrials次
                    int source = std::rand() % n;
                    tests[num_tests].numVertices_from(testsLabels[num_tests][source]);//到source的最少边数
                }
                timer[5].stop();

                // 运行并计时实验程序2的算法2
                timer[6].start();
                for (int num_tests = 0; num_tests < numTrials; num_tests++) { //实验numTrials次
                    int source = std::rand() % n;
                    int destination = std::rand() % n;
                    tests[num_tests].dijkstra(testsLabels[num_tests][source], testsLabels[num_tests][destination]);//实验在顶点数n边数为e的图里计算最短路径
                }
                timer[6].stop();

                // 运行并计时实验程序2的算法3
                timer[7].start();
                for (int num_tests = 0; num_tests < numTrials; num_tests++) { //实验numTrials次
                    int source = std::rand() % n;
                    int destination = std::rand() % n;
                    tests[num_tests].floyd(testsLabels[num_tests][source], testsLabels[num_tests][destination]);//最短路径
                }
                timer[7].stop();

                // 运行并计时实验程序2的算法4
                timer[8].start();
                for (int num_tests = 0; num_tests < numTrials; num_tests++) { //实验numTrials次
                    int source = std::rand() % n;
                    int destination = std::rand() % n;
                    //int k = std::rand() % n;
                    tests[num_tests].kthShortestPath(testsLabels[num_tests][source], testsLabels[num_tests][destination], 5);//第K短路径
                }
                timer[8].stop();

                // 运行并计时实验程序2的算法5
                timer[9].start();
                for (int num_tests = 0; num_tests < numTrials; num_tests++) { //实验numTrials次
                    int source = std::rand() % n;
                    int destination = std::rand() % n;
                    tests[num_tests].allNonRepetitionPaths(testsLabels[num_tests][source], testsLabels[num_tests][destination], 10);//计算所有路径（限制10个顶点）
                }
                timer[9].stop();

                // 存储统计的运行时间以便做回归分析
                for(int i=0; i<10; ++i){
                    runtimeSequence[i].push_back(timer[i].diff_in_ms());
                }

                // 每轮输出统计的运行时间
                cout<<"\nn= "<<n<<" e= "<<e;
                for(int i=0; i<10; ++i){
                    cout<<" "<<algorithmtype[i]<<" "<<std::fixed<<std::setprecision(3)<<runtimeSequence[i].back();
                }
            }
        }
        cout<<endl<<endl;

        // 使用最小二乘法拟合十个运行时间与顶点数、边数的关系
        // 得到拟合系数coefficients、拟合程度(SSE, RSquared)、运行时间增长曲线的平均梯度模AverageGradientMagnitude。
        vector<Eigen::VectorXd> coefficients;
        vector<double> SSE, RSquared, AverageGradientMagnitude;

        // 使用二元三次多项式回归拟合运行时间和顶点数、边数的关系
        for (size_t t = 0; t < 10; ++t) {
            coefficients.push_back(multivariateCubicPolynomialRegression(vertexSequence, edgeSequence, runtimeSequence[t]));
            std::cout << "Algorithm " << algorithmtype[t] << " BCPRC: " << std::fixed << std::setprecision(6) << coefficients[t].transpose() << std::endl;

            // Calculate SSE and R-squared
            vector<double> SSE_RSquared = calculateSSE_RSquared(coefficients[t], vertexSequence, edgeSequence, runtimeSequence[t]);
            double sse = SSE_RSquared[0];
            double rSquared = SSE_RSquared[1];

            // Calculate average gradient magnitude
            double averageGradientMagnitude = calculateAverageGradientMagnitude_10(coefficients[t], vertexSequence, edgeSequence);

            AverageGradientMagnitude.push_back(averageGradientMagnitude);
            SSE.push_back(sse);
            RSquared.push_back(rSquared);
        }

        ss_Speed.push_back(std::stringstream());
        ss_Speed.push_back(std::stringstream());

        for(int i=0; i<10; ++i){
            ss_Speed[0] << "\n" << setw(25) << algorithmtype[i] << ": SSE= " 
            << std::scientific << std::setprecision(2) << setw(6) << SSE[i]  << " ; R-Squared= " 
            << std::fixed << std::setprecision(2) << setw(5) << RSquared[i] << " ; AGM= " 
            << std::scientific << std::setprecision(3) << setw(10) << AverageGradientMagnitude[i] << endl;
        }

        ss_Speed[0]<<endl<<endl;
        cout<<ss_Speed[0].str();
        ss_Speed[1] << std::setiosflags(std::ios::fixed) << std::setprecision(2);//设置输出小数形式
        ss_Speed[1] << endl;
        ss_Speed[1] << "       numVertices_from's AGM : O(n+e) AGM = " << AverageGradientMagnitude[5] / AverageGradientMagnitude[0] << endl;
        ss_Speed[1] << "               Dijkstra's AGM : O(n*n) AGM = " << AverageGradientMagnitude[6] / AverageGradientMagnitude[1] << endl;
        ss_Speed[1] << "               Dijkstra's AGM : O(elogn) AGM = " << AverageGradientMagnitude[6] / AverageGradientMagnitude[2] << endl;
        ss_Speed[1] << "                  Floyd's AGM : O(n*n*n) AGM = " << AverageGradientMagnitude[7] / AverageGradientMagnitude[4] << endl;
        ss_Speed[1] << "       Kth_ShortestPath's AGM : Dijkstra AGM = " << AverageGradientMagnitude[8] / AverageGradientMagnitude[6] << endl;
        ss_Speed[1] << "All_nonrepetition_paths's AGM : numVertices_from AGM = " << AverageGradientMagnitude[9] / AverageGradientMagnitude[5] << endl;
        ss_Speed[1] << endl;

        std::cout << ss_Speed[1].str();

        return ss_Speed;
    }
};

#endif // __UTILITIESSPEEDTEST_H__