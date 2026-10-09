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
    // stores a square n x n matrix and all of our matrix operations
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
        // makes an n x n matrix filled with zeros
        matrixSize = size;
        entireMatrix.resize(size);

        for (int i = 0; i < size; i++) {
            entireMatrix[i].resize(size);
        }
    }

    void setCell(int column, int row, MatrixDataType newData){
        // sets the value at (column, row)
        entireMatrix[row][column] = newData;
    }

    // it's convenient for spliting if we have the functionality to 
    // set entire rows of data at once 
    void setRow(int rowIndex, std::vector<MatrixDataType> entireRow){
        // replaces a whole row at once
        entireMatrix[rowIndex] = entireRow;
    }

    MatrixDataType getCell(int column, int row){
        // returns the value at (column, row)
        return entireMatrix[row][column];
    }

    // we also want to be able to see the size from outside the class, mostly so we can
    // tell when multiply returns NULL (which ends up as a 0x0 matrix)
    int getSize(){
        // returns n, the size of the matrix
        return matrixSize;
    }

    std::vector<Matrix<MatrixDataType>> splitMatrix() {
        // splits the matrix into four quadrants, returned as <top left, top right, bottom left, bottom right>

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
        // returns this matrix + other matrix, cell by cell
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
    Matrix<MatrixDataType> blockMultiply(Matrix<MatrixDataType> otherMatrix){
        // recursive block multiplication, 8 sub-multiplies all the way down to 1x1

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
        Matrix<MatrixDataType> C11 = A11.blockMultiply(B11).add(A12.blockMultiply(B21));
        // C12 = A11B12 + A12B22
        Matrix<MatrixDataType> C12 = A11.blockMultiply(B12).add(A12.blockMultiply(B22));
        // C21 = A21B11 + A22B21
        Matrix<MatrixDataType> C21 = A21.blockMultiply(B11).add(A22.blockMultiply(B21));
        // C22 = A21B12 + A22B22
        Matrix<MatrixDataType> C22 = A21.blockMultiply(B12).add(A22.blockMultiply(B22));

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
        // returns this matrix - other matrix, cell by cell
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
        // recursive strassen multiplication, 7 sub-multiplies all the way down to 1x1

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

    // so both of the recursive algorithms I tested are really slow 
    Matrix<MatrixDataType> simpleMultiply(const Matrix<MatrixDataType>& otherMatrix) const {
        // regular triple loop multiplication, no recursion

        // need to filter to square matricies that match in dimensions 
        int exponent = std::round(std::log2(matrixSize));
        if((1 << exponent) != matrixSize){
            return NULL;
        }

        if(matrixSize != otherMatrix.matrixSize){
            return NULL;
        }

        Matrix<MatrixDataType> matrixProduct(matrixSize);

        // now, to multiply, we loop over size n three times for our n^3 computation time 

        // we first go through the rows of matrix A (this matrix) and grab a value
        for (int rowAIndex = 0; rowAIndex < matrixSize; rowAIndex++) {
            for (int columnAIndex = 0; columnAIndex < matrixSize; columnAIndex++) {
                MatrixDataType aValue = entireMatrix[rowAIndex][columnAIndex];
                // then, for every row in the product, we need to multiply this 
                // value by the corresponding row in matrix B (other matrix) and add 
                // that to the product matrix
                for (int columnBIndex = 0; columnBIndex < matrixSize; columnBIndex++) {
                    // basically, this has the effect of choosing one cell in A, multiplying it 
                    // by every column in B, and then adding it to C at the correct row (A) and column (B)
                    matrixProduct.entireMatrix[rowAIndex][columnBIndex] += aValue * otherMatrix.entireMatrix[columnAIndex][columnBIndex];
                }
            }
        }
        return matrixProduct;
    }

    // and finally, our hybrid! simple is super fast on small matricies, but strassen scales better
    // as the matricies get bigger. so we use strassen to split up the big matricies, and then once
    // they get small enough (matrixSize <= threshold), we switch over to simple multiplication
    Matrix<MatrixDataType> hybridMultiply(Matrix<MatrixDataType> otherMatrix, int threshold){
        // strassen until matrixSize <= threshold, then simple multiplication

        // Matrices must be the same size and a power of 2
        if(matrixSize != otherMatrix.matrixSize){
            return NULL;
        }
        int exponent = std::round(std::log2(matrixSize));
        if((1 << exponent) != matrixSize){
            return NULL;
        }

        // this is our new base case! instead of recursing all the way down to 1x1,
        // we stop at the threshold n=threshold and let simple multiplication take over
        if(matrixSize <= threshold){
            return simpleMultiply(otherMatrix);
        }

        // other wise we just do strassen multiplication 
        // let's declare our matrix C 
        Matrix<MatrixDataType> matrixProduct(matrixSize);

        // and everything else is exactly the same as strassen, we split into four quadrants
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

        // then calculate our seven M matricies, but recursing with hybridMultiply this time
        // M1 = (A11 + A22) * (B11 + B22)
        Matrix<MatrixDataType> M1 = (A11.add(A22)).hybridMultiply(B11.add(B22), threshold);
        // M2 = (A21 + A22) * B11
        Matrix<MatrixDataType> M2 = (A21.add(A22)).hybridMultiply(B11, threshold);
        // M3 = A11 * (B12 - B22)
        Matrix<MatrixDataType> M3 = A11.hybridMultiply(B12.subtract(B22), threshold);
        // M4 = A22 * (B21 - B11)
        Matrix<MatrixDataType> M4 = A22.hybridMultiply(B21.subtract(B11), threshold);
        // M5 = (A11 + A12) * B22
        Matrix<MatrixDataType> M5 = (A11.add(A12)).hybridMultiply(B22, threshold);
        // M6 = (A21 - A11) * (B11 + B12)
        Matrix<MatrixDataType> M6 = (A21.subtract(A11)).hybridMultiply(B11.add(B12), threshold);
        // M7 = (A12 - A22) * (B21 + B22)
        Matrix<MatrixDataType> M7 = (A12.subtract(A22)).hybridMultiply(B21.add(B22), threshold);

        // and funnel them into our four C quadrants with the same formulas
        // C11 = M1 + M4 - M5 + M7
        Matrix<MatrixDataType> C11 = ((M1.add(M4)).subtract(M5)).add(M7);
        // C12 = M3 + M5
        Matrix<MatrixDataType> C12 = M3.add(M5);
        // C21 = M2 + M4
        Matrix<MatrixDataType> C21 = M2.add(M4);
        // C22 = M1 - M2 + M3 + M6
        Matrix<MatrixDataType> C22 = ((M1.subtract(M2)).add(M3)).add(M6);

        // and then combine all four quads into a single product, same as before
        int halfSize = matrixSize / 2;

        for(int rowIndex = 0; rowIndex < halfSize; rowIndex++){
            for(int columnIndex = 0; columnIndex < halfSize; columnIndex++){
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
    // builds a matrix from a vector of rows
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
    // unit tests for the basic matrix functions (part 1)
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
    // unit tests for all four multiplication functions (part 2)
    std::cout << "\nMultiply + Strassen Tests\n";

    // 1x1 base case
    Matrix<int> a1 = makeMatrix<int>({{3}});
    Matrix<int> b1 = makeMatrix<int>({{4}});
    std::cout << "1x1 block 3*4 (expected 12): " << a1.blockMultiply(b1).getCell(0, 0) << "\n";
    std::cout << "1x1 strassen 3*4 (expected 12): " << a1.strassenMultiply(b1).getCell(0, 0) << "\n";
    std::cout << "1x1 simple 3*4 (expected 12): " << a1.simpleMultiply(b1).getCell(0, 0) << "\n";

    // 2x2 matrix multiplicaton 
    // [1 2] * [5 6] = [19 22]
    // [3 4]   [7 8]   [43 50]
    Matrix<int> a2 = makeMatrix<int>({{1,2},{3,4}});
    Matrix<int> b2 = makeMatrix<int>({{5,6},{7,8}});
    Matrix<int> m2 = a2.blockMultiply(b2);
    Matrix<int> st2 = a2.strassenMultiply(b2);
    Matrix<int> si2 = a2.simpleMultiply(b2);
    std::cout << "2x2 block row0 col1 (expected 22): " << m2.getCell(1, 0) << "\n";
    std::cout << "2x2 block row1 col0 (expected 43): " << m2.getCell(0, 1) << "\n";
    std::cout << "2x2 strassen row0 col1 (expected 22): " << st2.getCell(1, 0) << "\n";
    std::cout << "2x2 strassen row1 col0 (expected 43): " << st2.getCell(0, 1) << "\n";
    std::cout << "2x2 simple row0 col1 (expected 22): " << si2.getCell(1, 0) << "\n";
    std::cout << "2x2 simple row1 col0 (expected 43): " << si2.getCell(0, 1) << "\n";

    // 4x4 A*A (two levels of recursion)
    // row0 = [90 100 110 120], row3 = [426 484 542 600]
    Matrix<int> a4 = makeMatrix<int>({{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}});
    Matrix<int> m4 = a4.blockMultiply(a4);
    Matrix<int> st4 = a4.strassenMultiply(a4);
    Matrix<int> si4 = a4.simpleMultiply(a4);
    std::cout << "4x4 block row0 col3 (expected 120): " << m4.getCell(3, 0) << "\n";
    std::cout << "4x4 block row3 col0 (expected 426): " << m4.getCell(0, 3) << "\n";
    std::cout << "4x4 strassen row0 col3 (expected 120): " << st4.getCell(3, 0) << "\n";
    std::cout << "4x4 strassen row3 col0 (expected 426): " << st4.getCell(0, 3) << "\n";
    std::cout << "4x4 simple row0 col3 (expected 120): " << si4.getCell(3, 0) << "\n";
    std::cout << "4x4 simple row3 col0 (expected 426): " << si4.getCell(0, 3) << "\n";

    // testing when dimensions don't align
    std::cout << "2x2 * 4x4 block size (expected 0): " << a2.blockMultiply(a4).getSize() << "\n";
    std::cout << "2x2 * 4x4 strassen size (expected 0): " << a2.strassenMultiply(a4).getSize() << "\n";
    std::cout << "2x2 * 4x4 simple size (expected 0): " << a2.simpleMultiply(a4).getSize() << "\n";

    // testing a matrix that is not a power of 2
    Matrix<int> a3 = makeMatrix<int>({{1,2,3},{4,5,6,},{7,8,9}});
    std::cout << "3x3 block size (expected 0): " << a3.blockMultiply(a3).getSize() << "\n";
    std::cout << "3x3 strassen size (expected 0): " << a3.strassenMultiply(a3).getSize() << "\n";
    std::cout << "3x3 simple size (expected 0): " << a3.simpleMultiply(a3).getSize() << "\n";

    // hybrid tests, we pass in a int threshold now as well
    Matrix<int> hy1 = a4.hybridMultiply(a4, 1);
    Matrix<int> hy2 = a4.hybridMultiply(a4, 2);
    Matrix<int> hy4 = a4.hybridMultiply(a4, 4);
    std::cout << "4x4 hybrid threshold 1 row0 col3 (expected 120): " << hy1.getCell(3, 0) << "\n";
    std::cout << "4x4 hybrid threshold 1 row3 col0 (expected 426): " << hy1.getCell(0, 3) << "\n";
    std::cout << "4x4 hybrid threshold 2 row0 col3 (expected 120): " << hy2.getCell(3, 0) << "\n";
    std::cout << "4x4 hybrid threshold 2 row3 col0 (expected 426): " << hy2.getCell(0, 3) << "\n";
    std::cout << "4x4 hybrid threshold 4 row0 col3 (expected 120): " << hy4.getCell(3, 0) << "\n";
    std::cout << "4x4 hybrid threshold 4 row3 col0 (expected 426): " << hy4.getCell(0, 3) << "\n";
    std::cout << "2x2 * 4x4 hybrid size (expected 0): " << a2.hybridMultiply(a4, 2).getSize() << "\n";
    std::cout << "3x3 hybrid size (expected 0): " << a3.hybridMultiply(a3, 2).getSize() << "\n";
}

// Yay! all our unit tests pass! now it's benchmarking time!! 

// commenting out to import class
// int main() {
//     testMatrix();
//     testMultiply();
//     return 0;
// }
