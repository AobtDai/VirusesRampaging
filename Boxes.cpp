#include<random>
#include<ctime>
#include<set>

#include "Boxes.h"

using namespace std;

VaccineEvent::VaccineEvent(int vindex, int timecost){
    vindex_=vindex;
    timecost_=timecost;
}

Boxes::Boxes() {
    int n;
    cin>>n;
    VT_ = new VirusTree(NULL, n);
    VT_->InputInit();
    VT_->chkhead();
    VT_->SetHeadw2();
    VT_->DFSrace(VT_->head());
    VT_->PrintCosOfRace();
    random_device sd;
    default_random_engine engine;
    engine.seed(sd()*time(NULL));
    //随机初始感染率，默认为max(10,0.01*tot)
    int boxnum;
    cin>>boxnum;
    for(int i=0;i<boxnum;i++) {
        int x,y;//初始人口的输入和隔离反应时间
        cin>>x>>y;
        Box *B = new Box(i,x,y);
        AllBoxes_.push_back(B);
        B->vnum_.insert(0);
        uniform_int_distribution<int> Initnum(0, x-1);
        int InitInfect=max(min(10.0,B->exinum()*1.0), 0.01*B->exinum());
        for(int k=0;k<InitInfect;k++) {
            int id=Initnum(engine);

            if(B->peo_[id]->InsVirs_.empty()) {
                VitoIn *VinP = new VitoIn(0,1,0);
                B->peo_[id]->InsVirs_.push_back(VinP);
                B->peoinj1_[0]++;
            }
        }
    }
    cin>>mipro_;
    //Test LCA Fuction:
    bool ifLCA;
    cout<<"Do you want to find LCAs? If want, please input 1, or input 0."<<endl;
    // cerr<<"Do you want to find LCAs? If want, please input 1, or input 0."<<endl;
    while(true) {
        cin>>ifLCA;
        if(ifLCA==0) break;
        // cerr<<"Please input the indexes of the two node you want research:"<<endl;
        cout<<"Please input the indexes of the two node you want research:"<<endl;
        int a,b;
        cin>>a>>b;
        // cerr<<"The LCA of "<<a<<" and "<<b<<" is: "<<VT_->FindLCA(a,b)<<endl;
        cout<<"The LCA of "<<a<<" and "<<b<<" is: "<<VT_->FindLCA(a,b)<<endl;
        // cerr<<"Do you want to continue? If want, please input 1, or input 0."<<endl;
        cout<<"Do you want to continue? If want, please input 1, or input 0."<<endl;
    }
}

void Boxes::DFSVT(priority_queue<VaccineEvent*, vector<VaccineEvent*>, cmp> &pq, Virus* V) {
    if(V==NULL) return ;
    if(V->lagflag()==true) return ;//绝种的不必考虑
    VaccineEvent* NewEvent = new VaccineEvent(V->index(),V->totw2());
    pq.push(NewEvent);
    for(int i=0; i<V->sons().size(); i++) {
        Son* Sto=V->sons()[i];
        Virus* to=VT_->viruses_[Sto->son_];
        DFSVT(pq, to);
    }
}

void Boxes::GetVC() {
    priority_queue<VaccineEvent*, vector<VaccineEvent*>, cmp> pq;
    DFSVT(pq,VT_->head());
    while(!pq.empty()) {
        VEs_.emplace(pq.top());
        pq.pop();
    }
}

inline bool Boxes::IfOver() { //判断人类是否全军覆没
    int tot=0;
    for(auto &box : AllBoxes_) {
        tot+=box->exinum();
    }
    if(tot==0) return true;
    else return false;
}

inline bool Boxes::IfVic() { //判断人类是否胜利
    for(auto &box : AllBoxes_) {
        if(box->vnum_.size()>0) return false;
    }
    return true;
}

