#ifndef ESSENCE_H
#define ESSENCE_H

#include <string>

using namespace std;

class Essence
{
private:
  string name;
  int masse_V;
  bool type; // représente le type de l'essence : 0 pour résineux, 1 pour feuillu
  
public:
  // Constructors
  Essence(string nom, int masse, bool t);
  Essence(const Essence& other);
  // Getters and setters
  const string getName(){return name;};
  const int getMasse(){return masse_V;};
  const bool getType(){return type;};
  
  void setName(string x);
  void setMasse(int x);
  void setType(bool t);
  // Operators
  virtual Essence& operator=(const Essence& other);
  // Others Methods
  
};

#endif // ESSENCE_H
