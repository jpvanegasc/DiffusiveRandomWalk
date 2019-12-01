#include<iostream>
#include<cmath> 

#include"Random64.hpp"

const int Lx = 100;
const int Ly = 100;

class RWLattice{
    private:
        int coffee[Lx][Ly];
    public:
        void cream(void){
            coffee[0][0] = 1;
        }
}