void Boxes::BoxMove() {
    if(AllBoxes_.size()==1) return ;//只有一个沙箱不用move
    random_device sd;
    default_random_engine engine;
    engine.seed(sd()*time(NULL));
    uniform_int_distribution<int> Boxnum(0, AllBoxes_.size()-1);
    // 设置随机数引擎，随机选择目标沙箱
    
    for(auto &box : AllBoxes_) {
        int totflux=box->peo_.size();
        int num=mipro_*totflux;
        for(int i=totflux-num; i<totflux; i++) {//取尾部
            int TarBox=Boxnum(engine);
            box->moveper(i);
            AllBoxes_[TarBox]->mig_.push_back(box->peo_[i]);//move to Target Box
        }
        for(int i=totflux-num; i<totflux; i++) {//删除尾部
            box->peo_.pop_back();
        }
    }
    
    for(auto &box : AllBoxes_) {//将mig并入并改变“移民”的具体参数
        int addnum=box->mig_.size();
        int totflux=box->peo_.size();
        for(int i=totflux; i<totflux+addnum; i++) {
            box->peo_.push_back(box->mig_[i-totflux]);
            box->cgexi(box->exinum()+1);
            box->peo_[i]->cgboxid(box->id());//改变沙箱归属编号
            box->peo_[i]->cgid(i);//改变人员内部编号
            //以下改变map和set
            for(auto &VinP : box->peo_[i]->InsVirs_) {
                if(VinP->vstate_==1) box->peoinj1_[VinP->vid_]++;//潜伏期
                else box->peoinj2_[VinP->vid_]++;//爆发期
                if(box->peoinj1_[VinP->vid_]+box->peoinj2_[VinP->vid_]+box->peoinj3_[VinP->vid_]==1) 
                    box->vnum_.insert(VinP->vid_);
            }
        }
        box->MaintainRandom(box->mig_.size());//随机规则:交换
        while(!box->mig_.empty()) box->mig_.pop_back();
    }
}

inline void Boxes::UpdateIndiState(Box* box){
    random_device sd;
    default_random_engine engine;
    engine.seed(sd()*time(NULL));
    uniform_real_distribution<double> ZeroToOne(0.0, 1.0);
    // 设置随机数引擎，随机选择后续三元组

    //以下更新非隔离人口数组
    for(auto &indi : box->peo_) {
        for(auto &VinP : indi->InsVirs_) {
            int vid=VinP->vid_;
            if(VinP->iswell_==true) continue;
            //因为一个是24h，一个是一天，所以要特判
            vector<double> pp(VT_->viruses_[vid]->p());
            if(VinP->vstate_==1) { //潜伏
                double x=ZeroToOne(engine);
                x = x*((pp[0]+pp[1])/1.0);
                if(x<pp[0]) {// 康复情况，持续治疗不管
                    indi->Recover[vid]=true;
                    VinP->iswell_=true;
                    int vidt=vid;
                    while(!VT_->viruses_[vidt]->fa().empty()) {
                    //向上爬找免疫更新
                        int favid=VT_->viruses_[vidt]->fa(0);
                        if(indi->Recover[favid]==true) break;
                        //之前祖先已经更新过则break
                        indi->Recover[favid]=true;
                        vidt=favid;
                    }
                    box->newcure[vid]++;
                }
            }
            else {//爆发情况 VinP->vstate_==2
                double x=ZeroToOne(engine);
                if(x<pp[0]) {// 康复情况，持续治疗不管
                    indi->Recover[vid]=true;
                    int vidt=vid;
                    VinP->iswell_=true;
                    while(!VT_->viruses_[vidt]->fa().empty()) {
                    //向上爬找免疫更新
                        int favid=VT_->viruses_[vidt]->fa(0);
                        if(indi->Recover[favid]==true) break;
                        //之前祖先已经更新过则break
                        indi->Recover[favid]=true;
                        vidt=favid;
                    }
                    box->newcure[vid]++;
                }
                else if(x>pp[0]+pp[1]){//死亡情况
                    if(indi->isdead()==false) box->newdead[vid]++;
                    indi->cgdead(true);
                }
            }
        }
        if(indi->isdead()==true) {
            box->cgexi(box->exinum()-1);//总人数--
            for(auto &VinP : indi->InsVirs_) {
                int vid=VinP->vid_;
                if(VinP->vstate_==2) box->peoinj2_[vid]--;
                else box->peoinj1_[vid]--;
                if(box->peoinj1_[vid]+box->peoinj2_[vid]+box->peoinj3_[vid]==0) 
                    box->vnum_.erase(vid);
            }
        }
        else {
            for(auto &VinP : indi->InsVirs_) {
                int vid=VinP->vid_;
                if(VinP->iswell_==true) {
                    if(VinP->vstate_==1) {
                        box->peoinj1_[vid]--;
                    }
                    else {
                        box->peoinj2_[vid]--;
                    }
                    if(box->peoinj1_[vid]+box->peoinj2_[vid]+box->peoinj3_[vid]==0) 
                        box->vnum_.erase(vid);
                }
            }
        }
    }

    //以下更新隔离人口数组(内部实现同上，除对死亡病例处理不同)
    for(auto &indi : box->peoiso_) {
        for(auto &VinP : indi->InsVirs_) {
            int vid=VinP->vid_;
            if(VinP->iswell_==true) continue;
            vector<double> pp(VT_->viruses_[vid]->p());
            if(VinP->vstate_==1) { //潜伏
                double x=ZeroToOne(engine);
                x = x*((pp[0]+pp[1])/1.0);
                if(x<pp[0]) {// 康复情况，持续治疗不管
                    indi->Recover[vid]=true;
                    int vidt=vid;
                    VinP->iswell_=true;
                    while(!VT_->viruses_[vidt]->fa().empty()) {
                    //向上爬找免疫更新
                        int favid=VT_->viruses_[vidt]->fa(0);
                        if(indi->Recover[favid]==true) break;
                        //之前祖先已经更新过则break
                        indi->Recover[favid]=true;
                        vidt=favid;
                    }
                    box->newcure[vid]++;
                }
            }
            else {//爆发情况
                double x=ZeroToOne(engine);
                if(x<pp[0]) {// 康复情况，持续治疗不管
                    indi->Recover[vid]=true;
                    int vidt=vid;
                    VinP->iswell_=true;
                    while(!VT_->viruses_[vidt]->fa().empty()) {
                    //向上爬找免疫更新
                        int favid=VT_->viruses_[vidt]->fa(0);
                        if(indi->Recover[favid]==true) break;
                        //之前祖先已经更新过则break
                        indi->Recover[favid]=true;
                        vidt=favid;
                    }
                    box->newcure[vid]++;
                }
                else if(x>pp[0]+pp[1]){//死亡情况
                    if(indi->isdead()==false) box->newdead[vid]++;
                    indi->cgdead(true);
                }
            }
        }
        if(indi->isdead()==true) {
            box->cgexi(box->exinum()-1);//总人数--
            for(auto &VinP : indi->InsVirs_) {
                int vid=VinP->vid_;
                box->peoinj3_[vid]--;
                if(box->peoinj1_[vid]+box->peoinj2_[vid]+box->peoinj3_[vid]==0) 
                    box->vnum_.erase(vid);
            }
        }
        else {
            for(auto &VinP : indi->InsVirs_) {
                int vid=VinP->vid_;
                if(VinP->iswell_==true) {
                    box->peoinj3_[vid]--;
                }
                if(box->peoinj1_[vid]+box->peoinj2_[vid]+box->peoinj3_[vid]==0) 
                    box->vnum_.erase(vid);
            }
        }
    }
}

