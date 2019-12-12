#include<iostream>
#include<cmath>
#include<string>
#include<fstream>

#include"Random64.hpp"

const int Lx = 200;
const int Ly = 200;
const int Nx = 20, Ny = 20;
const int N = Nx*Ny;

Crandom ran64(1);
const double p = 0.5;

const int cellsx = 8;
const int cellsy = 8;
const int step_x = Lx/cellsx;
const int step_y = Ly/cellsy;

class RWLattice{
    private:
        int *coffee = NULL;
    public:
        RWLattice(void);
        ~RWLattice();
        int get1D(int ix, int iy);
        void initialize(void);
        void propagate(void);
        bool check(void);
        double entropy(void);
        void save(std::string filename);
};
/* Allocate memory for lattice */
RWLattice::RWLattice(void){
    coffee = new int[Lx*Ly];
}
/* Free memory */
RWLattice::~RWLattice(){
    delete[] coffee;
}
/* Transfor from 2D to 1D index*/
int RWLattice::get1D(int ix, int iy){
    return ix*Ly + iy;
}
/* Initial conditions*/
void RWLattice::initialize(void){
    // All zeros
    #pragma omp parallel for
    for(int i=0; i<Lx*Ly; i++)
        coffee[i] = 0;
    // Place molecules in a rectangle centered on the lattice
    int x_start = Lx/2 - Nx/2, x_end = Lx/2 + Nx/2;
    int y_start = Ly/2 - Ny/2, y_end = Ly/2 + Ny/2;
    #pragma omp parallel for
    for(int ix=x_start; ix<x_end; ix++)
        for(int iy=y_start; iy<y_end; iy++){
            int pos = get1D(ix, iy);
            coffee[pos] = 1;
        }
    if(check() == false) throw "Bad initialization";
}
/**
 * One step of Random Walk for each molecule on the lattice. 
 * 
 * Two or more molecules are allowed on the same spot, boundaries are closed.
*/
void RWLattice::propagate(void){
    for(int ix=0; ix<Lx; ix++)
        for(int iy=0; iy<Ly; iy++){
            int pos = get1D(ix, iy);
            if(coffee[pos] == 0) continue;

            double r_1 = ran64.r(); double r_2 = ran64.r();
            if(p > r_1){ // Moves in x
                if(p > r_2 && ix != Lx-1) coffee[get1D(ix+1, iy)]++; // Right
                else if(ix != 0) coffee[get1D(ix-1, iy)]++; // Left
                else if(ix == 0 || ix == Lx-1) continue;
            }
            else{ // Moves in y
                if(p > r_2 && iy != Ly-1) coffee[get1D(ix, iy+1)]++; // Up
                else if(iy != 0) coffee[get1D(ix, iy-1)]++; // Down
                else if(iy == 0 || iy == Ly-1) continue;
            }
            coffee[pos]--;

        }
}
/**
 * Check that molecules don't "dissapear" from the lattice
 * @return true if everything is alright, false if a molecule is missing
*/
bool RWLattice::check(void){
    int sum = 0;
    #pragma opm parallel for reduction(+:sum)
    for(int i=0; i<Lx*Ly; i++)
        sum += coffee[i];
    
    if(sum == N) return true;
    else return false;
}
/* Calculates the Shannon Entropy for the lattice in a given state*/
double RWLattice::entropy(void){
    double S = 0, P = 1;
    for(int ix=0; ix<Lx; ix+=step_x)
        for(int iy=0; iy<Ly; iy+=step_y){
            S += P*std::log10(P);
    }
    return S;
}
/* Saves file in gnuplot splot format*/
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