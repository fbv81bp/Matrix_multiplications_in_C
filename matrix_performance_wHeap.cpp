#include <iostream>
#include <chrono>
using namespace std::chrono;

int main()
{
    //matrix size parameters
    const int m = 500;
    const int n = 600;
    const int k = 700;

    int *A = new int[m*n];
    int *B = new int[n*k];
    int *Bt = new int[k*n];
    int *C1 = new int[m*k];
    int *C2 = new int[m*k];
    int *C3 = new int[m*k];
    int *C4 = new int[m*k];
    int i = 0;
    int j = 0;
    int l = 0;

    std::cout << "creating matrices\n";
    for (i = 0; i < m; i++) for (int j = 0; j < n; j++) A[i * n + j] = i + j + 1;
    for (i = 0; i < n; i++) for (int j = 0; j < k; j++) B[i * k + j] = (i - j) * 3;
    for (i = 0; i < n; i++) for (int j = 0; j < k; j++) Bt[j * n + i] = B[i * k + j];
    for (i = 0; i < m; i++) for (int j = 0; j < k; j++) C1[i * k + j] = 0;
    for (i = 0; i < m; i++) for (int j = 0; j < k; j++) C2[i * k + j] = 0;
    for (i = 0; i < m; i++) for (int j = 0; j < k; j++) C3[i * k + j] = 0;
    for (i = 0; i < m; i++) for (int j = 0; j < k; j++) C4[i * k + j] = 0;

    std::chrono::steady_clock::time_point begin1 = std::chrono::steady_clock::now();

    std::cout << "trivial\n";
    for (i = 0; i < m; i++)
        for (j = 0; j < k; j++)
            for (l = 0; l < n; l++)
                C1[i * k + j] += A[i * n + l] * B[l * k + j];

    std::chrono::steady_clock::time_point begin2 = std::chrono::steady_clock::now();

    std::cout << "improving cache hits on B\n";
    for (i = 0; i < m; i++)
        for (l = 0; l < n; l++)
            for (j = 0; j < k; j++)
                C2[i * k + j] += A[i * n + l] * B[l * k + j];

    std::chrono::steady_clock::time_point begin3 = std::chrono::steady_clock::now();
    
    std::cout << "using multiply accumlate unit\n";
    int accu = 0;
    for (i = 0; i < m; i++)
        for (j = 0; j < k; j++)
        {
            accu = 0;
            for (l = 0; l < n; l++)
                accu += A[i * n + l] * B[l * k + j];
            C3[i * k + j] = accu;
        }

    std::chrono::steady_clock::time_point begin4 = std::chrono::steady_clock::now();
    
    std::cout << "using both previous solutions by transposing B to Bt\n";
    for (i = 0; i < m; i++)
        for (j = 0; j < k; j++)
        {
            accu = 0;
            for (l = 0; l < n; l++)
                accu += A[i * n + l] * Bt[j * n + l];
            C4[i * k + j] = accu;
        }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    bool equals;

    equals = true;
    for (i = 0; i < m; i++)
        for (int j = 0; j < k; j++)
            if (C1[i * k + j] != C2[i * k + j]) equals = false;
    std::cout << "\n";
    std::cout << "C1 equals C2: " << equals;

    equals = true;
    for (i = 0; i < m; i++)
        for (int j = 0; j < k; j++)
            if (C1[i * k + j] != C3[i * k + j]) equals = false;
    std::cout << "\n";
    std::cout << "C1 equals C3: " << equals;

    equals = true;
    for (i = 0; i < m; i++)
        for (int j = 0; j < k; j++)
            if (C1[i * k + j] != C4[i * k + j]) equals = false;
    std::cout << "\n";
    std::cout << "C1 equals C4: " << equals << "\n";

    std::cout << "Time difference with heap C1 = " << std::chrono::duration_cast<std::chrono::milliseconds>(begin2 - begin1).count() << "[ms]" << std::endl;
    std::cout << "Time difference with heap C2 = " << std::chrono::duration_cast<std::chrono::milliseconds>(begin3 - begin2).count() << "[ms]" << std::endl;
    std::cout << "Time difference with heap C3 = " << std::chrono::duration_cast<std::chrono::milliseconds>(begin4 - begin3).count() << "[ms]" << std::endl;
    std::cout << "Time difference with heap C4 = " << std::chrono::duration_cast<std::chrono::milliseconds>(end    - begin4).count() << "[ms]" << std::endl;

    //for (i = 0; i < m; i++) {
    //    std::cout << "\n";
    //    for (int j = 0; j < k; j++) std::cout << C1[i * k + j] << " ";
    //}

    std::cin.get();
}
