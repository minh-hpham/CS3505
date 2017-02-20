
/*
 * Teammates: Minh Pham (U0975798) and To Tang (U0529788)
 */

#ifndef INVENTORY_H
#define INVENTORY_H
#include <map>
#include <queue>
#include <vector>
#include <string>
#include "boost/date_time/gregorian/gregorian.hpp"
class inventory
{
 private:
  // warehouse is a map.Each value is a map with key is UPC_code of food item 
  // and value contains all the received days of that food item
  std::map<std::string,std::map<std::string,std::queue<int> > > warehouse;
  // upc_days is a map of upc_code and its life time
  std::map<std::string,int > upc_days;
  // upc_name is a map of upc_code and its name
  std::map<std::string,std::string > upc_name;
  // popular is a map of upc_code and its requested number
  std::map<std::string, int> popular;
  int current;

  // helper method
  void addFoodItem(std::string & line);
  void addReceive(std::string & line);
  void checkExpire(std::queue <int > & dates, const int shelfLife );
  void addRequest(std::string & line);
  void nextDay();
  void end();

 public:
  inventory(std::string _fileName);
  ~inventory();
};

#endif
