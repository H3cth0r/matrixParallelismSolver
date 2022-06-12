#include <iostream>
#include <omp.h>
#include <chrono>
#include <ctime>

// Defining constants
#define N 10000          // Matrix size
#define THREADS 20      // number of threads


int a[N];
int B[N][N];            // Creating matrixes with custom size
int c[N];               // Second matrix with custom size





// Method for initiali<zing the matrix
void initializeMatrices();
void prodmv();
void prodmv_parallel();





int main(){

    // Considere speedup = T1/TP
    // Where T1 : sequential time
    // TP : time using multiple processors
    // Sequential execution time start
    auto start_time_sequential = std::chrono::high_resolution_clock::now();
    prodmv();
    // Sequential execution time end
    auto end_time_sequential = std::chrono::high_resolution_clock::now();
    // Total sequential time
    auto T1 = std::chrono::duration_cast<std::chrono::microseconds>(end_time_sequential - start_time_sequential);
    std::cout << "Sequential time: " << T1.count() << " microseconds\n";

    // Parallel execution
    auto start_time_parallel = std::chrono::high_resolution_clock::now();
    prodmv_parallel();
    auto end_time_parralel = std::chrono::high_resolution_clock::now();
    auto TP = std::chrono::duration_cast<std::chrono::microseconds>(end_time_parralel - start_time_parallel);
    std::cout << "Sequential time: " << TP.count() << " microseconds\n";

    auto speedup = T1.count()/TP.count();
    std::cout << "Speedup: " << speedup << "\n";

    return 0;

}

void initializeMatrices(){
    std::cout << "Initilizing matrices ...\n";

        // initializing matrix a
        #pragma omp parallel for shared(a) schedule(static) num_threads(THREADS)
        for(int i = 0; i <= N; i++){ 
            a[i] = 1;
        }
        

        #pragma omp parallel for shared(B) schedule(static) num_threads(THREADS)
        for(int i = 0; i <= N; i++){
            for (int j = 0; j <= N; j++){ 
                B[i][j] = i + j;
            }
        }
        
        
        #pragma omp parallel for shared(c) schedule(static) num_threads(THREADS)
        for(int i = 0; i <= N; i++){ 
            c[i] = i; 
        }
    
    std::cout << "End initialization\n";
}

// // Sequential implementation of the problem
void prodmv(){
    int i, j;
    double sum;
    for(i = 0; i < N; i++){
        sum = 0;
        for(j = 0; j < N; j++){
            sum += B[i][j] * c[j];
        }
        a[i] = sum;
    }
}

void prodmv_parallel(){

    #pragma omp parallel for shared(a, B, c) schedule(static) num_threads(THREADS)
    for(int i = 0; i < N; i++){
        double sum = 0;
        for(int j = 0; j < N; j++){
            sum += B[i][j] * c[j];
        }
        a[i] = sum;
    }
}