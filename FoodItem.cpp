#include <string>
#include "FoodItem.h"
#include "boost/date_time/gregorian/gregorian.hpp"
using namespace std;
using namespace boost::gregorian;
FoodItem::FoodItem(string code, string foodName,days shelfLife)
{
  this->UPC_code = code;
  this->name = foodName;
  this->life = shelfLife;
}

FoodItem:: FoodItem(const FoodItem & other)
{
  this->UPC_code = other.UPC_code;
  this->name = other.name;
  this->life= other.life;
}
