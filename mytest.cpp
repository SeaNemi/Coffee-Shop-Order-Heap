/***************************
 ** File:    mytest.cpp
 ** Project: CMSC 341, proj2, Spring 2023
 ** Author:  William Whatley
 ** Date:    4/08/2023
 ** Section: Marron
 ** Email:   w59@umbc.edu
 **
 ** This file is written as a test for cqueue.cpp. Numerous cases are tested to ensure
 ** that the code works properly. Normal cases, edge cases, and error cases are tested, alongside
 ** other tests such as testing the assignment operator, inserting and removing nodes, and switching between different heap types
 **
*****************************/

#include "cqueue.h"
#include <random>

int priorityFn1(const Order &order);// works with a MAXHEAP
int priorityFn2(const Order &order);// works with a MINHEAP

//global constants
const int NORMAL_CASE = 600; //NORMAL_CASE is 600, as suggested by the website

//random class taken from driver and used for testing purposes
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution
};

class Tester{
    public:
        void testCondition(bool var); //basic test condition which prints output
        
        //randomFill helper, overloaded for certain scenarios
        void randomFill(CQueue& cqueue, int numToFill);

        //minheap insertion and removal
        bool minheapInsertion(CQueue& cqueue, int num);
        bool minheapRemoval(CQueue& cqueue, int num);

        //maxheap insertion and removal tested
        bool maxheapInsertion(CQueue& cqueue, int num);
        bool maxheapRemoval(CQueue& cqueue, int num);

        //property tests to ensure each property holds up
        bool minheapTest(bool result, const Node* curr, prifn_t priorFn);
        bool maxheapTest(bool result, const Node* curr, prifn_t priorFn);

        //property tests for leftist heaps
        bool leftistNPLProperty(CQueue& cqueue, int num);
        bool leftistHeapTest(CQueue& cqueue, int num);
        bool NPLTest(bool result, const Node* curr);
        bool leftistTest(bool result, const Node* curr);

        //functions which test the assignment operator
        bool normalAssignment(CQueue& cqueue);
        bool edgeAssignment(CQueue& cqueue);
        bool assignmentHelper(bool result, const Node* lhsPtr, const Node* rhsPtr);

        //copy constructor tests
        bool normalCopyConstructor(CQueue& cqueue);
        bool edgeCopyConstructor(CQueue& cqueue);

        //priorityFunction and structure tests
        bool testPriorityFunction(CQueue& cqueue);
        bool testSetStructure(CQueue& cqueue);
        bool find(bool result, const Node* node, int orderID);

        //mergeWithQueue test
        bool edgeMergeWithQueue(CQueue& cqueue);
        bool testMergeWithQueue(CQueue& cqueue);

        //thrown error tests
        bool errorDequeue(CQueue& cqueue);
        bool errorMerge(CQueue& cqueue);
};

