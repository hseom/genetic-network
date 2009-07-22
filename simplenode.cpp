/** 
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of Florida
Copyright (C) 2005  P. Oscar Boykin <boykin@pobox.com>, University of Florida
Copyright (C) 2005  Tae Woong Choi <twchoi@ufl.edu>, University of Florida

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.    
*/

#include "simplenode.h"

using namespace Starsky;
using namespace std;
 
SimpleNode::SimpleNode()
{
}

SimpleNode::SimpleNode(const int addr, const int ttl, const int connection_limit, const float prob, const bool g_id)
//SimpleNode::SimpleNode(const int addr)
{
  _address = addr;
  _ttl = ttl;
  _conn = connection_limit;
  _probability = prob;
  _cachehits = 0;
  _queryhits = 0;
  _rxmessage = 0;
  _querymessage = 0;
  _procmessage = 0;
  _group_id = g_id;
}

int SimpleNode::getAddress()
{
  return _address;
}

int SimpleNode::getTTL()
{
    return _ttl;
}

int SimpleNode::getConnectionlimit()
{
    return _conn;
}

std::set<std::string> SimpleNode::getItem() {
    return _itemSet;
}

bool SimpleNode::searchItem(std::string qItem)
{ 
  _procmessage++;
    if(_itemSet.find(qItem) != _itemSet.end() ) {
        //_cachehits++;
        //_procmessage++;
        return true;
    }
    else {
      //_procmessage++;
      return false;
    }
}

void SimpleNode::insertItem(std::string item)
{
    _itemSet.insert(item); 
}

void SimpleNode::deleteItem(std::string item)
{
    _itemSet.erase(item);
}

void SimpleNode::token_init(int neighbor_addr)
{    
    int init_token = 2;
    //map<int, int>::iterator iter = _tokenManage.find(neighbor_addr);
    //if(iter == _tokenManage.end() ){
        _tokenManage.insert(make_pair(neighbor_addr, init_token));
    //}

    //cout << _address << "\t"<< neighbor_addr << endl;
}

void SimpleNode::token_increase(int neighbor_addr)
{
    map<int, int>::iterator iter = _tokenManage.find(neighbor_addr);
    //if( iter == _tokenManage.end() ){
      //_tokenManage.insert(make_pair(neighbor_addr, 3));
      //cout << neighbor_addr << "\t" << 11 << "\t" << "token increased" << endl;
    //}
    //else {
      iter->second++;
      //cout << neighbor_addr << "\t" << iter->second << "\t" << "token increased" << endl;
    //}
}

void SimpleNode::token_decrease(int neighbor_addr)
{
    //cout << "decrease function called" << endl;
    map<int, int>::iterator iter = _tokenManage.find(neighbor_addr);
    //if( iter == _tokenManage.end() ){
      //_tokenManage.insert(make_pair(neighbor_addr, 1));
      //cout << neighbor_addr << "\t" << 9 << "\t" << "token decreased" << endl;
    //}
    //else {
      iter->second--;
      //cout << neighbor_addr << "\t" << iter->second << "\t" << "token decreased" << endl;
    //}
}

bool SimpleNode::token_check(int neighbor_addr)
{
    map<int, int>::iterator iter = _tokenManage.find(neighbor_addr);
    if( iter == _tokenManage.end() ){
        //_tokenManage.insert(make_pair(neighbor_addr, 2));
        return false;
    }
    else {
        if(iter->second > 0) {
            //cout << iter->second << endl;
            return true;
        }
        else {
            //cout << iter->second << endl;
            return false;
        }
    }
}

int SimpleNode::getCachehits()
{
    return _cachehits;
}

void SimpleNode::countrxmessage()
{
    _rxmessage++;
}

int SimpleNode::getrxmessage()
{
    return _rxmessage;
}

int SimpleNode::getprmessage()
{
    return _procmessage;
}

float SimpleNode::getprob()
{
    return _probability;
}

void SimpleNode::hitcount()
{
    _queryhits++;
}

void SimpleNode::querycount()
{
    _querymessage++;
}

int SimpleNode::getQueryhits()
{
    return _queryhits;
}

int SimpleNode::getQuerymessage()
{
    return _querymessage;
}

bool SimpleNode::getGroupid()
{
    return _group_id;
}

void SimpleNode::printItems() {
  std::set<std::string>::iterator it;
  for (it = _itemSet.begin(); it !=_itemSet.end(); it++) {
    cout << _address << "\t" << *it << ", " ;
  }
  cout << endl;
}
