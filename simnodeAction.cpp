
#include "simnodeAction.h"
#include "netmodeler.h"
#include <memory>
#define FOREACH(it,col) for(it=col.begin();it != col.end();it++)

using namespace Starsky;
using namespace std;

FailAction::FailAction(EventScheduler& sched, Random& r, SimpleNetwork& sn) : _sched(sched), _r(r), _sn(sn)
{
}

void FailAction::Execute() 
{
  int ran_no = _r.getInt((_sn.node_vec.size()-1));
  SimpleNode* fail_node = _sn.node_vec[ran_no];
  auto_ptr<NodeIterator> ni (_sn.getNeighborIterator(fail_node) );
  while (ni->moveNext() ) {
    SimpleNode* neighbor = dynamic_cast<SimpleNode*> (ni->current() );
    Edge* broken_edge = _sn.getEdge(fail_node, neighbor);
    if(broken_edge != 0) {
      _sn.remove(broken_edge);
    }
  }
  _sn.node_vec.erase(_sn.node_vec.begin()+ran_no);
  _sn.remove(fail_node);
}

LeaveAction::LeaveAction(EventScheduler& sched, Random& r, SimpleNetwork& sn) : _sched(sched), _r(r), _sn(sn)
{
}

void LeaveAction::Execute()
{
  int ran_no = _r.getInt(_sn.node_vec.size()-1);

  SimpleNode* leave_node = _sn.node_vec[ran_no];

  auto_ptr<Network> nei_net (_sn.getNeighbors(leave_node) );
  auto_ptr<NodeIterator> n_it (nei_net->getNodeIterator() );
  while(n_it -> moveNext() ) {
    SimpleNode* cn = dynamic_cast<SimpleNode*>(n_it->current()); 
  }
  std::set<Edge*> edge_to_add;
  std::set<Edge*> edge_to_break;

  auto_ptr<NodeIterator> ni (_sn.getNeighborIterator(leave_node) );
  while (ni->moveNext() ) {
    SimpleNode* neighbor = dynamic_cast<SimpleNode*> (ni->current() );
    Edge* break_edge = _sn.getEdge(leave_node, neighbor);
    edge_to_break.insert(break_edge);
    if(_r.getDouble01() > 0.5) {
      getNewConnection(_sn, leave_node, neighbor, edge_to_add);
    }
  }
  set<Edge*>::iterator be;
  for (be=edge_to_break.begin();be!=edge_to_break.end();be++) {
      Edge* edgetobreak = *be;
      _sn.remove(edgetobreak);
  }
  set<Edge*>::iterator ne;
  for (ne=edge_to_add.begin();ne!=edge_to_add.end();ne++) {
      Edge* edgetoadd = *ne;
      cout<<"aa"<<endl;
      _sn.add(edgetoadd);
  }
  _sn.node_vec.erase(_sn.node_vec.begin()+ran_no);
  _sn.remove(leave_node);
}
void LeaveAction::getNewConnection(SimpleNetwork& sn, SimpleNode* ns, SimpleNode* nei, std::set<Edge*> add_edge)
{
  int ran_no;
  bool fresh_newnei = false;
  while (!fresh_newnei) {
    ran_no = _r.getInt(_sn.node_vec.size()-1);
    SimpleNode* new_neighbor = _sn.node_vec[ran_no];
    if (new_neighbor!= ns && new_neighbor!=nei /*&& _sn.getEdge(nei, new_neighbor) != 0*/) {
      //_sn.add(Edge(nei, new_neighbor));
      Edge* new_edge = new Edge(nei, new_neighbor);
      add_edge.insert(new_edge);
      fresh_newnei = true;
    }
    else {
      fresh_newnei = false;
    }
  }
}

JoinAction::JoinAction(EventScheduler& sched, Random& r, SimpleNetwork& sn) : _sched(sched), _r(r), _sn(sn)
{
}

