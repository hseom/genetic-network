#ifndef starsky__simplenode_h
#define starsky__simplenode_h

#include "node.h"
#include "random.h"
#include "ran1random.h"
#include <set>
#include <map>
#include <iostream>
#include <vector>

namespace Starsky {
  
  /**
   * Simplenode class which every node in the network has address
   */
  //template<typename T>
  class SimpleNode : public Node {
    protected:
      int _address;
      int _ttl;
      int _conn;
      std::set<std::string> _itemSet;
      std::map<int, int> _tokenManage; //token table
      std::map<int, bool> _tokenStartCheck; //token method start check
      std::map<int, int> _freebieManage; //freebie table
      std::map<int, int> _messageIDtable; //message ID table
      float _probability;  // probability of query ignore
      int _rxmessage;
      int _txmessage;
      int _querymessage;
      int _procmessage;
      int _messageID;
      int _queryhits;
      int _cachehits;
      bool _group_id;
      int _qNum;
      bool _lastedge;
      //bool _tokenStart;

      void printItems();

     public:
      SimpleNode();
      /**
       * @param addr node's address 
       * @param item object to holdi
       * @param own if true, delete the item when we are deleted
       */
      SimpleNode(const int addr, const int ttl, const int connection_limit, const float prob, bool g_id);

      //int getQnumber();

      //void IncQnumber();

      int token_value(int neighbor_addr);

      int getAddress();

      int getTTL();

      int getConnectionlimit();
      /**
       * return to the pointer to the object being contained.
       */
      //std::set<std::string> getItem() const { return _itemSet; }
      std::set<std::string> getItem();
       /**
       * return true if this node has qItem.
       */
      bool searchItem(std::string qItem);
      /**
       * @param item, insert item to a node
       */
      void insertItem(std::string item);
      /**
       * @param item, insert obj to a node
       */
      void deleteItem(std::string item);

      void token_start(int neighbor_addr);

      bool token_check(int neighbor_addr);

      void token_startinit(int neighbor_addr);
 /*     
      //is token table for neighbor maken?
      void token_make(int neighbor);

      bool token_makecheck(int neighbor_addr);

      void token_makechange(int neighbor_addr);
      //is token table for neighbor maken?
*/
      void token_init(int neighbor_addr);

      void token_increase(int neighbor_addr);

      void token_decrease(int neighbor_addr);

      bool token_valuecheck(int neighbor_addr);

      bool token_there(int neighbor_addr);

      int size_tokenTable();

      void hitcount();

      void querycount();

      void countrxmessage();

      void counttxmessage();

      int getCachehits();

      int getQueryhits();

      int getrxmessage();

      int gettxmessage();

      int getprmessage();

      int getQuerymessage();

      int getMID();
      
      void increaseMID();
      
      bool messageIDcheck(int origin_node, int messageID);  

      int messageIDtablesize();    

      float getprob();

      bool getGroupid();

      void lastEdgechange();

      void lastEdgeinit();

      bool lastEdgecheck();

      void freebieNumInit(int neighbor_addr);

      int freebieNumGet(int neighbor_addr);

      int freebieNumIncrease(int neighbor_addr);

      bool freebieTableCheck(int neighbor_addr);
   };
}
#endif
