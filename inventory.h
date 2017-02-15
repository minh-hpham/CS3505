#ifndef INVENTORY_H
#define INVENTORY_H

#include "FoodItem.h"
#include <map>
#include <queue>
#include <vector>
#include <string>
//#include <fstream>
class inventory
{
 private:
  std::map<std::string,std::queue<FoodItem> > warehouse;
  std::vector<std::string> lines;
  std::vector<std::string> upc_list;
  //date current;


 public:
  inventory(std::string _fileName);
  ~inventory();
};

#endif
