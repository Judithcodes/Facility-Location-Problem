#ifndef FLP_H
#define FLP_H

#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>
using namespace std;

class FLP {
public:
  FLP();
  void input();
  void initialize();
  void generateSolution();
  int getTotalCost();
  bool update();
  bool update1();
  int findBestFacility(int customerIndex);
  int findRandomOne(int customerIndex);
  void greedy();
  void SA();
  void create_new();
private:
  // 输入内容
  int facilityNum; // 工厂数量
  int customerNum; // 顾客数量
  vector<int> capacity; // 工厂容量
  vector<int> openCost; // 工厂开设费用
  vector<double> demand; // 顾客需求

  // // [i,j]第i个工厂对第j个顾客的费用
  vector<vector<double>> assignmentCost;

  vector<int> presentCapacity; // 当前时刻工厂已分配容量

  // 输出内容
  vector<int> openState; // 工厂开放状态
  vector<int> totalCost; // 工厂总支出
  vector<int> facilityChosen; // 顾客选择的工厂
  // 约束条件
  vector<bool> isSatisfied; // 顾客是否满足
};

#endif