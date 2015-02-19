#ifndef PAIRING_PRIORITY_QUEUE_H
#define PAIRING_PRIORITY_QUEUE_H

#include "eecs281_priority_queue.h"
#include <vector>

//A specialized version of the 'priority_queue' ADT implemented as a pairing priority_queue.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class pairing_priority_queue : public eecs281_priority_queue<TYPE, COMP_FUNCTOR> {
public:
    typedef unsigned size_type;
    
    //Description: Construct a priority_queue out of an iterator range with an optional
    //             comparison functor.
    //Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    pairing_priority_queue(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR());
    
    //Description: Construct an empty priority_queue with an optional comparison functor.
    //Runtime: O(1)
    pairing_priority_queue(COMP_FUNCTOR comp = COMP_FUNCTOR());
    
    //Description: Copy constructor.
    //Runtime: O(n)
    pairing_priority_queue(const pairing_priority_queue& other);
    
    //Description: Copy assignment operator.
    //Runtime: O(n)
    pairing_priority_queue& operator=(const pairing_priority_queue& rhs);
    
    //Description: Destructor
    //Runtime: O(n)
    ~pairing_priority_queue();
    
    //Description: Assumes that all elements inside the priority_queue are out of order and
    //             'rebuilds' the priority_queue by fixing the priority_queue invariant.
    //Runtime: O(n)
    virtual void fix();
    
    //Description: Add a new element to the priority_queue. This has been provided for you,
    //             in that you should implement push functionality in the
    //             add_node function.
    //Runtime: Amortized O(1)
    virtual void push(const TYPE& val) { add_node(val); }
    
    //Description: Remove the most extreme (defined by 'compare') element from
    //             the priority_queue.
    //Note: We will not run tests on your code that would require it to pop an
    //element when the priority_queue is empty. Though you are welcome to if you are
    //familiar with them, you do not need to use exceptions in this project.
    //Runtime: Amortized O(log(n))
    virtual void pop();
    
    //Description: Return the most extreme (defined by 'compare') element of
    //             the priority_queue.
    //Runtime: O(1)
    virtual const TYPE& top() const;
    
    //Description: Get the number of elements in the priority_queue.
    //Runtime: O(1)
    virtual size_type size() const { return sizeTotal; }
    
    //Description: Return true if the priority_queue is empty.
    //Runtime: O(1)
    virtual bool empty() const { return !sizeTotal; }
    
    class Node {
        //This node class will allow external users to update the priority of
        //elements that are already inside the priority_queue.
    public:
        //***Add any constructors you need here.
        Node() { elt = TYPE(); }
        Node(TYPE val) { elt = val; }
    public:
        //Description: Allows access to the element at that Node's position.
        //Runtime: O(1) - this has been provided for you.
        const TYPE& operator*() const { return elt; }
        
        //The following line allows you to access any private data members of this
        //Node class from within the pairing_priority_queue class. (ie: myNode.elt is a legal
        //statement in pairing_priority_queue's add_node() function).
        friend pairing_priority_queue;
    private:
        TYPE elt;
    private:
        Node *parent;
        std::vector<Node*> data;
        
    };
    
    //Description: Updates the priority of an element already in the priority_queue by
    //             replacing the element refered to by the Node with new_value.
    //             Must maintain priority_queue invariants.
    //
    //PRECONDITION: The new priority, given by 'new_value' must be more extreme
    //              (as defined by comp) than the old priority.
    //
    //Runtime: As discussed in reading material.
    void updateElt(Node* node, const TYPE &new_value);
    
    //Description: Add a new element to the priority_queue. Returns a Node* corresponding
    //             to the newly added element.
    //Runtime: Amortized O(1)
    Node* add_node(const TYPE& val);
    
private:
    int sizeTotal;
    Node *root;
    void destructor(Node *root);
    Node* equal(Node *root);
    Node* meld(Node *a, Node *b);
    
};

template<typename TYPE, typename COMP_FUNCTOR>
template<typename InputIterator>
pairing_priority_queue<TYPE, COMP_FUNCTOR>::pairing_priority_queue(
                                                                   InputIterator start,
                                                                   InputIterator end,
                                                                   COMP_FUNCTOR comp
                                                                   ) {
    this->compare = comp;
    for (InputIterator i = start; i != end; ++i) {
        this->add_node(*i);
        this->sizeTotal++;
    }
}

template<typename TYPE, typename COMP_FUNCTOR>
pairing_priority_queue<TYPE, COMP_FUNCTOR>::pairing_priority_queue(COMP_FUNCTOR comp) {
    
    this->compare = comp;
    this->sizeTotal = 0;
}

template<typename TYPE, typename COMP_FUNCTOR>
pairing_priority_queue<TYPE, COMP_FUNCTOR>::pairing_priority_queue(const pairing_priority_queue& other) {
    
    *this = other;
}

template<typename TYPE, typename COMP_FUNCTOR>
pairing_priority_queue<TYPE, COMP_FUNCTOR>&
pairing_priority_queue<TYPE, COMP_FUNCTOR>::operator=(const pairing_priority_queue& rhs) {
    
    Node *node = rhs.root;
    if (this != &rhs) {
        this->root = equal(node);
    }
    this->sizeTotal = rhs.sizeTotal;
    return *this;
}

