#include<iostream>
#include<cmath>
#include<string>
#include<fstream>

#include"Random64.hpp"

const int Lx = 200;
const int Ly = 200;

class RWLattice{
    private:
        int *coffee = NULL;
    public:
        RWLattice(void);
        ~RWLattice();
        int get1D(int ix, int iy);
        void initialize(void);
        void propagate(void);
        void save(std::string filename);
};

RWLattice::RWLattice(void){
    coffee = new int[Lx*Ly];
}

RWLattice::~RWLattice(){
    delete[] coffee;
}

int RWLattice::get1D(int ix, int iy){
    return ix*Ly + iy;
}

void RWLattice::initialize(void){
    for (int ix=0; ix<Lx; ix++)
        for (int iy=0; iy<Ly; iy++){
            int pos = get1D(ix, iy);
            coffee[pos] = 0;
            if(ix >= 90 && ix <= 110)
                if(iy >= 90 && iy <= 110)
                    coffee[pos] = 1;
        }
}

void RWLattice::propagate(void){
    for (int ix=0; ix<Lx; ix++)
        for (int iy=0; iy<Ly; iy++){
            int pos = get1D(ix, iy);
            coffee[pos] = 0;
        }
}

void RWLattice::save(std::string filename){
    std::ofstream File(filename);
    for(int ix=0; ix<Lx; ix++){
        for(int iy=0; iy<Ly; iy++){
            int pos = get1D(ix, iy);
            File << ix << '\t' << iy << '\t' << coffee[pos]  << '\n';
            }
        File << '\n';
    }
    File << std::endl;
    File.close();
}