#include "Individual.h"

using namespace std;

VitoIn::VitoIn(int vid, int vstate, int injday)
    :vid_(vid), vstate_(vstate),injday_(injday),iswell_(false){}

bool Individual::isdead(){return isdead_;}
bool Individual::isisolated(){return isisolated_;}
int Individual::boxindex(){return boxindex_;} 
int Individual::id(){return id_;}

void Individual::cgdead(int x){isdead_=x;}
void Individual::cgiso(int x){isisolated_=x;}
void Individual::cgboxid(int x){boxindex_=x;}
void Individual::cgid(int x){id_=x;}

Individual::Individual(bool isdead, bool isisolated, int boxindex, int id)
    :isdead_(isdead), isisolated_(isisolated), boxindex_(boxindex), id_(id){}