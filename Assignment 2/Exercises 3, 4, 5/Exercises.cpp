/*
For the exercises, I just wanted to copy over my implentations of linked lists, stacks, and queues, 
without comments so that the actual file would a bit easier to sift through 
*/

#include <iostream> 
#include <string>

template <typename T> 
class LLNode{
    public: 
        LLNode<T>* prevNodePtr = NULL;   // pointer to previous node
        T storedData;                       // the actual data in the node 
        LLNode<T>* nextNodePtr = NULL;   // pointer to the next node 
};

template <typename T>
class LinkedList{
    LLNode<T>* headNodePtr = NULL; // a pointer to a headNode
    LLNode<T>* tailNodePtr = NULL; // a pointer to a tailNode 
  
    public:
    bool isEmpty(){ //have this first since we reference it in other methods 
        if(headNodePtr == NULL){
            return true;
        }
        return false;
    }

    int pushFront(T newData){  //adding data, need input 
        LLNode<T>* newNodePtr = new LLNode<T>(); // initializing newNode as a pointer to a node
        newNodePtr->storedData = newData; 
        
        if(isEmpty()){
            headNodePtr = newNodePtr; // pointer to pointer
            tailNodePtr = newNodePtr; // pointer to pointer 
        }
        else{ 
            newNodePtr->nextNodePtr = headNodePtr;
            headNodePtr->prevNodePtr = newNodePtr;
            headNodePtr = newNodePtr;
        }
        return 0; // and of course we return zero! 
    }

    int pushBack(T newData){   //adding data, need input 
        LLNode<T>* newNodePtr = new LLNode<T>(); 
        newNodePtr->storedData = newData;

        if (isEmpty()){
            headNodePtr = newNodePtr; //pointer and pointer 
            tailNodePtr = newNodePtr; //pointer and pointer 
        }
        else{ 
            newNodePtr->prevNodePtr = tailNodePtr;
            tailNodePtr->nextNodePtr = newNodePtr;
            tailNodePtr = newNodePtr;
        }
        return 0; // and of course return zero on success 
    }

    T popFront(){
        if (isEmpty()){
            return NULL;
        }
        else{
            T returnValue = headNodePtr->storedData; // save the data of the first node 
            LLNode<T>* newFirstNode = headNodePtr->nextNodePtr;
            headNodePtr = newFirstNode;
            if(headNodePtr == NULL){
                tailNodePtr = NULL; 
            }
            else{ 
                headNodePtr->prevNodePtr = NULL;
            } 
            return returnValue;
        }
    }

    T popBack(){
        if (isEmpty()){
            return NULL; //return NULL if there is no value 
        }
        else{
            T returnValue = tailNodePtr->storedData; // save the data of the first node 
            LLNode<T>* newLastNode = tailNodePtr->prevNodePtr;
            tailNodePtr = newLastNode;

            if(tailNodePtr == NULL){
                headNodePtr = NULL; 
            }
            else{ 
                tailNodePtr->nextNodePtr = NULL;
            } 
            return returnValue;
        }
    }

    T peekFront(){
        if(headNodePtr != NULL){
            return headNodePtr->storedData; 
        }
        else{
            return NULL;
        }
    }

    T peekBack(){
        if(tailNodePtr != NULL){
            return tailNodePtr->storedData; 
        }
        else{
            return NULL;
        }
    }
};

 
template <typename T>
class Stack{
    LinkedList<T> StackLL; //declare our linked list 

    public: 
    int push(T data){
        return StackLL.pushBack(data);
    }

    T pop(){
        return StackLL.popBack();
    }

    T peek(){
        return StackLL.peekBack(); 
    }

    bool isEmpty(){
        return StackLL.isEmpty();
    }

};

template <typename T>
class Queue{
    LinkedList<T> QueueLL; //declare our linked list 

    public: 
    int enqueue(T data){
        return QueueLL.pushBack(data);
    }

    T dequeue(){
        return QueueLL.popFront();
    }

    T peek(){
        return QueueLL.peekFront(); 
    }

    bool isEmpty(){
        return QueueLL.isEmpty();
    }

};



// ################# EXERCISE 3 - Reverse a Stack #################

// okay so let's reverse a stack! 
template <typename T>
Stack<T> stackReverser(Stack<T> originalStack) {
    // okay we can start by declaring a queue that we will load values into. 
    // the value on the top of the original stack will be the first value
    // we want to put on the bottom of the stack 
    Queue<T> tempQueue;

    // then we can loop through the stack and load the elements into a queue top to bottom
    // loop through as long as stack is not empty 
    while(!originalStack.isEmpty()){
        tempQueue.enqueue(originalStack.pop());
    } 

    // and then we can loop through the queue and reload them into the stack, but now reversed! 
    while (!tempQueue.isEmpty()){
        originalStack.push(tempQueue.dequeue());
    }

    // then we clean up the temporary Queue and return the original stack! 
    return originalStack;
}

