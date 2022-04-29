#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <time.h>
#include <random>
#include <iomanip> // i/o manipulator

using std::cin;
using std::cout;
using std::endl;
using std::ofstream;
using std::pair;
using std::setw; // sets field width for next item of output
using std::string;
using std::vector;

typedef vector<vector<int>> vec2D;
// mod function for periodic boundary conditions
#define mod(val, mod_val) ((val % mod_val) + mod_val) % mod_val

// SUBROUTINE DECLARATIONS //
// MONTE CARLO SUBROUTINES
void metropolisAlgo(vec2D &lattice, int n, int iterations, float Temp, float &energy,
                    int J, vector<float> &net_spin_avgs, vector<float> &net_energies, float H);
float avg(vector<float> net_arr, int N);
float stdev(vector<float> arr, float mean);
// LATTICE SUBROUTINES
int sum(vec2D &lattice, int n); // sum all spins in lattice
void randomizeLattice(vec2D &lattice, int n);
void displayLattice(vec2D &lattice, int n);
void storeLattice(vec2D &lattice, int n, string fileName);
float getInit_LatticeEnergy(vec2D &lattice, int n, float J);

int main()
{
    // Default Initial Vars
    int n = 30;    // n x n grid size
    int N = n * n; // number of spins in lattice
    float J = 7;   // interaction strength
    float H = 0;   // external magnetic field

    // BEGIN TEMPERATURE SWEEPING
    ofstream F("../Data/MvsT100.dat");
    ofstream F2("../Data/EvsT.dat");
    ofstream F3("../Data/CvsT.dat");
    ofstream F4("../Data/logMvslogT.dat");
    int sweeps = 20000;
    float Tc = 2 * J / (log(1 + sqrt(2)));
    float dT = 0.1;
    float T_init = 12;
    float T_final = 20;
    cout << "Tc = " << Tc << endl;
    for (float T = T_init; T < T_final; T += dT)
    {
        // initialize lattice
        vec2D spin_lattice(n, vector<int>(n, 1));
        // store final few MC sweeps values in respective array
        vector<float> net_spin_avgs;
        vector<float> net_energies;
        // init energy of alligned lattice eliminating double counting
        float energy = -2 * N * J;
        net_energies.push_back(energy); // add initial energy
        // RUN THE METROPOLIS ALGORITHM - FILLS UP ARRAYS ABOVE
        metropolisAlgo(spin_lattice, n, sweeps, T, energy, J, net_spin_avgs, net_energies, H);
        // Calculate average M from Stored Values
        float M = avg(net_spin_avgs, N);
        float Eavg = avg(net_energies, N);
        float logM = log(M);
        float E_stdev = stdev(net_energies, Eavg);
        float C = (pow(E_stdev, 2)) / (pow(T, 2) * N);
        // store values
        F << T << " " << M << endl;
        F2 << T << " " << Eavg << endl;
        F3 << T << " " << C << endl;
        F4 << log(Tc - T) << " " << logM << endl;
        cout << "T = " << T << "/" << T_final << endl;
    }

    //// PART C - MAGNETIC FIELD ////
    float dH = 0.5;
    float H_final = 15;
    float T = Tc / 3.0;
    sweeps = 15000;

    ofstream f("../Data/MvsHSMALL.dat");
    for (H = H_final * -1; H <= H_final; H += dH)
    {
        vec2D spin_lattice(n, vector<int>(n, 1));
        vector<float> net_spin_avgs;
        vector<float> net_energies;
        float energy = -2 * N * J;      // init energy of alligned lattice eliminating double counting
        net_energies.push_back(energy); // add initial energy
        metropolisAlgo(spin_lattice, n, sweeps, T, energy, J, net_spin_avgs, net_energies, H);
        float M = avg(net_spin_avgs, N);
        f << H << " " << M << endl;
        cout << "H = " << H << "/" << H_final << endl;
    }

    T = 1.5 * Tc;
    ofstream f2("../Data/MvsHBIG.dat");
    for (H = H_final * -1; H <= H_final; H += dH)
    {
        vec2D spin_lattice(n, vector<int>(n, 1));
        vector<float> net_spin_avgs;
        vector<float> net_energies;
        float energy = -2 * N * J;      // init energy of alligned lattice eliminating double counting
        net_energies.push_back(energy); // add initial energy
        metropolisAlgo(spin_lattice, n, sweeps, T, energy, J, net_spin_avgs, net_energies, H);
        float M = avg(net_spin_avgs, N);
        f2 << H << " " << M << endl;
        cout << "H = " << H << "/" << H_final << endl;
    }

    return 0;
}

