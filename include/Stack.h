#pragma once

#include "LinkedList.h"

class Stack : public LinkedList {
public:
    Stack() = default;

    Stack(const Stack& other) {
        if (other.head) {
            head = std::make_unique<Node>(other.head->data);
            Node* current = head.get();
            Node* otherCurrent = other.head->next.get();
            while (otherCurrent) {
                current->next = std::make_unique<Node>(otherCurrent->data);
                current = current->next.get();
                otherCurrent = otherCurrent->next.get();
            }
        }
        size = other.size;
    }

    void push(const Card& card) {
        auto newNode = std::make_unique<Node>(card);
        newNode->next = std::move(head);
        head = std::move(newNode);
        size++;
    }

    Card pop() {
        if (isEmpty()) {
            throw std::runtime_error("Stack is Empty");
        }
        Card card = head->data; // Make a copy of the data
        head = std::move(head->next); // Move ownership to the next node
        size--;
        return card;
    }

    Card peek() const {
        if (isEmpty()) {
            throw std::runtime_error("Stack is Empty");
        }
        return head->data; // Return a copy of the data
    }

    using LinkedList::isEmpty;
    using LinkedList::getSize;
};