void testExerciseThree(){
    std::cout << "\n################# EXERCISE 3 - Reverse a Stack #################" << "\n";
    Stack<int> originalStack;

    originalStack.push(3);
    originalStack.push(5);
    originalStack.push(17);
    originalStack.push(99);
    originalStack.push(14);

    Stack<int> reversedStack = stackReverser(originalStack);
    std::cout << "Original Stack is Top -> 14, 99, 17, 5, 3 <- Bottom \n";
    std::cout << "Reversed Stack is Top -> 3, 5, 17, 99, 14 <- Bottom \n";
    std::cout << "Reversed stack should pop 3: " << reversedStack.pop() << "\n";
    std::cout << "Reversed stack should pop 5: " << reversedStack.pop() << "\n";
    std::cout << "Reversed stack should pop 17: " << reversedStack.pop() << "\n";
    std::cout << "Reversed stack should pop 99: " << reversedStack.pop() << "\n";
    std::cout << "Reversed stack should pop 14: " << reversedStack.pop() << "\n";
    std::cout << "Reversed stack should be empty (1): " << reversedStack.isEmpty() << "\n";
}


// ################# EXERCISE 4 - Parentheses Problem #################

// implementation below 
// so for some reason using a pointer to a char can either point to 
// a single char or a char array (a string) since a string has a 
// predefined order kinda. not sure how this works with other arrays. 
class Solution{
    public:
    bool isValid(std::string stringtocheck) {
        // so first we need to loop through each character
        std::string openOptions = "([{";
        std::string closeOptions = ")]}";
        Stack<char> closeStack;

        for (char character : stringtocheck){
            // if it's an open parentheses, it will always be accepted as a valid character
            // the last recorded open paranthesis needs to be closed first 
            // all parentheses needs to be closed 

            // strategy -> for each open, add the corresponding close to the back of the stack
            // each close, pop out the back and see if it matches 
            // return valid if there are no invalid matches and we end with an empty stack
            if(openOptions.find(character) != std::string::npos){
                int index = openOptions.find(character);
                closeStack.push(closeOptions[index]);
            }
            else if (closeOptions.find(character) != std::string::npos){
                if(character != closeStack.pop()){
                    return false;
                }
            }
            else{
                std::cout << "Invalid Char:" << character << std::endl; // so for some reason you need to declare an endline? bro why
                return false;
            }
        }
        return closeStack.isEmpty();
    }
};

void testExerciseFour(){
    std::cout << "\n################# EXERCISE 4 - Parentheses Problem #################" << "\n";
    std::cout << "103/103 tests passed on leetcode B)" << "\n";
}


// ################# EXERCISE 5 - Copy Stack #################
// so we need to copy a stack but can only use one queue as temporary storage
// so how can we do this with one queue 
// we already know how to reverse it, loop through stack elements, enque, then loop through 
// queue and dequeue. so all we need to do in order to copy the stack is to reverse it twice! 
// our reverser only uses one queue, so we already fit the definition! 

template <typename T>
Stack<T> stackCopier(Stack<T> originalStack){
    Stack<T> copyStack = originalStack;
    copyStack = stackReverser(copyStack);
    copyStack = stackReverser(copyStack);
    return copyStack;
}

// can do a simple test! 
void testExerciseFive(){
    std::cout << "\n################# EXERCISE 5 - Copy Stack #################" << "\n";
    Stack<int> originalStack;

    originalStack.push(3);
    originalStack.push(5);
    originalStack.push(17);
    originalStack.push(99);
    originalStack.push(14);

    Stack<int> copiedStack = stackCopier(originalStack);

    std::cout << "Original Stack is Top -> 14, 99, 17, 5, 3 <- Bottom \n";
    std::cout << "Copied Stack is Top -> 14, 99, 17, 5, 3 <- Bottom \n";
    std::cout << "Copied stack should pop 14: " << copiedStack.pop() << "\n";
    std::cout << "Copied stack should pop 99: " << copiedStack.pop() << "\n";
    std::cout << "Copied stack should pop 17: " << copiedStack.pop() << "\n";
    std::cout << "Copied stack should pop 5: " << copiedStack.pop() << "\n";
    std::cout << "Copied stack should pop 3: " << copiedStack.pop() << "\n";
    std::cout << "Copied stack should be empty (1): " << copiedStack.isEmpty() << "\n";
}


// and now we just gotta run our tests
int main(){
    testExerciseThree();
    testExerciseFour();
    testExerciseFive();
}