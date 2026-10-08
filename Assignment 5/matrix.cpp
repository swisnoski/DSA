/* 
Sam Wisnoski 

Assignment 5 - Divide and Conquer and Dynamic Programming

10/6/2026

*/

#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>
#include <cmath>



/* 
######### PART 1: MATRIX CLASS ##########

For the first part of the assignment, we will be learning about a divide-and-conquer algorithm for matrix multiplication called Strassen’s Algorithm. 

To get started, create a C++ class capable of storing square matrices. I won’t be overly prescriptive about how you write your class, but 
you should support basic operations like creating a matrix of size n n, setting / getting values at specified row and column indices, dividing 
the matrix into four n / 2 × n / 2 n/2×n/2 matrices (this is needed for Strassen). Don’t worry about matrix multiplication yet, that’s coming!

So - first, we need to store data, which we can do with a vector of vectors 

This makes indexing really easy, because we can just index the row first, and then the column of the vectors.
Same to set a new value, we can just index twice and pass in the new value. 

To divide a matrix into four smaller matrixes, (and assuming that our n dimension is even) can do something along the lines of: 
* split the list of vectors in half, which gives us a top (n/2 x n) matrix and a bottom (n/2 x n) matrix 
* iterate through each of those halves and take half of each list and add it as a vector to a new vector 
* return all four matrices 

And then we can worry about matrix multiplication for part 2
*/

template <typename MatrixDataType>
class Matrix{
    private:
    // we can initialize the vector to store our vectors to store our data 
    std::vector<std::vector<MatrixDataType>> entireMatrix;
    int matrixSize;


    public:
    // in order to actually set elements properly, we need to have a way to auto 
    // set a size n for the matrix. vectors have a helpful resize function 
    // that just expands everything to zero

    // so we can use a constructor to set the size of the matrix, 
    // which means we have to pass in a size value upon initialization of the matrix 
    Matrix(int size) {
        matrixSize = size;
        entireMatrix.resize(size);

        for (int i = 0; i < size; i++) {
            entireMatrix[i].resize(size);
        }
    }

    void setCell(int column, int row, MatrixDataType newData){
        entireMatrix[row][column] = newData;
    }

    // it's convenient for spliting if we have the functionality to 
    // set entire rows of data at once 
    void setRow(int rowIndex, std::vector<MatrixDataType> entireRow){
        entireMatrix[rowIndex] = entireRow;
    }

    MatrixDataType getCell(int column, int row){
        return entireMatrix[row][column];
    }

    // we also want to be able to see the size from outside the class, mostly so we can
    // tell when multiply returns NULL (which ends up as a 0x0 matrix)
    int getSize(){
        return matrixSize;
    }

    std::vector<Matrix<MatrixDataType>> splitMatrix() {
        // so we can initialize four matrices 
        // we will iterate through each row of the matrix and split it into a left and right half. 
        // then, if the row is in the top half of the matrix, we will place it in the top matrices, 
        // and if it's in the bottom, we will instead place the row in the bottom matrices 
        Matrix<MatrixDataType> matrixTopRight(matrixSize/2);
        Matrix<MatrixDataType> matrixTopLeft(matrixSize/2);
        Matrix<MatrixDataType> matrixBottomRight(matrixSize/2);
        Matrix<MatrixDataType> matrixBottomLeft(matrixSize/2);

        // also I want to use "left half/right half" variables to make this easier to read 
        std::vector<MatrixDataType> leftHalf;
        std::vector<MatrixDataType> rightHalf; 

        for(int rowIndex = 0; rowIndex < matrixSize; rowIndex++){
            // get the current row and split it into two halves  
            
            leftHalf = std::vector<MatrixDataType>(entireMatrix[rowIndex].begin(), entireMatrix[rowIndex].begin() + matrixSize / 2);
            rightHalf = std::vector<MatrixDataType>(entireMatrix[rowIndex].begin() + matrixSize / 2, entireMatrix[rowIndex].end());

            // then we check if we need to place in the bottom or top 
            if(rowIndex < (entireMatrix.size()/2)){
                // place in top 
                matrixTopLeft.setRow(rowIndex, leftHalf);
                matrixTopRight.setRow(rowIndex, rightHalf); 
            }
            else{
                matrixBottomLeft.setRow(rowIndex - matrixSize/2, leftHalf);
                matrixBottomRight.setRow(rowIndex - matrixSize/2, rightHalf);
            }
        }

        // and finally, we want to return all of our Matrices 
        std::vector<Matrix<MatrixDataType>> splitMatrixVector = {matrixTopLeft, matrixTopRight, matrixBottomLeft, matrixBottomRight};
        return splitMatrixVector;
    }

