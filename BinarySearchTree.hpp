/***************************************
Make a binary search tree
Author: Ben Lazzeroni
Date Completed: 5/12/2022
Description: Implement private member functions for the class binary search tree.
***************************************/
#ifndef BINARY_SEARCH_TREE_HPP 
#define BINARY_SEARCH_TREE_HPP 

#include <iostream>
#include "LinkedList.hpp"
using std::size_t;
using std::cout;


template <typename T>
class BinarySearchTree
{
    private:

    struct Node
    {
        Node* leftChild;
        Node* rightChild;
        T data;

        Node(T d) : data(d), leftChild(nullptr), rightChild(nullptr){}
    }* root_;


    bool insert(T &data, Node* &subRoot) {
      if (subRoot == nullptr) {
        subRoot = new Node(data);
        return true;
      } else if (data > subRoot->data) {
        if (data != subRoot->data) {
          return insert(data, subRoot->rightChild);
        }
      }
      
      if (data != subRoot->data) {
        return insert(data, subRoot->leftChild);
      }
      
      return false;
    };

    Node* findMinimumNode(Node* subRoot) {
      if (subRoot == nullptr || subRoot->leftChild == nullptr) {
        return subRoot;
      }

      findMinimumNode(subRoot->leftChild);

    };

    Node* findMaximumNode(Node* subRoot) {
      if (subRoot == nullptr || subRoot->rightChild == nullptr) {
        return subRoot;
      }
      
      findMaximumNode(subRoot->rightChild);
    };

    bool remove(T &data, Node* &subRoot) {
      if (subRoot == nullptr) {
        return false;
      }

      if(subRoot != nullptr) {
        if (data == subRoot->data) {
          if (subRoot->leftChild == nullptr && subRoot->rightChild == nullptr) {
            delete subRoot;
            subRoot = nullptr;
            return true;
          } else if (subRoot->leftChild != nullptr && subRoot->rightChild != nullptr) {
            subRoot->data = findMaximumNode(subRoot->leftChild)->data;
            remove(findMaximumNode(subRoot->leftChild)->data, subRoot->leftChild);
            return true;
          } else if (subRoot->leftChild == nullptr) {
            Node* temp = subRoot;
            Node* link = nullptr;
            link = subRoot->rightChild;
            subRoot = link;
            delete temp;
            temp = nullptr;
            return true;
          } else if (subRoot->rightChild == nullptr) {
            Node* temp = subRoot;
            Node* link = nullptr;
            link = subRoot->leftChild;
            subRoot = link;
            delete temp;
            temp = nullptr;
            return true;
          }
        }
      }

      if (data < subRoot->data) {
        return remove(data, subRoot->leftChild);
      }
      if (data > subRoot->data) {
        return remove(data, subRoot->rightChild);
      }
      
      return false;
    };

    size_t size(Node* subRoot) {
      if (subRoot == nullptr) {
        return 0;
      }

      return size(subRoot->leftChild) + 1 + size(subRoot->rightChild);
    };

    void clear(Node* &subRoot) {
      if (subRoot != nullptr) {
        clear(subRoot->leftChild);
        clear(subRoot->rightChild);
        delete subRoot;
        subRoot = nullptr;
      }
    };

    void getSortedList(Node* subRoot, LinkedList<T> &returnList) {
      if (subRoot->leftChild != nullptr) {
        getSortedList(subRoot->leftChild, returnList);
      }

      returnList.pushBack(subRoot->data);

      if (subRoot->rightChild != nullptr) {
        getSortedList(subRoot->rightChild, returnList);
      } 

    };




    public:

    BinarySearchTree() : root_(nullptr){}

    bool insert(T data)
    {
        return insert(data, root_);
    }

    T const * findMinimum()
    {
        Node* min = findMinimumNode(root_);
        if (min != nullptr)
        {
            return &min->data;
        }

        return nullptr;
    }

    T const * findMaximum()
    {
        Node* max = findMaximumNode(root_);
        if (max != nullptr)
        {
            return &max->data;
        }

        return nullptr;
   }

    bool remove(T data)
    {
        return remove(data, root_);
    }

    size_t size()
    {
        return size(root_);
    }

    void clear()
    {
        clear(root_);
    }

    LinkedList<T> getSortedList()
    {
        LinkedList<T> returnList;
        getSortedList(root_, returnList);
        return returnList;
    }

    ~BinarySearchTree()
    {
        clear(root_);
    }
};

#endif
