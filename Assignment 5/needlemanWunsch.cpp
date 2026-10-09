/* 
Sam Wisnoski 

Assignment 5 - Divide and Conquer and Dynamic Programming

10/8/2026

*/

#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>
#include <string>

/*
This is part three! This is where we implement some gene algorithms! Boy oh boy! 

We are going to implement the needleman-wunch algorithm because I watched the video and it seems like I can mostly wrap my head around the 
algorithm pretty easily. 

Here are the basic steps from wikipedia: 

First, we fill in a matrix of values 
Start with a zero in the first row, first column (not including the cells containing nucleotides).
Move through the cells row by row, calculating the score for each cell. The score is calculated by 
comparing the scores of the cells neighboring to the left, top or top-left (diagonal) of the cell and 
adding the appropriate score for match, mismatch or indel. 

Take the maximum of the candidate scores for each of the three possibilities: 
    [1] The path from the top or left cell represents an indel pairing, so take the scores of the left and the top cell, and add the score for indel to each of them. 
        Value 1: Top cell + indel value 
        Value 2: Bottom cell + indel value 
    [2] The diagonal path represents a match/mismatch, so take the score of the top-left diagonal cell and add the score for match if the 
        corresponding bases (letters) in the row and column are matching or the score for mismatch if they do not. 
        Value 3: top-left cell + match score 

The resulting score for the cell is the highest of the three candidate scores. If the value does not have a top/left score (as if, for example you are on 
the top or left edge of the graph, you rule out that posibility).

Although to be honest it might just be easier to set the top left and right rows manually since then we don't have to worry about those edge cases. 

We can go through the graph and follow the steps above to compute the entire matrix. After computing the matrix, we 
already have the ideal alignment score in the bottom right square of the algorithm. We can do some tracing from bottom 
right to top left to compute the ideal matching. But let's just fill out the graph first and then we can worry about the rest later. 
*/

// ###### STEP ONE: FILLING OUT THE GRAPH ######

class needlemanWunsch{
    private:
    // we only need to store integers here, so we can create a matrix of integers 
    std::vector<std::vector<int>> entireMatrix;
    std::string sequenceA;
    std::string sequenceB;


    public:
    int indelCost = -1;
    int matchCost = 1;
    int mismatchCost = -1;
    int finalValue;

    // in order to actually set elements properly, we need to have a way to auto 
    // set a size n for the matrix. for needleman wunch, instead of passing in a size,
    // we will pass in two strings and set up the matrix. 

    // so we can use a constructor to set the size of the matrix, 
    // which means we have to pass in a size value upon initialization of the matrix 
    needlemanWunsch(std::string sequenceAinput, std::string sequenceBinput) {
        sequenceA = sequenceAinput;
        sequenceB = sequenceBinput;

        // set the matrix size equal to the length of one of the sequences + 1,
        // to account for the top and left rows that we fill in manually with indel costs 
        int rowSize = sequenceB.length() + 1;
        int columnSize = sequenceA.length() + 1;

        // resize the number of rows 
        entireMatrix.resize(rowSize);

        // resize each of the columns
        for (int rowIndex = 0; rowIndex < rowSize; rowIndex++) {
            entireMatrix[rowIndex].resize(columnSize);
        }

        // so now we have a matrix that is the size of the sequence +1, we want to fill in the top 
        // and left rows based on the indel cost. 
        // set top left cell as a 0
        setCell(0, 0, 0);

        // set the top row by looping through the cells and adding indelCost 
        for(int columnIndex = 1; columnIndex < columnSize; columnIndex++){
            int leftCell = getCell(columnIndex - 1, 0);
            setCell(columnIndex, 0, leftCell + indelCost);
        }

        // set the left row via the same process 
        for(int rowIndex = 1; rowIndex < rowSize; rowIndex++){
            int topCell = getCell(0, rowIndex - 1);
            setCell(0, rowIndex, topCell + indelCost);
        }

        // and now we need to loop through every other cell and fill it out. if we index first by rows and secondarily
        // by columns, that guarentees that as we fill out cells, we will know the top and left values 
        for (int rowIndex = 1; rowIndex < rowSize; rowIndex++){
            for (int columnIndex = 1; columnIndex < columnSize; columnIndex++){
                // we retrive the left and top cells 
                int leftCell = getCell(columnIndex - 1, rowIndex);
                int topCell = getCell(columnIndex, rowIndex - 1);
                int topleftCell = getCell(columnIndex - 1, rowIndex -1);

                // Value 1: Top cell + indel value; Value 2: Bottom cell + indel value; Value 3: top-left cell + match score
                int valueOne = topCell + indelCost;
                int valueTwo = leftCell + indelCost; 
                int valueThree;
                if(sequenceA[columnIndex - 1] == sequenceB[rowIndex - 1]){
                    valueThree = topleftCell + matchCost;
                } 
                else{
                    valueThree = topleftCell + mismatchCost; 
                }
                setCell(columnIndex, rowIndex, std::max({valueOne, valueTwo, valueThree}));
            }
        }
        finalValue = getCell(columnSize - 1, rowSize - 1);
        // and now, our entire array should be initialized! 
    }

    // just copy our setCell and getCell from the original matrix class
    void setCell(int column, int row, int newData){
        entireMatrix[row][column] = newData;
    }

