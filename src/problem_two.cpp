#include <iostream>
#include <omp.h>
#include <chrono>

// Defining constants
#define N 11000          // Matrix size
#define M 11000
#define THREADS 20      // number of threads

double A[M][N];
double AA[M][N];
double pres = 0;

void initializeMatrices();
double funcion();
void parallelProduct();

int main(){
    
    initializeMatrices();
    double res;
    auto start_time_sequential = std::chrono::high_resolution_clock::now();
    res = funcion();
    auto end_time_sequential = std::chrono::high_resolution_clock::now();
    auto T1 = std::chrono::duration_cast<std::chrono::microseconds>(end_time_sequential - start_time_sequential);
    std::cout << "Sequential time: " << T1.count() << " microseconds\n";
    std::cout << "sequential result: " << res << "\n";



    auto start_time_parallel = std::chrono::high_resolution_clock::now();
    parallelProduct();
    auto end_time_parralel = std::chrono::high_resolution_clock::now();
    auto TP = std::chrono::duration_cast<std::chrono::microseconds>(end_time_parralel - start_time_parallel);
    std::cout << "Parallel time: " << TP.count() << " microseconds\n";
    std::cout << "parallel result: " << pres << "\n"; 

    auto speedup = T1.count()/TP.count();
    std::cout << "Speedup: " << speedup << "\n";

    return 0;
}

void initializeMatrices(){
    std::cout << "Initilizing matrices ...\n";

        #pragma omp parallel for shared(A, AA) schedule(static) num_threads(THREADS)
        for(int i = 0; i < M; i++){
            for (int j = 0; j < N; j++){ 
                A[i][j]     = i + j;
                AA[i][j]    = i + j;
            }
        }
        
    std::cout << "End initialization\n";
}

double funcion()
{
    double suma;
    for(int i = 0; i < M-1; i++){
        for(int j = 0; j < N; j++){
            A[i][j] = 2.0 * A[i + 1][j];
        }
    }

    suma = 0;
    
    for(int i = 0; i < M; i++){
       for(int j = 0; j < N; j++){
           suma = suma + A[i][j];
       }
    }
    
    return suma;
}

void parallelProduct(){
    double suma = 0;
    #pragma omp parallel for shared(AA, suma) schedule(static) num_threads(THREADS)
        # pragma omp
        for(int i = 0; i < M-1; i++){
            for(int j = 0; j < N; j++){
                AA[i][j] = 2.0 * AA[i + 1][j];
            }
        }

        #pragma omp
        for(int i = 0; i < M; i++){
            for(int j = 0; j < N; j++){
                pres = pres + AA[i][j];                
            }
        }

}