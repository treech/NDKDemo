//
// Created by ygq on 2024/11/1.
//
#include "log.h"

#ifndef YGQ_TAG
#define YGQ_TAG

template<class E>
struct Node {
    E value;
    Node<E> *next;

    Node(E value, Node<E> *next) {
        this->value = value;
        this->next = next;
    }
};

template<class E>
class LinkedList {
private:
    int len = 0;
    Node<E> *head = nullptr;
public:
    void push(E e);

    Node<E> *findNode(int index);

    E get(int index);

    int size();
};

template<class E>
E LinkedList<E>::get(int index) {
    return findNode(index)->value;
}

template<class E>
Node<E> *LinkedList<E>::findNode(int index) {
    Node<E> *last = head;
    for (int i = 0; i < index; i++) {
        last = last->next;
    }
    return last;
}

template<class E>
int LinkedList<E>::size() {
    return len;
}

template<class E>
void LinkedList<E>::push(E e) {
    LOGI("push before linkedList size:%d", len);
    Node<E> *new_node = new Node<E>(e, nullptr);
    if (head) {
        Node<E> *last = findNode(len - 1);
        last->next = new_node;
    } else {
        head = new_node;
    }
    len++;
}

#endif
