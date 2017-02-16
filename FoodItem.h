#ifndef FOODITEM_H
#define FOODITEM_H
#include "boost/date_time/gregorian/gregorian.hpp"
class FoodItem
{
  friend class inventory;
 private:
  std::string UPC_code;
  std::string name;
  boost::gregorian::date received;
  boost::gregorian::days life;
 public:
  FoodItem(std::string code, std::string name,boost::gregorian::days shelfLife);
  FoodItem(const FoodItem & other);
};
#endif
