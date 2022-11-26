/***************************************
LinkedList.cpp
Author: Ben Lazzeroni
Date Completed: 3/29/22
Description: linked list
***************************************/
#ifndef LINKED_LIST_HPP 
#define LINKED_LIST_HPP

#include <stdexcept>
#include <sstream>

template <typename T>
class LinkedList {
  private:

  struct Node {
    Node* next;
    T data;

    Node(T d, Node* n) : data(d), next(n){}
  }* head_;

    public:

   LinkedList() : head_(nullptr){};

   LinkedList(LinkedList const &other) {
      Node* iter = other.head_;

      while (iter != nullptr) {
        pushBack(iter->data);
        iter = iter->next;
      }
   }

   LinkedList & operator=(LinkedList const &rhs) {
     if (this != &rhs) {
       clear();

       Node* iter = rhs.head_;

       while (iter != nullptr) {
         pushBack(iter->data);
         iter = iter->next;
       }
     }
     return *this;
   }

    size_t size() {
      size_t counter(0);
      Node* iter = head_;
      while (iter != nullptr) {
        iter = iter->next;
        ++counter;
      }
      return counter;
    }

    void pushFront(T value) {
      head_ = new Node(value, head_);
    };
        
    void pushBack(T value) {
      if (head_ == nullptr) {
        pushFront(value);
      } else {
        Node* iter = head_;
        while (iter->next != nullptr) {
          iter = iter->next;
        }
        iter->next = new Node(value, nullptr);
      }
    }

    T at(size_t index) {
      if (index+1 > size()) {
        throw std::out_of_range("Index out of range");
      }

      Node* iter = head_;
      for (size_t i = 0; i < index; ++i) {
        iter = iter->next;
      }
      return iter->data;
    }

    T popFront() {
      if (head_ == nullptr) {
        throw std::logic_error("Can't pop from empty list");
      }

      T returnData = head_->data;

      Node* temp = head_->next;
      delete head_;
      head_ = temp;

      return returnData;
    }

    bool removeAt(size_t index) {
      if (index+1 > size()) {
        return false;
      }

      if (index == 0) {
        popFront();
      } else {
        Node* iter = head_;
        for (size_t i(0); i < index-1; ++i) {
          iter = iter->next;
        }

        Node* nodeToRemove = iter->next;
        iter->next = nodeToRemove->next;
        delete nodeToRemove;
      }
      return true;
    }

    void clear() {
      while (head_ != nullptr) {
        popFront();
      }
    }

    ~LinkedList() {
      clear();
    }

    class Iterator {
      private:
      Node* current_;

      public:

      Iterator(Node* head) : current_(head){}

      T* current(){
        if (current_ == nullptr) {
          return nullptr;
        }
        return &current_->data;
      }

      void next() {
        if (current_ != nullptr) {
          current_ = current_->next;
        }
      }

      bool hasNext() {
        return current_ != nullptr;
      }

      bool hasNextNext() {
        return current_->next != nullptr;
      }

    };

    Iterator generateIterator() {
      return Iterator(head_);
    }

};

#endif



