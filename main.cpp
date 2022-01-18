#include<iostream>
#include<vector>

#include "Boxes.h"

using namespace std;

int main()
{
    Boxes* simu = new Boxes();
    simu->Run();
    cerr<<"Finished. The outcome is in output.out"<<endl;
    system("pause");
    return 0;
}
