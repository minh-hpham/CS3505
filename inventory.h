#ifndef INVENTORY_H
#define INVENTORY_H

#include "FoodItem.h"
#include <map>
#include <queue>
#include <vector>
#include <string>
#include "boost/date_time/gregorian/gregorian.hpp"
class inventory
{
 private:
  std::map<std::string,std::map<std::string,std::queue< boost::gregorian::date > > > warehouse;
  std:: vector<std::string>warehouse_list;
  std::vector<std::string> lines;
  std::map<std::string, FoodItem> upc_list;
  std::map<std::string, int> popular;
  boost::gregorian::date current;

  void addFoodItem(std::string & line);
  void addWarehouse(std::string &  line);
  void addStartDate(std::string & line);
  void addReceive(std::string & line);
  void checkExpire(std::queue < boost::gregorian::date > dates, boost::gregorian::days shelfLife );
  void addRequest(std::string & line);
  void nextDay();
  void end();

 
  


 public:
  inventory(std::string _fileName);
  ~inventory();
};

#endif
