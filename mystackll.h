#ifndef myStackLL_H
#define myStackLL_H

#include <cstdlib>
#include <iostream>

enum MYSTACKLL_ERRORS {
    LL_STACK_EMPTY,
    LL_STACK_FULL,
    LL_BAD_SIZE,
};

template<typename T>
struct node {
    T data;
    node *next;
    node(const T &data) {
        this->data = data;
        next = NULL;
    }
};

template<typename T>
class myStackLL {

public:

    myStackLL(size_t capacity = 10);
    ~myStackLL();
    myStackLL(const myStackLL<T> &other);
    myStackLL<T> &operator=(const myStackLL<T> &other);

    bool empty();
    bool full();
    void push(const T &x);
    void clear();
    T pop();
    T peek();
    size_t size();
    size_t capacity();

private:

    T *stack;
    int tos;   // "top of stack"

    node<T> *head;
    size_t mySize, myCapacity;

    void copy(const myStackLL<T> &other);
    void deleteAll(node<T> *&ptr);

};

template<typename T>
myStackLL<T>::myStackLL(size_t capacity) {
    if(capacity < 2)
        throw LL_BAD_SIZE;
    myCapacity = capacity;
    mySize = 0;
    head = NULL;
}

template<typename T>
myStackLL<T>::~myStackLL() {
    deleteAll(head);
    head = NULL;
    mySize = myCapacity = 0;
}

template<typename T>
myStackLL<T>::myStackLL(const myStackLL<T> &other) {
    copy(other);
}

template<typename T>
myStackLL<T> &myStackLL<T>::operator=(const myStackLL<T> &other) {
    if(this != &other) {
        deleteAll(other);   //// FISHY!!!
        copy(other);
    }
    return *this;
}

template<typename T>
bool myStackLL<T>::empty() {
    return !mySize;
}

template<typename T>
bool myStackLL<T>::full() {
    return mySize == myCapacity;
}

template<typename T>
void myStackLL<T>::push(const T &x) {
    if(full())
        throw LL_STACK_FULL;
    if(empty()) {
        head = new node<T>(x);
    }
    else {
        node<T> *newNode = new node<T>(x);
        newNode->next = head;
        head = newNode;
    }
    ++mySize;
}

template<typename T>
void myStackLL<T>::clear() {
    deleteAll(head);
    head = NULL;
    mySize = 0;
}

template<typename T>
T myStackLL<T>::pop() {
    if(empty())
        throw LL_STACK_EMPTY;
    T tempData = head->data;
    head = head->next;
    --mySize;
    return tempData;
}

template<typename T>
T myStackLL<T>::peek() {
    if(empty())
        throw LL_STACK_EMPTY;
    return head->data;
}

template<typename T>
size_t myStackLL<T>::size() {
    return mySize;
}

template<typename T>
size_t myStackLL<T>::capacity() {
    return myCapacity;
}

template<typename T>
void myStackLL<T>::copy(const myStackLL<T> &other) {
    for(node<T> *ptr = other.head; ptr; ptr = ptr->next)
        push(ptr->data);
}

template<typename T>
void myStackLL<T>::deleteAll(node<T> *&ptr) {
    if(ptr)
        deleteAll(ptr->next);
    delete ptr;
}

#endif // myStackLL_H
