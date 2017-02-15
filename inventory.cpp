#include "inventory.h"
#include <iostream>
#include "FoodItem.h"
#include <map>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

inventory::inventory(string fileName)
{
  ifstream in(fileName.c_str());
  string line;
  while(getline(in,line))
    {
    
    }
  in.close();
}

inventory:: ~inventory()
{

}
