#include "FLP.h"
#include <algorithm>
#include <cassert>


#define T0 50000.0 // 初始温度
#define T_end (1e-8) //
#define q 0.98 // 退火系数
#define L 3000 // 每个温度时的迭代次数

FLP::FLP() {
  input();
  initialize();
}
void FLP::input() {
  cin >> facilityNum >> customerNum;

  capacity = vector<int>(facilityNum, 0);
  openCost = vector<int>(facilityNum, 0);
  for (int i = 0; i < facilityNum; i++) {
    cin >> capacity[i] >> openCost[i];
  }

  demand = vector<double>(customerNum, 0);
  for (int i = 0; i < customerNum; i++) {
    cin >> demand[i];
  }

  assignmentCost = vector<vector<double>>(facilityNum, vector<double>(customerNum, 0));
  for (int i = 0; i < facilityNum; i++) {
    for (int j = 0; j < customerNum; j++) {
      cin >> assignmentCost[i][j];
    }
  }

  // Test
  /*
  cout << facilityNum << " " << customerNum << endl;
  cout << capacity[4] << " " << openCost[4] << endl;
  cout << demand[7] << endl;
  cout << assignmentCost[0][3] << endl;
  */
}

void FLP::initialize() {
  presentCapacity = vector<int>(facilityNum, 0);
  isSatisfied = vector<bool>(customerNum, false);
  facilityChosen = vector<int>(customerNum, -1);
  openState = vector<int>(facilityNum, 0);
  totalCost = vector<int>(facilityNum, 0);
  srand(time(0));
  // 初始化工厂开放状态
  for (int i = 0; i < facilityNum; i++) {
    openState[i] = 1;
  }
  /*
  for (int i = 0 ; i < facilityNum; i++) {
    cout << openState[i];
  }
  */
}

void FLP::generateSolution() {
  std::fill(presentCapacity.begin(), presentCapacity.end(), 0);
  std::fill(totalCost.begin(), totalCost.end(), 0);
  double r1 = ((double)rand()) / (RAND_MAX + 1.0);
  double r2 = ((double)rand()) / (RAND_MAX + 1.0);
  int pos1 = (int)(facilityNum * r1); // 第一个交叉点位置
  int pos2 = (int)(facilityNum * r2); // 第二个交叉点位置
  
  double ratio = rand() % 100 / (double)101;
  if (ratio <= 0.33) {
    if (openState[pos1] == 1) {
      openState[pos1] = 0;
    }
    else {
      openState[pos1] = 1;
    }
  }
  else if (ratio <= 0.66) {
    if (pos1 > pos2) {
      int t = pos1;
      pos1 = pos2;
      pos2 = t;
    }
    vector<int> v = openState;
    int j = pos2;
    for (int i = pos1; i <= pos2; i++) {
      openState[i] = v[j];
      j--;
    }
  }
  else {
    int x = openState[pos1];
    openState[pos2] = openState[pos1];
    openState[pos1] = x; 
  }
  
  /*
  cout << "New Solution: " << endl;
  for (int i = 0; i < facilityNum; i++) {
    cout << openState[i] << " ";
  }
  cout << endl;
  */
}

bool FLP::update() {
  // 根据开关状态更新工厂开设费用
  for (int i = 0; i < facilityNum; i++) {
    if (openState[i] == 1) {
      totalCost[i] = openCost[i];
    }
  }
  // 给每个用户分配工厂（贪心）
  for (int j = 0; j < customerNum; j++) {
    int index = findBestFacility(j);
    //int index = findRandomOne(j);
    //cout << "index: " << index;
    if (index == -1) {
      return false;
    }
    facilityChosen[j] = index;
    presentCapacity[index] += demand[j];
    isSatisfied[j] = true;
    // 加上顾客费用
    totalCost[index] += assignmentCost[index][j];
  }
  return true;
}

int FLP::getTotalCost() {
  int total = 0;
  for (int i = 0; i < facilityNum; i++) {
    total += totalCost[i];
  }
  return total;
}

int FLP::findBestFacility(int customerIndex) {
  int temp = INT_MAX;
  int index = -1;
  for (int i = 0; i < facilityNum; i++) {
    // 工厂开放且容量不超出限制
    if (openState[i] == 1 && presentCapacity[i] + demand[customerIndex] <= capacity[i]) {
      if (assignmentCost[i][customerIndex] < temp) {
        temp = assignmentCost[i][customerIndex];
        index = i;
      }
    }
  }
  return index;
}

int FLP::findRandomOne(int customerIndex) {
  int temp = INT_MAX;
  vector<int> indices;
  for (int i = 0; i < facilityNum; i++) {
    // 工厂开放且容量不超出限制
    if (openState[i] == 1 && presentCapacity[i] + demand[customerIndex] <= capacity[i]) {
      indices.emplace_back(i);
    }
  }
  if (indices.size() == 0) {
    return -1;
  }
  double r1 = ((double)rand()) / (RAND_MAX + 1.0);
  int pos = (int)(indices.size() * r1);
  return indices[pos];
}

