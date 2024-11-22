//
// Created by ygq on 2024/11/1.
//
#include "log.h"

#ifndef YGQ_TAG
#define YGQ_TAG

template<class E>
struct Node {
    E value;
    Node<E> *pre;
    Node<E> *next;

    Node(E value, Node<E> *pre, Node<E> *next) {
        this->value = value;
        this->pre = pre;
        this->next = next;
    }
};

template<class E>
class LinkedList {
private:
    int len = 0;
    Node<E> *head = nullptr;
    Node<E> *end = nullptr;
public:
    void push(E e);

    void insert(int index, E e);

    E remove(int index);

    Node<E> *findNode(int index);

    E get(int index);

    int size();

    void linkLast(E e);

    void linkBefore(Node<E> *cur, E e);

    ~LinkedList();
};

template<class E>
LinkedList<E>::~LinkedList() {
    Node<E> *new_head = head;
    while (new_head) {
        Node<E> *temp = new_head->next;
        delete new_head;
        new_head = temp;
    }
    head = NULL;
    end = NULL;
}

/*
template<class E>
void LinkedList<E>::push(E e) {
    Node<E> *new_node = new Node<E>(e, nullptr);
    if (head) {
        Node<E> *last = findNode(len - 1);
        last->next = new_node;
    } else {
        head = new_node;
    }
    len++;
}*/

template<class E>
void LinkedList<E>::push(E e) {
    Node<E> *new_node = new Node<E>(e, end, nullptr);
    if (head) {
        end->next = new_node;
        end = new_node;
    } else {
        head = new_node;
        end = new_node;
    }
    len++;
}

template<class E>
void LinkedList<E>::linkLast(E e) {
    Node<E> *new_node = new Node<E>(e, end, nullptr);
    if (head) {
        end->next = new_node;
        end = new_node;
    } else {
        head = new_node;
        end = new_node;
    }
}

template<class E>
void LinkedList<E>::linkBefore(Node<E> *cur, E e) {
    Node<E> *new_node = new Node<E>(e, cur->pre, cur);
    cur->pre->next = new_node;
    cur->pre = new_node;
}

template<class E>
void LinkedList<E>::insert(int index, E e) {
    if (index > len - 1 || index < 0) {
        LOGE("index:%d illegal,must <= %d or > 0", index, len - 1);
        return;
    }
    Node<E> *node = findNode(index);
    linkBefore(node, e);
    len++;
}

template<class E>
E LinkedList<E>::remove(int index) {
    if (index > len - 1 || index < 0) {
        LOGE("index:%d illegal,must <= %d or > 0", index, len - 1);
        return E();
    }
    Node<E> *cur = findNode(index);
    if (cur->pre) {
        cur->pre->next = cur->next;
    } else {//头节点
        head = cur->next;
    }
    if (cur->next) {
        cur->next->pre = cur->pre;
    } else {//尾节点
        end = cur->pre;
    }
    E value = cur->value;
    delete cur;
    len--;
    return value;
}

template<class E>
E LinkedList<E>::get(int index) {
    return findNode(index)->value;
}

template<class E>
Node<E> *LinkedList<E>::findNode(int index) {
    if (index > len >> 1) {
        //从后往前遍历
        Node<E> *cur = end;
        for (int i = len - 1; i > index; --i) {
            cur = cur->pre;
        }
        return cur;
    } else {
        //从前往后遍历
        Node<E> *cur = head;
        for (int i = 0; i < index; ++i) {
            cur = cur->next;
        }
        return cur;
    }
}

template<class E>
int LinkedList<E>::size() {
    return len;
}

#endif
