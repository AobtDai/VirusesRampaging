// 相当于一个顶层模块QwQ
#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<random>

#include "VirusTree.h"
#include "Virus.h"
#include "Box.h"

using namespace std;

class VaccineEvent{
public:
    int vindex_;//针对哪一款病毒
    int timecost_;//研发好的时间节点
    VaccineEvent(int vindex, int timecost);
};

struct cmp{
	bool operator()(VaccineEvent* a, VaccineEvent* b){
		return a->timecost_>b->timecost_;
	}
};

class Boxes{
private:
    VirusTree *VT_;
    vector<Box*> AllBoxes_;
    queue<VaccineEvent*> VEs_;
    double mipro_;//流动比率

    void DFSVT(priority_queue<VaccineEvent*, vector<VaccineEvent*>, cmp> &pq, Virus* V);
    void GetVC();
    inline bool IfOver();//判断人类是否全军覆没
    inline bool IfVic();//判断人类是否胜利
    void BoxMove();
    inline void UpdateIndiState(Box* box);
    inline void RemoveUpd(Box* box);
    inline void MakeMutation(int day, Box* box);
    inline void MakeIso(Box* box);//用来处理天数增加时的新增事件
    inline bool PrintDay(int day);
    inline int GetMutateNum(int day, int vid, default_random_engine& engine);

public:
    Boxes();
    void Run();
};
