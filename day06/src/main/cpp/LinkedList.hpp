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

    void insert(int index, E e);

    void remove(int index);

    Node<E> *findNode(int index);

    E get(int index);

    int size();
};

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

template<class E>
void LinkedList<E>::insert(int index, E e) {
    if (index > len - 1 || index < 0) {
        LOGE("index:%d illegal,must <= %d or > 0", index, len - 1);
        return;
    }
    Node<E> *new_node = new Node<E>(e, nullptr);
    if (index == 0) {
        new_node->next = head;
        head = new_node;
    } else {
        Node<E> *pre = findNode(index - 1);
        new_node->next = pre->next;
        pre->next = new_node;
    }
    len++;
}

template<class E>
void LinkedList<E>::remove(int index) {
    if (index > len - 1 || index < 0) {
        LOGE("index:%d illegal,must <= %d or > 0", index, len - 1);
        return;
    }
    if (index == 0) {
        Node<E> *cur = findNode(index);
        head = cur->next;
        delete cur;
    } else {
        Node<E> *pre = findNode(index - 1);
        Node<E> *cur = pre->next;
        pre->next = cur->next;
        delete cur;
    }
    len--;
}

template<class E>
E LinkedList<E>::get(int index) {
    return findNode(index)->value;
}

template<class E>
Node<E> *LinkedList<E>::findNode(int index) {
    Node<E> *last = head;
    for (int i = 0; i < index; ++i) {
        last = last->next;
    }
    return last;
}

template<class E>
int LinkedList<E>::size() {
    return len;
}

#endif
