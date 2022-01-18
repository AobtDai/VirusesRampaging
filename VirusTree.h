#pragma once
#include<iostream>
#include<vector>

#include "Virus.h"

using namespace std;

class VirusTree{
private:
    Virus* head_; //树根
    int n_; // 点数
    int nlog2_; //方便LCA, 若为-1，则未预处理
    
public:
    vector<Virus*> viruses_;//病毒集
    Virus* head();

    void InputInit();
    void getnlog2();
    void chkhead();
    void PreP(int u, int father);
    int FindLCA(int a,int b);
    void DFSrace(Virus *V); // 解决第三问
    void SetHeadw2();
    void PrintCosOfRace(); //输出第三问的结果 

    VirusTree(Virus* head, int n);
};

//Refreence:
//https://www.cnblogs.com/lbssxz/p/11114819.html(LCA)