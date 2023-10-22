// CMSC 341 - Spring 2023 - Project 3
#include "cqueue.h"

//overloaded constructor
//creates an empty cqueue object
CQueue::CQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure){
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = priFn;

  //if statement checks to ensure that the data sent is valid or else defaults to skewed minheap
  m_heapType = ((heapType == MINHEAP) || (heapType = MAXHEAP)) ? heapType: MINHEAP;
  m_structure = ((structure == SKEW) || (structure == LEFTIST)) ? structure: SKEW;
}

//destructor
//clears the function to delete everything, then sets all variables to the default
CQueue::~CQueue(){
    clear(); //clear called
}

//clear
//clears the function and sets m_heap to nullptr
void CQueue::clear(){
    recursiveClear(m_heap); //recursiveClear called starting with the heap
    m_heap = nullptr; //m_heap set to nullptr since only dynamically allocated data
    m_size = 0; //size set to zero
}

//copy constructor
CQueue::CQueue(const CQueue& rhs){
    //m_heap set to nullptr and m_size set to zero
    m_heap = nullptr;
    m_size = 0;
    *this = rhs; //this is set equal to rhs
}

//assignmentOperator
//assigns data equally to one another
CQueue& CQueue::operator=(const CQueue& rhs) {
    //if statement checks to ensure that no self assignment is occuring
    if (this != &rhs){
        clear(); //clear called
        
        //member variables set to member variables of left side
        m_priorFunc = rhs.m_priorFunc;
        m_heapType = rhs.m_heapType;
        m_structure = rhs.m_structure;
        m_size = rhs.m_size;

        //if statement checks to see if m_heap equals nullptr, then returns
        if (rhs.m_heap == nullptr){
            return *this;
        }
        //else, preOrderAssignment called with the rhs m_heap
        else{
            m_heap = preOrderAssignment(rhs.m_heap);
        }
    }

    return *this; //this returned
}

//mergeWithQueue
//the rhs CQueue object is merged with the lhs CQueue object, and rhs is left empty
void CQueue::mergeWithQueue(CQueue& rhs) {
    //if statement checks to ensure that the type and structure match up
    if ((rhs.m_heapType != m_heapType) || (rhs.m_structure != m_structure)){
        throw domain_error("Domain error");
    }
    else{
        if (this != &rhs){
            //merges the lhs's m_heap with the rhs's m_heap
            m_heap = merge(m_heap, rhs.m_heap); //merge called
        }
        
        //m_size changes to be the normal size plus the new size
        m_size += rhs.m_size;

        //rhs's m_heap set to nullptr to avoid memory issues and size set to zero
        rhs.m_heap = nullptr;
        rhs.m_size = 0;
    }
}

//insertOrder
//a new order is inserted by merging it with the existing heap of orders
void CQueue::insertOrder(const Order& order) {
    //newNode declared and initialized with the order
    Node* newNode = new Node(order);

    //merge called to insert the newNode into the heap, so it can be determined what priority it is
    //starts with m_heap since we do not know what will be changed
    m_heap = merge(m_heap, newNode);
    
    ++m_size; //m_size increased by one
}

//numOrders
//returns the number of items within the heap, AKA the m_size
int CQueue::numOrders() const{
    return m_size;
}

//getPriorityFunction
//returns the current priority function of the queue
prifn_t CQueue::getPriorityFn() const {
    return m_priorFunc; //m_priorFunc returned
}

//getNextOrder
//returns the priority order
Order CQueue::getNextOrder() {
    //if m_heap is a nullptr, an out of range error is thrown
    if (m_heap == nullptr){
        throw out_of_range("Out of Range");
    }
    else{
        Node* returnedNode = m_heap; //Node* returnedNode is set equal to m_heap
        Order myOrder = returnedNode->getOrder(); //myOrder holds returnedNode's order
        m_heap = merge(m_heap->m_left, m_heap->m_right); //merges the two subtrees of the tree deleted
        delete returnedNode; //deletes the old m_heap entirely
        --m_size; //m_size reduced by one
        return myOrder; 
    }
}

//setPriorityFn
//Changes the priority function and then changes the heap
void CQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
    //if statement checks to ensure the heapType is valid
    if ((heapType != MINHEAP) && (heapType != MAXHEAP)){
        return;
    }
    //else if the heap type and priFn are the same, nothing must be changed
    else if (priFn == m_priorFunc && heapType == m_heapType){
        return;
    }
    else{
    //else, a new heap is created, which has the same structure but different members than this
        CQueue* newHeap = new CQueue(priFn, heapType, m_structure);

        //priorityFunction helper adds the nodes into the new heap
        priorityFuncHelper(m_heap, *newHeap);

        *this = *newHeap; //this is set equal to newHeap, so that the new structure takes over
        delete newHeap; //newHeap deleted
    }
}