    int getCell(int column, int row){
        return entireMatrix[row][column];
    }

    // and now let's do a traceback
    /*
    Mark a path from the cell on the bottom right back to the cell on the top left by following the direction of the arrows. 
    From this path, the sequence is constructed by these rules:

        [1] A diagonal arrow represents a match or mismatch, so the letter of the column and the letter of the row of the origin cell will align.
        [2] A horizontal or vertical arrow represents an indel. Vertical arrows will align a gap ("-") to the letter of the row (the "side" sequence), 
            horizontal arrows will align a gap to the letter of the column (the "top" sequence).
        [3] If there are multiple arrows to choose from, they represent a branching of the alignments. If two or more branches all belong to paths 
            from the bottom right to the top left cell, they are equally viable alignments. In this case, note the paths as separate alignment candidates.
    */
    std::vector<std::string> align(){
        // okay so the video talked about this a bit more clearly, but basically we check for a match, and if the columns/rows match, we 
        // move diagonally. If they don't match, we move either up or left, whichever is higher. If they are equally high, then we branch.
    
        std::string alignedA = "";
        std::string alignedB = "";

        // declare two ints to track as we iterate through the rows/columns
        int column = sequenceA.length();
        int row = sequenceB.length();

        // as long as we haven't reached the top left
        while (column > 0 || row > 0) {
            int currentScore = getCell(column, row);

            // if we can track back diagonally, then we want to move one 
            if (column > 0 && row > 0) {
                // depending on if we matched or mismatched we have a different diagonal cost 
                int cost;

                if (sequenceA[column - 1] == sequenceB[row - 1]) {
                    cost = matchCost;
                } else {
                    cost = mismatchCost;
                }

                if (currentScore == getCell(column - 1, row - 1) + cost) {
                    // if diagonal, add to both sequence A and B 
                    alignedA += sequenceA[column - 1];
                    alignedB += sequenceB[row - 1];
                    column = column - 1;
                    row = row - 1;
                    continue;
                }
            }

            // then, if our diagonal doesn't work out, we check if we had moved up,
            // and then check if we can move left  
            if (row > 0 && currentScore == getCell(column, row - 1) + indelCost) {
                // if we move up, add to sequence B and add a gap to sequence A 
                alignedA += '-';
                alignedB += sequenceB[row - 1];
                row = row - 1;
            }
            // if we don't move diagonal or up, we must move left
            else {
                // add to A and add a gap to B
                alignedA += sequenceA[column - 1];
                alignedB += '-';
                column = column - 1;
            }
        }

        // we then have to reverse our strings because we built them backwards
        std::reverse(alignedA.begin(), alignedA.end());
        std::reverse(alignedB.begin(), alignedB.end());

        std::vector<std::string> alignVector = {alignedA, alignedB};
        return alignVector;
    }
};


int main() {
    std::string genomeSnippet = "TGGCGACAACCGTAGCGGAATATTTTCGCGACCAGGGAAAACGGGTCGTGCTTTTTATCGATTCCATGACCCGTTATGCGCGTGCTTTGCGAGACGTGGCACTGGCGTCGGGAGAGCGTCCGGCTCGTCGAGGTTATCCCGCCTCCGTATTCGATAATTTGCCCCGCTTGCTGGAACGCCCAGGGGCGACCAGCGAGGGAAGCATTACTGCCTTTTATACGGTACTGCTGGAAAGCGAGGAAGAGGCGGACCCGATGGCGGATGAAATTCGCTCTATCCTTGACGGTCACCTGTATCTGAGCAGAAAGCTGGCCGGGCAGGGACATTACCCGGCAATCGATGTACTGAAAAGCGTAAGCCGCGTTTTT";
    std::string testAgainst = "TGGCCACCACGATAGCAGAATTTTTTCGCGATAATGGAAAGCGAGTCGTCTTGCTTGCCGACTCACTGACGCGTTATGCCAGGGCCGCACGGGAAATCGCTCTGGCCGCCGGAGAGACCGCGGTTTCTGGAGAATATCCGCCAGGCGTATTTAGTGCATTGCCACGACTTTTAGAACGTACGGGAATGGGAGAAAAAGGCAGTATTACCGCATTTTATACGGTACTGGTGGAAGGCGATGATATGAATGAGCCGTTGGCGGATGAAGTCCGTTCACTGCTTGATGGACATATTGTACTATCCCGACGGCTTGCAGAGAGGGGGCATTATCCTGCCATTGACGTGTTGGCAACGCTCAGCCGCGTTTTT";
    needlemanWunsch myNeedlemanWunsch(genomeSnippet, testAgainst);
    std::vector<std::string> alignVector = myNeedlemanWunsch.align();
    std::cout << "Size of A: " << genomeSnippet.length() << "\nSize of B: " << testAgainst.length() << "\nMatch result: " << myNeedlemanWunsch.finalValue;

    std::cout << "\nAlignment:\n";
    for (size_t i = 0; i < alignVector[0].length(); i += 80) {
    std::cout << alignVector[0].substr(i, 80) << '\n';
    std::cout << alignVector[1].substr(i, 80) << "\n\n";
}
    return 0;
}