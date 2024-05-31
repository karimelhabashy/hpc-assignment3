#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int **generate2DArray(int r, int c){

    int **_2DArray = (int **)malloc(r * sizeof(int *));

    for (int i = 0; i < r; i++)
    {
        _2DArray[i] = (int *)malloc(c * sizeof(int));

        for (int j = 0; j < c; j++)
        {
            _2DArray[i][j] = rand() % 8;
        }
    }

    return _2DArray;
}
int *searchKey(int **_2DArray, int rows, int cols, int key) {
    int *indices = (int *)malloc(rows * cols * sizeof(int));
    for (int i = 0; i < rows * cols; i++) {
        indices[i] = -1;
    }
    int r = 0;

#pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (_2DArray[i][j] == key) {
                int index = omp_get_thread_num() * 2 + r * 2;
                indices[index] = i;
                indices[index + 1] = j;
                printf("Process %d found key %d at index (%d, %d)\n", omp_get_thread_num(), key, i, j);
#pragma omp atomic
                r++;
            }
        }
    }

    return indices;
}



int main() {
    int rows, cols, key;

    printf("Enter the number of rows and columns: ");
    scanf("%d %d", &rows, &cols);
    printf("Enter the key to search for: ");
    scanf("%d", &key);

    srand(time(NULL));
    int **_2DArray = generate2DArray(rows, cols);
    int *result = searchKey(_2DArray, rows, cols, key);


    for(int l = 0; l < rows; l++){
        for (int k = 0; k < cols; k++){
            printf("%d, ", _2DArray[l][k]);
        }
        printf("\n");
    }


    for(int i = 0; i < rows*cols; i+=2){
        if(result[i] == -1){
            continue;
        }
        int j = i + 1;
        printf("Key found at index %d,%d.\n", result[i], result[j]);
    }



    for (int i = 0; i < rows; i++) {
        free(_2DArray[i]);
    }
    free(_2DArray);
    free(result);

    return 0;
}
