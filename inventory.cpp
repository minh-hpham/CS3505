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

/*
 NOTE: seem like we should make the warehouse as a map of map of each type of 
FoodItem. 
PROBLEM: then it's hard to determine which food is expired to remove it 
because we will have many queue of FoodItem

 */


inventory::inventory(string fileName)
{
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
  FoodItem f(code,name,days(atoi(life.c_str())));
  upc_list.insert(make_pair(code,f)); // make a map  of all food type
}

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
	  this->current=getDate(word);
	}
    } 
}
void inventory::addReceive(std::string & line)
{
}
void inventory::addRequest(std::string & line)
{
}
void inventory::nextDay()
{
}
void inventory::end()
{
}
