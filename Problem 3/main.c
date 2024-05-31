#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "time.h"

int main() {
    int n, numberOfProcesses = 8, *data;

    double   globalSumSqDiff = 0.0, globalSum = 0.0;
    double globalMean = 0.0, stddev = 0.0, variance = 0.0;

    printf("Enter the number of elements per process (n): ");
    scanf("%d", &n);

    data = (int*)malloc(n * numberOfProcesses * sizeof(int));



    #pragma omp parallel
    {
        int start =  omp_get_thread_num() * n;
        int end = start + n;
        double localSum = 0.0;

        srand(omp_get_thread_num()*time(NULL));
        for (int i = start; i < end; i++) {
            data[i] = rand() % 100;
        }

        for (int i = start; i < end; i++)
        {
            localSum += data[i];
        }

        globalSum += localSum;
    }

    globalMean = globalSum / (n * numberOfProcesses);

    for (int i = 0; i < n * numberOfProcesses; i++){
        printf("%d, " , data[i]);
    }
    printf("\n");

    #pragma omp parallel
    {
        int start = omp_get_thread_num() * n;
        int end = start + n;
        double localSumSqDiff = 0.0;

        for (int i = start; i < end; i++)
        {
            localSumSqDiff += (data[i] - globalMean)*(data[i] - globalMean);
        }

        globalSumSqDiff += localSumSqDiff;
    }

    variance = globalSumSqDiff/(n*numberOfProcesses);

    stddev = sqrt(variance);
    printf("Standard deviation: %.6f\n", stddev);
    free(data);
    return 0;
}
