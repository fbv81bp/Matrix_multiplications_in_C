#include <iostream>
#include <chrono>
using namespace std::chrono;

int main()
{
    //matrix size parameters
    const int m = 2;
    const int n = 3;
    const int k = 4;
    
    int A[m][n];
    int B[n][k];
    int Bt[k][n];
    int C1[m][k];
    int C2[m][k];
    int C3[m][k];
    int C4[m][k];
    int i = 0;
    int j = 0;
    int l = 0;

    for (i = 0; i < m; i++) for (int j = 0; j < n; j++) A[i][j] = i + j + 1;
    for (i = 0; i < n; i++) for (int j = 0; j < k; j++) B[i][j] = (i - j) * 3;
    for (i = 0; i < n; i++) for (int j = 0; j < k; j++) Bt[j][i] = B[i][j];
    for (i = 0; i < m; i++) for (int j = 0; j < k; j++) C1[i][j] = 0;
    for (i = 0; i < m; i++) for (int j = 0; j < k; j++) C2[i][j] = 0;
    for (i = 0; i < m; i++) for (int j = 0; j < k; j++) C4[i][j] = 0;

    std::chrono::steady_clock::time_point begin1 = std::chrono::steady_clock::now();

    //trivial
    for (i = 0; i < m; i++)
        for (j = 0; j < k; j++)
            for (l = 0; l < n; l++)
                C1[i][j] += A[i][l] * B[l][j];

    std::chrono::steady_clock::time_point begin2 = std::chrono::steady_clock::now();

    //improving cache hits on B
    for (i = 0; i < m; i++)
        for (l = 0; l < n; l++)
            for (j = 0; j < k; j++)
                C2[i][j] += A[i][l] * B[l][j];

    std::chrono::steady_clock::time_point begin3 = std::chrono::steady_clock::now();
    
    //using multiply accumlate unit
    int accu = 0;
    for (i = 0; i < m; i++)
        for (j = 0; j < k; j++)
        {
            accu = 0;
            for (l = 0; l < n; l++)
                accu += A[i][l] * B[l][j];
            C3[i][j] = accu;
        }

    std::chrono::steady_clock::time_point begin4 = std::chrono::steady_clock::now();
    
    //using both previous solutions by transposing B to Bt
    for (i = 0; i < m; i++)
        for (j = 0; j < k; j++)
        {
            accu = 0;
            for (l = 0; l < n; l++)
                accu += A[i][l] * B[l][j];
            C4[i][j] = accu;
        }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    bool equals;

    equals = true;
    for (i = 0; i < m; i++)
        for (int j = 0; j < k; j++)
            if (C1[i][j] != C2[i][j]) equals = false;
    std::cout << "\n";
    std::cout << "C1 equals C2: " << equals;

    equals = true;
    for (i = 0; i < m; i++)
        for (int j = 0; j < k; j++)
            if (C1[i][j] != C3[i][j]) equals = false;
    std::cout << "\n";
    std::cout << "C1 equals C3: " << equals;

    equals = true;
    for (i = 0; i < m; i++)
        for (int j = 0; j < k; j++)
            if (C1[i][j] != C4[i][j]) equals = false;
    std::cout << "\n";
    std::cout << "C1 equals C4: " << equals << "\n";

    std::cout << "Time difference C1 = " << std::chrono::duration_cast<std::chrono::microseconds>(begin1 - begin2).count() << "[us]" << std::endl;
    std::cout << "Time difference C2 = " << std::chrono::duration_cast<std::chrono::microseconds>(begin2 - begin3).count() << "[us]" << std::endl;
    std::cout << "Time difference C3 = " << std::chrono::duration_cast<std::chrono::microseconds>(begin3 - begin4).count() << "[us]" << std::endl;
    std::cout << "Time difference C4 = " << std::chrono::duration_cast<std::chrono::microseconds>(begin4 - end).count() << "[us]" << std::endl;

}