    // ############# PART TWO ###############
    
    // okay so part two will also be added to this matrix class, which is where we implement 
    // basic multiplication and strassenMultiply. Then, we will implement an optimal version 
    // of this algorithm that combines BOTH multiple and the strassen algorithm. 

    // so for basic matrix multiplication, we are going to implement block-based matrix multiplication, 
    // since i think it will make implementing strassens easier if we do a recursive matrix multiplication 
    
    // also in order to do block multiplication we need to be able to add matricies together 
    Matrix<MatrixDataType> add(Matrix<MatrixDataType> otherMatrix){
        Matrix<MatrixDataType> matrixSum(matrixSize);
        // there is probably a better way to do this, but for now it's easiest to 
        // just iterate through and add them all together 
        for(int rowIndex = 0; rowIndex < matrixSize; rowIndex++){
            for(int columnIndex = 0; columnIndex < matrixSize; columnIndex++){
                matrixSum.setCell(columnIndex, rowIndex, getCell(columnIndex, rowIndex) + otherMatrix.getCell(columnIndex, rowIndex));
            }
        }
        return matrixSum;
    }

    /*
    Multiply [this] matrix by [other].
    You can implement this either using block-based matrix multiplication or
    traditional matrix multiplication (the kind you learn about in math classes!)
    @return [this]*[other] if the dimensions are compatible and null otherwise

    Block based multiplication, I choose you! 
    */
    Matrix<MatrixDataType> multiply(Matrix<MatrixDataType> otherMatrix){
        // let's declare our matrix C 
        // entireMatrix is matrix A, otherMatrix is matrix B, matrixProduct is matrix C
        Matrix<MatrixDataType> matrixProduct(matrixSize); 

        if(matrixSize == 0){
            return NULL;
        }

        // Matrices must be the same size
        if(matrixSize != otherMatrix.matrixSize){
            return NULL;
        }

        // Oh! And of course we need our base cases! That's how we know to stop recursion! 
        if(matrixSize == 1){
            Matrix<MatrixDataType> matrixProduct(1);
            matrixProduct.setCell(0,0,getCell(0, 0)*otherMatrix.getCell(0,0));
            return matrixProduct;
        }

        // okay so this actually took me a while to figure out, but basically in order to check if
        // a number is a power of 2, we can check if k where k = log2(n) is a whole number. 
        // so we find the log2(n) = k and round k to the nearest whole number. Then we can do "1 << exponent", 
        // which essentially is the same as checking if 2^k is equal to n, our matrix size. If log2(n) = k was already a 
        // whole number, then rounding won't change anything and 2^k should equal n. If log2(n) = k was NOT a whole number,
        // then rounding it will cause 2^k to no longer be equal to n. 
        int exponent = std::round(std::log2(matrixSize));
        if((1 << exponent) != matrixSize){
            return NULL;
        }

        // so to do block based matrix multiplication, we split matrix A and B into smaller matricies, 
        // then do standard matrix multiplication with the smaller matricies. 
        std::vector<Matrix<MatrixDataType>> thisMatrixVector = splitMatrix();
        std::vector<Matrix<MatrixDataType>> otherMatrixVector = otherMatrix.splitMatrix();
        // Vector 1 = <A11, A12, A21, A22>, Vector 2 = <B11, B12, B21, B22> 

        Matrix<MatrixDataType> A11 = thisMatrixVector[0];
        Matrix<MatrixDataType> A12 = thisMatrixVector[1];
        Matrix<MatrixDataType> A21 = thisMatrixVector[2];
        Matrix<MatrixDataType> A22 = thisMatrixVector[3];

        Matrix<MatrixDataType> B11 = otherMatrixVector[0];
        Matrix<MatrixDataType> B12 = otherMatrixVector[1];
        Matrix<MatrixDataType> B21 = otherMatrixVector[2];
        Matrix<MatrixDataType> B22 = otherMatrixVector[3];

        // so now we have our matrix vectors, we simply multiply them back to get our matrix Product 
        // C11 = A11B11+A12B21, C12 = A11B12+A12B22, C21 = A21B11+A22B21, C22 = A21B12+A22B22

        // we already have our recursive multiply and we just added an add function, so we can 
        // just calculate each part of the C matrix with our built in matrix operations 
        // C11 = A11B11 + A12B21
        Matrix<MatrixDataType> C11 = A11.multiply(B11).add(A12.multiply(B21));
        // C12 = A11B12 + A12B22
        Matrix<MatrixDataType> C12 = A11.multiply(B12).add(A12.multiply(B22));
        // C21 = A21B11 + A22B21
        Matrix<MatrixDataType> C21 = A21.multiply(B11).add(A22.multiply(B21));
        // C22 = A21B12 + A22B22
        Matrix<MatrixDataType> C22 = A21.multiply(B12).add(A22.multiply(B22));

        // and now we have all four C matricies, so we just need to combine them into one final matrix and return it! 

        int halfSize = matrixSize / 2;

        for(int rowIndex = 0; rowIndex < halfSize; rowIndex++){
            for(int columnIndex = 0; columnIndex < halfSize; columnIndex++){
                // we can loop through all four matricies at once and just copy the data over from each! 
                matrixProduct.setCell(columnIndex, rowIndex, C11.getCell(columnIndex, rowIndex));
                matrixProduct.setCell(columnIndex + halfSize, rowIndex, C12.getCell(columnIndex, rowIndex));
                matrixProduct.setCell(columnIndex, rowIndex + halfSize, C21.getCell(columnIndex, rowIndex));
                matrixProduct.setCell(columnIndex + halfSize, rowIndex + halfSize, C22.getCell(columnIndex, rowIndex));
            }
        }


        return matrixProduct;
    }

