#ifndef PQUEUEARRAY_H
#define PQUEUEARRAY_H
#include <cstdlib>
#include <iostream>

enum PQ_ERRORS { Q_EMPTY, Q_FULL, Q_BAD_SIZE };

template<typename Data, typename Priority = int>
class pQueueArray {

public:

    pQueueArray(size_t capacity = 16);
    ~pQueueArray();
    pQueueArray(const pQueueArray<Data, Priority> &other);
    pQueueArray<Data, Priority> &operator=(const pQueueArray<Data, Priority> &other);

    size_t size();
    size_t capacity();
    bool empty();
    bool full();

    void enqueue(const Data &d, const Priority &p);
    Data dequeue();
    Data peek();
    void clear();

private:

    size_t mySize, myCapacity;
    size_t head, tail;
    Data *queueArray;
    Priority *pArray;

    void copy(const pQueueArray<Data, Priority> &other);
    void deleteAll();
    bool isGreaterOrEqual(size_t i1, size_t i2);
    bool isGreater(size_t i1, size_t i2);
    bool isLessOrEqual(size_t i1, size_t i2);
    bool isLess(size_t i1, size_t i2);

};

template<typename Data, typename Priority>
pQueueArray<Data, Priority>::pQueueArray(size_t capacity) {
    if(capacity < 2)
        throw Q_BAD_SIZE;
    myCapacity = capacity;
    mySize = head = tail = 0;
    queueArray = new Data[capacity + 1];
    pArray = new Priority[capacity + 1];
}

template<typename Data, typename Priority>
pQueueArray<Data, Priority>::~pQueueArray() {
    deleteAll();
}

template<typename Data, typename Priority>
pQueueArray<Data, Priority>::pQueueArray(const pQueueArray<Data, Priority> &other) {
    copy(other);
}

template<typename Data, typename Priority>
pQueueArray<Data, Priority> &pQueueArray<Data, Priority>::operator=(const pQueueArray<Data, Priority> &other) {
    if(this != &other) {
        deleteAll();
        copy(other);
    }
    return *this;
}

template<typename Data, typename Priority>
size_t pQueueArray<Data, Priority>::size() {
    return mySize;
}

template<typename Data, typename Priority>
size_t pQueueArray<Data, Priority>::capacity() {
    return myCapacity;
}

template<typename Data, typename Priority>
bool pQueueArray<Data, Priority>::empty() {
    return !mySize;
}

template<typename Data, typename Priority>
bool pQueueArray<Data, Priority>::full() {
    return mySize == myCapacity;
}

template<typename Data, typename Priority>
void pQueueArray<Data, Priority>::enqueue(const Data &d, const Priority &p) {

    if(full()) throw Q_FULL;

    int insertIndex = -1;
    ++mySize;

    if(tail == myCapacity)
        tail = 0;
    else
        ++tail;

    // Finds where to insert by priority
    for(size_t i = head; i < tail; ++i) {
        if(p <= pArray[i])
            insertIndex = i;
    }

    // Inserts to start if highest priority
    if(insertIndex == -1) {
        for(int i = tail; i > head ; --i) {
            queueArray[i] = queueArray[i-1];
            pArray[i] = pArray[i-1];
        }
        queueArray[head] = d;
        pArray[head] = p;
    }
    // Inserts at index
    else {
        for(size_t i = tail; i > insertIndex; --i) {
            queueArray[i] = queueArray[i-1];
            pArray[i] = pArray[i-1];
        }
        queueArray[insertIndex+1] = d;
        pArray[insertIndex+1] = p;
    }
}

template<typename Data, typename Priority>
Data pQueueArray<Data, Priority>::peek() {
    if(empty()) throw Q_EMPTY;
    return queueArray[head];
}

template<typename Data, typename Priority>
Data pQueueArray<Data, Priority>::dequeue() {

    if(empty()) throw Q_EMPTY;

    Data temp = queueArray[head];
    queueArray[head] = Data();
    pArray[head] = Priority();
    if(head == myCapacity)
        head = 0;
    else
        ++head;
    --mySize;
    return temp;

}

template<typename Data, typename Priority>
void pQueueArray<Data, Priority>::clear() {
    for(size_t i = 0; i < myCapacity; ++i) {
        queueArray[i] = Data();
        pArray[i] = Priority();
    }
    head = tail = mySize = 0;
}

template<typename Data, typename Priority>
void pQueueArray<Data, Priority>::copy(const pQueueArray<Data, Priority> &other) {
    myCapacity = other.myCapacity;
    queueArray = new Data[other.myCapacity];
    pArray = new Priority[other.myCapacity];
    head = other.head;
    tail = other.tail;
    for(size_t i=head; i < myCapacity; ++i) {
        queueArray[i] = other.queueArray[i];
        pArray[i] = other.pArray[i];
    }
}

template<typename Data, typename Priority>
void pQueueArray<Data, Priority>::deleteAll() {
    for(size_t i = 0; i < myCapacity; ++i) {
        queueArray[i] = Data();
        pArray[i] = Priority();
    }
    delete [] queueArray;
    delete [] pArray;
    mySize = myCapacity = head = tail = 0;
}

template<typename Data, typename Priority>
bool pQueueArray<Data, Priority>::isGreaterOrEqual(size_t i1, size_t i2) {
    return pArray[i1] >= pArray[i2];
}

template<typename Data, typename Priority>
bool pQueueArray<Data, Priority>::isGreater(size_t i1, size_t i2) {
    return pArray[i1] > pArray[i2];
}

template<typename Data, typename Priority>
bool pQueueArray<Data, Priority>::isLessOrEqual(size_t i1, size_t i2) {
    return pArray[i1] <= pArray[i2];
}

template<typename Data, typename Priority>
bool pQueueArray<Data, Priority>::isLess(size_t i1, size_t i2) {
    return pArray[i1] < pArray[i2];
}

#endif // PQUEUEARRAY_H
