/*
Linked List time baby let's goooo

Here are the functions I need to implement: 

interface LinkedList<T> {
    /
      Adds the element [data] to the front of the linked list.
    
    fun pushFront(data: T)

    /
      Adds the element [data] to the back of the linked list.
    /
    fun pushBack(data: T)

    /
      Removes an element from the front of the list. If the list is empty, it is unchanged.
      @return the value at the front of the list or nil if none exists
     /
    fun popFront(): T?

    /
      Removes an element from the back of the list. If the list is empty, it is unchanged.
      @return the value at the back of the list or nil if none exists
     /
    fun popBack(): T?

    /
      @return the value at the front of the list or nil if none exists
     /
    fun peekFront(): T?

    /
      @return the value at the back of the list or nil if none exists
     /
    fun peekBack(): T?

    /
      @return true if the list is empty and false otherwise
     /
    fun isEmpty(): Boolean
}

*/

#include <iostream> 
using namespace std; 

// there is no interface in c++ (afaik), so we can just make this 
// a class and then inherit from this class? but how is that different from 
// an interface? just because we can't "group" the classes together like 
// in the kotlin example? 

//so using a template we can make a "generic datatype " 
// this basically says "this is a thing with a type" hence typename 

// we only need typename T for pushFront and pushBack 


// ok so to actually make a linked list we need to create data "blocks" 
// that include the data of typename t, and then a head and a tail that are of type node
template <typename T> // so the TYPE is T. so a variable is then "T data"
class LLNode{
    public: 
        LLNode<T>* prevNodePtr = NULL;   // pointer to previous node
        T storedData;                       // the actual data in the node 
        LLNode<T>* nextNodePtr = NULL;   // pointer to the next node 
};
// we can use pointers to point to the next and previous nodes
// a pointer points at something. to access the stored thing, use *pointer.
// to access the pointer itself, use JUST pointer. this will be very important. 
// to access an attribute of the pointed at object, just pointer->attribute 
// confusing but I'm learning. 


template <typename T>
class LinkedList{
    LLNode<T>* headNodePtr = NULL; // a pointer to a headNode
    LLNode<T>* tailNodePtr = NULL; // a pointer to a tailNode 
  
    public:
    //@return true if the list is empty and false otherwise
    bool isEmpty(){ //have this first since we reference it in other methods 
        // all we need to do is check if the header points to anything 
        if(headNodePtr == NULL){
            return true;
        }
        return false;
    }


    //Adds the element [data] to the front of the linked list.
    int pushFront(T newData){  //adding data, need input 
        // so we to add a new node first 
        LLNode<T>* newNodePtr = new LLNode<T>(); // initializing newNode as a pointer to a node
        newNodePtr->storedData = newData; 
        
        // in order to do this right we need to check if our list is empty
        // if it's empty, we simply assign the head/tail nodes and leave our
        // newNode pointing to NULL 
        if(isEmpty()){
            headNodePtr = newNodePtr; // pointer to pointer
            tailNodePtr = newNodePtr; // pointer to pointer 
        }
        else{ 
            // otherwise, if it's not empty, we need to reassign the nextNode value of the newNode to 
            // the current "first" Node. Then, we can that current "first" node prevNode value to that 
            // of our newNode. And last, we reset the header to that of our new node. 
            newNodePtr->nextNodePtr = headNodePtr;
            headNodePtr->prevNodePtr = newNodePtr;
            headNodePtr = newNodePtr;
        }
        return 0; // and of course we return zero! 
    }

    //Adds the element [data] to the back of the linked list.
    int pushBack(T newData){   //adding data, need input 
        // we can basically do the same thing we did with the head node just starting with Tail
        //first we create a new node 
        LLNode<T>* newNodePtr = new LLNode<T>(); 
        newNodePtr->storedData = newData;

        // and then we want to assign the new node. if it's empty, 
        // we assign the head and tail nodes accordingly
        if (isEmpty()){
            headNodePtr = newNodePtr; //pointer and pointer 
            tailNodePtr = newNodePtr; //pointer and pointer 
        }
        else{ 
            // and if it's not empty, then we need to make the old LAST node 
            // nextNode reference the new last node and vise versa 
            newNodePtr->prevNodePtr = tailNodePtr;
            tailNodePtr->nextNodePtr = newNodePtr;
            tailNodePtr = newNodePtr;
        }
        return 0; // and of course return zero on success 
    }

