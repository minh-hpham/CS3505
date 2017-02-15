#include <string>
#include "FoodItem.h"
#include "boost/date_time/gregorian/gregorian.hpp"
using namespace std;
using namespace boost::gregorian;
FoodItem::FoodItem(string code, string name, string _date)
{
  this->UPC_code = code;
  this->_name = name;
  //this->received=from_us_string(_date);
}