inline void Boxes::RemoveUpd(Box* box) {
// 因为对序列是否有序无要求，所以使用交换删除法
    for(int i=0; i<box->peo_.size(); i++) {
        auto indi=box->peo_[i];
        int flg=false;
        while(indi->isdead()==true) {//remove deads
            if(i<box->peo_.size()-1) {//not the last one
                swap(box->peo_[i], box->peo_.back());
                indi=box->peo_[i];
                indi->cgid(i);
                box->peo_.pop_back();
            }
            else {
                box->peo_.pop_back();
                flg=true;
                break;
            }
        }
        if(flg) break;
        for(int j=0; j<indi->InsVirs_.size(); j++) {
            auto VinP=indi->InsVirs_[j];
            int vid=VinP->vid_;
            while(VinP->iswell_==true) { //remove invalid virus
                if(j<indi->InsVirs_.size()-1) {
                    swap(indi->InsVirs_[j], indi->InsVirs_.back());
                    VinP=indi->InsVirs_[j];
                    vid=VinP->vid_;
                    indi->InsVirs_.pop_back();
                }
                else {
                    indi->InsVirs_.pop_back();
                    break;
                }
            }
        }
    }
    //隔离数组：
    for(int i=0; i<box->peoiso_.size(); i++) {
        auto indi=box->peoiso_[i];
        int flg=false;
        while(indi->isdead()==true) {//remove deads
            if(i<box->peoiso_.size()-1) {//not the last one
                swap(box->peoiso_[i], box->peoiso_.back());
                indi=box->peoiso_[i];
                indi->cgid(i);
                box->peoiso_.pop_back();
            }
            else {
                box->peoiso_.pop_back();
                flg=true;
                break;
            }
        }
        if(flg) break;
        for(int j=0; j<indi->InsVirs_.size(); j++) {
            auto VinP=indi->InsVirs_[j];
            int vid=VinP->vid_;
            while(VinP->iswell_==true) { //remove invalid virus
                if(j<indi->InsVirs_.size()-1) {
                    swap(indi->InsVirs_[j], indi->InsVirs_.back());
                    VinP=indi->InsVirs_[j];
                    vid=VinP->vid_;
                    indi->InsVirs_.pop_back();
                }
                else {
                    indi->InsVirs_.pop_back();
                    break;
                }
            }
        }
    }
}

