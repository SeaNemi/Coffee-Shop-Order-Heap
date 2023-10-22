// CMSC 341 - Spring 2023 - Project 3
#ifndef CQUEUE_H
#define CQUEUE_H
#include <stdexcept>
#include <iostream>
#include <string>
using namespace std;
class Grader;   // forward declaration (for grading purposes)
class Tester;   // forward declaration
class CQueue;   // forward declaration
class Order;    // forward declaration
#define EMPTY Order("",1,0)
const int MINCUSTID = 100001;// minimum customer ID
const int MAXCUSTID = 999999;// maximum customer ID
// use a different seed in the Random class to get different series of numbers 
const int MINORDERID = 100001;// minimum order ID
const int MAXORDERID = 999999;// maximum order ID
enum MEMBERSHIP {TIER1, TIER2, TIER3, TIER4, TIER5, TIER6};// use with MinHeap
enum ITEM {COFFEE, LATTE, SOFTDRINK, MILK, WATER, ICEDTEA};// use with MinHeap
enum COUNT {ONE, PAIR, HALFDOZEN, DOZEN};// use with MaxHeap
const int MINPOINTS = 0; // the points colleted so far, use with MaxHeap
const int MAXPOINTS = 5000; // the points colleted so far, use with MaxHeap

enum HEAPTYPE {MINHEAP, MAXHEAP};
enum STRUCTURE {SKEW, LEFTIST};
// Priority function pointer type
typedef int (*prifn_t)(const Order&);

class Order{
    // stores a customer's order
    public:
    friend class Grader; // for grading purposes
    friend class Tester; // for testing purposes
    friend class CQueue;
    Order(ITEM item = COFFEE, COUNT count = ONE, 
            MEMBERSHIP membership = TIER5, int points = 0, 
            int customerID = 0, int orderID = 0)
    {
        m_item = item; m_count = count; m_membership = membership;
        m_points = points; m_customerID = customerID; m_orderID = orderID;
    }
    ITEM getItem() const {return m_item;}
    int getOrderID() const {return m_orderID;}
    COUNT getCount() const {return m_count;}
    int getCustomerID() const {return m_customerID;}
    MEMBERSHIP getMemebership() const {return m_membership;}
    int getPoints() const {return m_points;}
    void setItem(ITEM item){m_item=item;}
    void setOrderID(int id){m_orderID=id;}
    void setCount(COUNT count){m_count=count;}
    void setCustomerID(int id){m_customerID=id;}
    void setMembership(MEMBERSHIP membership){m_membership=membership;}
    void setPoints(int points){m_points=points;}
    // Overloaded assignment operator
    const Order& operator=(const Order& rhs){
        if (this != &rhs){
            m_item = rhs.m_item;
            m_count = rhs.m_count;
            m_membership = rhs.m_membership;
            m_points = rhs.m_points;
            m_customerID = rhs.m_customerID;
            m_orderID = rhs.m_orderID;
        }
        return *this;
    }
    string getTierString() const {
        string result = "UNKNOWN";
        switch (m_membership)
        {
        case TIER1: result = "Tier 1"; break;
        case TIER2: result = "Tier 2"; break;
        case TIER3: result = "Tier 3"; break;
        case TIER4: result = "Tier 4"; break;
        case TIER5: result = "Tier 5"; break;
        case TIER6: result = "Tier 6"; break;
        default: break;
        }
        return result;
    }
    string getItemString() const {
        string result = "UNKNOWN";
        switch (m_item)
        {
        case COFFEE: result = "Coffee"; break;
        case LATTE: result = "Latte"; break;
        case SOFTDRINK: result = "Soft Drink"; break;
        case MILK: result = "Milk"; break;
        case WATER: result = "Water"; break;
        case ICEDTEA: result = "Iced Tea"; break;
        default: break;
        }
        return result;
    }
    string getCountString() const {
        string result = "UNKNOWN";
        switch (m_count)
        {
        case ONE: result = "1"; break;
        case PAIR: result = "2"; break;
        case HALFDOZEN: result = "6"; break;
        case DOZEN: result = "12"; break;
        default: break;
        }
        return result;
    }
    // Overloaded insertion operator
    friend ostream& operator<<(ostream& sout, const Order &order );
    
    private:
    int m_customerID;// a unique ID number identifying the customer
    int m_orderID;   // a unique ID number identifying the order
    // data to be used for priority calculation
    int m_points;   // points collected by customer
    MEMBERSHIP m_membership; // the customer membership tier
    ITEM m_item;    // the ordered item
    COUNT m_count;  // the count of ordered item
      
};
class Node{
    // this is a node in the skew/leftist heap
    public:
    friend class Grader; // for grading purposes
    friend class Tester; // for testing purposes
    friend class CQueue;
    Node(Order order) {  
        m_order = order;
        m_right = nullptr;
        m_left = nullptr;
        m_npl = 0;
    }
    Order getOrder() const {return m_order;}
    void setNPL(int npl) {m_npl = npl;}
    int getNPL() const {return m_npl;}
    // Overloaded insertion operator
    friend ostream& operator<<(ostream& sout, const Node& node);

    private:
    Order m_order;    // order information
    Node * m_right;   // right child
    Node * m_left;    // left child
    int m_npl;        // null path length for leftist heap
};
class CQueue{
    // stores the skew/leftist heap, minheap/maxheap
    public:
    friend class Grader; // for grading purposes
    friend class Tester; // for testing purposes
    
    CQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure);
    ~CQueue();
    CQueue(const CQueue& rhs);
    CQueue& operator=(const CQueue& rhs);
    void insertOrder(const Order& order);
    Order getNextOrder(); // Return the highest priority order
    void mergeWithQueue(CQueue& rhs);
    void clear();
    int numOrders() const; // Return number of orders in queue
    void printOrdersQueue() const; // Print the queue using preorder traversal
    prifn_t getPriorityFn() const;
    // Set a new priority function. Must rebuild the heap!!!
    void setPriorityFn(prifn_t priFn, HEAPTYPE heapType);
    HEAPTYPE getHeapType() const;
    STRUCTURE getStructure() const;
    // Set a new data structure (skew/leftist). Must rebuild the heap!!!
    void setStructure(STRUCTURE structure);
    void dump() const; // For debugging purposes

    private:
    Node * m_heap;          // Pointer to the root of skew heap
    int m_size;             // Current size of the heap
    prifn_t m_priorFunc;    // Function to compute priority
    HEAPTYPE m_heapType;    // either a MINHEAP or a MAXHEAP
    STRUCTURE m_structure;  // skew heap or leftist heap

    void dump(Node *pos) const; // helper function for dump

    /******************************************
     * Private function declarations go here! *
     ******************************************/

    void recursiveClear(Node* curr); //helper for clear and the destructor

    Node* merge(Node* leftNode, Node* rightNode); //helper which helps with merging nodes in queues
    void swap(Node*& leftNode, Node*& rightNode); //helper for merge
    void updateNPL(Node* node); //helper for updating the NPL, specifically used in merge
    
    void preorderTraversal(const Node* curr) const; //helper for printOrdersQueue
    Node* preOrderAssignment(const Node* rhsNode);
    void priorityFuncHelper(const Node* curr, CQueue& newCQueue); //helper for setPriorityFunc and setStructure
};
#endif