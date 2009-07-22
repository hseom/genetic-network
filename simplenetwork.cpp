/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
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

#include "simplenetwork.h"

using namespace Starsky;
using namespace std;

SimpleNetwork::SimpleNetwork(Ran1Random& r) : Network(), _r(r) {}
//SimpleNetwork::SimpleNetwork(int nodes, Ran1Random& r) : _r(r) {
//    node_vec.reserve(nodes);
//    create(nodes);
//}
//void SimpleNetwork::create(int n)
//{
//}
void SimpleNetwork::create(int n, int ttl, int connection_limit, float q_prob, bool g_id) {
  //int connection_limit = 6;
  int node_size = 1000;
  for(int i = 0 ; i < n ; i++) {
    int addr = (int)(_r.getDouble01() * node_size);  
    //cout << 'f' << endl;
    if (node_vec.size() == 0) {
      SimpleNode* new_node = new SimpleNode(addr, ttl, connection_limit, q_prob, g_id);
      node_vec.push_back(new_node);
      add(new_node);
    }
    else if (node_vec.size() == 1) {
      //cout << "node size 1" << endl;
      SimpleNode* new_node = new SimpleNode(addr, ttl, connection_limit, q_prob, g_id); 
      node_vec.push_back(new_node);
      add(new_node);
      SimpleNode* prev_node1 = node_vec[0]; 
      add(Edge(prev_node1, new_node));
      prev_node1->token_init(new_node->getAddress());
      new_node->token_init(prev_node1->getAddress());
    }
    else {
      SimpleNode* new_node = new SimpleNode(addr, ttl, connection_limit, q_prob, g_id);
      //node_vec.push_back(new_node);
      //add(new_node);
      bool neighbor_check = false;
      int ran_no1;
      int ran_no2;
      //cout << 'f' << endl;
      while(!neighbor_check){
          ran_no1 = _r.getInt((node_vec.size()-1));
          ran_no2 = _r.getInt((node_vec.size()-1));
          SimpleNode* prev_node1 = node_vec[ran_no1];
          SimpleNode* prev_node2 = node_vec[ran_no2];
          //cout << 'f' << endl;
          if ((ran_no1 != ran_no2) && getDegree(prev_node1) < prev_node1->getConnectionlimit() && getDegree(prev_node2) < prev_node2->getConnectionlimit()) {
              //if (prev_node1->getAddress() != new_node->getAddress() && prev_node2->getAddress() != new_node->getAddress()) {
                  add(Edge(prev_node1, new_node));
                  //cout << prev_node1->getAddress() << endl;
                  add(Edge(prev_node2, new_node));
                  //cout << prev_node2->getAddress() << endl;
                  prev_node1->token_init(new_node->getAddress());
                  prev_node2->token_init(new_node->getAddress());
                  new_node->token_init(prev_node1->getAddress());
                  new_node->token_init(prev_node2->getAddress());
                  neighbor_check = true;
              //}
          }
          node_vec.push_back(new_node);
          add(new_node);
      }
    }
  }
}        
   
/*

 SimpleNode* tmp1 = first;
    SimpleNode* tmp2 = second;
    SimpleNode* tmp3;
    cout << "Network size : " << node_vec.size() << endl;    
    //We made the first, second node and first edge, now lets make the rest.
    for(int k = 2; k < n; k++) {
      tmp3 = new SimpleNode(k, _r.getDouble01());
      node_vec.push_back(tmp3);
      add(tmp3);
      add(Edge(tmp1, tmp3));
      add(Edge(tmp2, tmp3));
      tmp1 = tmp2;
      tmp2 = tmp3;
    }
    //Close the circle
    add(Edge(tmp1, first));
    add(Edge(tmp1, second));
    add(Edge(tmp2, first));
}
*/
