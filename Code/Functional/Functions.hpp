#include<iostream>
#include<cmath>
#include<fstream>
#include<string>

#include "Constants.hpp"

void random_step(int *Mol, int i, double r1, double r2)
{
    /*
     * Performs a random step.
     */ 

    if (p > r1){ // Moves in x
        if (p > r2){ // Moves to right
            if (Mol[i*2] < (L/2)-1) Mol[i*2]++;
        } else { // Moves to left
            if (Mol[i*2] > -(L/2)+1) Mol[i*2]--;
        }
    } else { // Moves in y
        if (p > r2){ // Moves up
            if (Mol[i*2 + 1] < (L/2)-1) Mol[i*2 + 1]++;
        } else { // Moves to down
            if (Mol[i*2 + 1] > -(L/2)+1) Mol[i*2 + 1]--;
        }
    }
    
}
void draw(int *balls, int t)
{
    //Init file
    std::string time= std::to_string(t);
    std::string filename = "drawing_t_" + time + ".txt";
    std::ofstream File(filename);

    // Init lattice
    int Lattice[L][L];
    for (int ii = 0; ii < L; ++ii)
        for (int jj = 0; jj < L; ++jj)
            Lattice[ii][jj] = 0;
    
    // Insert balls
    for(int i = 0; i < Nmol; ++i) {
        Lattice[balls[i*2] + L/2][-balls[i*2 + 1] + L/2]++;
    }

    // Output in splot format
    for (int ii = 0; ii < L; ++ii){
        for (int jj = 0; jj < L; ++jj)
            File << ii - L/2 << "\t" << jj - L/2 << "\t" <<  Lattice[ii][jj] << std::endl;
        File << std::endl;
    }

}
void initialDrop(int *mol, int size)
{
    int xi, yi, i = 0;
    size = 20; 
    xi = - size/2; yi = - size/2;
    for(int x = xi; x < (xi + size); x++)
        for(int y = yi; y < (yi + size); y++){
            mol[2*i] = x;   mol[2*i + 1] = y;
            i++;  
        }
}
double entropy(int *mol){

    //Init Grid
    int Ngrid = 8;
    int Grid[Ngrid][Ngrid];
    for (int ii = 0; ii < Ngrid; ++ii)
        for (int jj = 0; jj < Ngrid; ++jj)
            Grid[ii][jj] = 0;

    //Count balls
    int GridSize = L/Ngrid; 
    for(int i = 0; i < Nmol; ++i) {
        Grid[(mol[i*2] + L/2)/GridSize][(-mol[i*2+1] + L/2)/GridSize]++;
    }

    // Calculate P and S
    double P;
    double S= 0;
    for (int ii = 0; ii < Ngrid; ++ii){
        for (int jj = 0; jj < Ngrid; ++jj){
            P = Grid[ii][jj]*1.0/Nmol;
            if(P != 0)
                S += -P*std::log(P);
        }
    }

    return S;
}

double DropSize(int *mol)
{
    double size=0;
    for (int i=0; i <Nmol; ++i){
        double r2= (mol[2*i]*mol[2*i]) + (mol[2*i + 1]*mol[2*i + 1]);
        size += r2;
    }
    size = std::sqrt(size*1.0/Nmol);
    

    return size;
}