int main(){
    Tester tester;
    CQueue* newCQueue;

    cout << "**********************" << endl;
    cout << "***** BEGIN TEST *****" << endl;
    cout << "**********************" << endl << endl;
    {
        cout << "\n*** TEST BLOCK  ONE***" << endl << endl;
        cout << "This will test edge caes of insertion of MINHEAP" << endl << endl;

        newCQueue = new CQueue(priorityFn1, MINHEAP, SKEW); //cqueue initialized

        cout << "minheapInsertion with edge case of priorFunc1, MINHEAP, SKEW: \n\t";
        bool testResult = tester.minheapInsertion(*newCQueue, 1);
        tester.testCondition(testResult);
        delete newCQueue;

        cout << "minheapInsertion with edge case of priorFunc1, MINHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn1, MINHEAP, LEFTIST); //cqueue initialized
        testResult = tester.minheapInsertion(*newCQueue, 1);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "minheapInsertion with edge case of priorFunc2, MINHEAP, SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MINHEAP, SKEW); //cqueue initialized
        testResult = tester.minheapInsertion(*newCQueue, 1);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "minheapInsertion with edge case of priorFunc2, MINHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn2, MINHEAP, LEFTIST); //cqueue initialized
        testResult = tester.minheapInsertion(*newCQueue, 1);
        tester.testCondition(testResult); 
        delete newCQueue;
    
        cout << "\n***END TEST BLOCK ONE***" << endl;  
    }
    {
        cout << "\n*** TEST BLOCK TWO***" << endl << endl;
        cout << "This will test normal cases of insertion of MINHEAP" << endl << endl;

        newCQueue = new CQueue(priorityFn1, MINHEAP, SKEW); //cqueue initialized

        cout << "minheapInsertion with normal case of priorFunc1, MINHEAP, SKEW: \n\t";
        bool testResult = tester.minheapInsertion(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult);
        delete newCQueue;

        cout << "minheapInsertion with normal case of priorFunc1, MINHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn1, MINHEAP, LEFTIST); //cqueue initialized
        testResult = tester.minheapInsertion(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "minheapInsertion with normal case of priorFunc2, MINHEAP, SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MINHEAP, SKEW); //cqueue initialized
        testResult = tester.minheapInsertion(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "minheapInsertion with normal case of priorFunc2, MINHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn2, MINHEAP, LEFTIST); //cqueue initialized
        testResult = tester.minheapInsertion(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult); 
        delete newCQueue;
    
        cout << "\n***END TEST BLOCK TWO***" << endl; 
    }
    {
        cout << "\n*** TEST BLOCK THREE***" << endl << endl;
        cout << "This will test normal cases of removal of MINHEAP" << endl << endl;

        newCQueue = new CQueue(priorityFn1, MINHEAP, SKEW); //cqueue initialized

        cout << "minheapRemoval with normal case of priorFunc1, MINHEAP, SKEW: \n\t";
        bool testResult = tester.minheapRemoval(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult);
        delete newCQueue;

        cout << "minheapRemoval with normal case of priorFunc1, MINHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn1, MINHEAP, LEFTIST); //cqueue initialized
        testResult = tester.minheapRemoval(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "minheapRemoval with normal case of priorFunc2, MINHEAP, SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MINHEAP, SKEW); //cqueue initialized
        testResult = tester.minheapRemoval(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "minheapRemoval with normal case of priorFunc2, MINHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn2, MINHEAP, LEFTIST); //cqueue initialized
        testResult = tester.minheapRemoval(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult); 
        delete newCQueue;
    
        cout << "\n***END TEST BLOCK THREE***" << endl; 
    }
    {
	    cout << "\n*** TEST BLOCK FOUR***" << endl << endl;
        cout << "This will test edge caes of insertion of MAXHEAP" << endl << endl;

        newCQueue = new CQueue(priorityFn1, MAXHEAP, SKEW); //cqueue initialized

        cout << "maxheapInsertion with edge case of priorFunc1, MAXHEAP, SKEW: \n\t";
        bool testResult = tester.maxheapInsertion(*newCQueue, 1);
        tester.testCondition(testResult);
        delete newCQueue;

        cout << "maxheapInsertion with edge case of priorFunc1, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn1, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.maxheapInsertion(*newCQueue, 1);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "maxheapInsertion with edge case of priorFunc2, MAXHEAP, SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, SKEW); //cqueue initialized
        testResult = tester.maxheapInsertion(*newCQueue, 1);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "maxheapInsertion with edge case of priorFunc2, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.maxheapInsertion(*newCQueue, 1);
        tester.testCondition(testResult); 
        delete newCQueue;
        cout << "\n***END TEST BLOCK FOUR***" << endl;  
    }
    {
        cout << "\n*** TEST BLOCK FIVE***" << endl << endl;
        cout << "This will test normal cases of insertion of MAXHEAP" << endl << endl;

        newCQueue = new CQueue(priorityFn1, MAXHEAP, SKEW); //cqueue initialized

        cout << "maxheapInsertion with normal case of priorFunc1, MAXHEAP, SKEW: \n\t";
        bool testResult = tester.maxheapInsertion(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult);
        delete newCQueue;

        cout << "maxheapInsertion with normal case of priorFunc1, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn1, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.maxheapInsertion(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "maxheapInsertion with normal case of priorFunc2, MAXHEAP, SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, SKEW); //cqueue initialized
        testResult = tester.maxheapInsertion(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "maxheapInsertion with normal case of priorFunc2, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.maxheapInsertion(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult); 
        delete newCQueue;
    
        cout << "\n***END TEST BLOCK FIVE***" << endl; 
    }
    {
        cout << "\n*** TEST BLOCK SIX***" << endl << endl;
        cout << "This will test normal cases of removal of MAXHEAP" << endl << endl;

        newCQueue = new CQueue(priorityFn1, MAXHEAP, SKEW); //cqueue initialized

        cout << "maxheapRemoval with normal case of priorFunc1, MAXHEAP, SKEW: \n\t";
        bool testResult = tester.maxheapRemoval(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult);
        delete newCQueue;

        cout << "maxheapRemoval with normal case of priorFunc1, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn1, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.maxheapRemoval(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "maxheapRemoval with normal case of priorFunc2, MAXHEAP, SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, SKEW); //cqueue initialized
        testResult = tester.maxheapRemoval(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "maxheapRemoval with normal case of priorFunc2, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.maxheapRemoval(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult); 
        delete newCQueue;
    
        cout << "\n***END TEST BLOCK SIX***" << endl; 
    }
    {
        cout << "\n*** TEST BLOCK SEVEN***" << endl << endl;
        cout << "This will test normal cases of LEFTIST heaps to ensure NPL property upholds" << endl << endl;

        newCQueue = new CQueue(priorityFn1, MINHEAP, LEFTIST); //cqueue initialized

        cout << "leftistNPLProperty with normal case of priorFunc1, MINHEAP, LEFTIST: \n\t";
        bool testResult = tester.leftistNPLProperty(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult);
        delete newCQueue;

        cout << "leftistNPLProperty with normal case of priorFunc1, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn1, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.leftistNPLProperty(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "leftistNPLProperty with normal case of priorFunc2, MINHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn2, MINHEAP, LEFTIST); //cqueue initialized
        testResult = tester.leftistNPLProperty(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "leftistNPLProperty with normal case of priorFunc2, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.leftistNPLProperty(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult); 
        delete newCQueue;
    
        cout << "\n***END TEST BLOCK SEVEN***" << endl; 
    }
    {
        cout << "\n*** TEST BLOCK EIGHT***" << endl << endl;
        cout << "This will test normal cases of LEFTIST heaps to ensure the leftist heap property upholds" << endl << endl;

        newCQueue = new CQueue(priorityFn1, MINHEAP, LEFTIST); //cqueue initialized

        cout << "leftistHeapTest with normal case of priorFunc1, MINHEAP, LEFTIST: \n\t";
        bool testResult = tester.leftistHeapTest(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult);
        delete newCQueue;

        cout << "leftistHeapTest with normal case of priorFunc1, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn1, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.leftistHeapTest(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "leftistHeapTest with normal case of priorFunc2, MINHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn2, MINHEAP, LEFTIST); //cqueue initialized
        testResult = tester.leftistHeapTest(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "leftistHeapTest with normal case of priorFunc2, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.leftistHeapTest(*newCQueue, NORMAL_CASE);
        tester.testCondition(testResult); 
        delete newCQueue;
    
        cout << "\n***END TEST BLOCK EIGHT***" << endl;
    }
    {
        cout << "\n*** TEST BLOCK NINE***" << endl << endl;
        cout << "This will check to see if setPriorityFunction works properly" << endl << endl;

        newCQueue = new CQueue(priorityFn1, MAXHEAP, SKEW); //cqueue initialized
        
        cout << "testPriorityFunction with case of priorFunc1 and SKEW: \n\t";
        bool testResult = tester.testPriorityFunction(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;
        
        cout << "testPriorityFunction with case of priorFunc1 and LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn1, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.testPriorityFunction(*newCQueue);
        tester.testCondition(testResult); 
        delete newCQueue;
        
        cout << "testPriorityFunction with case of priorFunc2 and SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, SKEW); //cqueue initialized
        testResult = tester.testPriorityFunction(*newCQueue);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "testPriorityFunction with case of priorFunc2 and LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.testPriorityFunction(*newCQueue);
        tester.testCondition(testResult); 
        delete newCQueue;
        
        cout << "\n***END TEST BLOCK NINE***" << endl;
    }
    {
        cout << "\n*** TEST BLOCK TEN***" << endl << endl;
        cout << "This will check to see if setStructure works properly" << endl;
        cout << "Used only twice since each case is checked within the function" << endl << endl;

        newCQueue = new CQueue(priorityFn1, MAXHEAP, SKEW); //cqueue initialized
        
        cout << "testSetStructure starting with a SKEW: \n\t";
        bool testResult = tester.testSetStructure(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;
        
        cout << "testSetStructure starting with a LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn1, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.testSetStructure(*newCQueue);
        tester.testCondition(testResult); 
        delete newCQueue;
    
        cout << "\n***END TEST BLOCK TEN***" << endl;
    }
    {
        cout << "\n*** TEST BLOCK ELEVEN***" << endl << endl;
        cout << "This will check to see if mergeWithQueue works  in edge cases" << endl << endl;

        newCQueue = new CQueue(priorityFn1, MINHEAP, SKEW); //cqueue initialized
        
        cout << "edgeMergeWithQueue with case of priorFunc1 and MINHEAP: \n\t";
        bool testResult = tester.edgeMergeWithQueue(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;
        
        cout << "edgeMergeWithQueue with case of priorFunc1 and MAXHEAP: \n\t";
        newCQueue = new CQueue(priorityFn1, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.edgeMergeWithQueue(*newCQueue);
        tester.testCondition(testResult); 
        delete newCQueue;
        
        cout << "edgeMergeWithQueue with case of priorFunc2 and MINHEAP: \n\t";
        newCQueue = new CQueue(priorityFn2, MINHEAP, SKEW); //cqueue initialized
        testResult = tester.edgeMergeWithQueue(*newCQueue);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "edgeMergeWithQueue with case of priorFunc2 and MAXHEAP: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.edgeMergeWithQueue(*newCQueue);
        tester.testCondition(testResult); 
        delete newCQueue;
        
        cout << "\n***END TEST BLOCK ELEVEN***" << endl;        
    }
    {
        cout << "\n*** TEST BLOCK TWELVE***" << endl << endl;
        cout << "This will check to see if mergeWithQueue works properly" << endl;
       
      	newCQueue = new CQueue(priorityFn1, MINHEAP, SKEW); //cqueue initialized
        
        cout << "testMergeWithQueue with case of MINHEAP and SKEW: \n\t";
        bool testResult = tester.testMergeWithQueue(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;
        
        cout << "testMergeWithQueue with case of MINHEAP and LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn1, MINHEAP, LEFTIST); //cqueue initialized
        testResult = tester.testMergeWithQueue(*newCQueue);
        tester.testCondition(testResult); 
        delete newCQueue;
        
        cout << "testMergeWithQueue with case of MAXHEAP and SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, SKEW); //cqueue initialized
        testResult = tester.testMergeWithQueue(*newCQueue);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "testMergeWithQueue with case of MAXHEAP and LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.testMergeWithQueue(*newCQueue);
        tester.testCondition(testResult); 
        delete newCQueue;

        cout << "\n***END TEST BLOCK TWELVE***" << endl;
    }
    {
        cout << "\n*** TEST BLOCK THIRTEEN ***" << endl << endl;
	    cout << "This will test to ensure that the assignment operator works properly in all normal cases" << endl << endl;
        
        newCQueue = new CQueue(priorityFn2, MINHEAP, LEFTIST); //cqueue initialized

        //normal copy constructor tested
        cout << "normalAssignment starting with priorFn2, MINHEAP, LEFTIST: \n\t";
        bool testResult = tester.normalAssignment(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //normalAssignment tested again
        cout << "normalAssignment starting with priorFn2, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.normalAssignment(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //normalAssignment tested again
        cout << "normalAssignment starting with priorFn2, MINHEAP, SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MINHEAP, SKEW); //cqueue initialized
        testResult = tester.normalAssignment(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //normalAssignment tested again
        cout << "normalAssignment starting with priorFn2, MAXHEAP, SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, SKEW); //cqueue initialized
        testResult = tester.normalAssignment(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //normalAssignment then tested for one of the priority 1 function cases

        //normal copy constructor tested
        cout << "normalAssignment starting with priorFn1, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn1, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.normalAssignment(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        cout << "\n***END TEST BLOCK THIRTEEN***" << endl; 
    }
    {
        cout << "\n*** TEST BLOCK FOURTEEN ***" << endl << endl;
	    cout << "This will test to ensure that the assignment operator works properly in all edge cases" << endl << endl;
        
        newCQueue = new CQueue(priorityFn2, MINHEAP, LEFTIST); //cqueue initialized

        //edge copy constructor tested
        cout << "edgeAssignment starting with priorFn2, MINHEAP, LEFTIST: \n\t";
        bool testResult = tester.edgeAssignment(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //edgeAssignment tested again
        cout << "edgeAssignment starting with priorFn2, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.edgeAssignment(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //edgeAssignment tested again
        cout << "edgeAssignment starting with priorFn2, MINHEAP, SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MINHEAP, SKEW); //cqueue initialized
        testResult = tester.edgeAssignment(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //edgeAssignment tested again
        cout << "edgeAssignment starting with priorFn2, MAXHEAP, SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, SKEW); //cqueue initialized
        testResult = tester.edgeAssignment(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //edgeAssignment then tested for one of the priority 1 function cases

        //edge copy constructor tested
        cout << "edgeAssignment starting with priorFn1, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn1, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.edgeAssignment(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        cout << "\n***END TEST BLOCK FOURTEEN***" << endl; 
    }
    {
        cout << "\n*** TEST BLOCK FIFTEEN ***" << endl << endl;
        cout << "This will test to ensure that the copy constructor works properly in normal cases" << endl << endl;
        
        newCQueue = new CQueue(priorityFn2, MINHEAP, LEFTIST); //cqueue initialized

        //normal copy constructor tested
        cout << "normalCopyConstructor starting with priorFn2, MINHEAP, LEFTIST: \n\t";
        bool testResult = tester.normalCopyConstructor(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //normalCopyConstructor tested again
        cout << "normalCopyConstructor starting with priorFn2, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.normalCopyConstructor(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //normalCopyConstructor tested again
        cout << "normalCopyConstructor starting with priorFn2, MINHEAP, SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MINHEAP, SKEW); //cqueue initialized
        testResult = tester.normalCopyConstructor(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //normalCopyConstructor tested again
        cout << "normalCopyConstructor starting with priorFn2, MAXHEAP, SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, SKEW); //cqueue initialized
        testResult = tester.normalCopyConstructor(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //normalCopyConstructor then tested for one of the priority 1 function cases

        //normal copy constructor tested
        cout << "normalCopyConstructor starting with priorFn1, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn1, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.normalCopyConstructor(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        cout << "\n***END TEST BLOCK FIFTEEN***" << endl;   
    }
    {
        cout << "\n*** TEST BLOCK SIXTEEN ***" << endl << endl;
        cout << "This will test to ensure that the copy constructor works properly in all edge cases" << endl << endl;
        
        newCQueue = new CQueue(priorityFn2, MINHEAP, LEFTIST); //cqueue initialized

        //edge copy constructor tested
        cout << "edgeCopyConstructor starting with priorFn2, MINHEAP, LEFTIST: \n\t";
        bool testResult = tester.edgeCopyConstructor(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //edgeCopyConstructor tested again
        cout << "edgeCopyConstructor starting with priorFn2, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.edgeCopyConstructor(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //edgeCopyConstructor tested again
        cout << "edgeCopyConstructor starting with priorFn2, MINHEAP, SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MINHEAP, SKEW); //cqueue initialized
        testResult = tester.edgeCopyConstructor(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //edgeCopyConstructor tested again
        cout << "edgeCopyConstructor starting with priorFn2, MAXHEAP, SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, SKEW); //cqueue initialized
        testResult = tester.edgeCopyConstructor(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //edgeCopyConstructor then tested for one of the priority 1 function cases

        //edge copy constructor tested
        cout << "edgeCopyConstructor starting with priorFn1, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn1, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.edgeCopyConstructor(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        cout << "\n***END TEST BLOCK SIXTEEN ***" << endl;
    } 
    {
        cout << "\n*** TEST BLOCK SEVENTEEN ***" << endl << endl;
        cout << "This will test to ensure that the out of range error works properly for getNextOrder" << endl << endl;
        
        newCQueue = new CQueue(priorityFn2, MINHEAP, LEFTIST); //cqueue initialized

        //edge copy constructor tested
        cout << "errorDequeue starting with priorFn2, MINHEAP, LEFTIST: \n\t";
        bool testResult = tester.errorDequeue(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //edgeCopyConstructor tested again
        cout << "errorDequeue starting with priorFn2, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.errorDequeue(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //errorDequeue tested again
        cout << "errorDequeue starting with priorFn2, MINHEAP, SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MINHEAP, SKEW); //cqueue initialized
        testResult = tester.errorDequeue(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //errorDequeue tested again
        cout << "errorDequeue starting with priorFn2, MAXHEAP, SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, SKEW); //cqueue initialized
        testResult = tester.errorDequeue(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //errorDequeue then tested for one of the priority 1 function cases

        //errorDequeue tested
        cout << "errorDequeue starting with priorFn1, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn1, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.errorDequeue(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        cout << "\n***END TEST BLOCK SEVENTEEN***" << endl;
    } 
    {
        cout << "\n*** TEST BLOCK EIGHTEEN ***" << endl << endl;
        cout << "This will test to ensure that the domain error works properly for mergeWithQueue" << endl << endl;
        
        newCQueue = new CQueue(priorityFn2, MINHEAP, LEFTIST); //cqueue initialized

        //errorMerge tested
        cout << "errorMerge starting with priorFn2, MINHEAP, LEFTIST: \n\t";
        bool testResult = tester.errorMerge(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //errorMerge tested again
        cout << "errorMerge starting with priorFn2, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.errorMerge(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //errorMerge tested again
        cout << "errorMerge starting with priorFn2, MINHEAP, SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MINHEAP, SKEW); //cqueue initialized
        testResult = tester.errorMerge(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //errorMerge tested again
        cout << "errorMerge starting with priorFn2, MAXHEAP, SKEW: \n\t";
        newCQueue = new CQueue(priorityFn2, MAXHEAP, SKEW); //cqueue initialized
        testResult = tester.errorMerge(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        //errorMerge then tested for one of the priority 1 function cases

        //errorMerge tested
        cout << "errorMerge starting with priorFn1, MAXHEAP, LEFTIST: \n\t";
        newCQueue = new CQueue(priorityFn1, MAXHEAP, LEFTIST); //cqueue initialized
        testResult = tester.errorMerge(*newCQueue);
        tester.testCondition(testResult);
        delete newCQueue;

        cout << "\n***END TEST BLOCK EIGHTEEN ***" << endl;        
    }

    cout << "\n**********************" << endl;
    cout << "***** END TEST *******" << endl;
    cout << "**********************" << endl;

    return 0;
}

//priorityFunctions done

int priorityFn1(const Order &order) {
    //this function works with a MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [0-5003]
    //the highest priority would be 3+5000 = 5003
    //the lowest priority would be 0+0 = 0
    //the larger value means the higher priority
    int priority = static_cast<int>(order.getCount()) + order.getPoints();
    return priority;
}

int priorityFn2(const Order &order) {
    //this funcction works with a MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [0-10]
    //the highest priority would be 0+0 = 0
    //the lowest priority would be 5+5 =10
    //the smaller value means the higher priority
    int priority = static_cast<int>(order.getItem()) + static_cast<int>(order.getMemebership());
    return priority;
}

//testCondition
//displays the output of the test whether it passed or failed
void Tester::testCondition(bool var){
    if (var){
        cout << "This test has passed" << endl << endl;
    }
    else{
        cout << "This test has failed" << endl << endl;
    }
}

//randomFill
//fills the cQueue sent with random order
void Tester::randomFill(CQueue& aqueue, int numToFill){
    Random orderIdGen(MINORDERID,MAXORDERID);
    Random customerIdGen(MINCUSTID,MAXCUSTID);
    customerIdGen.setSeed(0);
    Random membershipGen(0,5); // there are six tiers
    Random pointsGen(MINPOINTS,MAXPOINTS);
    Random itemGen(0,5); // there are six items
    Random countGen(0,3); // there are three possible quantity values
    for (int i = 0; i < numToFill; i++){
        Order anOrder(static_cast<ITEM>(itemGen.getRandNum()),
                    static_cast<COUNT>(countGen.getRandNum()),
                    static_cast<MEMBERSHIP>(membershipGen.getRandNum()),
                    pointsGen.getRandNum(),
                    customerIdGen.getRandNum(),
                    orderIdGen.getRandNum());
        aqueue.insertOrder(anOrder);
    }
}

//minheapTest
//checks to ensure the MINHEAP property upholds for every node
bool Tester::minheapTest(bool result, const Node* curr, prifn_t priorFn){
    if (curr == nullptr){
        return result;
    }
    else{
        //minheap test called using preorder traversal
        result = result && minheapTest(result, curr->m_left, priorFn);
        result = result && minheapTest(result, curr->m_right, priorFn);
        
        //curr, left and right priority are declared and initialized
        //checks if nullptr, if so returns a value out of range so always tue
        int currPriority = priorFn(curr->getOrder());
        int leftPriority = ((curr->m_left == nullptr) ? 999999: priorFn(curr->m_left->getOrder()));
        int rightPriority = ((curr->m_right == nullptr) ? 999999: priorFn(curr->m_right->getOrder()));

        //if statement checks to make sure both are greater than curr, and if so true is returned
        if ((currPriority <= leftPriority) && (currPriority <= rightPriority)){
            return result && true;
        }
        else{
            return result && false; //else, false is returned
        }
    }
}

//maxheapTest
//recursive test which ensures maxheap property upholds
bool Tester::maxheapTest(bool result, const Node* curr, prifn_t priorFn){
    if (curr == nullptr){
        return result;
    }
    else{
        //maxheap test called using postorder traversal
        result = result && maxheapTest(result, curr->m_left, priorFn);
        result = result && maxheapTest(result, curr->m_right, priorFn);
        
        //curr, left and right priority are declared and initialized
        //checks if nullptr, if so returns a value out of range so always true
        int currPriority = priorFn(curr->getOrder());
        int leftPriority = ((curr->m_left == nullptr) ? -1: priorFn(curr->m_left->getOrder()));
        int rightPriority = ((curr->m_right == nullptr) ? -1: priorFn(curr->m_right->getOrder()));
        
        //if statement checks to make sure both are less than curr, and if so true is returned
        if ((currPriority >= leftPriority) && (currPriority >= rightPriority)){
            return result && true;
        }
        else{
            return result && false; //else, false is returned
        }
    }
}

//minheapInsertion
//Checks to ensure that the minheap property is upheld for every item inserted
bool Tester::minheapInsertion(CQueue& cqueue, int num){
    bool result = true;

    //first checks to ensure the cqueue sent is in fact a minheap
    result = result && (cqueue.m_heapType == MINHEAP);

    //random variables created
    Random orderIdGen(MINORDERID,MAXORDERID);
    Random customerIdGen(MINCUSTID,MAXCUSTID);
    customerIdGen.setSeed(0);
    Random membershipGen(0,5); // there are six tiers
    Random pointsGen(MINPOINTS,MAXPOINTS);
    Random itemGen(0,5); // there are six items
    Random countGen(0,3); // there are three possible quantity values

    //for loop goes through based on the number sent
    //after each insertion, the minheap property is tested to ensure it upholds
    for (int i = 0; i < num; i++){
        Order anOrder(static_cast<ITEM>(itemGen.getRandNum()),
                    static_cast<COUNT>(countGen.getRandNum()),
                    static_cast<MEMBERSHIP>(membershipGen.getRandNum()),
                    pointsGen.getRandNum(),
                    customerIdGen.getRandNum(),
                    orderIdGen.getRandNum());
        cqueue.insertOrder(anOrder);
        result = result && minheapTest(result, cqueue.m_heap, cqueue.m_priorFunc);
    }

    //checks to ensure size is equal to num
    result = result && (cqueue.m_size == num);

    //after testing, the result is then returned
    return result;
}

//minheapRemoval
//removes elements from a minheap to ensure that the property still upholds
bool Tester::minheapRemoval(CQueue& cqueue, int num){
    bool result = true;

    //first checks to ensure cqueue is a minheap
    result = result && (cqueue.m_heapType == MINHEAP);

    //calls upon minheapInsertion to fill the cqueue
    result = result && minheapInsertion(cqueue, num);

    //array created for popped orders
    Order poppedOrders[num];
    try{
        //first, half of the nodes are removed
        //after each removal, checks to ensure leftist heap is still upheld
        //every order popped off is then sent to an array
        for (int i = 0; i < int(num/2); i++){
            poppedOrders[i] = cqueue.getNextOrder(); //popped order added to array
            result = result && minheapTest(result, cqueue.m_heap, cqueue.m_priorFunc);
            
            //if statement checks to see if the popped order has a greater than or equal to priority than the previous one popped
            if (i > 0){
                result = result && (cqueue.m_priorFunc(poppedOrders[i]) >= cqueue.m_priorFunc(poppedOrders[i -1]));
            } 
        }

        //checks to ensure cqueue size is half of what it was previous
        if (num % 2 == 0){
            result = result && (cqueue.m_size == int(num/2));
        }
        else{
            result = result && (cqueue.m_size == int(num/2) + 1);
        }

        //one final test to ensure property is upheld
        result = result && minheapTest(result, cqueue.m_heap, cqueue.m_priorFunc);

        //the final half are then removed
        for (int i = num/2; i < num; i++){
            poppedOrders[i] = cqueue.getNextOrder(); //popped order added to array
            result = result && minheapTest(result, cqueue.m_heap, cqueue.m_priorFunc);
            
            //array checks again to ensure property is upheld
            result = result && (cqueue.m_priorFunc(poppedOrders[i]) >= cqueue.m_priorFunc(poppedOrders[i -1]));
        }

        //checks if m_size is zero and if m_heap is a nullptr, which it should be
        result = result && (cqueue.m_size == 0);
        result = result && (cqueue.m_heap == nullptr);
    }
    //catches out of range and returns false
    catch(const out_of_range &range){
        result = false;
    }
    return result;
}

//maxheapInsertion
//Checks to ensure that the maxheap property is upheld for every item inserted
bool Tester::maxheapInsertion(CQueue& cqueue, int num){
    bool result = true;

    //first checks to ensure the cqueue sent is in fact a maxheap
    result = result && (cqueue.m_heapType == MAXHEAP);

    //random variables created
    Random orderIdGen(MINORDERID,MAXORDERID);
    Random customerIdGen(MINCUSTID,MAXCUSTID);
    customerIdGen.setSeed(0);
    Random membershipGen(0,5); // there are six tiers
    Random pointsGen(MINPOINTS,MAXPOINTS);
    Random itemGen(0,5); // there are six items
    Random countGen(0,3); // there are three possible quantity values

    //for loop goes through based on the number sent
    //after each insertion, the maxheap property is tested to ensure it upholds
    for (int i = 0; i < num; i++){
        Order anOrder(static_cast<ITEM>(itemGen.getRandNum()),
                    static_cast<COUNT>(countGen.getRandNum()),
                    static_cast<MEMBERSHIP>(membershipGen.getRandNum()),
                    pointsGen.getRandNum(),
                    customerIdGen.getRandNum(),
                    orderIdGen.getRandNum());
        cqueue.insertOrder(anOrder);
        result = result && maxheapTest(result, cqueue.m_heap, cqueue.m_priorFunc);
    }

    //checks to ensure size is equal to num
    result = result && (cqueue.m_size == num);

    //after testing, the result is then returned
    return result;
}

//maxheapRemoval
//removes elements from a maxheap to ensure that the property still upholds
bool Tester::maxheapRemoval(CQueue& cqueue, int num){
    bool result = true;

    //first checks to ensure cqueue is a maxheap
    result = result && (cqueue.m_heapType == MAXHEAP);

    //calls upon maxheapInsertion to fill the cqueue
    result = result && maxheapInsertion(cqueue, num);

    //array created for popped orders
    Order poppedOrders[num];
    
    //try catch block used to ensure no errors occur
    try{
        //for loop goes through and removes half of the nodes
        //each removal is popped onto the array, which is then checked to ensure as the index becomes greater, the numbers are lower
        for (int i = 0; i < int(num/2); i++){
            poppedOrders[i] = cqueue.getNextOrder(); //popped order added to array
            result = result && maxheapTest(result, cqueue.m_heap, cqueue.m_priorFunc); //tested each time to ensure maxheap property upholds
            
            //if statement checks to see if the popped order has a less than or equal to priority than the previous one popped
            //i > 0 to prevent seg faults
            if (i > 0){
                result = result && (cqueue.m_priorFunc(poppedOrders[i]) <= cqueue.m_priorFunc(poppedOrders[i -1]));
            } 
        }

        //checks to ensure cqueue size is half of what it was previous
        if (num % 2 == 0){
            result = result && (cqueue.m_size == int(num/2));
        }
        else{
            result = result && (cqueue.m_size == int(num/2) + 1);
        }

        //one final test to ensure property is upheld
        result = result && maxheapTest(result, cqueue.m_heap, cqueue.m_priorFunc);

        //the final half are then removed, maxheap property once again tested
        for (int i = num/2; i < num; i++){
            poppedOrders[i] = cqueue.getNextOrder(); //popped order added to array
            result = result && maxheapTest(result, cqueue.m_heap, cqueue.m_priorFunc);
            
            //array checks again to ensure property is upheld
            result = result && (cqueue.m_priorFunc(poppedOrders[i]) <= cqueue.m_priorFunc(poppedOrders[i -1]));
        }

        //checks if m_size is zero and if m_heap is a nullptr, which it should be
        result = result && (cqueue.m_size == 0);
        result = result && (cqueue.m_heap == nullptr);
    }
    //catches out of range and returns false
    catch(const out_of_range &range){
        result = false;
    }
    return result;
}

//leftistNPLProperty
//Tests to ensure that the leftist heap stays consistent after insertions
bool Tester::leftistNPLProperty(CQueue& cqueue, int num){
    bool result = true;

    //first checks to ensure that the cqueue is in fact leftist
    result = result && (cqueue.m_structure == LEFTIST);

    //random variables created
    Random orderIdGen(MINORDERID,MAXORDERID);
    Random customerIdGen(MINCUSTID,MAXCUSTID);
    customerIdGen.setSeed(0);
    Random membershipGen(0,5); // there are six tiers
    Random pointsGen(MINPOINTS,MAXPOINTS);
    Random itemGen(0,5); // there are six items
    Random countGen(0,3); // there are three possible quantity values

    //for loop inserts as many nodes as the user would like, and the NPL property is checked each time
    for (int i = 0; i < num; i++){
        Order anOrder(static_cast<ITEM>(itemGen.getRandNum()),
                    static_cast<COUNT>(countGen.getRandNum()),
                    static_cast<MEMBERSHIP>(membershipGen.getRandNum()),
                    pointsGen.getRandNum(),
                    customerIdGen.getRandNum(),
                    orderIdGen.getRandNum());
        cqueue.insertOrder(anOrder);
        result = result && NPLTest(result, cqueue.m_heap);
    }

    //checks to ensure size is equal to num
    result = result && (cqueue.m_size == num);

    //if statement determines which type of heap it is, and checks to ensure the heapType property is upheld
    if (cqueue.m_heapType == MINHEAP){
        result = result && minheapTest(result, cqueue.m_heap, cqueue.m_priorFunc);
    }
    else{
        result = result && maxheapTest(result, cqueue.m_heap, cqueue.m_priorFunc);
    }

    //all nodes are then removed, and NPL test is run again to ensure it upholds
    for (int i = 0; i < num; i++){
        cqueue.getNextOrder();
        result = result && NPLTest(result, cqueue.m_heap);
    }

    //checks to ensure m_size is zero
    result = result && (cqueue.m_size == 0);

    return result; //result returned
}

//leftistHeapTest
//Checks to ensure that the leftist node upholds for every insertion
bool Tester::leftistHeapTest(CQueue& cqueue, int num){
    bool result = true;

    //first checks to ensure that the cqueue is in fact leftist
    result = result && (cqueue.m_structure == LEFTIST);

    //random variables created
    Random orderIdGen(MINORDERID,MAXORDERID);
    Random customerIdGen(MINCUSTID,MAXCUSTID);
    customerIdGen.setSeed(0);
    Random membershipGen(0,5); // there are six tiers
    Random pointsGen(MINPOINTS,MAXPOINTS);
    Random itemGen(0,5); // there are six items
    Random countGen(0,3); // there are three possible quantity values

    //for loop inserts as many nodes as the user would like, and the leftist test is run each time
    for (int i = 0; i < num; i++){
        Order anOrder(static_cast<ITEM>(itemGen.getRandNum()),
                    static_cast<COUNT>(countGen.getRandNum()),
                    static_cast<MEMBERSHIP>(membershipGen.getRandNum()),
                    pointsGen.getRandNum(),
                    customerIdGen.getRandNum(),
                    orderIdGen.getRandNum());
        cqueue.insertOrder(anOrder);
        result = result && leftistTest(result, cqueue.m_heap);
    }

    //checks to ensure size is equal to num, and the NPL property is also checked
    result = result && (cqueue.m_size == num);
    result = result && NPLTest(result, cqueue.m_heap);

    //if statement determines which type of heap it is, and checks to ensure the heapType property is upheld
    if (cqueue.m_heapType == MINHEAP){
        result = result && minheapTest(result, cqueue.m_heap, cqueue.m_priorFunc);
    }
    else{
        result = result && maxheapTest(result, cqueue.m_heap, cqueue.m_priorFunc);
    }

    //all nodes are then removed, and leftist test is run again to ensure it upholds
    for (int i = 0; i < num; i++){
        cqueue.getNextOrder();
        result = result && leftistTest(result, cqueue.m_heap);
    }

    //checks to ensure m_size is zero
    result = result && (cqueue.m_size == 0);

    return result; //result returned
}

//NPLTest
//Checks the node of every node to ensure that the updateNPL function works properly
bool Tester::NPLTest(bool result, const Node* curr){
    if (curr == nullptr){
        return result;
    }
    else{
        //result called for left and right nodes for postorder traversal
        result = result && NPLTest(result, curr->m_left);
        result = result && NPLTest(result, curr->m_right);
        
        //two if statements used to check the NPL of both left and right
        //ternary operator used to prevent seg faults
        int leftNPL = ((curr->m_left == nullptr) ? 0 : curr->m_left->getNPL());
        int rightNPL = ((curr->m_right == nullptr) ? 0 : curr->m_right->getNPL());

        //correctNPL is created based on which NPL is lower, as it should have the lower value
        int correctNPL = 1 + ((leftNPL < rightNPL) ? leftNPL : rightNPL);

        //if statement checks to ensure the NPL of curr is equal to the NPL it should be
        if (curr->getNPL() == correctNPL){
            return true;
        }
        else{
            return false;
        }
    }
}

//leftistTest
//Checks to ensure that the leftist property of the left node having a greater NPL than the right is upheld
bool Tester::leftistTest(bool result, const Node* curr){
    if (curr == nullptr){
        return result;
    }
    else{
        //result called for left and right nodes for postorder traversal
        result = result && leftistTest(result, curr->m_left);
        result = result && leftistTest(result, curr->m_right);
        
        //two if statements used to check the NPL of both left and right
        //ternary operator used to prevent seg faults
        int leftNPL = ((curr->m_left == nullptr) ? 0 : curr->m_left->getNPL());
        int rightNPL = ((curr->m_right == nullptr) ? 0 : curr->m_right->getNPL());

        //if statement checks to ensure the NPL of the left node is greater than the right one
        if (leftNPL >= rightNPL){
            return true;
        }
        //else, false is returned, since the right node should not be greater
        else{
            return false;
        }
    }
}

//testPriorityFunction
//tests the setter for priority function to ensure it works properly
bool Tester::testPriorityFunction(CQueue& cqueue){
    bool result = true;
    
    //current values of the heap are saved for later use
    prifn_t currFn = cqueue.m_priorFunc;
    HEAPTYPE currHeap = cqueue.m_heapType;
    //opposite values determines which values are opposite, and assigns the opposite
    HEAPTYPE oppHeap = ((cqueue.m_heapType == MINHEAP) ? MAXHEAP : MINHEAP);
    prifn_t oppFn = ((cqueue.m_priorFunc == priorityFn2) ? priorityFn1 : priorityFn2);


    //cqueue randomly filled, however an array keeps track of all of the order IDs as being inserted
    //array created
    int orderIDs[NORMAL_CASE];
    
    //random variables created
    Random orderIdGen(MINORDERID,MAXORDERID);
    Random customerIdGen(MINCUSTID,MAXCUSTID);
    customerIdGen.setSeed(0);
    Random membershipGen(0,5); // there are six tiers
    Random pointsGen(MINPOINTS,MAXPOINTS);
    Random itemGen(0,5); // there are six items
    Random countGen(0,3); // there are three possible quantity values

    //for loop inserts a normal case amount of nodes, and list of order IDs added to array
    for (int i = 0; i < NORMAL_CASE; i++){
        Order anOrder(static_cast<ITEM>(itemGen.getRandNum()),
                    static_cast<COUNT>(countGen.getRandNum()),
                    static_cast<MEMBERSHIP>(membershipGen.getRandNum()),
                    pointsGen.getRandNum(),
                    customerIdGen.getRandNum(),
                    orderIdGen.getRandNum());
        cqueue.insertOrder(anOrder);
        orderIDs[i] = anOrder.getOrderID();
    }

    int tempint = cqueue.m_size; //m_size saved for later check

    //for loop is done so that the item switches to the opposite heap, then back
    for (int i = 0; i < 2; i++){
        
        //if statement changes depending on which iteration it is
        if (i % 2 == 0){
            //first test done is changing the heap from a MINHEAP to MAXHEAP, or vice versa, with the same priority function
            cqueue.setPriorityFn(cqueue.m_priorFunc, oppHeap); 
            result = result && (cqueue.m_heapType == oppHeap);         
        }
        else{
            //set back to original state and compared to previous node
            cqueue.setPriorityFn(cqueue.m_priorFunc, currHeap);
            //first checks to ensure that the heap type actually changed, and that m_size is the same
            result = result && (cqueue.m_heapType == currHeap);
        }

        result = result && (cqueue.m_size == tempint);

        //for loop goes through and ensures each orderID is found
        for (int i = 0; i < NORMAL_CASE; i++){
            result = result && find(false, cqueue.m_heap, orderIDs[i]);
        }

        //then if statement checks to ensure properties hold up
        if ((cqueue.m_heapType == MINHEAP) && (cqueue.m_structure == LEFTIST)){
            result = result && minheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
            result = result && leftistTest(result, cqueue.m_heap);
        }
        else if ((cqueue.m_heapType == MAXHEAP) && (cqueue.m_structure == LEFTIST)){
            result = result && maxheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
            result = result && leftistTest(result, cqueue.m_heap);
        }
        else if ((cqueue.m_heapType == MINHEAP) && (cqueue.m_structure == SKEW)){
            result = result && minheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
        }
        else{
            result = result && maxheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
        }
    }

    //next test is done is to test changing the priority function to the opposite to see if changes occur
    //heap type is also changed
    //for loop goes three times to test the changes to see if they uphold
    for (int i = 0; i < 3; i++){
        switch (i){
            case 0:
                cqueue.setPriorityFn(oppFn, currHeap);  
                //checks to ensure the member variables check
                result = result && (cqueue.m_heapType == currHeap);
                result = result && (cqueue.m_priorFunc == oppFn);
                break;
            case 1:
                cqueue.setPriorityFn(oppFn, oppHeap);
                //checks to ensure the member variables check
                result = result && (cqueue.m_heapType == oppHeap);
                result = result && (cqueue.m_priorFunc == oppFn);
                break;
            //case 2 returns the function back to start, and checks to see if it remains the same as check
            case 2:
                cqueue.setPriorityFn(currFn, currHeap);
                //checks to ensure the member variables check
                result = result && (cqueue.m_heapType == currHeap);
                result = result && (cqueue.m_priorFunc == currFn);
                break;
            //default is just set to basic change
            default:
                cqueue.setPriorityFn(oppFn, oppHeap);
                //checks to ensure the member variables check
                result = result && (cqueue.m_heapType == oppHeap);
                result = result && (cqueue.m_priorFunc == oppFn);
        }

        //size checked each time to ensure it didn't change
        result = result && (cqueue.m_size == tempint);

        //for loop goes through and ensures each orderID is found
        for (int i = 0; i < NORMAL_CASE; i++){
            result = result && find(false, cqueue.m_heap, orderIDs[i]);
        }

        //then if statement checks to ensure properties hold up
        if ((cqueue.m_heapType == MINHEAP) && (cqueue.m_structure == LEFTIST)){
            result = result && minheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
            result = result && leftistTest(result, cqueue.m_heap);
        }
        else if ((cqueue.m_heapType == MAXHEAP) && (cqueue.m_structure == LEFTIST)){
            result = result && maxheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
            result = result && leftistTest(result, cqueue.m_heap);
        }
        else if ((cqueue.m_heapType == MINHEAP) && (cqueue.m_structure == SKEW)){
            result = result && minheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
        }
        else{
            result = result && maxheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
        }
    }

    CQueue* check = new CQueue(cqueue);

    //final checks that if the heap and priorfn is the same as it currently is, nothing changes
    cqueue.setPriorityFn(currFn, currHeap);
    result = result && assignmentHelper(result, cqueue.m_heap, check->m_heap);
    
    delete check; //check deleted
    return result; //result returned
}

//testSetStructure
//checks to ensure that the structure changes
bool Tester::testSetStructure(CQueue& cqueue){
    bool result = true;
    
    //current and opposite of each value kept track of
    STRUCTURE currStruct = cqueue.m_structure;
    STRUCTURE oppStruct = ((cqueue.m_structure == LEFTIST) ? SKEW: LEFTIST);

    prifn_t currFn = cqueue.m_priorFunc;
    prifn_t oppFn = ((cqueue.m_priorFunc == priorityFn2) ? priorityFn1 : priorityFn2);

    HEAPTYPE currHeap = cqueue.m_heapType;
    HEAPTYPE oppHeap = ((cqueue.m_heapType == MINHEAP) ? MAXHEAP : MINHEAP);
    
    
    //cqueue randomly filled, however an array keeps track of all of the order IDs as being inserted
    //array created
    int orderIDs[NORMAL_CASE];
    
    //random variables created
    Random orderIdGen(MINORDERID,MAXORDERID);
    Random customerIdGen(MINCUSTID,MAXCUSTID);
    customerIdGen.setSeed(0);
    Random membershipGen(0,5); // there are six tiers
    Random pointsGen(MINPOINTS,MAXPOINTS);
    Random itemGen(0,5); // there are six items
    Random countGen(0,3); // there are three possible quantity values

    //for loop inserts a normal case amount of nodes, and list of order IDs added to array
    for (int i = 0; i < NORMAL_CASE; i++){
        Order anOrder(static_cast<ITEM>(itemGen.getRandNum()),
                    static_cast<COUNT>(countGen.getRandNum()),
                    static_cast<MEMBERSHIP>(membershipGen.getRandNum()),
                    pointsGen.getRandNum(),
                    customerIdGen.getRandNum(),
                    orderIdGen.getRandNum());
        cqueue.insertOrder(anOrder);
        orderIDs[i] = anOrder.getOrderID();
    }

    int tempint = cqueue.m_size; //m_size saved for later check
    
    //for loop goes through and changes 
    for (int i = 0; i < 5; i++){
        //switch statement changes depending on the scenario
        switch(i){
            //case 0 changes it to the opposite struct
            case 0:
                cqueue.setStructure(oppStruct); //setStructure called
                
                //checks to ensure the member variables changed or remained the same
                result = result && (cqueue.m_structure == oppStruct);
                result = result && (cqueue.m_heapType == currHeap);
                result = result && (cqueue.m_priorFunc == currFn);
                break;
            //case 1 changes the structure back to the original
            case 1:
                cqueue.setStructure(currStruct); //setStructure called and back to original struct
                
                //checks to ensure the member variables changed or remained the same
                result = result && (cqueue.m_structure == currStruct);
                result = result && (cqueue.m_heapType == currHeap);
                result = result && (cqueue.m_priorFunc == currFn);
                break;
            //in case 2, everything is changed to the opposite
            case 2:
                cqueue.setStructure(oppStruct);
                cqueue.setPriorityFn(oppFn, oppHeap);

                //checks to ensure the member variables changed or remained the same
                result = result && (cqueue.m_structure == oppStruct);
                result = result && (cqueue.m_heapType == oppHeap);
                result = result && (cqueue.m_priorFunc == oppFn);

                break;
            //case 3 is that the structure is set back to the original, but the heapType are still the opposite
            case 3:
                cqueue.setStructure(currStruct);

                //checks to ensure the member variables changed or remained the same
                result = result && (cqueue.m_structure == currStruct);
                result = result && (cqueue.m_heapType == oppHeap);
                result = result && (cqueue.m_priorFunc == oppFn);

                break;
            //default is that it is changed to the original variables, reached when i = 4
            default:
                cqueue.setStructure(currStruct);
                cqueue.setPriorityFn(currFn, currHeap);

                //checks to ensure the member variables changed or remained the same
                result = result && (cqueue.m_structure == currStruct);
                result = result && (cqueue.m_heapType == currHeap);
                result = result && (cqueue.m_priorFunc == currFn);
        }

        //size checked each time to ensure it didn't change
        result = result && (cqueue.m_size == tempint);

        //for loop goes through and ensures each orderID is found
        for (int i = 0; i < NORMAL_CASE; i++){
            result = result && find(false, cqueue.m_heap, orderIDs[i]);
        }

        //then if statement checks to ensure properties hold up
        if ((cqueue.m_heapType == MINHEAP) && (cqueue.m_structure == LEFTIST)){
            result = result && minheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
            result = result && leftistTest(result, cqueue.m_heap);
        }
        else if ((cqueue.m_heapType == MAXHEAP) && (cqueue.m_structure == LEFTIST)){
            result = result && maxheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
            result = result && leftistTest(result, cqueue.m_heap);
        }
        else if ((cqueue.m_heapType == MINHEAP) && (cqueue.m_structure == SKEW)){
            result = result && minheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
        }
        else{
            result = result && maxheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
        }
    }

    CQueue* check = new CQueue(cqueue);

    //final checks that if the structure is the same as it currently is, nothing changes
    cqueue.setStructure(currStruct);
    result = result && assignmentHelper(result, cqueue.m_heap, check->m_heap);
    
    delete check; //check deleted
    return result;
}

//find
//helper function which finds IDs within the a CQueue
bool Tester::find(bool result, const Node* curr, int orderID){
    if (curr == nullptr){
        return false;
    }
    else{
        //postorder traversal of the object
        result = result || find(result, curr->m_left, orderID);
        result = result || find(result, curr->m_right, orderID);
        
        //if order ID is found, true is returned
        if (curr->getOrder().getOrderID() == orderID){
            return true;
        }
        else{
            return result || false; //else, false is returned
        }
    }
}

//edgeMergeWithQueue
//tests edge cases of mergeWithQueue
bool Tester::edgeMergeWithQueue(CQueue& cqueue){
    bool result = true;
    
    //try block ensures that no domain errors occur
    try{

        //testCQueue created with the cqueue sent
        CQueue* testCQueue = new CQueue(cqueue);

        //cqueue is then randomly filled
        randomFill(cqueue, NORMAL_CASE);

        //once it is filled, it is attempted to be merged with the other queue
        cqueue.mergeWithQueue(*testCQueue);

        //checks to ensure that testCQueue remains empty
        result = result && (testCQueue->m_heap == nullptr);
        result = result && (testCQueue->m_size == 0);

        //cqueue cleared, testCQueue filled, and tested again
        cqueue.clear();
        randomFill(*testCQueue, NORMAL_CASE);

        //mergeWithQueue called for this edge case
        cqueue.mergeWithQueue(*testCQueue);

        //checks to ensure that testCQueue became empty
        result = result && (testCQueue->m_heap == nullptr);
        result = result && (testCQueue->m_size == 0);        

        //checks and sees if the size changed
        result = result && (cqueue.m_size == NORMAL_CASE);

        delete testCQueue; //testCQueue deleted
    }
    //catch exists to catch any potential domain_errors, and changes result to false
    catch(const domain_error &domain){
        result = false;
    }

    return result;
}

//testMergeWithQueue
//tests the mergeWithQueue function to ensure that it merges properly
bool Tester::testMergeWithQueue(CQueue& cqueue){
    bool result = true;
    
    //current and opposite heaps and priorityFunctions kept track up
    prifn_t currFn = cqueue.m_priorFunc;
    HEAPTYPE currHeap = cqueue.m_heapType;
    
    //function takes place in the try catch block in case of any potential exceptions thrown
    try{
        //testCQueue declared
        CQueue* testCQueue = new CQueue(currFn, currHeap, cqueue.m_structure);
        
        randomFill(cqueue, NORMAL_CASE); //cqueue randomly filled
        //testCQueue randomly filled then merged with original queue
        randomFill(*testCQueue, NORMAL_CASE);
        cqueue.mergeWithQueue(*testCQueue);

        //checks to ensure that testCQueue became empty
        result = result && (testCQueue->m_heap == nullptr);
        result = result && (testCQueue->m_size == 0);

        //testCQueue deleted
        delete testCQueue;

        //checks to see if m_size multipled by 2
        result = result && (cqueue.m_size == NORMAL_CASE *2);

        //checks to ensure nothing changed
        result = result && (cqueue.m_heapType == currHeap);
        result = result && (cqueue.m_priorFunc == currFn);

        //then if statement checks to ensure properties hold up
        if ((cqueue.m_heapType == MINHEAP) && (cqueue.m_structure == LEFTIST)){
            result = result && minheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
            result = result && leftistTest(result, cqueue.m_heap);
        }
        else if ((cqueue.m_heapType == MAXHEAP) && (cqueue.m_structure == LEFTIST)){
            result = result && maxheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
            result = result && leftistTest(result, cqueue.m_heap);
        }
        else if ((cqueue.m_heapType == MINHEAP) && (cqueue.m_structure == SKEW)){
            result = result && minheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
        }
        else{
            result = result && maxheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
        }
    }
    //catch exists to catch any potential domain_errors, and changes result to false
    catch(const domain_error &domain){
        result = false;
    }

    return result; //result returned
}

//normalAssignment
//tests the assignment operator in normal cases
bool Tester::normalAssignment(CQueue& cqueue){
    bool result = true;

    //randomFill called
    randomFill(cqueue, NORMAL_CASE);

    //opposite values determines which values are opposite, and creates the opposite
    HEAPTYPE oppHeap = ((cqueue.m_heapType == MINHEAP) ? MAXHEAP : MINHEAP);
    STRUCTURE oppStruct = ((cqueue.m_structure == LEFTIST) ? SKEW: LEFTIST);

    //three queues created, one with the copy constructor, one with the same as the sent queue and the other with opposite
    CQueue* sameQueue = new CQueue(cqueue); 
    CQueue* newQueue = new CQueue(cqueue.m_priorFunc, cqueue.m_heapType, cqueue.m_structure);
    CQueue* oppQueue = new CQueue(cqueue.m_priorFunc, oppHeap, oppStruct);

    //first checked is sameQueue to ensure it is exactly the same as cqueue, but in different locations
    result = result && (cqueue.m_heap != sameQueue->m_heap);
    result = result && assignmentHelper(result, cqueue.m_heap, sameQueue->m_heap);
    
    //newQueue and oppQueue are both filled, newQueue plus one to prevent potential repeats
    randomFill(*newQueue, NORMAL_CASE + 1);
    randomFill(*oppQueue, NORMAL_CASE);

    //first assignment is to the newQueue, to see if everything changed properly
    cqueue = *newQueue;
    //member variables are checked
    result = result && (cqueue.m_size == newQueue->m_size);
    result = result && (cqueue.m_heapType == newQueue->m_heapType);
    result = result && (cqueue.m_structure == newQueue->m_structure);
    result = result && (cqueue.m_priorFunc == newQueue->m_priorFunc);
    result = result && (cqueue.m_heap != newQueue->m_heap);

    //assignment helper called for final check, and also checked to ensure not equal to same heap
    result = result && assignmentHelper(result, cqueue.m_heap, newQueue->m_heap);
    result = result && !(assignmentHelper(result, cqueue.m_heap, sameQueue->m_heap));

    //tests then run to ensure properties hold up
    if ((cqueue.m_heapType == MINHEAP) && (cqueue.m_structure == LEFTIST)){
        result = result && minheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
        result = result && leftistTest(result, cqueue.m_heap);
    }
    else if ((cqueue.m_heapType == MAXHEAP) && (cqueue.m_structure == LEFTIST)){
        result = result && maxheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
        result = result && leftistTest(result, cqueue.m_heap);
    }
    else if ((cqueue.m_heapType == MINHEAP) && (cqueue.m_structure == SKEW)){
        result = result && minheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
    }
    else{
        result = result && maxheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
    }

    //the second one is then on oppHeap, assignment performed
    cqueue = *oppQueue;
    //member variables are checked   
    result = result && (cqueue.m_size == oppQueue->m_size);
    result = result && (cqueue.m_heapType == oppQueue->m_heapType);
    result = result && (cqueue.m_structure == oppQueue->m_structure);
    result = result && (cqueue.m_priorFunc == oppQueue->m_priorFunc);
    result = result && (cqueue.m_heap != oppQueue->m_heap);

    //assignment helper called for final check, and also checked to ensure not equal to same heap
    result = result && assignmentHelper(result, cqueue.m_heap, oppQueue->m_heap);
    result = result && !(assignmentHelper(result, cqueue.m_heap, newQueue->m_heap));

    //tests then run to ensure properties hold up
    if ((cqueue.m_heapType == MINHEAP) && (cqueue.m_structure == LEFTIST)){
        result = result && minheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
        result = result && leftistTest(result, cqueue.m_heap);
    }
    else if ((cqueue.m_heapType == MAXHEAP) && (cqueue.m_structure == LEFTIST)){
        result = result && maxheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
        result = result && leftistTest(result, cqueue.m_heap);
    }
    else if ((cqueue.m_heapType == MINHEAP) && (cqueue.m_structure == SKEW)){
        result = result && minheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
    }
    else{
        result = result && maxheapTest(result, cqueue.m_heap, cqueue.getPriorityFn());
    }

    //all three temporary CQueues deleted
    delete sameQueue;
    delete newQueue; 
    delete oppQueue;
    return result;
}

//edgeAssignment
//Tests the assignment operator in edge cases
bool Tester::edgeAssignment(CQueue& cqueue){
    bool result = true;

    //CQueue declared and initialized
    CQueue* newQueue = new CQueue(cqueue.m_priorFunc, cqueue.m_heapType, cqueue.m_structure);

    //assignment used with cQueue and newQueue
    cqueue = *newQueue;
    //all member variables checked, and ensured it is a nullptr
    result = result && (cqueue.m_size == 0);
    result = result && (cqueue.m_heapType == newQueue->m_heapType);
    result = result && (cqueue.m_structure == newQueue->m_structure);
    result = result && (cqueue.m_priorFunc == newQueue->m_priorFunc);
    result = result && (cqueue.m_heap == nullptr);

    //newQueue deleted, then one object inserted
    delete newQueue;
    newQueue = new CQueue(cqueue.m_priorFunc, cqueue.m_heapType, cqueue.m_structure);
    randomFill(*newQueue, 1);

    //assignment used with cQueue and newQueue
    cqueue = *newQueue;
    //all member variables checked, and ensured it is a nullptr
    result = result && (cqueue.m_size == 1);
    result = result && (cqueue.m_heapType == newQueue->m_heapType);
    result = result && (cqueue.m_structure == newQueue->m_structure);
    result = result && (cqueue.m_priorFunc == newQueue->m_priorFunc);
    result = result && (cqueue.m_heap != nullptr);
    result = result && (cqueue.m_heap->getOrder().getOrderID() == newQueue->m_heap->getOrder().getOrderID());

    delete newQueue;
    //result returned
    return result;
}

//assignmentHelper
//Helps to ensure that both heaps are equal to each other data wise, but not memory wise
bool Tester::assignmentHelper(bool result, const Node* lhsPtr, const Node* rhsPtr){
    if ((lhsPtr == nullptr) && (rhsPtr == nullptr)){
        return result && true;
    }
    else if ((lhsPtr != nullptr) && (rhsPtr != nullptr)){
        bool result = true;

        //m_left and m_right are both checked
        result = result && assignmentHelper(result, lhsPtr->m_left, rhsPtr->m_left);
        result = result && assignmentHelper(result, lhsPtr->m_right, rhsPtr->m_right);
        
        //if statement checks to ensure all orders are exactly the same by checking the ID, which was established to be unique
        if ((lhsPtr->getOrder().getOrderID() == rhsPtr->getOrder().getOrderID())){
            //second if statement checks to ensure the ptrs do not equal each other, and also checks some other variables to ensure also the same
            if ((lhsPtr != rhsPtr) && (lhsPtr->m_npl == rhsPtr->m_npl)){
                return result && true;
            }
            else{
                return result && false; //else, false is returned
            }
        }
        else{
            return result && false; //else, false is returned
        }
    }
    else{
        return result && false; //else, result and false is returned
    }  
}

//normalCopyConstructor
//tests copy constructor for normal cases
bool Tester::normalCopyConstructor(CQueue& cqueue){
    bool result = true;

    //randomFill called
    randomFill(cqueue, NORMAL_CASE);

    //first testQueue created to test if equal
    CQueue* sameQueue = new CQueue(cqueue); 

    //first checked is sameQueue to ensure it is exactly the same as cqueue, but in different locations
    result = result && assignmentHelper(result, cqueue.m_heap, sameQueue->m_heap);

    //then all member variables are checked
    result = result && (cqueue.m_size == sameQueue->m_size);
    result = result && (cqueue.m_heapType == sameQueue->m_heapType);
    result = result && (cqueue.m_structure == sameQueue->m_structure);
    result = result && (cqueue.m_priorFunc == sameQueue->m_priorFunc);
    result = result && (cqueue.m_heap != sameQueue->m_heap);

    //tests then run to ensure properties hold up
    if ((sameQueue->m_heapType == MINHEAP) && (sameQueue->m_structure == LEFTIST)){
        result = result && minheapTest(result, sameQueue->m_heap, sameQueue->getPriorityFn());
        result = result && leftistTest(result, sameQueue->m_heap);
    }
    else if ((sameQueue->m_heapType == MAXHEAP) && (sameQueue->m_structure == LEFTIST)){
        result = result && maxheapTest(result, sameQueue->m_heap, sameQueue->getPriorityFn());
        result = result && leftistTest(result, sameQueue->m_heap);
    }
    else if ((sameQueue->m_heapType == MINHEAP) && (sameQueue->m_structure == SKEW)){
        result = result && minheapTest(result, sameQueue->m_heap, sameQueue->getPriorityFn());
    }
    else{
        result = result && maxheapTest(result, sameQueue->m_heap, sameQueue->getPriorityFn());
    }

    //cqueue cleared, then refilled
    cqueue.clear();

    randomFill(cqueue, NORMAL_CASE + 25);

    delete sameQueue; //sameQueue deleted
    //double copy constructor done, to show that when copied twice, it'll still equal the same as the original
    CQueue* tempQueue = new CQueue(cqueue);
    sameQueue = new CQueue(*tempQueue);

    //first checked is sameQueue to ensure it is exactly the same as cqueue, but in different locations
    result = result && assignmentHelper(result, cqueue.m_heap, sameQueue->m_heap);

    //then all member variables are checked
    result = result && (cqueue.m_size == sameQueue->m_size);
    result = result && (cqueue.m_heapType == sameQueue->m_heapType);
    result = result && (cqueue.m_structure == sameQueue->m_structure);
    result = result && (cqueue.m_priorFunc == sameQueue->m_priorFunc);
    result = result && (cqueue.m_heap != sameQueue->m_heap);

    //tests then run to ensure properties hold up
    if ((sameQueue->m_heapType == MINHEAP) && (sameQueue->m_structure == LEFTIST)){
        result = result && minheapTest(result, sameQueue->m_heap, sameQueue->getPriorityFn());
        result = result && leftistTest(result, sameQueue->m_heap);
    }
    else if ((sameQueue->m_heapType == MAXHEAP) && (sameQueue->m_structure == LEFTIST)){
        result = result && maxheapTest(result, sameQueue->m_heap, sameQueue->getPriorityFn());
        result = result && leftistTest(result, sameQueue->m_heap);
    }
    else if ((sameQueue->m_heapType == MINHEAP) && (sameQueue->m_structure == SKEW)){
        result = result && minheapTest(result, sameQueue->m_heap, sameQueue->getPriorityFn());
    }
    else{
        result = result && maxheapTest(result, sameQueue->m_heap, sameQueue->getPriorityFn());
    }

    //everything is then emptied
    cqueue.clear();
    delete sameQueue;
    delete tempQueue;

    //final test is done with assignment operator, then copied over using copy constructor
    randomFill(cqueue, NORMAL_CASE + 50); //size slightly changed
    tempQueue = new CQueue(cqueue.m_priorFunc, cqueue.m_heapType, cqueue.m_structure);
    *tempQueue = cqueue;
    sameQueue = new CQueue(*tempQueue);

    //first checked is sameQueue to ensure it is exactly the same as cqueue, but in different locations
    result = result && assignmentHelper(result, cqueue.m_heap, sameQueue->m_heap);

    //then all member variables are checked
    result = result && (cqueue.m_size == sameQueue->m_size);
    result = result && (cqueue.m_heapType == sameQueue->m_heapType);
    result = result && (cqueue.m_structure == sameQueue->m_structure);
    result = result && (cqueue.m_priorFunc == sameQueue->m_priorFunc);
    result = result && (cqueue.m_heap != sameQueue->m_heap);

    //tests then run to ensure properties hold up
    if ((sameQueue->m_heapType == MINHEAP) && (sameQueue->m_structure == LEFTIST)){
        result = result && minheapTest(result, sameQueue->m_heap, sameQueue->getPriorityFn());
        result = result && leftistTest(result, sameQueue->m_heap);
    }
    else if ((sameQueue->m_heapType == MAXHEAP) && (sameQueue->m_structure == LEFTIST)){
        result = result && maxheapTest(result, sameQueue->m_heap, sameQueue->getPriorityFn());
        result = result && leftistTest(result, sameQueue->m_heap);
    }
    else if ((sameQueue->m_heapType == MINHEAP) && (sameQueue->m_structure == SKEW)){
        result = result && minheapTest(result, sameQueue->m_heap, sameQueue->getPriorityFn());
    }
    else{
        result = result && maxheapTest(result, sameQueue->m_heap, sameQueue->getPriorityFn());
    }


    //all two temporary CQueues deleted
    delete sameQueue;
    delete tempQueue; 

    return result;
}

//edgeCopyConstructor
//tests copy constructor for edge cases
bool Tester::edgeCopyConstructor(CQueue& cqueue){
    bool result = true;

    randomFill(cqueue, 1);//cqueue filled with one object

    //first edge case is a copy construtor with an object with only one node
    CQueue* testCQueue = new CQueue(cqueue);

    result = result && (testCQueue->m_size == 1);
    result = result && (testCQueue->m_heapType == cqueue.m_heapType);
    result = result && (testCQueue->m_structure == cqueue.m_structure);
    result = result && (testCQueue->m_priorFunc == cqueue.m_priorFunc);
    result = result && (testCQueue->m_heap != nullptr);

    //assignment helper called to double check
    result = result && assignmentHelper(result, testCQueue->m_heap, cqueue.m_heap);

    cqueue.clear(); //cqueue set to an empty object

    //two copy constructors are used
    //the reason this is done is to prove that since the members variables are not initialized until...
    //the assignment operator is called, the assignment operator still works correctly 
    CQueue* tempCQueue = new CQueue(cqueue);
    CQueue* emptyCQueue = new CQueue(*tempCQueue);

    //all member variables are checked between cqueue and emptyCQueue to ensure they match
    result = result && (emptyCQueue->m_size == 0);
    result = result && (emptyCQueue->m_heapType == cqueue.m_heapType);
    result = result && (emptyCQueue->m_structure == cqueue.m_structure);
    result = result && (emptyCQueue->m_priorFunc == cqueue.m_priorFunc);
    result = result && (emptyCQueue->m_heap == nullptr);

    //all dynamically allocated objects are deleted
    delete testCQueue;
    delete tempCQueue;
    delete emptyCQueue;

    return result;
}

//errorDequeue
//tests to see if an out of range error is caught
bool Tester::errorDequeue(CQueue& cqueue){
    bool result = true;

    //all are booleans used to ensure that the test is done correctly
    bool pureEmpty = false;
    bool emptyClear = false;
    bool fullEmpty = false;
    bool assignEmpty = false;

    //first checks to ensure that the object is empty
    result = result && (cqueue.m_heap == nullptr);
    
    //pure empty tested
    try{
        cqueue.getNextOrder();
    }
    catch(const out_of_range &range){
        pureEmpty = true;
    }

    //next, the cqueue is cleared
    randomFill(cqueue, NORMAL_CASE);
    result = result && (cqueue.m_size == NORMAL_CASE);

    //next, clear is called, then again tested to see if it actually clears
    cqueue.clear();
    
    //empty clear tested
    try{
        cqueue.getNextOrder();
    }
    catch(const out_of_range &range){
        emptyClear = true;
    }

    //next, randomly filled again, and each order is then removed
    randomFill(cqueue, NORMAL_CASE);
    try{
        for (int i = 0; i <= NORMAL_CASE; i++){
            cqueue.getNextOrder();
        }
    }
    catch(const out_of_range &range){
        fullEmpty = true;
    }

    //finally, is the assignment operator
    CQueue* newQueue = new CQueue(cqueue);
    
    //tries to catch the assignment error
    try{
        newQueue->getNextOrder();
    }
    catch(const out_of_range &range){
        assignEmpty = true;
    }

    //results of everything combined
    result = result && pureEmpty && emptyClear && fullEmpty && assignEmpty;

    delete newQueue;
    return result;
}

//errorMerge
//attempts to get the errors for merge cases
bool Tester::errorMerge(CQueue& cqueue){
    bool result = true;

    //all are booleans used to ensure that the test is done correctly
    bool pureEmpty = false;
    bool fullAttempt = false;
    bool heapAttempts = false;
    bool heapFullAttempt = false;
    bool goodTest = true;

    //first checks to ensure that the object is empty
    result = result && (cqueue.m_heap == nullptr);
    
    //testCQueue used to attempt to merge
    CQueue* testCQueue = new CQueue(cqueue);

    //opposite structure recorded
    STRUCTURE oppStruct = ((cqueue.m_structure == LEFTIST) ? SKEW: LEFTIST);
    HEAPTYPE oppHeap = ((cqueue.m_heapType == MINHEAP) ? MAXHEAP : MINHEAP);


    testCQueue->setStructure(oppStruct);

    //pure empty tested
    try{
        cqueue.mergeWithQueue(*testCQueue);
    }
    catch(const domain_error &domain){
        pureEmpty = true;
    }

    //next, both are filled, then they are attempted to be merged
    randomFill(cqueue, NORMAL_CASE);
    randomFill(*testCQueue, NORMAL_CASE);
    result = result && (cqueue.m_size == NORMAL_CASE);
    result = result && (testCQueue->m_size == NORMAL_CASE);

    //next, they are attemped to be merge
    try{
        cqueue.mergeWithQueue(*testCQueue);
    }
    catch(const domain_error &domain){
        fullAttempt = true;
    }

    //next, clear is called for both
    cqueue.clear();
    testCQueue->clear();

    //then, the structures are the same, but the heaptypes are swapped
    testCQueue->setStructure(cqueue.m_structure);
    testCQueue->setPriorityFn(cqueue.m_priorFunc, oppHeap);

    //tries again this time with different heap types
    try{
        cqueue.mergeWithQueue(*testCQueue);
    }
    catch(const domain_error &domain){
        heapAttempts = true;
    }

    //next, randomly filled
    randomFill(cqueue, NORMAL_CASE);
    randomFill(*testCQueue, NORMAL_CASE);
    result = result && (cqueue.m_size == NORMAL_CASE);
    result = result && (testCQueue->m_size == NORMAL_CASE);

    //finally, tested again with full but different heap types
    try{
        cqueue.mergeWithQueue(*testCQueue);
    }
    catch(const domain_error &domain){
        heapFullAttempt = true;
    }

    //finally, heap type as the same, to prove that the try remains the same
    cqueue.clear();
    testCQueue->clear();

    //finally, changed to be the same structure
    testCQueue->setStructure(cqueue.m_structure);
    testCQueue->setPriorityFn(cqueue.m_priorFunc, cqueue.m_heapType);

    //tested, so that goodTest remains true
    try{
        cqueue.mergeWithQueue(*testCQueue);
    }
    catch(const domain_error &domain){
        goodTest = false;
    }

    //result then combined with the results of the other tests
    result = result && pureEmpty && fullAttempt && heapAttempts && heapFullAttempt && goodTest;

    delete testCQueue; //testCQueue deleted
    return result;
}