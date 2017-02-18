#include "inventory.h"
#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <string>
#include <iterator>
#include <fstream> //read lines in file
#include <sstream> //read words in line

using namespace std;

inventory::inventory(string fileName)
{

  this->current = 1;

  ifstream in(fileName.c_str());
  string line;
  while(getline(in,line))
    {  
      if(line.find("FoodItem") == 0)
	addFoodItem(line);     
      else if(line.find("Receive:") == 0)
        addReceive(line);
      else if(line.find("Request:") == 0)
        addRequest(line);
      else if(line.find("Next") == 0)
	nextDay();   
      else if(line.find("End") == 0)
	{
	  end();
	  // Stop reading the instructions and stop modifying inventories
	  break;
	}
      else{}          
   }
  in.close();
}

inventory:: ~inventory()
{

}

/*
 * For every food item, find its code, name, life time 
 * and add those information to the corresponding maps
 */
void inventory::addFoodItem(std::string & line)
{
  istringstream ss(line);
  string code,name;
  int  life;
  while(true)
    {
      string word;
      ss >> word;
      
      if(ss.fail())
	break;
      // get the UPC code of food item
      else if (word.compare("Code:") == 0)
	{
	  ss >> word;
	  code = word;
	    
	}
      // get the life time of food item
      else if(word.compare("life:") == 0)
	{
	  ss >> word;
	  life = atoi(word.c_str());
	}
      // get the name of food item
      else if(word.compare("Name:") == 0)
	{
	  ss >> word;
	  while(ss.fail() == false)
	  {	    
	    name.append(word);
	    name.append(" ");
	    ss >> word;
	  }
	}
    }
  // upc_days is a map of upc_code and its life time
  upc_days[code] = life;
  // upc_name is a map of upc_code and its name
  upc_name[code] = name;
  
 
}

/*
 *Record the number of item the warehouse receives
 */
void inventory::addReceive(std::string & line)
{
  istringstream ss(line);
  string city,UPC;
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
	  // quantities of the items that warehouse receives
	  quantity=atoi(word.c_str());
	  ss >> word;
	  // name of the warehouse
	  city=word;
	}
    }

  for (int i=0; i<quantity; i++)
    {
      // warehouse is a map.Each value is a map 
      // with key is UPC_code of food item 
      // and value contains all the received days of that food item
      warehouse[city][UPC].push(current);
    }
       
}

/*
 * Remove all the expired food
 */
void inventory::checkExpire( queue <int> & dates, const int shelfLife)
{
  while(!dates.empty()&& ((this->current - dates.front()) > shelfLife)){
    dates.pop(); // if expired,pop from the queue
  }
}

void inventory::addRequest(std::string & line)
{

  istringstream ss(line);
  string city, UPC;
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
	  // requested quantities
	  quantity=atoi(word.c_str());
	  ss >> word;
	  city=word;
	}
    } 
    
  // update popular requested map
  // This map is used to show the popular products
  popular[UPC]+= quantity;

  // warehouse doesn't exist  do nothin
  if (warehouse.find(city) == warehouse.end())
    { 
    }
  else
    {
       // item doesn't exist do nothing
      if (warehouse[city].find(UPC)==warehouse[city].end())
	{ 
	}
      else // item exists
	{	   
	 
	  if((warehouse[city][UPC]).size() > quantity)
	    {
	      for (int i=0; i<quantity; i++)
		{
		  // remove items when it is requested
		  (warehouse[city][UPC]).pop();
		}
	    }
	  else
	    {
	      // remove the food item from the warehouse if its stock is 
	      // not enough to supply the request
	      (warehouse[city]).erase(UPC); 
	    }
	}
    }
 
}

/*
 * Increment the current day and remove all the expired food
 */
void inventory::nextDay()
{
   // Increment current day
  this->current = this->current + 1;

  typedef map<string, queue<int> > InnerMap;
  typedef map<string, InnerMap> OuterMap;
  // Remove all expired food
  for(OuterMap::iterator outer_iter=warehouse.begin(); outer_iter!=warehouse.end(); ++outer_iter) {
    InnerMap &val = outer_iter->second;
    for(InnerMap::iterator inner_iter=val.begin(); inner_iter!=val.end(); ++inner_iter) {
      
       int shelfLife = upc_days[inner_iter->first]; 
       checkExpire(inner_iter->second, shelfLife);
    }
  }
}

/*
 *Output the Unstocked Products, Well-stocked products, and Most requested products
 */
void inventory::end()
{
  cout<<"Report by Minh Pham and To Tang"<<endl;
  cout<<endl;
 
  // create list of all stocked UPC
  typedef map<string, queue<int> > InnerMap;
  typedef map<string, InnerMap> OuterMap;
  // Stocked maps the food item with the number of warehouse that store it
  map<string,int> stocked;
  // Search all the warehouses to add to stocked
  for(OuterMap::iterator outer_iter=warehouse.begin(); outer_iter!=warehouse.end(); ++outer_iter) {
    InnerMap &val = outer_iter->second;
    cout<<endl;
    cout<<outer_iter->first<<endl;
    for(InnerMap::iterator inner_iter=val.begin(); inner_iter!=val.end(); ++inner_iter) {
      cout<<inner_iter->first<<" "<<(inner_iter->second).size()<<endl;
      if(!(inner_iter->second).empty())
	stocked[inner_iter->first]++;
      
    }
  }
  // debug
  for(map<string, int>::iterator it=stocked.begin(); it!=stocked.end();++it)
    {      
	cout<<it->first<<" "<<it->second<<endl;
    }
  
  // Unstocked products are stored in none of the warehouse
  // Their values in stocked map is 0
  cout<<"Unstocked Products:"<<endl;
  for(map<string, string>::iterator it=upc_name.begin(); it!=upc_name.end();++it)
    {
      if(stocked[it->first] == 0)
	cout<<it->first<<" "<<it->second<<endl;
    }
  cout<<endl;

  // Well-stocked products are stored in more than 1 warehouse
  // Their values in stocked map is greater than 1
  cout<<"Well-Stocked Products:"<<endl;
  for(map<string, string>::iterator it=upc_name.begin(); it!=upc_name.end();++it)
    {
      if(stocked[it->first] > 1)
	cout<<it->first<<" "<<it->second<<endl;
    }
  cout<<endl;

  // Most popular products are requested the most
  // Their values in the popular map are among the highest
  cout<<"Most Popular Products:"<<endl;
  // Find maximum value and remove its pair from the map
  // Continue seaching the maximum value until we find the three most popular products
  // (or less than 3 if the total food type is less than 3)
  int n = popular.size() < 3? popular.size():3;
  for(int i = 0; i < n; i++)
    {
      int max = 0;
      string code;
      for(map<string, int>::iterator it=popular.begin(); it!=popular.end();++it)
	{
	  if(it->second > max)
	    {
	      max = it->second;
	      code = it->first;
	    }
	}
      cout<<code<<" "<<upc_name[code]<<endl;
      popular.erase(code);
    }
   cout<<endl;

}

// MAIN
int main(int argc, char *argv[] )
{
  inventory h(argv[1]);

  return 0;
}