    // okay, and now we can move on to strassenMultiply, which is basically the same thing, except that 
    // we have a slightly different method of rebuilding the C matrix, using 7 operations instead of 8 
    
    // those operations also now include subtraction, yay! we can just reuse our add class but subtract instead 
    Matrix<MatrixDataType> subtract(Matrix<MatrixDataType> otherMatrix){
        Matrix<MatrixDataType> matrixSum(matrixSize);
        // there is probably a better way to do this, but for now it's easiest to 
        // just iterate through and add them all together 
        for(int rowIndex = 0; rowIndex < matrixSize; rowIndex++){
            for(int columnIndex = 0; columnIndex < matrixSize; columnIndex++){
                matrixSum.setCell(columnIndex, rowIndex, getCell(columnIndex, rowIndex) - otherMatrix.getCell(columnIndex, rowIndex));
            }
        }
        return matrixSum;
    }


    /*
    Multiply [this] matrix by [other].
    Your code should use Strassen's algorithm
    @return [this]*[other] if the dimensions are compatible and null otherwise
    */
    Matrix<MatrixDataType> strassenMultiply(Matrix<MatrixDataType> otherMatrix){
        // our strassen multiply is basically the exact same. 
        // we recurse down using strassenMultiplication by breaking the matricies into four 
        // matricies and then recombining them using the formula

        // let's declare our matrix C 
        // entireMatrix is matrix A, otherMatrix is matrix B, matrixProduct is matrix C
        Matrix<MatrixDataType> matrixProduct(matrixSize); 

        // Matrices must be the same size
        if(matrixSize != otherMatrix.matrixSize){
            return NULL;
        }

        // Oh! And of course we need our base cases! That's how we know to stop recursion! 
        if(matrixSize == 1){
            Matrix<MatrixDataType> matrixProduct(1);
            matrixProduct.setCell(0,0,getCell(0, 0)*otherMatrix.getCell(0,0));
            return matrixProduct;
        }

        // Matrix size must be a power of 2, we can just check if the current matrix is 
        // divisible by two recursively 
        int exponent = std::round(std::log2(matrixSize));
        if((1 << exponent) != matrixSize){
            return NULL;
        }

        // we do the same thing, assigning our vectors into four quadrants 
        std::vector<Matrix<MatrixDataType>> thisMatrixVector = splitMatrix();
        std::vector<Matrix<MatrixDataType>> otherMatrixVector = otherMatrix.splitMatrix();
        // Vector 1 = <A11, A12, A21, A22>, Vector 2 = <B11, B12, B21, B22> 

        Matrix<MatrixDataType> A11 = thisMatrixVector[0];
        Matrix<MatrixDataType> A12 = thisMatrixVector[1];
        Matrix<MatrixDataType> A21 = thisMatrixVector[2];
        Matrix<MatrixDataType> A22 = thisMatrixVector[3];

        Matrix<MatrixDataType> B11 = otherMatrixVector[0];
        Matrix<MatrixDataType> B12 = otherMatrixVector[1];
        Matrix<MatrixDataType> B21 = otherMatrixVector[2];
        Matrix<MatrixDataType> B22 = otherMatrixVector[3];

        // but then instead, we calculate seven M matricies using our strassenMultiplication 
        // M1 = (A11 + A22) * (B11 + B22)
        Matrix<MatrixDataType> M1 = (A11.add(A22)).strassenMultiply(B11.add(B22));
        // M2 = (A21 + A22) * B11
        Matrix<MatrixDataType> M2 = (A21.add(A22)).strassenMultiply(B11);
        // M3 = A11 * (B12 - B22)
        Matrix<MatrixDataType> M3 = A11.strassenMultiply(B12.subtract(B22));
        // M4 = A22 * (B21 - B11)
        Matrix<MatrixDataType> M4 = A22.strassenMultiply(B21.subtract(B11));
        // M5 = (A11 + A12) * B22
        Matrix<MatrixDataType> M5 = (A11.add(A12)).strassenMultiply(B22);
        // M6 = (A21 - A11) * (B11 + B12)
        Matrix<MatrixDataType> M6 = (A21.subtract(A11)).strassenMultiply(B11.add(B12));
        // M7 = (A12 - A22) * (B21 + B22)
        Matrix<MatrixDataType> M7 = (A12.subtract(A22)).strassenMultiply(B21.add(B22));

        // so now instead we have seven M matricies, which we then funnel into our four c quadrants according to
        // the following formulas. I have no idea why this works
        // C11 = M1 + M4 - M5 + M7
        Matrix<MatrixDataType> C11 = ((M1.add(M4)).subtract(M5)).add(M7);
        // C12 = M3 + M5
        Matrix<MatrixDataType> C12 = M3.add(M5);
        // C21 = M2 + M4
        Matrix<MatrixDataType> C21 = M2.add(M4);
        // C22 = M1 - M2 + M3 + M6
        Matrix<MatrixDataType> C22 = ((M1.subtract(M2)).add(M3)).add(M6);


        // and then once we have the C quads, we again use the same strat to iterate through and 
        // combine all the matricies together into a single product 
        int halfSize = matrixSize / 2;

        for(int rowIndex = 0; rowIndex < halfSize; rowIndex++){
            for(int columnIndex = 0; columnIndex < halfSize; columnIndex++){
                // we can loop through all four matricies at once and just copy the data over from each! 
                matrixProduct.setCell(columnIndex, rowIndex, C11.getCell(columnIndex, rowIndex));
                matrixProduct.setCell(columnIndex + halfSize, rowIndex, C12.getCell(columnIndex, rowIndex));
                matrixProduct.setCell(columnIndex, rowIndex + halfSize, C21.getCell(columnIndex, rowIndex));
                matrixProduct.setCell(columnIndex + halfSize, rowIndex + halfSize, C22.getCell(columnIndex, rowIndex));
            }
        }

        return matrixProduct;
    }
};


