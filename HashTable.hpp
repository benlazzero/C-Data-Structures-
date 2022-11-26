/***************************************
makeAHashTable.cpp
Author: Ben Lazzeroni
Date Completed: 4/15/22
Description: HashTable
***************************************/
#ifndef HASH_TABLE_HPP 
#define HASH_TABLE_HPP 

#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include "LinkedList.hpp"
using std::array;
using std::string;
using std::pair;
using std::size_t;
using std::cout;

template <typename T, size_t TABLE_SIZE=11>
class HashTable
{
    private:

    array<LinkedList<pair<string, T>>, TABLE_SIZE> table_;

    size_t hash(string const &key) const
    {
        size_t sum(0);
        for (char c : key)
        {
            sum += c;
        }
        
        return sum % TABLE_SIZE;
    }

    using listIter = typename LinkedList<pair<string, T>>::Iterator;

    public:

    T* get(string const &key) {

      for( int i = 0; i < table_.size(); i++) {
        for (listIter iter = table_.at(i).generateIterator(); iter.hasNext(); iter.next()) {
          if (iter.current()->first == key) {
            return &iter.current()->second;
          }
        }
      }

      return nullptr;
    
    };

    bool add(string const &key, T const &value){
      pair<string, T> data;
      data.first = key;
      data.second = value;

      if (get(key) == nullptr){
        table_.at(hash(data.first)).pushFront(data);
        return true;
      }
      
      return false;
    };

    void remove(string const &key){
        int table = hash(key);
        int indexCount = 0;
        

        for (listIter iter = table_.at(table).generateIterator(); iter.hasNext(); iter.next()) {
          if (iter.current()->first == key) {
             
            table_.at(hash(key)).removeAt(indexCount);
            break;
          }
          indexCount+=1;
        }
    };

    size_t size(){
      int count = 0;
      for( int i = 0; i < table_.size(); i++){
        int temp = table_.at(i).size();
        count += temp;
      }
      return count;
    };

    void clear(){
      for (int i = 0; i < table_.size(); i++) {
        table_.at(i).clear();
      }
    };


    std::vector<string> keys() {
      std::vector<string> returnList;
      returnList.reserve(size());

      for (LinkedList<pair<string, T>> &list : table_) {
        for (listIter iter = list.generateIterator(); iter.hasNext(); iter.next()) {
          returnList.push_back(iter.current()->first);
        }
      }
      return returnList;
    }
};





#endif
