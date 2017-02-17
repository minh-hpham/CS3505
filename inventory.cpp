#include "inventory.h"
#include <iostream>
//#include "FoodItem.h"
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
      cout<<line<<endl;
      /*
      if(line.find("FoodItem") == 0)
	cout<<"got food"<<endl;//addFoodItem(line);     
      else if(line.find("Receive:") == 0)
        cout<<"got receive"<<endl;
      else if(line.find("Request:") == 0)
	cout<<"got request"<<endl;
      else if(line.find("Next") == 0)
	//nextDay();
	cout<<"got next"<<endl;
     
      else{
	cout<<"got end"<<endl;//end();
	break;      
      }
      */
      
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
	  ss >> word;
	  while(ss.fail() == false)
	  {	    
	    name.append(word);
	    name.append(" ");
	    ss >> word;
	  }
	}
    }

  upc_days[code] = atoi(life.c_str());
  upc_name[code] = name;
  
 
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

void inventory::checkExpire( queue <int> & dates, const int  & shelfLife)
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

  int shelfLife = upc_days[UPC]; 
    
  //update popular request map
  cout<<popular[UPC]<<" "<<quantity<<endl;
  popular[UPC]+= quantity;
  
  if (warehouse.find(city) == warehouse.end()) // warehouse doesn't exist
    { // do nothing
    }
  else
    {
   
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
  this->current = this->current + 1;
}
void inventory::end()
{
  cout<<"Report by Minh Pham and To Tang"<<endl;
  cout<<endl;
 
  // create list of all stocked UPC
  typedef map<string, queue<int> > InnerMap;
  typedef map<string, InnerMap> OuterMap;

  map<string,int> stocked;
  for(OuterMap::iterator outer_iter=warehouse.begin(); outer_iter!=warehouse.end(); ++outer_iter) {
    InnerMap &val = outer_iter->second;
    for(InnerMap::iterator inner_iter=val.begin(); inner_iter!=val.end(); ++inner_iter) {
      stocked[inner_iter->first]++;
    }
  }
  cout<<"Unstocked Products:"<<endl;
  for(map<string, string>::iterator it=upc_name.begin(); it!=upc_name.end();++it)
    {
      if(stocked[it->first] == 0)
	cout<<it->first<<" "<<it->second<<endl;
    }
  cout<<endl;


  cout<<"Well-Stocked Products:"<<endl;
  for(map<string, string>::iterator it=upc_name.begin(); it!=upc_name.end();++it)
    {
      if(stocked[it->first] > 1)
	cout<<it->first<<" "<<it->second<<endl;
    }
  cout<<endl;
  cout<<"Most Popular Products:"<<endl;
 // most popular

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



  //debug map
cout<<"stocked"<<endl;
for(map<string, int>::iterator it=stocked.begin(); it!=stocked.end();++it)
    {
      
	cout<<it->first<<" "<<it->second<<endl;
    }
cout<<"popular"<<endl;
  for(map<string, int>::iterator it=popular.begin(); it!=popular.end();++it)
    {
      
	cout<<it->first<<" "<<it->second<<endl;
    }
  




}
int main(int argc, char *argv[] )
{
  inventory h(argv[1]);

  return 0;
}