    //Removes an element from the front of the list. If the list is empty, it is unchanged.
    //@return the value at the front of the list or nil if none exists
    T popFront(){
        // okay the rest of these SHOULD be easy at this point. 
        if (isEmpty()){
            return NULL;
        }
        else{
            
            //so basically we just want to cut out the first node
            // and also spit out it's value 
            T returnValue = headNodePtr->storedData; // save the data of the first node 
            //the next "first" node is the nextNode of node one 
            LLNode<T>* newFirstNode = headNodePtr->nextNodePtr;
            headNodePtr = newFirstNode;
            
            // and then the new first node needs to no longer point to the old node 
            // whoops! theres an edge case if the list only had one node 
            // we need to check if there is a next node 
            if(headNodePtr == NULL){
                //if there was only one item in the list, then the "wrong" reference 
                // is the tail
                tailNodePtr = NULL; 
            }
            else{
                // however if there was a second node, then the "wrong" ref is the prevNode
                // of that new first Node 
                headNodePtr->prevNodePtr = NULL;
            } 
            // and then we return the lost value 
            return returnValue;
        }
    }

    //Removes an element from the back of the list. If the list is empty, it is unchanged.
    //@return the value at the back of the list or nil if none exists
    T popBack(){
        // basically the same thing as above 
        if (isEmpty()){
            return NULL; //return NULL if there is no value 
        }
        else{
            
            //so now we just want to cut out the last node
            // and also spit out it's value 
            T returnValue = tailNodePtr->storedData; // save the data of the first node 
            //the next "first" node is the nextNode of node one 
            LLNode<T>* newLastNode = tailNodePtr->prevNodePtr;
            tailNodePtr = newLastNode;

            // and then again check if theres only one item 
            if(tailNodePtr == NULL){
                //if there was only one item in the list, then the "wrong" reference 
                // is the head
                headNodePtr = NULL; 
            }
            else{
                // however if there was a second node, then the "wrong" ref is the nextNode
                // of that new last Node 
                tailNodePtr->nextNodePtr = NULL;
            } 
            // and then we return the lost value 
            return returnValue;
        }
    }

    //@return the value at the front of the list or nil if none exists
    T peekFront(){
        // we can simply return the value of the head pointer (if it exists)
        if(headNodePtr != NULL){
            return headNodePtr->storedData; 
        }
        else{
            return NULL;
        }
    }

    //@return the value at the back of the list or nil if none exists
    T peekBack(){
        // we can simply return the value of the tail pointer (if it exists)
        if(tailNodePtr != NULL){
            return tailNodePtr->storedData; 
        }
        else{
            return NULL;
        }
    }
};

void testLinkedList(){
    std::cout << "\nLinkedList Tests\n";
    LinkedList<int> list;

    std::cout << "isEmpty on new list (expected 1): " << list.isEmpty() << "\n";
    std::cout << "peekFront on empty list (expected 0): " << list.peekFront() << "\n";
    std::cout << "peekBack on empty list (expected 0): " << list.peekBack() << "\n";
    std::cout << "popFront on empty list (expected 0): " << list.popFront() << "\n";
    std::cout << "popBack on empty list (expected 0): " << list.popBack() << "\n";
    std::cout << "pushFront(2) (expected 0): " << list.pushFront(2) << "\n";
    std::cout << "pushBack(3) (expected 0): " << list.pushBack(3) << "\n";
    std::cout << "pushFront(1) (expected 0): " << list.pushFront(1) << "\n";
    std::cout << "peekFront (expected 1): " << list.peekFront() << "\n";
    std::cout << "peekBack (expected 3): " << list.peekBack() << "\n";
    std::cout << "popFront (expected 1): " << list.popFront() << "\n";
    std::cout << "popBack (expected 3): " << list.popBack() << "\n";
    std::cout << "popFront (expected 2): " << list.popFront() << "\n";
    std::cout << "isEmpty after removals (expected 1): " << list.isEmpty() << "\n";
}


/*
next we have a stack! 
Needed functions: 
    /
    Add [data] to the top of the stack
    /
    fun push(data: T)
    /
    Remove the element at the top of the stack.  If the stack is empty, it remains unchanged.
    @return the value at the top of the stack or nil if none exists
    /
    fun pop(): T?
    /
    @return the value on the top of the stack or nil if none exists
    /
    fun peek(): T?
    /
    @return true if the stack is empty and false otherwise
    /
    fun isEmpty(): Boolean
*/