// and then we can test! 

// in order to test easily, we needed a better, easier way to declare and set matricies.
// we already have a function to set rows, so let's build a function where we can pass in a
// vector of vectors and instantly declare a matrix. In all honestly, 
// this should probably be part of the constructor, but whatever, I don't want to work 
// on the matrix class anymore  
template <typename MatrixDataType>
Matrix<MatrixDataType> makeMatrix(std::vector<std::vector<MatrixDataType>> rows) {
    Matrix<MatrixDataType> newMatrix(rows.size());
    for (int rowIndex = 0; rowIndex < rows.size(); rowIndex++) {
        newMatrix.setRow(rowIndex, rows[rowIndex]);
    }
    return newMatrix;
}

// and then now that we have a good way to declare matrix, we can run some useful tests! 
// to make this easier on myself, I'm splitting testing and benchmarking across a few different functions
// here. In this file, we will have two sets of unit tests, one that tests the basic functions 
// of the matrix class (more aligned with part 1) and one that tests multiplication (more aligned 
// with part 2). 

// Then, seperately, we will do benchmarking in a totally different file. 

void testMatrix() {
    std::cout << "\nMatrix Tests\n";

    // testing set cell and get cell 
    Matrix<int> cells(2);
    cells.setCell(1, 0, 7);  // row 0, column 1
    std::cout << "setCell(1, 0, 7) then getCell(1, 0) (expected 7): " << cells.getCell(1, 0) << "\n";

    // testing split matrix 
    Matrix<int> a2 = makeMatrix<int>({{1,2},{3,4}});
    std::vector<Matrix<int>> quads = a2.splitMatrix();
    std::cout << "split A11 (expected 1): " << quads[0].getCell(0, 0) << "\n";
    std::cout << "split A12 (expected 2): " << quads[1].getCell(0, 0) << "\n";
    std::cout << "split A21 (expected 3): " << quads[2].getCell(0, 0) << "\n";
    std::cout << "split A22 (expected 4): " << quads[3].getCell(0, 0) << "\n";

    // test addition and subtraction
    // [1 2] + [5 6] = [ 6  8]     [1 2] - [5 6] = [-4 -4]
    // [3 4]   [7 8]   [10 12]     [3 4]   [7 8]   [-4 -4]
    Matrix<int> b2 = makeMatrix<int>({{5,6},{7,8}});
    std::cout << "add row1 col1 (expected 12): " << a2.add(b2).getCell(1, 1) << "\n";
    std::cout << "subtract row0 col1 (expected -4): " << a2.subtract(b2).getCell(1, 0) << "\n";
}


