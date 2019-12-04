#include<iostream>
#include<cmath>
#include<fstream>
#include<string>

const int Lx = 200, Ly = 200;
const int D = 2, Q = 9;

const int size = Lx*Ly*Q;
const int x_mult = Ly*Q;
const int y_mult = Q;

const double tau = 0.55, Utau = 1.0/tau, UmUtau = 1-Utau;

class LatticeBoltzmann{
    private:
        double w[Q]; int V[D][Q];
        double *f = NULL,   *f_new = NULL;
    public:
        LatticeBoltzmann(void);
        ~LatticeBoltzmann(void);
        int get_1D(int ix, int iy);
        // density
        double rho(int ix, int iy);
        // U_x * rho
        double Jx(int ix, int iy);
        // U_y * rho
        double Jy(int ix, int iy);
        // Using f_new
        double Jx_new(int ix, int iy);
        // Using f_new
        double Jy_new(int ix, int iy);
        // eq function for fluids
        double f_eq(double rho0, double Ux0, double Uy0, int i);
        void collide(void);
        void propagate(void);
        void initialize(double rho0, double Ux0, double Uy0);
        void impose_fields(double v);
        void save(std::string filename, double v);
        void print(double v);
};

LatticeBoltzmann::LatticeBoltzmann(void){
    // weights
    w[0] = 4.0/9.0;
    w[1] = w[2] = w[3] = w[4] = 1.0/9.0;
    w[5] = w[6] = w[7] = w[8] = 1.0/36.0;
    // basis vectors
    V[0][0]=0; //x
    V[1][0]=0; //y

    V[0][1]=1;   V[0][2]=0;  V[0][3]=-1;  V[0][4]=0;   V[0][5]=1;   V[0][6]=-1;
    V[1][1]=0;   V[1][2]=1;  V[1][3]=0;   V[1][4]=-1;  V[1][5]=1;   V[1][6]=1;

    V[0][7]=-1;   V[0][8]=1;
    V[1][7]=-1;   V[1][8]=-1;
    // f and f_new
    f = new double[size];
    f_new = new double[size];
}

LatticeBoltzmann::~LatticeBoltzmann(void){
    delete[] f; delete[] f_new;
}

int LatticeBoltzmann::get_1D(int ix, int iy){
    return ix*x_mult + iy*y_mult;
}

double LatticeBoltzmann::rho(int ix, int iy){
    double rho = 0; int pos = get_1D(ix, iy);
    for(int i=0; i<Q; i++){
        rho += f[pos + i];
    }
    return rho;
}

double LatticeBoltzmann::Jx(int ix, int iy){
    double J_x = 0; int pos = get_1D(ix, iy);
    for(int i=0; i<Q; i++){
        J_x += f[pos + i] * V[0][i];
    }
    return J_x;
}

double LatticeBoltzmann::Jy(int ix, int iy){
    double J_y = 0; int pos = get_1D(ix, iy);
    for(int j=0; j<Q; j++){
        J_y += f[pos+j] * V[1][j];
    }
    return J_y;
}

double LatticeBoltzmann::Jx_new(int ix, int iy){
    double J_x = 0; int pos = get_1D(ix, iy);
    for(int i=0; i<Q; i++){
        J_x += f_new[pos + i] * V[0][i];
    }
    return J_x;
}

double LatticeBoltzmann::Jy_new(int ix, int iy){
    double J_y = 0; int pos = get_1D(ix, iy);
    for(int j=0; j<Q; j++){
        J_y += f_new[pos+j] * V[1][j];
    }
    return J_y;
}

double LatticeBoltzmann::f_eq(double rho0, double Ux0, double Uy0, int i){
    double UdotVi = Ux0*V[0][i] + Uy0*V[1][i];
    double U2 = Ux0*Ux0 + Uy0*Uy0;
    return rho0*w[i]*(1 + 3*UdotVi + 4.5*UdotVi*UdotVi - 1.5*U2);
}

void LatticeBoltzmann::collide(void){
    double rho0, Ux0, Uy0; int pos;
    for(int ix=0; ix<Lx; ix++)
        #pragma omp parallel for private(pos, rho0, Ux0, Uy0)
        for(int iy=0; iy<Ly; iy++){
                rho0 = rho(ix, iy);
                Ux0 = Jx(ix, iy)/rho0; Uy0 = Jy(ix, iy)/rho0;
                pos = get_1D(ix, iy);
                for(int i=0; i<Q; i++)
                    f_new[pos + i] = UmUtau*f[pos + i] + Utau*f_eq(rho0, Ux0, Uy0, i);
            }
}

void LatticeBoltzmann::propagate(void){
    for(int ix=0; ix<Lx; ix++)
        for(int iy=0; iy<Ly; iy++){
                int pos_new = get_1D(ix, iy);
                for(int i=0; i<Q; i++){ // Non periodic, non optimized
                    int x_pos = (ix + V[0][i]), y_pos = (iy + V[1][i]);
                    if(x_pos<0 || x_pos>=Lx) continue;
                    if(y_pos<0 || y_pos>=Ly) continue;
                    int pos = get_1D(x_pos, y_pos);
                    f[pos + i] = f_new[pos_new + i];
                }
            }
}

void LatticeBoltzmann::initialize(double rho0, double Ux0, double Uy0){
    for(int ix=0; ix<Lx; ix++)
        for(int iy=0; iy<Ly; iy++){
                int pos = get_1D(ix, iy);
                for(int i=0; i<Q; i++) f[pos + i] = f_eq(rho0, Ux0, Uy0, i);
            }
}

void LatticeBoltzmann::impose_fields(double v){
    int pos; double rho0;
    #pragma omp parallel for private(pos, rho0)
    for(int ix=0; ix<Lx; ix++)
        for(int iy=0; iy<Ly; iy++){
                // Wind tunnel
                if(ix==0){
                    pos = get_1D(ix, iy);
                    rho0 = rho(ix, iy);
                    for(int i=0; i<Q; i++) f_new[pos + i] = f_eq(rho0, v/2.0, 0, i);
                    continue;
                }
            }
}

void LatticeBoltzmann::save(std::string filename, double v){
    if(v == 0.0) std::cout << "v = 0" << std::endl;
    std::ofstream File(filename); double rho0, Ux0, Uy0;
    for(int ix=0; ix<Lx; ix+=4)
        for(int iy=0; iy<Ly; iy+=4){
                rho0 = rho(ix, iy);
                Ux0 = Jx(ix, iy)/rho0; Uy0 = Jy(ix, iy)/rho0;
                File << "something" << '\n';
            }
    File << std::endl;
    File.close();
}

void LatticeBoltzmann::print(double v){
    if(v == 0.0) std::cout << "v = 0" << std::endl;
    double rho0, Ux0, Uy0;
    for(int ix=0; ix<Lx; ix+=4)
        for(int iy=0; iy<Ly; iy+=4){
                rho0 = rho(ix, iy);
                Ux0 = Jx(ix, iy)/rho0; Uy0 = Jy(ix, iy)/rho0;
                std::cout << "magic" << '\n';
            }
    std::cout << std::endl;
}