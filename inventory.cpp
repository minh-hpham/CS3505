#include "inventory.h"
#include <iostream>
#include "FoodItem.h"
#include <map>
#include <queue>
#include <vector>
#include <string>
#include <fstream> //read lines in file
#include <sstream> //read words in line
#include "boost/date_time/gregorian/gregorian.hpp"
using namespace std;
using namespace boost::gregorian;


inventory::inventory(string fileName)
{

  string today = "01/01/2017";
  this->current = from_us_string(today);

  ifstream in(fileName.c_str());
  while(true)
    {
      string line;
      getline(in,line);
      if(in.fail())
	break;
      else if(line.find("FoodItem") == 0)
	addFoodItem(line);
      else if(line.find("Warehouse") == 0)
	addWarehouse(line);
      else if(line.find("Start") == 0)
	addStartDate(line);
      else if(line.find("Next") == 0)
	nextDay();
      else if(line.find("Receive") == 0)
	addReceive(line);
      else if(line.find("Request") == 0)
	addRequest(line);
      else
	end();
      
      
    }
  in.close();
}

inventory:: ~inventory()
{

}
void inventory::addFoodItem(std::string & line)
{
  istringstream ss(line);
  string code,name,life;
  while(true)
    {
      string word;
      ss >> word;
      
      if(ss.fail())
	break;

      else if (word.compare("Code:") == 0)
	{
	  ss >> word;
	  code = word;	
	}
      else if(word.compare("life:") == 0)
	{
	  ss >> word;
	  life = word;
	}
      else if(word.compare("Name:") == 0)
	{
	  do
	    {
	      ss >> word;
	      name.append(word);
	    }while(ss.fail() == false);
	}
    }

  int d = atoi(life.c_str());
  FoodItem f(code,name,days(d));
  upc_list.insert(std::pair<std::string,FoodItem>(code,f)); // make a map  of all food type
}
/*
// set date format
const std::locale fmt2(std::locale::classic(),
                       new boost::gregorian::date_input_facet("%m/%d/%Y"));
//get date from string
boost::gregorian::date getDate( const std::string& str)
{
  std::istringstream is(str);
  is.imbue(fmt2);
  boost::gregorian::date date;
  is >> date;
  return date;
}
*/
void inventory::addWarehouse(std::string & line)
{
  istringstream ss(line);
  string name;
  while(true)
    {
      string word;
      ss >> word;
      
      if(ss.fail())
	break;

      else if (word.compare("-") == 0)
	{
	  ss >> word;
	  warehouse_list.push_back(word);
	}
    } 
  
}
void inventory::addStartDate(std::string & line)
{
  istringstream ss(line);
  string date;
  while(true)
    {
      string word;
      ss >> word;
      
      if(ss.fail())
	break;

      else if (word.compare("date:") == 0)
	{
	  ss >> word;
	  date = word;
	  
	  //getDate(word);
	}
    }

  /*  boost::gregorian:: date d(from_us_string(date));
      this->current = d;*/
}
void inventory::addReceive(std::string & line)
{
  istringstream ss(line);
  string UPC, city;
  int quantity;
  map <string, queue <date> > aWarehouse;
  queue <date> receiveDate;
  while(true)
    {
      string word;
      ss >> word;
      
      if(ss.fail())
	break;

      else if (word.compare("receive:") == 0)
	{
	  ss >> word;
	  UPC=word;
	  ss >> word;
	  quantity=atoi(word.c_str());
	  ss >> word;
	  city=word;
	}
    } 
  
  if (warehouse.find(city) == warehouse.end()) // warehouse doesn't exist
    {
      for (int i=0; i<quantity; i++)
	{
	  receiveDate.push(current);
	}
      aWarehouse[UPC]=receiveDate;
      warehouse[city]=aWarehouse;
    }
  else // warehouse exist
    {
      aWarehouse=warehouse[city];
      if (aWarehouse.find(UPC)==aWarehouse.end()) // food item doesn't exist
	{
	  for (int i=0; i<quantity; i++)
	    {
	      receiveDate.push(current);
	    }
	  aWarehouse[UPC]=receiveDate;
	}
      else //food item exist, update quantity
	{
	  receiveDate=aWarehouse[UPC]; //get the queue to update
	  for (int i=0; i<quantity; i++)
	    {
	      receiveDate.push(current);
	    }
	}
    }
}

void inventory::checkExpire( queue <boost::gregorian::date> dates, days shelfLife )
{
  // boost::gregorian::date now = current;
  while (this->current - dates.front() > shelfLife){
    dates.pop(); // if expired,pop from the queue
  }
}

void inventory::addRequest(std::string & line)
{
  istringstream ss(line);
  string UPC, city;
  int quantity;
  map <string, queue <date> > aWarehouse;
  queue <date> receiveDate;
  
  while(true)
    {
      string word;
      ss >> word;
      
      if(ss.fail())
	break;

      else if (word.compare("Request:") == 0)
	{
	  ss >> word;
	  UPC=word;
	  ss >> word;
	  quantity=atoi(word.c_str());
	  ss >> word;
	  city=word;
	}
    } 
  days shelfLife = upc_list[UPC].life;
  //update popular request map
  popular[UPC]++;
  
  if (warehouse.find(city)==warehouse.end()) // warehouse doesn't exist
    { // do nothing
    }
  else
    {
      aWarehouse = warehouse[city];
      if (aWarehouse.find(UPC)==aWarehouse.end()) // item doesn't exist
	{ // do nothing
	}
      else // item exist
	{
	  checkExpire(aWarehouse[UPC],shelfLife);
	  //receiveDate =  checkExpire(aWarehouse[UPC],shelfLife);
	  // if (receiveDate.size()>quantity) // if has more the required #
	  if(aWarehouse.size() > quantity)
	    {
	      for (int i=0; i<quantity; i++)
		{
		   receiveDate.pop();
		   //aWarehouse.pop();
		}
	    }
	  else
	    {
	      aWarehouse.erase(UPC); // if doesn't have enough, get rid of item
	    }
	}
    }
}
void inventory::nextDay()
{
  date_duration dd(1);
  current = current + dd;
}
void inventory::end()
{
}