// and then we also need to test out our multiplication functions 
void testMultiply() {
    std::cout << "\nMultiply + Strassen Tests\n";

    // 1x1 base case
    Matrix<int> a1 = makeMatrix<int>({{3}});
    Matrix<int> b1 = makeMatrix<int>({{4}});
    std::cout << "1x1 multiply 3*4 (expected 12): " << a1.multiply(b1).getCell(0, 0) << "\n";
    std::cout << "1x1 strassen 3*4 (expected 12): " << a1.strassenMultiply(b1).getCell(0, 0) << "\n";

    // 2x2 matrix multiplicaton 
    // [1 2] * [5 6] = [19 22]
    // [3 4]   [7 8]   [43 50]
    Matrix<int> a2 = makeMatrix<int>({{1,2},{3,4}});
    Matrix<int> b2 = makeMatrix<int>({{5,6},{7,8}});
    Matrix<int> m2 = a2.multiply(b2);
    Matrix<int> s2 = a2.strassenMultiply(b2);
    std::cout << "2x2 multiply row0 col1 (expected 22): " << m2.getCell(1, 0) << "\n";
    std::cout << "2x2 multiply row1 col0 (expected 43): " << m2.getCell(0, 1) << "\n";
    std::cout << "2x2 strassen row0 col1 (expected 22): " << s2.getCell(1, 0) << "\n";
    std::cout << "2x2 strassen row1 col0 (expected 43): " << s2.getCell(0, 1) << "\n";

    // 4x4 A*A (two levels of recursion)
    // row0 = [90 100 110 120], row3 = [426 484 542 600]
    Matrix<int> a4 = makeMatrix<int>({{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}});
    Matrix<int> m4 = a4.multiply(a4);
    Matrix<int> s4 = a4.strassenMultiply(a4);
    std::cout << "4x4 multiply row0 col3 (expected 120): " << m4.getCell(3, 0) << "\n";
    std::cout << "4x4 multiply row3 col0 (expected 426): " << m4.getCell(0, 3) << "\n";
    std::cout << "4x4 strassen row0 col3 (expected 120): " << s4.getCell(3, 0) << "\n";
    std::cout << "4x4 strassen row3 col0 (expected 426): " << s4.getCell(0, 3) << "\n";

    // testing when dimensions don't align
    std::cout << "2x2 * 4x4 multiply size (expected 0): " << a2.multiply(a4).getSize() << "\n";
    std::cout << "2x2 * 4x4 strassen size (expected 0): " << a2.strassenMultiply(a4).getSize() << "\n";

    // testing a matrix that is not a power of 2
    Matrix<int> a3 = makeMatrix<int>({{1,2,3},{4,5,6,},{7,8,9}});
    std::cout << "3x3 multiply size (expected 0): " << a3.multiply(a3).getSize() << "\n";
    std::cout << "3x3 strassen size (expected 0): " << a3.strassenMultiply(a3).getSize() << "\n";
}

// Yay! all our unit tests pass! now it's benchmarking time!! 

int main() {
    testMatrix();
    testMultiply();
    return 0;
}
