#include<iostream>

#include "VirusTree.h"

using namespace std;

Virus* VirusTree::head(){return head_;}
VirusTree::VirusTree(Virus* head, int n):head_(head), n_(n), nlog2_(-1){}

void VirusTree::chkhead() {
    for(auto &V : viruses_) {
        if(V->fa().empty()) { //LCA处
            head_ = V;
            return ;
        }
    }
}

void VirusTree::InputInit() {
    for(int i=0;i<n_;i++) { // 读点
        int t;
        double v;
        vector<double> p;
        cin>>t>>v;
        for(int j=0;j<3;j++) {
            double x;
            cin>>x;
            p.push_back(x);
        }
        Virus* V = new Virus(i,t,v,p);
        viruses_.push_back(V);
    }
    for(int i=0;i<n_-1;i++) { // 读边
        int u,v,a,b,c;
        cin>>u>>v>>a>>b>>c;
        Son* S = new Son(v,a,b,c);
        viruses_[u]->pushson(S);
        viruses_[v]->cgfa(0,u);//将fa[0]初始化，防止溢出
    }
}

void VirusTree::getnlog2() {
    int x=n_, cnt=0;
    while(x) {
        cnt++;
        x/=2;
    }
    nlog2_ = cnt;
}

void VirusTree::PreP(int u, int father) {
    if(father==-1) viruses_[u]->cgdeep(1);
    else viruses_[u]->cgdeep(viruses_[father]->deep()+1); // 继承深度

    for(int i=1; (1<<i)<=viruses_[u]->deep(); i++) {
        viruses_[u]->cgfa(i, viruses_[viruses_[u]->fa(i-1)]->fa(i-1));
        //即f[u][i]=f[f[u][i-1]][i-1]
    }

    for(int i=0; i<viruses_[u]->sons().size(); i++) {
        Son* Sto=viruses_[u]->sons()[i];
        int to=Sto->son_;
        // viruses_[to]->cgfa(0,u);//将fa[0]初始化，防止溢出
        PreP(to, u);
    }
}

int VirusTree::FindLCA(int a, int b){
    if(nlog2_==-1){ 
        getnlog2(); 
        // initfa(n_);
        head_->cgfa(0,0);//对头指针的fa[0]初始化
        PreP(head_->index(), -1);
    }
    if(viruses_[a]->deep() < viruses_[b]->deep()) { //统一格式，让deep[a]>deep[b]
        swap(a,b);
    }
    for(int i=viruses_[a]->fa().size()-1; i>=0; i--) {
        // 以下即if(dep[f[x][i]]>=dep[y])x=f[x][i];
        if(viruses_[viruses_[a]->fa(i)]->deep() >= viruses_[b]->deep())
            a = viruses_[a]->fa(i);
        if(a==b) return a;
    }
    for(int i=viruses_[a]->fa().size()-1; i>=0; i--) {
        if(viruses_[a]->fa(i) != viruses_[b]->fa(i)) {
            a = viruses_[a]->fa(i);
            b = viruses_[b]->fa(i);
        }
    }
    return viruses_[a]->fa(0);
}

void VirusTree::SetHeadw2() { //初始化根节点疫苗研制信息
    int x;
    cin>>x;
    head_->cgtotw2(x);
}

void VirusTree::DFSrace(Virus *V) {//1、2、3:病毒变异，研发，研发后的变异
    if(V->sons().empty()) return ;//叶子节点return
    for(int i=0; i<V->sons().size(); i++) {
        Son* Sto=V->sons()[i];
        Virus* to=viruses_[Sto->son_];

        to->cgtotw1(V->totw1()+Sto->w1_);
        to->cgtotw2(V->totw2()+Sto->w2_);

        if(V->lagflag()==true) { //已经被阻断的情况（父节点疫苗提前研发好）
            to->cglag(true);
        }
        else if(V->totw13()+Sto->w1_ < V->totw2()) {
            // 父节点疫苗在变异株出现前也没研制好
            to->cgtotw13(V->totw13()+Sto->w1_);
        }
        else { // 父节点疫苗在变异途中研制好（按比例计算）
            double x=(1.0 - ((double)V->totw2() - (double)V->totw13()) 
            / (double)Sto->w1_) * (double)Sto->w3_;
            to->cgtotw13((x+1)+V->totw2());
            if(to->totw2() < to->totw13()) {
                to->cglag(true);
            }
        }

        DFSrace(to); 
    }
}

void VirusTree::PrintCosOfRace() {
    for(auto &V : viruses_) {
        if(V->lagflag()==true) {
            cout<<V->index()<<": This virus will not appear thanks to dear scientists.";
            cout<<endl<<endl;
        }
        else {
            cout<<V->index()<<": So sorry for that this virus will appear.\n";
            cout<<"It will take us "<<V->totw2()-V->totw13()<<" days to beat it.";
            cout<<endl<<endl;
        }
    }
}