void FLP::greedy() {
  // 初始化
  for (int i = 0; i < facilityNum; i++) {
    openState[i] = 0;
  }

  for (int j = 0; j < customerNum; j++) {
    int choose;
    int minCost = INT_MAX;
    for (int i = 0; i < facilityNum; i++) {
      // 容量足够
      if (presentCapacity[i] + demand[j] <= capacity[i]) {
        // 每个客户找出最小的花费的工厂
        if (assignmentCost[i][j] < minCost) {
          minCost = assignmentCost[i][j];
          choose = i;
        }
      }
    }
    presentCapacity[choose] += demand[j];
    //cout << j << " choose " << choose << endl;
    totalCost[choose] += assignmentCost[choose][j];
    isSatisfied[j] = true;
    facilityChosen[j] = choose;
  }

  for (int i = 0; i < facilityNum; i++) {
    //assert(presentCapacity[i]);
    if (presentCapacity[i] != 0) {
      openState[i] = 1;
      totalCost[i] += openCost[i];
    }
    else {
      openState[i] = 0;
    }
  }

  cout << getTotalCost() << endl;

  for (int i = 0; i < facilityNum; i++) {
    cout << openState[i] << " ";
  }
  cout << endl;
  for (int j = 0; j < customerNum; j++) {
    cout << facilityChosen[j] << " ";
  }
  cout << endl;

  // 检查是否所有顾客都满足
  for (int j = 0; j < customerNum; j++) {
    assert(isSatisfied[j]);
  }
}
/*
void FLP::greedy() {
  int best = INT_MAX;
  vector<int> bestOpenState;
  vector<int> bestFacilityChosen;
  for (int i = 0; i < 5000; i++) {
    generateSolution();
    while (update() == false) {
    generateSolution();
    }
    //best = min(best, getTotalCost());
    if (getTotalCost() < best) {
      best = getTotalCost();
      bestOpenState = openState;
      bestFacilityChosen = facilityChosen;
    }
    //cout << getTotalCost() << endl;
  }
  cout << endl;
  
  cout << best << endl;
  for (int i = 0; i < facilityNum; i++) {
    cout << bestOpenState[i] << " ";
  }
  cout << endl;
  for (int j = 0; j < customerNum; j++) {
    cout << bestFacilityChosen[j] << " ";
  }
  cout << endl;


  for (int i = 0; i < customerNum; i++) {
    if (isSatisfied[i] == 0) {
      cout << "Wrong!" << endl;
      break;
    }
  }
  cout << endl;
}
*/
void FLP::create_new() {
  generateSolution();
  while (update1() == false) {
    generateSolution();
  }
}

bool FLP::update1() {
  // 根据开关状态更新工厂开设费用
  for (int i = 0; i < facilityNum; i++) {
    if (openState[i] == 1) {
      totalCost[i] = openCost[i];
    }
  }
  // 给每个用户分配工厂（贪心）
  for (int j = 0; j < customerNum; j++) {
    int index;
    int r = rand() % 100 / (double)101;
    if (r <= 0.4) {
      index = findBestFacility(j);
    }
    else {
      index = findRandomOne(j);
    }
    //cout << "index: " << index;
    if (index == -1) {
      return false;
    }
    //cout << "i: " << index << " choose j: " << j << endl;
    facilityChosen[j] = index;
    presentCapacity[index] += demand[j];
    isSatisfied[j] = true;
    // 加上顾客费用
    totalCost[index] += assignmentCost[index][j];
  }
  return true;
}

void FLP::SA() {
  double T;
  T = T0;
  int count = 0;
  double f1, f2, df;
  double r;
  int best = INT_MAX;
  vector<int> bestOpenState;
  vector<int> bestFacilityChosen;

  // 初始化工厂开放状态
  for (int i = 0; i < facilityNum; i++) {
    double ratio = rand() % 100 / (double)101;
    if (ratio < 0.5) {
      openState[i] = 0;
    }
    else {
      openState[i] = 1;
    }
  }
  vector<int> origin = openState;
  update();

  f1 = getTotalCost();
  while (T > T_end) {
    for (int i = 0; i < L; i++) {
      origin = openState;
      create_new();

      f2 = getTotalCost();
      df = f2 - f1;
      if (df >= 0) {
        r = ((double)rand()) / (RAND_MAX);
        // 保留原来的解
        if (exp(-df / T) <= r) {
          openState = origin;
        }
      }
    }
    T *= q;
    if (best > getTotalCost()) {
      best = getTotalCost();
      bestOpenState = openState;
      bestFacilityChosen = facilityChosen;
    }
    count++;
  }

  cout << best << endl;
  for (int i = 0; i < facilityNum; i++) {
    cout << bestOpenState[i] << " ";
  }
  cout << endl;
  for (int j = 0; j < customerNum; j++) {
    cout << bestFacilityChosen[j] << " ";
  }
  cout << endl;

  // 检查是否所有顾客都满足
  for (int j = 0; j < customerNum; j++) {
    assert(isSatisfied[j]);
  }
}