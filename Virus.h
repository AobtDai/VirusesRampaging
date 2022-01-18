#pragma once
#include<iostream>
#include<vector>

using namespace std;

class Son{ // 存储边
public:
    int son_; // 孩子
    int w1_,w2_,w3_; //病毒变异，研发，研发后的变异
    Son(int son, int w1, int w2, int w3);
};

class Virus{ //等同于结点
private:
    int index_; //病毒编号
    int incblt_; //潜伏期:以天为单位//规定潜伏期不致死
    double trans_; //传染能力:每个感染者每小时会传染多少易感者，输入为爆发期的能力
    vector<double> p_; 
    //三元组：p[0]患者下一天康复概率,p[1]继续概率,p[2]死亡概率
    vector<Son*> sons_; //记录孩子及路径三元组
    bool isVaccine_; //标记是否被研制了疫苗
    
    //以下两行为LCA预处理数组
    int deep_;
    vector<int> fa_; //索引号代表2^i

    //以下为第三问辅助参量：
    int totw1_, totw2_, totw13_; 
    //1、2、13分别是不考虑疫苗作用的变异总时间、研发总时间、以及考虑疫苗后的变异总时间
    bool lagflag_; //如果病毒变异得比研发慢，则为True

public:
    int index();
    int incblt();
    double trans();
    vector<double> &p();
    vector<Son*> &sons();
    bool isVaccine();
    int deep();
    int fa(int i);
    vector<int> &fa();
    int totw1();
    int totw2();
    int totw13();
    bool lagflag();
    void cgfa(int i, int x);

    void cginfor(int incblt, double trans, vector<double> &p);
    void cgdeep(int x);
    void cgtotw1(int x);
    void cgtotw2(int x);
    void cgtotw13(int x);
    void cglag(bool x);
    void pushson(Son* S);
    void cgisVacc(bool x);

    Virus(int index, int incblt, double trans, vector<double> &p);
};