inline int Boxes::GetMutateNum(int day, int vid, default_random_engine& engine) {
    uniform_real_distribution<double> ZeroToOne(0.0, 1.0);
    int childnum=VT_->viruses_[vid]->sons().size();//k值
    if(childnum==0) return -1; //该病毒无孩子
    double dif=1.0/childnum;
    set<int> FirstChosen;//set放第一轮随机被选中且有效的子变异方向的编号
    for(int i=0; i<childnum; i++) {//随机儿子次
        int mark=(ZeroToOne(engine)/dif);
        mark%=childnum; //保险一点
        auto Sto=VT_->viruses_[vid]->sons()[mark];
        auto to=VT_->viruses_[Sto->son_];
        if(to->totw13()>=day) {
            FirstChosen.insert(Sto->son_);//符合变异条件则进set
        }
    }
    int k2=FirstChosen.size();
    if(k2==0) return -1;//没有可供变异的
    double dif2=1.0/k2;
    int mark2=(ZeroToOne(engine)/dif2);
    mark2%=k2;
    int v=-1, i=0;
    for(auto &choice : FirstChosen) {
        if(i==mark2) {
            v=choice;//v存储变异株的编号
            break;
        }
        i++;
    }
    return v;
}

inline void Boxes::MakeMutation(int day, Box* box) {
    random_device sd;
    default_random_engine engine;
    engine.seed(sd()*time(NULL));
    // 设置随机数引擎，随机选择变异
    //以下对非隔离人群处理
    for(auto &indi : box->peo_) {
        for(auto &VinP : indi->InsVirs_) {
            int v=GetMutateNum(day,VinP->vid_,engine);
            if(v==-1) continue;
            bool flg=false;//判断indi身上有没有变异株
            for(auto VinP2 : indi->InsVirs_) {
                if(v==VinP2->vid_) {
                    flg=true;
                    break;
                }
            }
            if(VT_->viruses_[v]->lagflag()==true) flg=true;
            //因为疫苗事件没有加入绝种事件，所以这里要判断，杜绝变异株的产生
            VitoIn* newVinP = new VitoIn(v,1,0);//感染日期MakeIso统一处理
            if(flg==false) {
                indi->InsVirs_.push_back(newVinP);
                box->peoinj1_[v]++;
                if(box->peoinj1_[v]+box->peoinj2_[v]+box->peoinj3_[v]==1)
                    box->vnum_.insert(v);    
            }
        }
    }
    //以下对隔离人群处理
    for(auto &indi : box->peoiso_) {
        for(auto &VinP : indi->InsVirs_) {
            int v=GetMutateNum(day,VinP->vid_,engine);
            if(v==-1) continue;
            bool flg=false;//判断indi身上有没有变异株
            for(auto VinP2 : indi->InsVirs_) {
                if(v==VinP2->vid_) {
                    flg=true;
                    break;
                }
            }
            if(VT_->viruses_[v]->lagflag()==true) flg=true;
            //因为疫苗事件没有加入绝种事件，所以这里要判断，杜绝变异株的产生
            VitoIn* newVinP = new VitoIn(v,1,0);//感染日期MakeIso统一处理
            if(flg==false) {
                indi->InsVirs_.push_back(newVinP);
                box->peoinj3_[v]++; //维护感染人数
                if(box->peoinj1_[v]+box->peoinj2_[v]+box->peoinj3_[v]==1)
                    box->vnum_.insert(v);    
            }
        }
    }
}