//setStructure
//Changes the structure of the heap, and thus reorders the heap based on the new structure
void CQueue::setStructure(STRUCTURE structure){
   //if statement checks to ensure that the structure is valid
   if ((structure != SKEW) && (structure != LEFTIST)){
    return;
   }
   //else if structure is the same, nothing must be changed
    else if (structure == m_structure){
        return;
    }
    else{   
        //else, a new heap is created, which has the same members as the current heap except for the structure
        CQueue* newHeap = new CQueue(m_priorFunc, m_heapType, structure);

        //priorityFuncHelper called to help add them to the end of the new heap
        priorityFuncHelper(m_heap, *newHeap);

        *this = *newHeap; //this is set equal to newHeap, so that the new structure takes over
        delete newHeap; //newHeap deleted
    }
}

//getStructure
//returns the m_structure of the queue
STRUCTURE CQueue::getStructure() const {
    return m_structure;
}

//getHeapType
//returns heap type
HEAPTYPE CQueue::getHeapType() const{
    return m_heapType;
}

//printorders queue
//using preorder traversal, prints the current amount of items within the queue
void CQueue::printOrdersQueue() const {
    preorderTraversal(m_heap); //preorderTraversal called with m_heap, or the root, starting
}

//dump
//dumps out the function
void CQueue::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    dump(m_heap);
  }
  cout << endl;
}

//dump
//overloaded function which dumps the items in the queue if not empty
void CQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.getPoints();
    else
        cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.getPoints() << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

//operator<<
//done for orders, and nodes, to output the orders
ostream& operator<<(ostream& sout, const Order& order) {
  sout << "Order ID: " << order.getOrderID() 
        << ", customer ID: " << order.getCustomerID()
        << ", # of points: " << order.getPoints()
        << ", membership tier: " << order.getTierString()
        << ", item ordered: " << order.getItemString()
        << ", quantity: " << order.getCountString();
  return sout;
}
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getOrder();
  return sout;
}

//recursiveClear
//uses recursion to completely delete the heap, using postorder traversal
void CQueue::recursiveClear(Node* curr){
    //if statement checks to ensure that curr is not a nullptr
    if (curr != nullptr){
        //postorder traversal used to completely delete the heap
        recursiveClear(curr->m_left);
        recursiveClear(curr->m_right);
        delete curr; //curr is deleted
    }
}


//merge
//helper function which helps with merging
Node* CQueue::merge(Node* leftNode, Node* rightNode){
    Node* newSubtree = nullptr; //newSubtree set to nullptr

    //if statement checks if both are nullptrs, which is our base case
    if ((leftNode == nullptr) && (rightNode == nullptr)){
        return newSubtree; //newSubtree, or nullptr, is returned
    }
    //else statement checks if the left isn't a nullptr, but the right one is
    else if ((leftNode != nullptr) && (rightNode == nullptr)){
        newSubtree = leftNode; //newSubtree takes on the left side
    }
    //else if leftNode is nullptr but right is not a nullptr, the newSubtree becomes the right
    else if ((leftNode == nullptr) && (rightNode != nullptr)){
        newSubtree = rightNode; //newSubtree takes on the right
    }

    //else, both are not nullptrs, and newSubtree has to be determined
    else{
        //if statement determines if a MINHEAP or MAXHEAP
        //this determines which gets set to the left and which is set to the right
        if (m_heapType == MINHEAP){
            //if statement determines which value is lesser, then makes 
            //priorFunction called to determine the valueS
            if (m_priorFunc(leftNode->m_order) <= m_priorFunc(rightNode->m_order)){
                leftNode->m_right = merge(leftNode->m_right, rightNode);
                newSubtree = leftNode; //the newSubtree becomes the ptr to the left node
            }
            //else, the left subtree is compared and merged
            else{
                rightNode->m_right = merge(rightNode->m_right, leftNode);
                newSubtree = rightNode; //newSubtree takes on the right node
            }
        }
        else{
            //if statement determines which value is greater since MAXHEAP, then makes 
            //m_priorFunc called
            if (m_priorFunc(leftNode->m_order) >= m_priorFunc(rightNode->m_order)){
                leftNode->m_right = merge(leftNode->m_right, rightNode);
                newSubtree = leftNode; //the newSubtree becomes the ptr to the left node
            }
            //else, the left subtree is compared and merged
            else{
                rightNode->m_right = merge(rightNode->m_right, leftNode);
                newSubtree = rightNode; //newSubtree is set equal to right
            }
        }
    }

    //if statement determines which type of heap it is, then determines whether to swap or not
    if (m_structure == SKEW){
        swap(newSubtree->m_left, newSubtree->m_right); //swaps newSubtree's left and right value, as required in skew heaps
    }
    //else, it is a leftist heap, since only two types of heaps
    else{
        //if statement checks if m_right is not a nullptr and determines if a swap is needed
        if ((newSubtree->m_right != nullptr)){
            //if statement is if m_left is not a nullptr
            if (newSubtree->m_left != nullptr){
                //if statement determines which NPL is lesser 
                //if the right one is greater than the left, it is swapped based on the leftist heap theorem, since we want a left skew
                if (newSubtree->m_right->getNPL() > newSubtree->m_left->getNPL()){
                    //if this is true, they are swapped
                    swap(newSubtree->m_left, newSubtree->m_right);
                }
            }
            //else, if m_left is a nullptr, a swap is automatically needed since skew is on right side
            else{
                swap(newSubtree->m_left, newSubtree->m_right); //swaps nullptr to right side
            }
        }

        //NPL updated for the newSubtree
        updateNPL(newSubtree);
    }
    //newSubtree is returned
    return newSubtree;
}