// template <typename T>
// class Stack : LinkedList { //this is how you do inheritence in C++ apparently 
// };

// okay so I was originally thinking of doing class inheritence but I think it might actually be simpler just to have a 
// linked list instance managed by a class 

// so we can do something like 
template <typename T>
class Stack{
    LinkedList<T> StackLL; //declare our linked list 

    public: 
    // Add [data] to the top of the stack
    int push(T data){
        return StackLL.pushBack(data);
    }

    // Remove the element at the top of the stack.  If the stack is empty, it remains unchanged.
    // @return the value at the top of the stack or nil if none exists
    T pop(){
        return StackLL.popBack();
    }

    //@return the value on the top of the stack or nil if none exists
    T peek(){
        return StackLL.peekBack(); 
    }

    //return true if the queue is empty and false otherwis
    bool isEmpty(){
        return StackLL.isEmpty();
    }

};

void testStack(){
    std::cout << "\nStack Tests\n";
    Stack<int> stack;

    std::cout << "isEmpty on new stack (expected 1): " << stack.isEmpty() << "\n";
    std::cout << "peek on empty stack (expected 0): " << stack.peek() << "\n";
    std::cout << "pop on empty stack (expected 0): " << stack.pop() << "\n";
    std::cout << "push(10) (expected 0): " << stack.push(10) << "\n";
    std::cout << "push(20) (expected 0): " << stack.push(20) << "\n";
    std::cout << "peek (expected 20): " << stack.peek() << "\n";
    std::cout << "pop (expected 20): " << stack.pop() << "\n";
    std::cout << "pop (expected 10): " << stack.pop() << "\n";
    std::cout << "isEmpty after removals (expected 1): " << stack.isEmpty() << "\n";
}

// yeah so that seems way easier to do than learning class inheritence although that might be the better option
// and actually mimic kotlin interfaces more... 
// oh well! Wanted to also note that it shouldn't matter if you use back or front functions as long as you
// are consistent 

//queue time! Needed functions: 
/*  /
    Add [data] to the end of the queue.
    /
    fun enqueue(data: T)
    /
    Remove the element at the front of the queue.  If the queue is empty, it remains unchanged.
    @return the value at the front of the queue or nil if none exists
    /
    fun dequeue(): T?
    /
    @return the value at the front of the queue or nil if none exists
    /
    fun peek(): T?
    /
    @return true if the queue is empty and false otherwise
    /
    fun isEmpty(): Boolean
*/
template <typename T>
class Queue{
    LinkedList<T> QueueLL; //declare our linked list 

    public: 
    // Add [data] to the end of the queue.
    int enqueue(T data){
        return QueueLL.pushBack(data);
    }

    // Remove the element at the front of the queue.  If the queue is empty, it remains unchanged.
    // @return the value at the front of the queue or nil if none exists
    T dequeue(){
        return QueueLL.popFront();
    }

    //@return the value at the front of the queue or nil if none exists
    T peek(){
        return QueueLL.peekFront(); 
    }

    //return true if the queue is empty and false otherwis
    bool isEmpty(){
        return QueueLL.isEmpty();
    }

};

void testQueue(){
    std::cout << "\nQueue Tests\n";
    Queue<int> queue;

    std::cout << "isEmpty on new queue (expected 1): " << queue.isEmpty() << "\n";
    std::cout << "peek on empty queue (expected 0): " << queue.peek() << "\n";
    std::cout << "dequeue on empty queue (expected 0): " << queue.dequeue() << "\n";
    std::cout << "enqueue(10) (expected 0): " << queue.enqueue(10) << "\n";
    std::cout << "enqueue(20) (expected 0): " << queue.enqueue(20) << "\n";
    std::cout << "peek (expected 10): " << queue.peek() << "\n";
    std::cout << "dequeue (expected 10): " << queue.dequeue() << "\n";
    std::cout << "dequeue (expected 20): " << queue.dequeue() << "\n";
    std::cout << "isEmpty after removals (expected 1): " << queue.isEmpty() << "\n";
}



// and now we need some unit tests, which we can add by simply declaring an instance of a linked list, stack, and queue, 
// performing some test operations, and then printing some outputs. Easy enough! 

int main() {
    testLinkedList();
    testStack();
    testQueue();
}