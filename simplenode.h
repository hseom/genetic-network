/*
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
      //token part
      std::map<int, int> _tokenManage;
      float _probability;  // probability of query ignore
      int _rxmessage;
      int _querymessage;
      int _procmessage;
      int _queryhits;
      int _cachehits;
      bool _group_id;
      //int _init_token = 2;

      void printItems();

     public:


      SimpleNode();
      /**
       * @param addr node's address 
       * @param item object to holdi
       * @param own if true, delete the item when we are deleted
       */
      SimpleNode(const int addr, const int ttl, const int connection_limit, const float prob, bool g_id);
      //SimpleNode(const int addr);

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
      /**
 *     * return # of hits
 *     */

      void token_init(int neighbor);

      void token_increase(int neighbor_addr);

      void token_decrease(int neighbor_addr);

      bool token_check(int neighbor_addr);

      void hitcount();

      void querycount();

      void countrxmessage();

      int getCachehits();

      int getQueryhits();

      int getrxmessage();

      int getprmessage();

      int getQuerymessage();      

      float getprob();

      bool getGroupid();

   };
}
#endif
