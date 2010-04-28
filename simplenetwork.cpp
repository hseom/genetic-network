#include "simplenetwork.h"

using namespace Starsky;
using namespace std;

SimpleNetwork::SimpleNetwork(Ran1Random& r) : Network(), _r(r) {}

void SimpleNetwork::create(int address, int ttl, int connection_limit, float q_prob, bool g_id, Random& ran_no) {
    int addr = address;
    if (node_vec.size() == 0) {
      SimpleNode* new_node = new SimpleNode(addr, ttl, connection_limit, q_prob, g_id);
      node_vec.push_back(new_node);
      add(new_node);
    }
    else if (node_vec.size() == 1) {
      SimpleNode* new_node = new SimpleNode(addr, ttl, connection_limit, q_prob, g_id); 
      node_vec.push_back(new_node);
      add(new_node);
      SimpleNode* prev_node1 = node_vec[0]; 
      if(!this->has(Edge(prev_node1, new_node))) {
          add(Edge(prev_node1, new_node));
          prev_node1->token_init(new_node->getAddress());
          new_node->token_init(prev_node1->getAddress());
      }
    }
    else {
      SimpleNode* new_node = new SimpleNode(addr, ttl, connection_limit, q_prob, g_id);
      bool neighbor_check = false;
      int ran_no1;
      int ran_no2;
      while(!neighbor_check){
          ran_no1 = ran_no.getInt((node_vec.size()-1));
	  //cout << "ran_no1: " << ran_no1 << endl;
          ran_no2 = ran_no.getInt((node_vec.size()-1));
	  //cout << "ran_no2: " << ran_no2 << endl;
          SimpleNode* prev_node1 = node_vec[ran_no1];
          SimpleNode* prev_node2 = node_vec[ran_no2];
	  //cout << "new node: " << new_node->getAddress() <<", node1: " << prev_node1->getAddress() << ", node2: " << prev_node2->getAddress() << endl;
          if ((ran_no1 != ran_no2) && getDegree(prev_node1) < prev_node1->getConnectionlimit() && getDegree(prev_node2) < prev_node2->getConnectionlimit()) {
	          if(!this->has(Edge(prev_node1, new_node))) {
                  	add(Edge(prev_node1, new_node));
                  	prev_node1->token_init(new_node->getAddress());
			new_node->token_init(prev_node1->getAddress());
		  }
                  if(!this->has(Edge(prev_node2, new_node))) {
			add(Edge(prev_node2, new_node));
			prev_node2->token_init(new_node->getAddress());
			new_node->token_init(prev_node2->getAddress());
		  }
                  neighbor_check = true;
          }
      }
      node_vec.push_back(new_node);
      add(new_node);
   }
}        

