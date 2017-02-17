#ifndef INVENTORY_H
#define INVENTORY_H

//#include "FoodItem.h"
#include <map>
#include <queue>
#include <vector>
#include <string>
#include "boost/date_time/gregorian/gregorian.hpp"
class inventory
{
 private:
  std::map<std::string,std::map<std::string,std::queue<int> > > warehouse;
  std::map<std::string,int > upc_days;
  std::map<std::string,std::string > upc_name;
  std::map<std::string, int> popular;
  int current;

  void addFoodItem(std::string & line);
  void addReceive(std::string & line);
  void checkExpire(std::queue <int > & dates, const int  & shelfLife );
  void addRequest(std::string & line);
  void nextDay();
  void end();

 
  


 public:
  inventory(std::string _fileName);
  ~inventory();
};

#endif