// SUBROUTINE DEFINITIONS //
void metropolisAlgo(vec2D &lattice, int n, int sweeps, float T, float &energy, int J,
                    vector<float> &net_spin_avgs, vector<float> &net_energies, float H)
{
    // Random Uniform Number generation routine for probabilities
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> distr(0, 1);

    // calculation for a single RANDOM spin
    for (int t = 1; t <= sweeps; t++)
    { // systematically iterate through the lattice
        int latticeSum = 0;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                int currSpin = lattice[i][j]; // current spin
                int flipSpin = -1 * currSpin; // opposite of chosen spin
                int left = lattice[mod(i - 1, n)][j];
                int right = lattice[mod(i + 1, n)][j];
                int top = lattice[i][mod(j + 1, n)];
                int bottom = lattice[i][mod(j - 1, n)];
                // SUM OF NEIGHBORS
                int sumNeigh = left + right + top + bottom;
                // ENERGY BEFORE OR AFTER A FLIP
                float E_i = -J * (currSpin * sumNeigh) - (H * currSpin);
                float E_f = -J * (flipSpin * sumNeigh) - (H * flipSpin);
                // CHANGE OF ENERGY IF SPIN FLIPPED
                float dE = (E_f - E_i);
                //    Probabilities!
                float randNum = distr(eng);
                float Bfactor = exp(-1 * dE / T);
                if ((dE > 0) && (randNum < Bfactor))
                {
                    lattice[i][j] = flipSpin;
                    energy += dE;
                }
                else if (dE <= 0) // if change in energy is negative... Always flip
                {
                    lattice[i][j] = flipSpin;
                    energy += dE;
                }
                latticeSum += lattice[i][j]; // Keep Track of Lattice Sum
            }
        }
        // RELAX CONDITION -  only store during later equilibrized sweeps
        if (t >= 0.85 * sweeps) // Add only final 15% of sweeps
        {
            float spinAvg = latticeSum / (float)(n * n);
            float E = energy / 2;
            net_spin_avgs.push_back(spinAvg); // add  up all the spins
            net_energies.push_back(E);        // divide by 2 to account for double counting
        }
    }
}

float getInit_LatticeEnergy(vec2D &lattice, int n, float J)
{
    float finalEnergy = 0;
    int max = n;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int currSpin = lattice[i][j];
            int left = lattice[mod(i - 1, max)][j];
            int right = lattice[mod(i + 1, max)][j];
            int top = lattice[i][mod(j + 1, max)];
            int bottom = lattice[i][mod(j - 1, max)];

            float sum = left + right + top + bottom;
            finalEnergy += currSpin * sum;
        }
    }
    finalEnergy *= -J; // divide by two to account for double counting

    return finalEnergy;
}

float avg(vector<float> net_arr, int N)
{
    float avg_Equil = 0;
    float sum = 0;
    int size = net_arr.size();
    for (int i = 0; i < size; i++)
    {
        sum += net_arr[i];
    }
    avg_Equil = sum / size;
    return avg_Equil;
}

int sum(vec2D &lattice, int n)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            sum += lattice[i][j];
        }
    }
    return sum;
}

float stdev(vector<float> arr, float mean)
{
    float standard_dev = 0;
    int size = arr.size();
    // calculate standard dev using mean
    for (int i = 0; i < size - 1; i++)
    {
        standard_dev += pow((arr[i] - mean), 2);
    }
    standard_dev /= size;
    standard_dev = sqrt(standard_dev);
    return standard_dev;
}

void randomizeLattice(vec2D &lattice, int n)
{
    // NOTE // - random number is called by using distr(eng()) which passes the
    // random number engine into the distribution and returns the random number.
    std::random_device rd;                             // non-deterministic uniform random bit generator
    std::default_random_engine eng(rd());              // use generator to SEED, engine wont repeat number sequences
    std::uniform_real_distribution<float> distr(0, 1); // initialize uniform distribution of numbers between 0 and 1

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            float randNum = distr(eng);
            if (randNum >= 0.75)
            {
                // flip the sign from + to -
                lattice[i][j] *= -1;
            }
            // else do nothing - spin stays positive
        }
    }
}

void storeLattice(vec2D &lattice, int n, string fileName)
{
    ofstream f(fileName);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            f << lattice[i][j] << " ";
        }
        f << endl;
    }
}

void displayLattice(vec2D &lattice, int n)
{
    int width = 3;
    int factor = width * 50;
    string border(factor, '-'); // constructor arg1 - factor multiplicationg for string at arg2
    cout << border << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << setw(width) << lattice[i][j] << " ";
        }
        cout << endl;
    }
    cout << border << endl;
}
