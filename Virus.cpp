#include "Virus.h"

using namespace std;
    
Son::Son(int son, int w1, int w2, int w3) 
    :son_(son), w1_(w1), w2_(w2), w3_(w3){}

int Virus::incblt(){return incblt_;}
double Virus::trans(){return trans_;}
vector<double>& Virus::p(){return p_;}
vector<Son*>& Virus::sons(){return sons_;}
bool Virus::isVaccine(){return isVaccine_;}
int Virus::index(){return index_;}
int Virus::totw1(){return totw1_;}
int Virus::totw2(){return totw2_;}
int Virus::totw13(){return totw13_;}
bool Virus::lagflag(){return lagflag_;}

int Virus::deep(){return deep_;}
int Virus::fa(int i) { return fa_[i];}
vector<int>& Virus::fa(){return fa_;};
void Virus::cgfa(int i, int x) { 
    if(i<fa_.size()) fa_[i]=x;
    else fa_.push_back(x);
}

void Virus::cgtotw1(int x){totw1_=x;}
void Virus::cgtotw2(int x){totw2_=x;}
void Virus::cgtotw13(int x){totw13_=x;}
void Virus::cgisVacc(bool x){isVaccine_=x;}
void Virus::cglag(bool x){lagflag_=x;}

void Virus::cginfor(int incblt, double trans, vector<double> &p) {
    incblt_ = incblt;
    trans_ = trans;
    for(auto &x : p) {
        p_.push_back(x);
    }
}
void Virus::cgdeep(int deep) { deep_=deep;}

void Virus::pushson(Son* S) { sons_.push_back(S); }

Virus::Virus(int index, int incblt, double trans, vector<double> &p)
        :index_(index),incblt_(incblt), trans_(trans), p_(p), sons_({}), 
        isVaccine_(false), totw1_(0), totw2_(0), totw13_(0), lagflag_(false){}