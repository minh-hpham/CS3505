#ifndef FOODITEM_H
#define FOODITEM_H
#include "boost/date_time/gregorian/gregorian.hpp"
class FoodItem
{
 private:
  std::string UPC_code;
  std::string _name;
  // boost::gregorian::date received;
 public:
  FoodItem(std::string code, std::string name, std::string date);
};
#endif
