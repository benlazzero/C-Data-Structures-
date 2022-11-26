/***************************************
main.cpp
Author: Ben Lazzeroni
Date Completed: 5/25/2022
Description: Pest Control( commandline args example =$ ./a.out bugs.xml 3 )
***************************************/
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "pugixml.hpp"
#include "LinkedList.hpp"
#include "BinarySearchTree.hpp"
using namespace std;

class Bug {
  private:
    string bugId_;
    string description_;
    int impact_;
    int date_;
    int time_;

  public:
    Bug(string id, string desc, int imp, int date, int time)
      : bugId_(id), description_(desc), impact_(imp), date_(date), time_(time)
    {}
    
    bool operator < (Bug const &rhs) {
      if (impact_ < rhs.impact_) {
        return true;
      }

      if (impact_ == rhs.impact_ && date_ < rhs.date_) {
        return true;
      }

      if (impact_ == rhs.impact_ && date_ == rhs.date_ && time_ < rhs.time_) {
        return true;
      }
      return false;
    }

    bool operator > (Bug const &rhs) {
      if (impact_ > rhs.impact_) {
        return true;
      }

      if (impact_ == rhs.impact_ && date_ > rhs.date_) {
        return true;
      }

      if (impact_ == rhs.impact_ && date_ == rhs.date_ && time_ > rhs.time_) {
        return true;
      }
      return false;
    }

    bool operator == (Bug const &rhs) {
      if (impact_ == rhs.impact_ && date_ == rhs.date_ && time_ == rhs.time_) {
        return true;
      }
      return false;
    }

    bool operator != (Bug const &rhs) {
      if (impact_ == rhs.impact_ && date_ == rhs.date_ && time_ == rhs.time_) {
        return false;
      }
      return true;
    }

    string getBugId() {
      return bugId_;
    }

    string getDescription() {
      return description_;
    }

    string getImpact() {
      if (impact_ == 1) {
        return "high";
      } else if (impact_ == 2) {
        return "medium";
      } else if (impact_ == 3) {
        return "low";
      } else {
        return "no impact";
      }
    }

    string getDate() {
      string dateString = to_string(date_);
      return dateString;
    }

    string getTime() {
      string timeString = to_string(time_);
      while (timeString.length() < 6) { // adding back 0's if parse removed 
        timeString = "0" + timeString;
      }
      return "T" + timeString + "Z";
    }
};

int PointerCharToInt(char* &devCountChar) {
  stringstream stream;
  stream<<devCountChar;
  return stoi(stream.str());
};

LinkedList<Bug> CreateBugsList(string const &filepath) {
  BinarySearchTree<Bug> bugTree;
  pugi::xml_document doc;
  if (!doc.load_file(filepath.c_str())) {
    cerr<<"Problem opening xml file.\n";
    exit(3);
  }
  
  pugi::xml_node rootNode = doc.child("bugs");
  for (pugi::xml_node subNode: rootNode.children()) {
    string id = subNode.attribute("id").value();
    string description = subNode.child("description").child_value();
    string high = "high";
    string medium = "medium";
    string low = "low";
    int impact = 0;
    if (subNode.child("impact").child_value() == high) {
      impact = 1;
    } else if (subNode.child("impact").child_value() == medium) {
      impact = 2;
    } else if (subNode.child("impact").child_value() == low) {
      impact = 3;
    } else {
      impact = 0;
    }

    string rawDate = subNode.child("created").child_value();
    int date = stoi(rawDate.substr(0, rawDate.find("T")));
    
    // a mess of parsing time
    string time = "";
    char zero = '0';
    time += rawDate.substr(15 - 6); // get the time section with 'Z'
    time.pop_back(); // remove the Z
    while (time.at(0) == zero) { // remove the trailing 0's I will add back with class method
      time.erase(0, 1);
    }
    int timeInt = stoi(time);
    //-----------------------
    
    Bug bug(id, description, impact, date, timeInt);
    bugTree.insert(bug);
  }
  return bugTree.getSortedList();
}

