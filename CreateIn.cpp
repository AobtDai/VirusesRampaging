// 该程序为大型病毒树数据生成程序

#include<iostream>
#include<random>
#include<ctime>
#include<unordered_map>

using namespace std;

int main()
{
    int n;
    cerr<<"Input range of tree: ";
    cin>>n;
    cout<<n<<endl;
    random_device sd;
    default_random_engine engine;
    engine.seed(sd()*time(NULL));
    uniform_int_distribution<int> rangeT(0, 31);
    uniform_real_distribution<double> rangeV(0.0, 100.0);
    uniform_real_distribution<double> rangeP(0.0, 1.0);
    uniform_int_distribution<int> rangeNODE(0, n-1);
    uniform_int_distribution<int> rangeW(0, 100);
    for(int i=0;i<n;i++) {
        cout<<rangeT(engine)<<" ";
        cout<<rangeV(engine)<<" ";
        double p0=rangeP(engine)/100;
        double p1=rangeP(engine);
        while(p1>1.0-p0) p1/=2.0;
        cout<<p0<<" "<<p1<<" "<<1.0-p0-p1<<endl;
    }
    for(int i=1;i<n;i++) {
        int j=rangeNODE(engine)%i;
        cout<<j<<" "<<i<<" ";
        int w1=rangeW(engine);
        int w2=rangeW(engine);
        while(w2<=w1) w2=rangeW(engine)+1;
        int w3=rangeW(engine);
        while(w3<=w1) w3=rangeW(engine)+1;
        cout<<w1<<" "<<w2<<" "<<w3<<endl;
    }
    // cout<<rangeW(engine)/10+1<<endl;
    cout<<90<<endl;
    cout<<3<<endl;
    cout<<10000<<" "<<2<<endl;
    cout<<12200<<" "<<3<<endl;
    cout<<40000<<" "<<5<<endl;
    cout<<0.01<<endl;
}