template<typename TYPE, typename COMP_FUNCTOR>
pairing_priority_queue<TYPE, COMP_FUNCTOR>::~pairing_priority_queue() {
    
    if (!(this->sizeTotal)) {
        return;
    }
    this->root->parent = nullptr;
    destructor(this->root);
    
}

template<typename TYPE, typename COMP_FUNCTOR>
void pairing_priority_queue<TYPE, COMP_FUNCTOR>::fix() {
    
    if (!(this->sizeTotal)) {
        return;
    }
    pairing_priority_queue<TYPE, COMP_FUNCTOR> queue;
    while (!(this->empty())) {
        queue.add_node(this->top());
        this->pop();
    }
    *this = queue;
}

template<typename TYPE, typename COMP_FUNCTOR>
void pairing_priority_queue<TYPE, COMP_FUNCTOR>::pop() {
    
    if (sizeTotal == 0) {
        return;
    }
    
    std::vector<Node*> newData = this->root->data;
    
    for (auto i: this->root->data) {
        (*i).parent = nullptr;
    }
    delete this->root;
    
    if (newData.empty()) {
        sizeTotal = 0;
        return;
    }
    else {
        int i = 0;
        
        if (newData.size() % 2) {
            i++;
        }
        while (i < int(newData.size())) {
            newData[i] = meld(newData[i], newData[i + 1]);
            i += 2;
        }
        this->root = newData[0];
        int j = 1;
        if (newData.size() % 2) {
            
            while (j < int(newData.size())) {
                this->root = meld(this->root, newData[j]);
                j += 2;
            }
        }
        else {
            
            j++;
            while (j < int(newData.size())) {
                this->root = meld(this->root, newData[j]);
                j += 2;
            }
        }
    }
    
    this->sizeTotal--;
    
}

template<typename TYPE, typename COMP_FUNCTOR>
const TYPE& pairing_priority_queue<TYPE, COMP_FUNCTOR>::top() const {
    
    return this->root->elt;
}

template<typename TYPE, typename COMP_FUNCTOR>
typename pairing_priority_queue<TYPE, COMP_FUNCTOR>::Node*
pairing_priority_queue<TYPE, COMP_FUNCTOR>::add_node(const TYPE& val) {
    
    Node *here = new Node(val);
    if (this->empty()) {
        this->root = here;
        this->sizeTotal = 1;
    }
    else {
        if (this->compare(val, this->root->elt)) {
            here->parent = this->root;
            this->root->data.push_back(here);
        }
        else {
            here->data.push_back(this->root);
            this->root->parent = here;
            this->root = here;
        }
        this->sizeTotal++;
    }
    return here;
}

template<typename TYPE, typename COMP_FUNCTOR>
void pairing_priority_queue<TYPE, COMP_FUNCTOR>::updateElt(Node* node, const TYPE &new_value) {
    
    if (node == this->root) {
        node->elt = new_value;
    }
    else {
        int i = 0;
        while (node->parent->data[i]->elt != node->elt) {
            i++;
        }
        node->elt = new_value;
        node->parent->data.erase(node->parent->data.begin() + i);
        this->root = meld(this->root, node);
    }
}

template<typename TYPE, typename COMP_FUNCTOR>
void pairing_priority_queue<TYPE, COMP_FUNCTOR>::destructor(Node *here) {
    
    if (here == nullptr)
        return;
    else{
        for (auto i: here->data) {
            destructor(i);
        }
        delete here;
    }
}

template<typename TYPE, typename COMP_FUNCTOR>
typename pairing_priority_queue<TYPE, COMP_FUNCTOR>::Node*
pairing_priority_queue<TYPE, COMP_FUNCTOR>::equal(Node *here) {
    
    if (here == nullptr) {
        return nullptr;
    }
    Node *node = new Node(here->elt);
    
    for (unsigned int i = 0; i < here->data.size(); ++i) {
        
        node->data.push_back(equal(here->data[i]));
        
        if (node->data[node->data.size() - 1] != nullptr) {
            node->data[i]->parent = node;
        }
        else {
            node->data.resize(node->data.size() - 1);
        }
    }
    return node;
}



template<typename TYPE, typename COMP_FUNCTOR>
typename pairing_priority_queue<TYPE, COMP_FUNCTOR>::Node*
pairing_priority_queue<TYPE, COMP_FUNCTOR>::meld(Node *a, Node *b) {
    if (a == nullptr) {
        return b;
    }
    else if (b == nullptr) {
        return a;
    }
    else if (this->compare(a->elt, b->elt)) {
        a->parent = b;
        b->data.resize(b->data.size() + 1, a);
        b->parent = nullptr;
        return b;
    }
    else {
        b->parent = a;
        a->data.resize(a->data.size() + 1, b);
        a->parent = nullptr;
        return a;
    }
}

#endif //PAIRING_PRIORITY_QUEUE_H