void JoinAction::Execute() {
  int addr = (int)(_r.getDouble01()*10000);
  int ttl = _r.getInt(10, 4);
  int conn = _r.getInt(10, 6);
  bool group_id = 0;
  SimpleNode* join_node = new SimpleNode(addr, ttl, conn, _r.getDouble01(), group_id);
  _sn.node_vec.push_back(join_node);
  _sn.add(join_node);

  int ran_no1 = _r.getInt(_sn.node_vec.size()-1);
  SimpleNode* neighbor1 = _sn.node_vec[ran_no1];
  _sn.add(Edge(join_node, neighbor1));
  copyItems(join_node, neighbor1);

  int ran_no2;
  bool fresh_neinode = false;
  while (!fresh_neinode) {
    ran_no2 = _r.getInt(_sn.node_vec.size()-1);
    if (ran_no1 != ran_no2) { 
      SimpleNode* neighbor2 = _sn.node_vec[ran_no2];
      _sn.add(Edge(join_node, neighbor2));
      copyItems(join_node, neighbor2);
      fresh_neinode = true;
    }
    else {
      fresh_neinode = false;
    }
  }
}

void JoinAction::copyItems(SimpleNode* ns, SimpleNode* nei) {
    set<string> st = nei->getItem();
    set<string>::iterator st_it;
    for(st_it = st.begin();st_it !=st.end();st_it++) {
        ns->insertItem(*st_it);
    }
}

CacheAction::CacheAction(EventScheduler& sched, Random& r, INodeSelector& ns, SimpleNetwork& sn, std::string item) : _sched(sched), _r(r), _ns(ns), _sn(sn), _item(item)
{
}

void CacheAction::Execute() {
  _ns.selectFrom(&_sn);
  SimpleNode* node = dynamic_cast<SimpleNode*> (_ns.select() );
  auto_ptr<SimpleMessage> cache_m (new SimpleMessage(node->getTTL()) );
  auto_ptr<SimpleNetwork> tmp_net (cache_m->visit(node, _sn));
  auto_ptr<NodeIterator> ni (tmp_net->getNodeIterator() );
  float cache_ignore;
  while (ni->moveNext() ) {
    SimpleNode* inNode = dynamic_cast<SimpleNode*> (ni->current() );
    cache_ignore = _r.getDouble01();
    if(inNode->getprob() < cache_ignore) {
        inNode->insertItem(_item);
    }
  }
}

QueryAction::QueryAction(EventScheduler& sched, Random& r, INodeSelector& ns, SimpleNetwork& sn, std::string item) : _sched(sched), _r(r), _ns(ns), _sn(sn), _item(item)
{
}
void QueryAction::Execute() {
  _ns.selectFrom(&_sn);
  SimpleNode* node = dynamic_cast<SimpleNode*> (_ns.select());
  node->querycount();
  auto_ptr<SimpleMessage> query_m (new SimpleMessage(node->getTTL()) );
  auto_ptr<SimpleNetwork> tmp_net (query_m->visit(node, _sn));
  auto_ptr<NodeIterator> ni (tmp_net->getNodeIterator());
  float query_ignore;
  float gotoniche;
  int hit_counts = 0;

  while (ni->moveNext() ) {
    SimpleNode* inNode = dynamic_cast<SimpleNode*> (ni->current() );
    inNode->countrxmessage();
    query_ignore = _r.getDouble01();
    gotoniche = _r.getDouble01();
    //if((inNode->getGroupid() == false && inNode->getprob()<query_ignore && inNode->token_check(node->getAddress())) || (inNode->getGroupid() == false && inNode->getprob()<query_ignore && gotoniche<0.05)) {
    if((inNode->getprob()<query_ignore && inNode->token_check(node->getAddress())) || (inNode->getprob()<query_ignore && gotoniche<0.05)) {
    //if(inNode->getprob()<query_ignore && inNode->token_check(node->getAddress())) {
        node->token_increase(inNode->getAddress());
        if(inNode->searchItem(_item)) {
            hit_counts++;
        }
    }
    //else if (gotoniche<0.1) {
    //    node->token_increase(inNode->getAddress());
    //    if(inNode->searchItem(_item)) {
    //        hit_counts++;
    //    }
    //}
    else {
        node->token_decrease(inNode->getAddress());
    }
  }

  if (hit_counts!=0) {
      node->hitcount();
  }
}