inline void Boxes::MakeIso(Box* box) {
    for(int i=0; i<box->peo_.size(); i++) {
        auto indi=box->peo_[i];
        bool flg=false;//判断是否能隔离，是为true
        for(auto &VinP : indi->InsVirs_) {
            VinP->injday_++;
            if(VinP->injday_==VT_->viruses_[VinP->vid_]->incblt()) {
            //达成爆发条件,因为我们在一天的末尾判断，所以上述判断用==
                VinP->vstate_=2;
                box->peoinj1_[VinP->vid_]--;
                box->peoinj2_[VinP->vid_]++;
            }
            if(flg==false && VinP->injday_ - VT_->viruses_[VinP->vid_]->incblt()>=box->ReactTime()){
            //达成隔离条件
                flg=true;
            }
        }
        if(flg==true) {
            indi->cgiso(true);
            for(auto &VinP : indi->InsVirs_) {
                box->peoinj3_[VinP->vid_]++;
                if(VinP->vstate_==2) box->peoinj2_[VinP->vid_]--;
                else box->peoinj1_[VinP->vid_]--;
            }
            box->peoiso_.push_back(indi);
            //下面是交换删除
            if(i==box->peo_.size()-1) {
                box->peo_.pop_back();
                break;
            }
            swap(box->peo_[i], box->peo_.back());
            box->peo_.pop_back();
            i--;//退一位，避免跳过indi
        }
    }
    
    //接下来是对隔离数组的处理——主要有出院操作
    for(int i=0; i<box->peoiso_.size(); i++) {
        auto indi=box->peoiso_[i];
        bool flg=true; // 判断这个患者能否出院，true为能
        for(auto &VinP : indi->InsVirs_) {
            VinP->injday_++;
            if(VinP->injday_==VT_->viruses_[VinP->vid_]->incblt()) {
            //达成爆发条件,因为我们在一天的末尾判断，所以上述判断用==
                VinP->vstate_=2;
            }
            if(flg==true && VinP->injday_ - VT_->viruses_[VinP->vid_]->incblt()>=box->ReactTime()){
                flg=false;
            }
        }
        if(flg==true) {
            indi->cgiso(false);
            for(auto &VinP : indi->InsVirs_) {
                box->peoinj3_[VinP->vid_]--;
                if(VinP->vstate_==2) box->peoinj2_[VinP->vid_]++;
                else box->peoinj1_[VinP->vid_]++;
            }
            box->peo_.push_back(indi);
            //下面是交换删除
            if(i==box->peoiso_.size()-1) {
                box->peoiso_.pop_back();
                break;
            }
            swap(box->peoiso_[i], box->peoiso_.back());
            box->peoiso_.pop_back();
            i--;//退一位，避免跳过indi
        }
    }
}

inline bool Boxes::PrintDay(int day) {
    if(IfOver()) {
        cout<<"Unfortunately, there is no survivors."<<endl;
        return true;
        // break;
    }
    if(IfVic()) {
        cout<<"Well done! All the viruses have been eradicated!"<<endl;
        return true;
        // break;
    }
    for(auto &box : AllBoxes_) {
        // cout<<"这里是"<<box->id()<<"号沙箱的情况：";
        cout<<"This is the situaiton of box "<<box->id()<<" : "<<endl;
        // cout<<"现存"<<box->exinum()<<"人"<<endl;
        cout<<box->exinum()<<" survivors now"<<endl;
        for(auto &v : box->vnum_) {
            // cout<<"\t第"<<v<<"号病毒："<<endl;
            // cout<<"\t"<<v<<"-Virus: "<<endl;
            cout<<" "<<v<<"-Virus: "<<endl;
            // cout<<"\t\t现存潜伏病例："<<box->peoinj1_[v]<<'\t';
            // cout<<"\t\tExisting latent cases: "<<box->peoinj1_[v]<<'\t';
            cout<<"  Existing latent cases: "<<box->peoinj1_[v]<<' ';
            // cout<<"\t现存爆发病例："<<box->peoinj2_[v]<<'\t';
            // cout<<"\tExisting outbreak cases: "<<box->peoinj2_[v]<<'\t';
            cout<<" Existing outbreak cases: "<<box->peoinj2_[v]<<' ';
            // cout<<"\t现存已隔离病例："<<box->peoinj3_[v]<<endl;
            // cout<<"\tExisting isolated cases: "<<box->peoinj3_[v]<<endl;
            cout<<" Existing isolated cases: "<<box->peoinj3_[v]<<endl;
            // cout<<"\t\t新增治愈病例："<<box->newcure[v]<<'\t';
            // cout<<"\t\tNewly cured cases: "<<box->newcure[v]<<'\t';
            cout<<"  Newly cured cases: "<<box->newcure[v]<<' ';
            // cout<<"\t新增死亡病例："<<box->newdead[v]<<endl;
            // cout<<"\tNew deaths: "<<box->newdead[v]<<endl;
            cout<<" New deaths: "<<box->newdead[v]<<endl;
        }
        box->newcure.clear();
        box->newdead.clear();//清空
    } 
    cout<<"++++++++++++++++++++++++++++++++++"<<endl;
    return false;
}

