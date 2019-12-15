#include<iostream>
#include<string>
#include<fstream>

#include"CA_RandomWalk.hpp"

int main(void){
    RWLattice CoffeeMug;
    std::ofstream File("S_vs_t.txt");
    int t_max = 1e4;

    CoffeeMug.initialize();
    
    for (int t=0; t<t_max; t++){
        File << t*400 << '\t' << CoffeeMug.entropy() << '\n';
        CoffeeMug.propagate();
        //if(CoffeeMug.check() == false) return 1;
    }
    
    File << std::endl; File.close();
    CoffeeMug.save("coffee.txt");
    
    return 0;
}