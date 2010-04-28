#include "simplenode.h"

using namespace Starsky;
using namespace std;
 
SimpleNode::SimpleNode()
{
}

SimpleNode::SimpleNode(const int addr, const int ttl, const int connection_limit, const float prob, const bool g_id)
{
  _address = addr;
  _ttl = ttl;
  _conn = connection_limit;
  _probability = prob;
  _cachehits = 0;
  _queryhits = 0;
  _rxmessage = 0;
  _txmessage = 0;
  _querymessage = 0;
  _procmessage = 0;
  //_messageID = 0;
  _group_id = g_id;
  _qNum = 0;
  _lastedge = false;
  //_tokenStart = false;
}

// node property return
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

float SimpleNode::getprob()
{
    return _probability;
}

bool SimpleNode::searchItem(std::string qItem)
{ 
    _procmessage++;
    if(_itemSet.find(qItem) != _itemSet.end() ) {
        return true;
    }
    else {
      return false;
    }
}

void SimpleNode::insertItem(std::string item)
{
    _itemSet.insert(item); 
}

// functions related to token method
void SimpleNode::token_start(int neighbor_addr)
{
	map<int, bool>::iterator iter = _tokenStartCheck.find(neighbor_addr);
	iter->second = true;
}

bool SimpleNode::token_check(int neighbor_addr)
{
	map<int, bool>::iterator iter = _tokenStartCheck.find(neighbor_addr);
	return iter->second;
}

void SimpleNode::token_startinit(int neighbor_addr)
{
	_tokenStartCheck.insert(make_pair(neighbor_addr, false));
}

void SimpleNode::token_init(int neighbor_addr)
{    
    int init_token = 0;
    _tokenManage.insert(make_pair(neighbor_addr, init_token));
}

int SimpleNode::token_value(int neighbor_addr)
{
    map<int, int>::iterator iter = _tokenManage.find(neighbor_addr);
    return iter->second;
}

void SimpleNode::token_increase(int neighbor_addr)
{
    map<int, int>::iterator iter = _tokenManage.find(neighbor_addr);
	    iter->second++;
}

void SimpleNode::token_decrease(int neighbor_addr)
{
    map<int, int>::iterator iter = _tokenManage.find(neighbor_addr);
	    iter->second--;
}

bool SimpleNode::token_valuecheck(int neighbor_addr)
{ 
    bool token;
    map<int, int>::iterator iter = _tokenManage.find(neighbor_addr);
    if( iter != _tokenManage.end() ){
	if(iter->second >= 0) {
	    token = true;
	}
	else {
	    token = false;
	}
    }
    //else {
    //	_tokenManage.insert(make_pair(neighbor_addr, 3));
    //	token = true;
    //}
    return token;
}

bool SimpleNode::token_there(int neighbor_addr)
{
	map<int, int>::iterator iter = _tokenManage.find(neighbor_addr);
	if(iter != _tokenManage.end()){
	       return true;
	}
	else {
	       return false;
	}
}	

int SimpleNode::size_tokenTable()
{
	return _tokenManage.size();
}

//functions related to freebie

void SimpleNode::freebieNumInit(int neighbor_addr)
{
        _freebieManage.insert(make_pair(neighbor_addr, 0));
}

int SimpleNode::freebieNumGet(int neighbor_addr)
{
	map<int, int>::iterator iter = _freebieManage.find(neighbor_addr);
	return iter->second;
}	

int SimpleNode::freebieNumIncrease(int neighbor_addr)
{
	map<int, int>::iterator iter = _freebieManage.find(neighbor_addr);
	iter->second++;
}

bool SimpleNode::freebieTableCheck(int neighbor_addr)
{
	map<int, int>::iterator iter = _freebieManage.find(neighbor_addr);
	if(iter != _freebieManage.end()){
	       return true;
	}
	else {
	       return false;
	}
}

//return hit or cost values
int SimpleNode::getCachehits()
{
    return _cachehits;
}

void SimpleNode::counttxmessage()
{
    _txmessage++;
}

int SimpleNode::gettxmessage()
{
    return _txmessage;
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

int SimpleNode::getMID()
{
   return _messageID;
}

void SimpleNode::increaseMID()
{
    _messageID++;
}

//for messageID comparison
bool SimpleNode::messageIDcheck(int origin_node, int messageID)
{ 
    map<int, int>::iterator iter = _messageIDtable.find(origin_node); // to implement message check table
    if( iter != _messageIDtable.end() ){
	if(iter->second == messageID) {
	    return true;
	}
	else {
	    iter->second = messageID;
	    return false;
	}
    }
    else {
        _messageIDtable.insert(make_pair(origin_node, messageID));
	return false;
    }
}

//for getting message ID table size
int SimpleNode::messageIDtablesize()
{
    return _messageIDtable.size();
}

bool SimpleNode::getGroupid()
{
    return _group_id;
}

void SimpleNode::lastEdgechange()
{
    _lastedge = true;
}

void SimpleNode::lastEdgeinit()
{
    _lastedge = false;
}

bool SimpleNode::lastEdgecheck()
{
    return _lastedge;
}

/*
void SimpleNode::printItems() {
  std::set<std::string>::iterator it;
  for (it = _itemSet.begin(); it !=_itemSet.end(); it++) {
    cout << _address << "\t" << *it << ", " ;
  }
}
*/
//get or delete items
std::set<std::string> SimpleNode::getItem() {
    return _itemSet;
}

void SimpleNode::deleteItem(std::string item)
{
    _itemSet.erase(item);
}
/*
void SimpleNode::token_make(int neighbor_addr)
{
    _tokenInit.insert(make_pair(neighbor_addr, false));
}

bool SimpleNode::token_makecheck(int neighbor_addr)
{
    map<int, bool>::iterator iter = _tokenInit.find(neighbor_addr);
    if( iter != _tokenInit.end() ){
        return iter->second;
    }
    else {
	_tokenInit.insert(make_pair(neighbor_addr, false));
	return false;
    }
}

void SimpleNode::token_makechange(int neighbor_addr)
{
    map<int, bool>::iterator iter = _tokenInit.find(neighbor_addr);
    iter->second = true;
}

int SimpleNode::getQnumber()
{ 
  return _qNum;
}

void SimpleNode::IncQnumber()
{
  _qNum++;
}
*/