void buildXml(LinkedList<Bug> &sortedBugs, int numberOfDevs) {
  vector<Bug> sortedBugsVect;
  for(int i = 0; i < sortedBugs.size(); i++) {
    sortedBugsVect.push_back(sortedBugs.at(i));
  }
  
  ofstream newReport("newReport.xml", ofstream::out);
  newReport<<"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
  newReport<<"<report>\n";

  for (int i = 0; i < numberOfDevs; ++i) {
    newReport<<"    <developer id=\"" + to_string(i + 1) + "\">\n";
    for (int t = 0; t < sortedBugsVect.size(); ++t) {
      if (sortedBugsVect.at(t).getImpact() == "high") {
        newReport<<"        <bug id=\"" + sortedBugsVect.at(t).getBugId() + "\">\n";
        newReport<<"            <description>" + sortedBugsVect.at(t).getDescription() + "</description>\n";
        newReport<<"            <impact>" + sortedBugsVect.at(t).getImpact() + "</impact>\n";
        newReport<<"            <created>" + sortedBugsVect.at(t).getDate() + sortedBugsVect.at(t).getTime() + "</created>\n";
        newReport<<"        </bug>\n";
        sortedBugsVect.erase(sortedBugsVect.begin() + t);
        break;
      }
    }

    for (int h = 0; h < sortedBugsVect.size(); ++h) {
      if (sortedBugsVect.at(h).getImpact() == "medium") {
        newReport<<"        <bug id=\"" + sortedBugsVect.at(h).getBugId() + "\">\n";
        newReport<<"            <description>" + sortedBugsVect.at(h).getDescription() + "</description>\n";
        newReport<<"            <impact>" + sortedBugsVect.at(h).getImpact() + "</impact>\n";
        newReport<<"            <created>" + sortedBugsVect.at(h).getDate() + sortedBugsVect.at(h).getTime() + "</created>\n";
        newReport<<"        </bug>\n";
        sortedBugsVect.erase(sortedBugsVect.begin() + h);
        break;
      }
    }

    for (int j = 0; j < sortedBugsVect.size(); ++j) {
      if (sortedBugsVect.at(j).getImpact() == "low") {
        newReport<<"        <bug id=\"" + sortedBugsVect.at(j).getBugId() + "\">\n";
        newReport<<"            <description>" + sortedBugsVect.at(j).getDescription() + "</description>\n";
        newReport<<"            <impact>" + sortedBugsVect.at(j).getImpact() + "</impact>\n";
        newReport<<"            <created>" + sortedBugsVect.at(j).getDate() + sortedBugsVect.at(j).getTime() + "</created>\n";
        newReport<<"        </bug>\n";
        sortedBugsVect.erase(sortedBugsVect.begin() + j);
        break;
      }
    }
    newReport<<"    </developer>\n";
  }
  
  if (sortedBugsVect.size() != 0) {
    newReport<<"    <remaining>\n";
    for (Bug k : sortedBugsVect) {
      newReport<<"        <bug id=\"" + k.getBugId() + "\">\n";
      newReport<<"            <description>" + k.getDescription() + "</description>\n";
      newReport<<"            <impact>" + k.getImpact() + "</impact>\n";
      newReport<<"            <created>" + k.getDate() + k.getTime() + "</created>\n";
      newReport<<"        </bug>\n";
    }
    newReport<<"    </remaining>\n";
  }
  newReport<<"</report>";
  newReport.close();
}

int main(int argc, char* argv[]) {
  if (argv[1] == NULL || argv[2] == NULL) {
    cout<<"CLA Error\nFormat: exe buglist-filepath totaldevelopers\nExample: ./a.out bugs.xml 10\n";
  } else {
    LinkedList<Bug> sortedBugs = CreateBugsList(argv[1]);
    if (sortedBugs.size() != 0) {
      buildXml(sortedBugs, PointerCharToInt(argv[2]));
    }
  }
  
  return 0;
}