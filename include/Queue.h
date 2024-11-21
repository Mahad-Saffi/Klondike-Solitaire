#pragma once

#include "LinkedList.h"

class Queue : protected LinkedList {
private:
    Node* tail;

public:
    Queue() : tail(nullptr) {}

    void enqueue(const Card& card) {
        auto newNode = std::make_unique<Node>(card);
        Node* newTail = newNode.get(); // Raw pointer to the new node
        if (isEmpty()) {
            head = std::move(newNode);
        } else {
            tail->next = std::move(newNode);
        }
        tail = newTail;
        size++;
    }

    Card dequeue() {
        if (isEmpty()) {
            throw std::runtime_error("Queue is Empty");
        }
        Card card = head->data; // Copy the data
        head = std::move(head->next); // Move ownership to the next node
        if (!head) {
            tail = nullptr;
        }
        size--;
        return card;
    }

    Card peek() const {
        if (isEmpty()) {
            throw std::runtime_error("Queue is Empty");
        }
        return head->data; // Return a copy of the data
    }

    using LinkedList::isEmpty;
    using LinkedList::getSize;
};