void Boxes::Run() {
    int day=1;
    GetVC();
    while(true) {
        // cerr<<"Processing, "<<day<<" now"<<endl;
        // cout<<"第"<<day<<"天："<<endl;
        cout<<"Is is "<<day<<" day now: "<<endl;
        // cout<<day<<": "<<endl;
        VaccineEvent* VE;
        while(!VEs_.empty()) {// 判断事件
            VE=VEs_.front();
            if(VE->timecost_!=day) break;
            VEs_.pop();
            VT_->viruses_[VE->vindex_]->cgisVacc(true);
        }

        BoxMove();//流动

        for(int i=1;i<=24;i++) {
            for(auto &box : AllBoxes_) {
                queue<pair<int,int>> q;//记录病毒编号和要感染多少人，避免数据被冲
                for(auto v : box->vnum_) {
                    if(VT_->viruses_[v]->isVaccine()==true) continue;//疫苗研制成功
                    int injnum=0;
                    injnum+=box->peoinj1_[v] * VT_->viruses_[v]->trans()/10;
                    injnum+=box->peoinj2_[v] * VT_->viruses_[v]->trans();
                    if(injnum>box->peo_.size()) injnum=box->peo_.size();//超出上限情况处理
                    q.emplace(v,injnum);
                }
                while(!q.empty()) {
                    auto [v,injnum]=q.front();
                    q.pop();
                    box->Infect(v,injnum);//统一进行传染操作
                    box->MaintainRandom(injnum);//再次随机，确保随机程度的稳定
                }
                box->MaintainRandom(box->exinum()/100);
                UpdateIndiState(box);
                RemoveUpd(box);// 清除死亡人口和个人中的已免疫病毒
                /*
                后续为状态处理，需要完成以下内容：
                1. 对感染的人做状态转移（包括peoiso_）：康复死亡或持续治疗
                注意转移后对box各人数的维护（仔细！）
                康复需要向上爬fa数组更新所有免疫直至有免疫，如果无其他病毒爆发，出院
                死亡要减减
                该过程要存储一个累计值以便输出使用
                */
            }
        }
        for(auto &box : AllBoxes_) {
            MakeMutation(day,box);
            MakeIso(box);
        }
        bool chk=PrintDay(day);
        if(chk==true) break;
        day++;
        /*(即规定了变异是在每天的最后时刻发生)
        2. 在1~24跑完后，即一天结束后，扫描所有box里所有人
        对符合要求的病毒做变异处理，注意相关参数的维护（细节），注意维护injday_
        3. 对有隔离要求的要隔离——从peo_移除（注意维护其它数组），加入到peoiso_
        4. 输出：每种病毒的感染情况、新增康复以及新增死亡
        感染情况可分为潜伏期、爆发期以及隔离救治期（确诊病例）。
        */
    }
}

// 要搞一个Box的病毒库，记录多少人感染了下标对应的病毒
// 即Box[2]=100代表该沙箱里有100个人感染了2号病毒(unordered_map)
// 搞一个event类，记录事件类型（疫苗或病毒）、发生的时间（病毒事件好像不需要？）

// 然后while(Day)中一天一天判断，和front的时间相同即是触发了特殊事件（注意！可能会触发多个事件）
// while里先实现沙箱的流动功能，将个人编号放入数组，每次流动数组的后面部分比例的人口
// （这样可以快速的改人的编号）
// 然后再随机——即按距离至少相差25%的限制交换数组两元素（同时可以把编号也交换了）
// 次数暂时设定为和流动人口数相同
// 移出的人口存放在目标沙箱的“移民”数组里，如此以避免一个人在一时段流动了两次
// 整个流动完成后再将“移民”归位
// 这样的移民操作可将复杂度搞到O(n)

// 之后再套一个for(1~24)代表一天内24h迭代（左边界为0-1h的情况判断）
// 先考虑已有病毒的传染，另传染优先级最高（可以理解为已进入0：00就传染完毕）
// 每次感染操作同样使用封装的随机方法（同上）（）
// 暂时设想最后将三元组状态转移
//（加上隔离和概率变异的操作）（总的来说是状态的更新,使状态与当前小时对应）
// 这里暂时设定是病毒变异后当前小时可传染
//（即初始）时是day=0，然后每进以此循环就++（此处day为某病毒在某个人上的感染天数）
// 如果触发了疫苗事件，则标记病毒，上面的for里就不传染它