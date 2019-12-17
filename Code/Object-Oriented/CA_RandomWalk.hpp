#include<iostream>
#include<cmath>
#include<string>
#include<fstream>

#include"Random64.hpp"
#include"Constants.hpp"

Crandom ran64(seed);

class RWLattice{
    private:
        int *coffee = nullptr;
    public:
        RWLattice(void);
        ~RWLattice();
        int get1D(int ix, int iy);
        void initialize(void);
        void propagate(void);
        void propagate_with_hole(void);
        int count(void);
        bool check(void);
        double entropy(void);
        double drop_size(void);
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
/* Transform from 2D to 1D index*/
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
            if(p >= r_1){ // Moves in x
                if(p >= r_2 && ix != Lx-1) coffee[get1D(ix+1, iy)]++; // Right
                else if(ix != 0) coffee[get1D(ix-1, iy)]++; // Left
                else if(ix == 0 || ix == Lx-1) continue;
            }
            else{ // Moves in y
                if(p >= r_2 && iy != Ly-1) coffee[get1D(ix, iy+1)]++; // Up
                else if(iy != 0) coffee[get1D(ix, iy-1)]++; // Down
                else if(iy == 0 || iy == Ly-1) continue;
            }
            coffee[pos]--;

        }
}
/**
 * One step of Random Walk for each molecule on the lattice, but there's a hole one one of 
 * the container walls, located centered on the vertical left wall.
 * 
 * Two or more molecules are allowed on the same spot, boundaries are closed (except for 
 * the hole, obviously).
*/
void RWLattice::propagate_with_hole(void){
    for(int ix=0; ix<Lx; ix++)
        for(int iy=0; iy<Ly; iy++){
            int pos = get1D(ix, iy);
            if(coffee[pos] == 0) continue;

            double r_1 = ran64.r(); double r_2 = ran64.r();
            if(p >= r_1){ // Moves in x
                // Hole
                if(iy >= hole_start && iy <= hole_end){
                    if(p >= r_2 && ix <= Lx-2) coffee[get1D(ix+1, iy)]++; // Right
                    else if(p < r_2 && ix != 0) coffee[get1D(ix-1, iy)]++; // Left
                    else if(ix == 0) continue;
                }
                else{
                    if(p >= r_2 && ix != Lx-1) coffee[get1D(ix+1, iy)]++; // Right
                    else if(ix != 0) coffee[get1D(ix-1, iy)]++; // Left
                    else if(ix == 0 || ix == Lx-1) continue;
                }
            }
            else{ // Moves in y
                if(p >= r_2 && iy != Ly-1) coffee[get1D(ix, iy+1)]++; // Up
                else if(iy != 0) coffee[get1D(ix, iy-1)]++; // Down
                else if(iy == 0 || iy == Ly-1) continue;
            }
            coffee[pos]--;

        }
}
/* Counts the number of molecules in the lattice */
int RWLattice::count(void){
    int sum = 0;
    #pragma opm parallel for reduction(+:sum)
    for(int i=0; i<Lx*Ly; i++)
        sum += coffee[i];
    
    return sum;
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
    double S = 0;
    /**
     * Calculates the probability for the system to be in state (ix,iy)
     * Examples: state (0,0) is the uppermost, leftmost cell 
     *           state (0,1) is the cell directly behind (0,0)
     *           state (1,0) is the cell directly to the right of (0,0)
     */
    auto P = [=](int cell_idx, int cell_idy)-> double {
        int start_x = cell_idx*step_x, start_y = cell_idy*step_y;
        int end_x = start_x + step_x, end_y = start_y + step_y;
        int sum = 0;
        #pragma omp parallel for reduction(+:sum)
        for(int ix=start_x; ix<end_x; ix++)
            for(int iy=start_y; iy<end_y; iy++){
                int pos = get1D(ix, iy);
                sum += coffee[pos];
            }
        if(sum == 0) return 1.0;
        else return sum/(1.0*N);
    };

    for(int ix=0; ix<cellsx; ix++)
        for(int iy=0; iy<cellsy; iy++){
            double P_i = P(ix, iy);
            S += P_i*std::log(P_i);
    }
    return -1.0*S;
}
/**
 * Calculates the size of the drop of cream using the root-mean-square distance of the 
 * molecules to the center of the lattice
 */
double RWLattice::drop_size(void){
    /* Calculates the distance from a molecule on cell (ix, iy) to the center of the lattice */
    double r_sum = 0;
    #pragma omp parallel for reduction(+:r_sum)
    for(int ix=0; ix<Lx; ix++)
        for(int iy=0; iy<Ly; iy++){
            int pos = get1D(ix, iy);
            if(coffee[pos] != 0) r_sum += std::sqrt((ix-x_half)*(ix-x_half) + (iy-y_half)*(iy-y_half));
        }
    
    return std::sqrt(r_sum/(1.0*N));
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