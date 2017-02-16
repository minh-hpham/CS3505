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
  upc_days[code] = days(d);
  upc_name[code] = name;
  //FoodItem f(code,name,days(d));
  //pair<const string, FoodItem> toInsert = pair<const string, FoodItem>(code, f);
  
 
}

void inventory::addReceive(std::string & line)
{
  istringstream ss(line);
  string UPC, city;
  int quantity;
    
  while(true)
    {
      string word;
      ss >> word;
      
      if(ss.fail())
	break;

      else if (word.compare("Receive:") == 0)
	{
	  ss >> word;
	  UPC=word;
	  ss >> word;
	  quantity=atoi(word.c_str());
	  ss >> word;
	  city=word;
	}
    }

  for (int i=0; i<quantity; i++)
    {
      warehouse[city][UPC].push(current);
    }
       
}

void inventory::checkExpire( queue <boost::gregorian::date> & dates, const boost::gregorian::days & shelfLife)
{
  while (this->current - dates.front() > shelfLife){
    dates.pop(); // if expired,pop from the queue
  }
}

void inventory::addRequest(std::string & line)
{
  istringstream ss(line);
  string UPC, city;
  int quantity;
  
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

  days shelfLife = upc_days[UPC];
  /* if(upc_list.find(UPC) != upc_list.end())
     shelfLife =  upc_list[UPC].life;*/
  
    
  //update popular request map
  popular[UPC]++;
  
  if (warehouse.find(city) == warehouse.end()) // warehouse doesn't exist
    { // do nothing
    }
  else
    {
      //aWarehouse = warehouse[city];
      if (warehouse[city].find(UPC)==warehouse[city].end()) // item doesn't exist
	{ // do nothing
	}
      else // item exist
	{
	   
	  checkExpire(warehouse[city][UPC], shelfLife);

	  if(warehouse[city][UPC].size() > quantity)
	    {
	      for (int i=0; i<quantity; i++)
		{
		  warehouse[city][UPC].pop();
		}
	    }
	  else
	    {
	      warehouse[city].erase(UPC); // if doesn't have enough, get rid of item
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
