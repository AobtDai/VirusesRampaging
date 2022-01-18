#pragma once
#include<iostream>
#include<vector>
#include<unordered_map>
#include<set>

#include "Individual.h"

using namespace std;

class Box{
private:
    int id_;//沙箱编号
    int exinum_;//沙箱现有人数
    int ReactTime_;//隔离反应时间

public:
    vector<Individual*> peo_;//无隔离
    unordered_map<int, int> peoinj1_;//感染各病毒的人数(病毒编号，人数)(潜伏期)
    unordered_map<int, int> peoinj2_;//感染各病毒的人数(病毒编号，人数)(爆发期)
    unordered_map<int, int> peoinj3_;//感染各病毒的人数(病毒编号，人数)(隔离期)
    set<int> vnum_;//存储现有病毒编号
    vector<Individual*> mig_;//移民数组（暂存用）
    vector<Individual*> peoiso_;//隔离数组
    unordered_map<int, int> newdead;//每种病毒新增死亡病例
    unordered_map<int, int> newcure;//每种病毒新增治愈病例

    int id();
    int exinum();
    int ReactTime();
    
    void cgid(int x);
    void cgexi(int x);
    void cgRT(int x);

    void moveper(int i);
    void MaintainRandom(int tot);
    void Infect(int v, int injnum);

    Box(int id,int ininum,int RT);
};
