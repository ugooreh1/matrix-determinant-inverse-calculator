        /*
    Name: Ugochukwu Oreh
    Student Number: 250158337
    Module: ELE118 Introduction to Systems Engineering and Software
    Assignment: C Programming Assignment - Matrices
    Reference: SVKG (2022) C Program for Gauss Elimination Method. Available at:
https://svkg.in/gauss-elimination-method/
(Accessed: 19 March 2026).
    */

    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>

    /* Maximum matrix size allowed */
    #define MAX_SIZE 10

    /* Small value used when checking if numbers are effectively zero */
    #define EPSILON 0.000001f


    /* Structure used to represent a matrix */
    struct matrix
    {
        float mValues[MAX_SIZE][MAX_SIZE];  /* matrix values */
        int nrows;                          /* number of rows */
        int ncols;                          /* number of columns */
    };


    /* Function prototypes */
    void printHeader(void);
    int getValidatedInteger(const char prompt[], int min, int max);
    void setMatrixSize(struct matrix *m);
    int loadMatrixFromFile(struct matrix *m, const char filename[]);
    void displayMatrix(struct matrix m);
    int isSquareMatrix(struct matrix m);
    void copyMatrix(struct matrix source, struct matrix *destination);
    void makeIdentityMatrix(struct matrix *m, int size);
    float determinantMatrix(struct matrix m, int *singularFlag);
    int inverseMatrix(struct matrix m, struct matrix *inverse);
    int askToRunAgain(void);


    /* Prints program title and instructions */
    void printHeader(void)
    {
        printf(" MATRIX PROGRAM IN C\n");
        printf(" Determinant and Inverse Calculator\n");
        printf("Matrix values will be read from matrix1.txt\n");
        printf("Allowed matrix size: 1 x 1 up to 10 x 10\n\n");
    }


    /*
    Function that ensures the user enters a valid integer.
    It keeps asking until a valid number within the specified range is entered.
    */
    int getValidatedInteger(const char prompt[], int min, int max)
    {
        int value;
        int result;

        result = 0;

        while (result == 0)
        {
            printf("%s", prompt);

            /* Check if the input is actually an integer */
            if (scanf("%d", &value) != 1)
            {
                printf("Invalid input. Please enter an integer.\n");

                /* Clear invalid characters from input buffer */
                while (getchar() != '\n')
                {
                }
            }

            /* Check if the integer is within the allowed range */
            else if (value < min || value > max)
            {
                printf("Value must be between %d and %d.\n", min, max);
            }

            else
            {
                result = 1;
            }
        }

        return value;
    }


    /*
    Asks the user for matrix dimensions (rows and columns)
    and stores them in the matrix structure.
    */
    void setMatrixSize(struct matrix *m)
    {
        m->nrows = getValidatedInteger("Enter number of rows (1 to 10): ", 1, 10);
        m->ncols = getValidatedInteger("Enter number of columns (1 to 10): ", 1, 10);
    }


    /*
    Loads matrix values from a text file into the matrix structure.
    Returns 1 if successful, 0 if there was an error.
    */
    int loadMatrixFromFile(struct matrix *m, const char filename[])
    {
        FILE *fp;
        int i;
        int j;

        /* Attempt to open the file */
        fp = fopen(filename, "r");

        if (fp == NULL)
        {
            printf("Error: could not open file %s\n", filename);
            return 0;
        }

        /* Read matrix values row by row */
        for (i = 0; i < m->nrows; i++)
        {
            for (j = 0; j < m->ncols; j++)
            {
                if (fscanf(fp, "%f", &m->mValues[i][j]) != 1)
                {
                    printf("Error: file does not contain enough valid numbers.\n");
                    fclose(fp);
                    return 0;
                }
            }
        }

        fclose(fp);
        return 1;
    }


    /* Displays the matrix in a neat table format */
    void displayMatrix(struct matrix m)
    {
        int i;
        int j;

        printf("\nMatrix (%d x %d):\n", m.nrows, m.ncols);

        for (i = 0; i < m.nrows; i++)
        {
            for (j = 0; j < m.ncols; j++)
            {
                printf("%10.3f ", m.mValues[i][j]);
            }
            printf("\n");
        }
    }


    /* Returns 1 if the matrix is square, otherwise returns 0 */
    int isSquareMatrix(struct matrix m)
    {
        if (m.nrows == m.ncols)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }


    /* Copies one matrix into another matrix */
    void copyMatrix(struct matrix source, struct matrix *destination)
    {
        int i;
        int j;

        destination->nrows = source.nrows;
        destination->ncols = source.ncols;

        for (i = 0; i < source.nrows; i++)
        {
            for (j = 0; j < source.ncols; j++)
            {
                destination->mValues[i][j] = source.mValues[i][j];
            }
        }
    }


    /* Creates an identity matrix of a given size */
    void makeIdentityMatrix(struct matrix *m, int size)
    {
        int i;
        int j;

        m->nrows = size;
        m->ncols = size;

        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                if (i == j)
                {
                    m->mValues[i][j] = 1.0f;
                }
                else
                {
                    m->mValues[i][j] = 0.0f;
                }
            }
        }
    }


    /*
    Calculates the determinant using Gaussian elimination.
    Returns the determinant value and sets singularFlag if matrix is singular.
    */
    float determinantMatrix(struct matrix m, int *singularFlag)
    {
        int i, j, k;
        int pivotRow;
        int n;
        float pivot, factor;
        float determinant;
        float temp;
        int swapCount;

        n = m.nrows;
        *singularFlag = 0;
        swapCount = 0;

        /* Determinant only defined for square matrices */
        if (n != m.ncols)
        {
            *singularFlag = 1;
            return 0.0f;
        }

        /* Perform Gaussian elimination */
        for (i = 0; i < n; i++)
        {
            pivotRow = i;

            while (pivotRow < n && fabs(m.mValues[pivotRow][i]) < EPSILON)
            {
                pivotRow++;
            }

            if (pivotRow == n)
            {
                *singularFlag = 1;
                return 0.0f;
            }

            /* Swap rows if necessary */
            if (pivotRow != i)
            {
                for (j = 0; j < n; j++)
                {
                    temp = m.mValues[i][j];
                    m.mValues[i][j] = m.mValues[pivotRow][j];
                    m.mValues[pivotRow][j] = temp;
                }
                swapCount++;
            }

            pivot = m.mValues[i][i];

            /* Eliminate values below the pivot */
            for (j = i + 1; j < n; j++)
            {
                factor = m.mValues[j][i] / pivot;

                for (k = i; k < n; k++)
                {
                    m.mValues[j][k] -= factor * m.mValues[i][k];
                }
            }
        }

        determinant = 1.0f;

        /* Multiply diagonal values */
        for (i = 0; i < n; i++)
        {
            determinant *= m.mValues[i][i];
        }

        /* Adjust sign if rows were swapped */
        if (swapCount % 2 != 0)
        {
            determinant = -determinant;
        }

        if (fabs(determinant) < EPSILON)
        {
            *singularFlag = 1;
            return 0.0f;
        }

        return determinant;
    }


    /*
    Calculates the inverse using Gauss-Jordan elimination.
    Returns 1 if inverse exists, otherwise 0.
    */
    int inverseMatrix(struct matrix m, struct matrix *inverse)
    {
        int i, j, k;
        int n;
        int pivotRow;
        float pivot, factor, temp;
        struct matrix workingMatrix;

        if (m.nrows != m.ncols)
        {
            return 0;
        }

        n = m.nrows;

        copyMatrix(m, &workingMatrix);
        makeIdentityMatrix(inverse, n);

        for (i = 0; i < n; i++)
        {
            pivotRow = i;

            while (pivotRow < n && fabs(workingMatrix.mValues[pivotRow][i]) < EPSILON)
            {
                pivotRow++;
            }

            if (pivotRow == n)
            {
                return 0;
            }

            /* Swap rows if needed */
            if (pivotRow != i)
            {
                for (j = 0; j < n; j++)
                {
                    temp = workingMatrix.mValues[i][j];
                    workingMatrix.mValues[i][j] = workingMatrix.mValues[pivotRow][j];
                    workingMatrix.mValues[pivotRow][j] = temp;

                    temp = inverse->mValues[i][j];
                    inverse->mValues[i][j] = inverse->mValues[pivotRow][j];
                    inverse->mValues[pivotRow][j] = temp;
                }
            }

            pivot = workingMatrix.mValues[i][i];

            if (fabs(pivot) < EPSILON)
            {
                return 0;
            }

            /* Normalize pivot row */
            for (j = 0; j < n; j++)
            {
                workingMatrix.mValues[i][j] /= pivot;
                inverse->mValues[i][j] /= pivot;
            }

            /* Eliminate other rows */
            for (j = 0; j < n; j++)
            {
                if (j != i)
                {
                    factor = workingMatrix.mValues[j][i];

                    for (k = 0; k < n; k++)
                    {
                        workingMatrix.mValues[j][k] -= factor * workingMatrix.mValues[i][k];
                        inverse->mValues[j][k] -= factor * inverse->mValues[i][k];
                    }
                }
            }
        }

        return 1;
    }


    /* Asks user if they want to run the program again */
    int askToRunAgain(void)
    {
        int choice;

        choice = getValidatedInteger("\nDo you want to run again? (1 = yes, 0 = no): ", 0, 1);

        return choice;
    }


    /* Main program controlling execution */
    int main(void)
    {
        struct matrix originalMatrix;
        struct matrix inverseResult;
        int runAgain;
        int singularFlag;
        float determinant;

        printHeader();

        runAgain = 1;

        while (runAgain == 1)
        {
            setMatrixSize(&originalMatrix);

            if (loadMatrixFromFile(&originalMatrix, "matrix1.txt") == 1)
            {
                printf("\nMatrix loaded successfully.\n");
                displayMatrix(originalMatrix);

                if (isSquareMatrix(originalMatrix) == 1)
                {
                    singularFlag = 0;
                    determinant = determinantMatrix(originalMatrix, &singularFlag);

                    if (singularFlag == 0)
                    {
                        printf("\nDeterminant = %.3f\n", determinant);

                        if (inverseMatrix(originalMatrix, &inverseResult) == 1)
                        {
                            printf("\nInverse matrix:\n");
                            displayMatrix(inverseResult);
                        }
                        else
                        {
                            printf("\nThis matrix does not have an inverse.\n");
                        }
                    }
                    else
                    {
                        printf("\nDeterminant = 0.000\n");
                        printf("This matrix is singular, so it has no inverse.\n");
                    }
                }
                else
                {
                    printf("\nThis is a non-square matrix.\n");
                    printf("Determinant and inverse are not defined.\n");
                }
            }
            else
            {
                printf("\nMatrix could not be loaded from the file.\n");
            }

            runAgain = askToRunAgain();
        }

        printf("\nProgram ended.\n");

        return 0;
    }
