#include<iostream>
#include<random>
#include<ctime>

#include "Box.h"

using namespace std;

int Box::id(){return id_;}
int Box::exinum(){return exinum_;}
int Box::ReactTime(){return ReactTime_;};

void Box::cgid(int x){id_=x;}
void Box::cgexi(int x){exinum_=x;}
void Box::cgRT(int x){ReactTime_=x;};

void Box::moveper(int i) {//统一处理一个人被移除后的参数变化
    exinum_--;
    for(auto &VinP : peo_[i]->InsVirs_) {
        if(VinP->vstate_==1) peoinj1_[VinP->vid_]--;//潜伏期
        else peoinj2_[VinP->vid_]--;//爆发期
        //设定隔离期无法流动
        if(peoinj1_[VinP->vid_]==0 && peoinj2_[VinP->vid_]==0 && peoinj3_[VinP->vid_]==0) 
            vnum_.erase(VinP->vid_);
    }
}

void Box::MaintainRandom(int tot) {//做tot次随机交换
    int dis=peo_.size()/4;//相差的距离
    random_device sd;
    default_random_engine engine;
    engine.seed(sd()*time(NULL));
    uniform_int_distribution<int> Peonum(0, peo_.size()-1);
    // 设置随机数引擎，随机选择人交换编号
    for(int i=0;i<tot;i++) {
        int x=Peonum(engine);
        int y=(x+dis)%peo_.size();
        swap(peo_[x],peo_[y]);
        peo_[x]->cgid(x);
        peo_[y]->cgid(y);
    }
}

void Box::Infect(int v, int injnum) {
    for(int i=peo_.size()-injnum; i<peo_.size(); i++) {//取尾部
        bool flg=true; //判断这次传染对第i个人是否有效，true代表有效
        if(peo_[i]->Recover[v]==true) continue;//已形成了免疫
        for(auto &VinP : peo_[i]->InsVirs_) {
            if(VinP->vid_==v) {
                flg=false;
                break;//正被感染
            }
        }
        if(flg==true) {
            VitoIn* VinP = new VitoIn(v,1,0);//感染日期后面统一处理
            peo_[i]->InsVirs_.push_back(VinP);
            peoinj1_[v]++; //维护感染人数
            if(peoinj1_[VinP->vid_]+peoinj2_[VinP->vid_]+peoinj3_[VinP->vid_]==1) 
                vnum_.insert(VinP->vid_);
        }
    }
}

Box::Box(int id,int ininum,int RT) {
    id_=id;
    exinum_=ininum;
    ReactTime_=RT;
    for(int i=0;i<ininum;i++) {
        Individual* indi=new Individual(/*true,*/false,false,id,i);
        peo_.push_back(indi);
    }
}