// 开始想弃置人类类的，但由于要保证沙箱流动的合理性与随机性，合理完成病毒传染的重置性
// 所以保留此类，但信息完整度和模型合理性的提高的代价是时空复杂度的上升

#pragma once
#include<iostream>
#include<unordered_map>

#include "Virus.h"

using namespace std;

class VitoIn{ //感染人体的病毒
public:
    int vid_;
    int vstate_; //状态: 1-潜伏，2-爆发
    int injday_; // 该病毒感染人体的天数
    bool iswell_;//用来在状态转移中暂时存储康复与否
    VitoIn(int vid, int vstate, int injday);
};

// 人类类中后增加数组内找携带病毒操作，先略
class Individual{
private:
    // bool iswell_; // 标记是否健康  //似乎不需要
    bool isdead_; // 标记是否死亡 
    bool isisolated_; //标记是否被隔离
    int boxindex_; //沙箱归属编号
    int id_;//沙箱内部编号(from 0)
    
public:
    vector<VitoIn*> InsVirs_; //感染该人体病毒
    unordered_map<int, bool> Recover; //记录已经形成免疫的病毒，true为曾感染现康复

    bool isdead(); 
    bool isisolated(); 
    int boxindex(); 
    int id();

    void cgdead(int x);
    void cgiso(int x);
    void cgboxid(int x);
    void cgid(int x);

    Individual(bool isdead, bool isisolated, int boxindex, int id);
};