//swap
//helper function which helps with swapping left and right heaps
void CQueue::swap(Node*& leftNode, Node*& rightNode){
    //memory addresses sent to that merge knows the changes which take place
    Node* temp = leftNode; //temp declared and initialized to leftNode
    leftNode = rightNode; //leftNode set equal to the right node
    rightNode = temp; //right node set equal to temp
}

//updateNPL
//updates the NPL for nodes
void CQueue::updateNPL(Node* node){
    //if statement checks to ensure that node is not a nullptr
    if (node != nullptr){
        //leftNPL and rightNPL declared and initialized
        //determines if a nullptr is present
        int leftNPL = ((node->m_left == nullptr) ? 0 : node->m_left->getNPL());
        int rightNPL = ((node->m_right == nullptr) ? 0 : node->m_right->getNPL());
        //newHeight is one plus the ternary operator depending on which NPL is lesser
        //if leftNPL is greater, rightNPL is set, else the leftNPL is set
        //it depends on which is lesser
        int newNPL = 1 + ((leftNPL < rightNPL) ? leftNPL : rightNPL);
        node->setNPL(newNPL); //node's new NPL is set
    }
}

//preorderTraversal
//prints out the items in the function by preorder traversal
void CQueue::preorderTraversal(const Node* curr) const{
    //if curr is a nullptr, that means that it will seg fault, so if statement checks for that
    //base case of this recursive function
    if (curr == nullptr){
        return;
    }
    else{
        //follows print parent first, then left child, then right child principle of preorde traversal 
        cout << "[" << m_priorFunc(curr->m_order) << "] " << *curr << endl;

        //preorderTraversal called for left and right children
        preorderTraversal(curr->m_left);
        preorderTraversal(curr->m_right);
    }
}

//preorderAssignment
//Helper function of the assignment operator, assigns a new node to curr for the lhs tree
Node* CQueue::preOrderAssignment(const Node* rhsNode){
    //if rhsNode is nullptr, nothing is done
    if (rhsNode == nullptr){
        return nullptr;
    }
    else{
        //curr set to be a new node with the same info as rhsNode, NPL set to be the same
        Node* curr = new Node(rhsNode->getOrder());
        curr->setNPL(rhsNode->getNPL());

        //preorderAssignment called with rhsNode going to the left first then right
        //set as left and right subtrees of curr
        curr->m_left = preOrderAssignment(rhsNode->m_left);
        curr->m_right = preOrderAssignment(rhsNode->m_right);

        return curr; //curr is returned
    }
}

//priorityFuncHelper
//uses postorder traversal to add nodes into the rebuilt heap
void CQueue::priorityFuncHelper(const Node* curr, CQueue& newCQueue){
    if (curr == nullptr){
        return;
    }
    else{
        //checks left first then checks right
        priorityFuncHelper(curr->m_left, newCQueue);
        priorityFuncHelper(curr->m_right, newCQueue);

        //order is then added into the function
        newCQueue.insertOrder(curr->m_order);
    }

}