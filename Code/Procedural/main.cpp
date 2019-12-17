#include<iostream>
#include<cmath>
#include<fstream>
#include <random>

#include "Functions.hpp"

int main(void)
{
    // Init random generator
    int seed = 1; //Random seed 
    std::mt19937 gen(seed);
    std::uniform_real_distribution<> dis(0, 1);
    std::uniform_int_distribution<> dis1(0, Nmol-1);

    //Init files
    std::ofstream Entropy("entropy.txt");

    // Initialize
    int *Mol= new int[Nmol * 2]; // A list with x and y position of all molecules
    for(int i = 0; i < 2 * Nmol; ++i) {
        Mol[i] = 0; 
    }
    initialDrop(Mol, 20);
    for(int t=0; t < T; ++t){
    // Select random molecule
    int r0 = dis1(gen);
    // Update selected molecule
    // ---Select if moving in x or y
    double r1 = dis(gen);
    double r2 = dis(gen);
    random_step(Mol, r0 , r1, r2);
    Entropy << t << "\t"<< entropy(Mol) << "\n";
    }
    //draw(Mol);
    Entropy.close();

    delete [] Mol;

    return 0;
}
