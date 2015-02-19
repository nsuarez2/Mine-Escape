#ifndef POORMAN_PRIORITY_QUEUE_H
#define POORMAN_PRIORITY_QUEUE_H

#include "eecs281_priority_queue.h"

//A specialized version of the 'priority_queue' ADT that is implemented with an
//underlying unordered array-based container that is linearly searches
//for the min every time it is requested.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class poorman_priority_queue : public eecs281_priority_queue<TYPE, COMP_FUNCTOR> {
public:
    typedef unsigned size_type;
    
    //Description: Construct a priority_queue out of an iterator range with an optional
    //             comparison functor.
    //Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    poorman_priority_queue(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR());
    
    //Description: Construct an empty priority_queue with an optional comparison functor.
    //Runtime: O(1)
    poorman_priority_queue(COMP_FUNCTOR comp = COMP_FUNCTOR());
    
    //Description: Does nothing for this implementation, as items can never be
    //             'out of order'. This function is implemented for you.
    //Runtime: O(1)
    virtual void fix() {}
    
    //Description: Add a new element to the priority_queue.
    //Runtime: Amortized O(1)
    virtual void push(const TYPE& val);
    
    //Description: Remove the most extreme (defined by 'compare') element from
    //             the priority_queue.
    //Note: We will not run tests on your code that would require it to pop an
    //element when the priority_queue is empty. Though you are welcome to if you are
    //familiar with them, you do not need to use exceptions in this project.
    //Runtime: O(n)
    virtual void pop();
    
    //Description: Return the most extreme (defined by 'compare') element of
    //             the priority_queue.
    //Runtime: O(n)
    virtual const TYPE& top() const;
    
    //Description: Get the number of elements in the priority_queue. This has been
    //             implemented for you.
    //Runtime: O(1)
    virtual size_type size() const { return data.size(); }
    
    //Description: Return true if the priority_queue is empty. This has been implemented
    //             for you.
    //Runtime: O(1)
    virtual bool empty() const { return data.empty(); }
private:
    //Note: This vector *must* be used your priority_queue implementation.
    std::vector<TYPE> data;
private:
    //***Add any additional member functions or data you require here.
};

template<typename TYPE, typename COMP_FUNCTOR>
template<typename InputIterator>
poorman_priority_queue<TYPE, COMP_FUNCTOR>::poorman_priority_queue(
                                                                   InputIterator start,
                                                                   InputIterator end,
                                                                   COMP_FUNCTOR comp
                                                                   ) {
    
    this->compare = comp;
    for (InputIterator i = start; i != end; ++i) {
        data.push_back(*i);
    }
    
}

template<typename TYPE, typename COMP_FUNCTOR>
poorman_priority_queue<TYPE, COMP_FUNCTOR>::poorman_priority_queue(COMP_FUNCTOR comp) {
    
    this->compare = comp;
}

template<typename TYPE, typename COMP_FUNCTOR>
void poorman_priority_queue<TYPE, COMP_FUNCTOR>::push(const TYPE& val) {
    
    data.push_back(val);
}

template<typename TYPE, typename COMP_FUNCTOR>
void poorman_priority_queue<TYPE, COMP_FUNCTOR>::pop() {
    
    size_type most = 0;
    
    for (size_type i = 0; i < data.size(); i++) {
        if (this->compare(data[most], data[i])) {
            most = i;
        }
    }
    data.erase(data.begin() + most);
}

template<typename TYPE, typename COMP_FUNCTOR>
const TYPE& poorman_priority_queue<TYPE, COMP_FUNCTOR>::top() const {
    
    size_type most = 0;
    
    for (size_type i = 0; i < data.size(); i++) {
        if (this->compare(data[most], data[i])) {
            most = i;
        }
    }
    return data[most];
}

#endif //POORMAN_PRIORITY_QUEUE_H
