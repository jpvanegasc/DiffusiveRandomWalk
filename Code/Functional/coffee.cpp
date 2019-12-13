#include<iostream>
#include<cmath>
#include <random>

//----- Global Constants ----- |
const double p = 0.5;
const int Nmol = 400;
const int v = 1; //Number of steps that a particle move each step.
const int L = 200;
const int T = 1000000;

//----- Function Declarations ----- |
void random_step(int *Mol, int i, double r1, double r2);
void draw(int *balls);
void initial(int *mol);

//----- MAIN ----- |
int main(void)
{
    // Init random generator
    int seed = 1; //Random seed 
    std::mt19937 gen(seed);
    std::uniform_real_distribution<> dis(0, 1);
    std::uniform_int_distribution<> dis1(0, Nmol-1);

    // Initialize
    int *Mol= new int[Nmol * 2]; // A list with x and y position of all molecules
    for(int i = 0; i < 2 * Nmol; ++i) {
        Mol[i] = 0; 
    }
    initial(Mol);
    for(int t=0; t < T; ++t){
    // Select random molecule
    int r0 = dis1(gen);
    // Update selected molecule
    // ---Select if moving in x or y
    double r1 = dis(gen);
    double r2 = dis(gen);
    random_step(Mol, r0 , r1, r2);
    }
    
    draw(Mol);

    delete [] Mol;

    return 0;
}

//----- FUNCTIONS ----- |
void random_step(int *Mol, int i, double r1, double r2)
{
    /*
     * Performs a random step.
     */ 

    if (p > r1){ // Moves in x
        if (p > r2){ // Moves to right
            Mol[i*2]++;
            if (Mol[i*2] == (L/2)) Mol[i*2] = -L/2 + 1;
        } else { // Moves to left
            Mol[i*2]--;
            if (Mol[i*2] == -(L/2)) Mol[i*2] = L/2 - 1;
        }
    } else { // Moves in y
        if (p > r2){ // Moves up
            Mol[i*2 + 1]++;
            if (Mol[i*2 + 1] == (L/2)) Mol[i*2 + 1] = -L/2 + 1;
        } else { // Moves to down
            Mol[i*2 + 1]--;
            if (Mol[i*2 + 1] == -(L/2)) Mol[i*2 + 1] = L/2 - 1;
        }
    }
    
}
void draw(int *balls)
{
    // Init lattice
    int Lattice[L][L];
    for (int ii = 0; ii < L; ++ii)
        for (int jj = 0; jj < L; ++jj)
            Lattice[ii][jj] = 0;
    
    // Insert balls
    for(int i = 0; i < Nmol; ++i) {
        Lattice[balls[i*2] + L/2][balls[i*2 + 1] + L/2]++;
    }

    // Output in pm3d map format
    for (int ii = 0; ii < L; ++ii){
        for (int jj = 0; jj < L; ++jj)
            std::cout << ii - L/2 << "\t" << jj - L/2 << "\t" <<  Lattice[ii][jj] << std::endl;
        std::cout << std::endl;
    }

}
void initial(int *mol)
{
    int ri, xi, yi, i = 0;
    ri = 20; 
    xi = - ri/2; yi = - ri/2;
    for(int x = xi; x < (xi + ri); x++)
        for(int y = yi; y < (yi + ri); y++){
            mol[2*i] = x;   mol[2*i + 1] = y;
            i++;  
        }

}