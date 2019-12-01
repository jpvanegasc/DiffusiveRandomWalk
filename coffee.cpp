#include<iostream>
#include<cmath>
#include<random>

//-----GLOBAL CONSTANTS-----
const int N = 200;
const int Nmol = 400; 

//-----FUNCTION DECLARATION-----
void initial(int * Mug);

//-----MAIN-----
int main(void)
{
    int *CoffeeMug = new int[N*N]; 
    for(int i = 0; i < N; ++i){
        for(int j = 0; j < N; ++j){
            CoffeeMug[i*N +j] = 0;
        }
    }
    initial(CoffeeMug);

    for(int i = 0; i < N; ++i){
        for(int j = 0; j < N; ++j){
            std::cout << i - N/2 << "\t" << j - N/2 << "\t" << CoffeeMug[i*N +j] << "\n";
        }
        std::cout << std::endl;
    }


    return 0;
}

//-----FUNCTIONS-----
void initial (int *Mug)
{
    int seed = 1;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(-10, 10);
    for(int n = 0; n < Nmol; ++n) {
        int xi= dis(gen) + N/2; int yi = dis(gen) + N/2;
        Mug[xi*N + yi] += 1;
    }
}