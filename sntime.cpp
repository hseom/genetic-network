#include <netmodeler.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <memory>

using namespace Starsky;
using namespace std;

//random string generator
std::set<std::string> rstringGenerator (int howmany, int length, Random& r ){
  std::set<std::string> items;
  for (int no=0; no < howmany; no++){
    std::string item;
    for (int i = 0; i < length; i++){
      int rand_no = (int) (r.getDouble01() * 122);
      if ( rand_no < 65 ) { rand_no = 65 + rand_no % 56;}
      if ( (rand_no > 90) && (rand_no < 97) ) { rand_no += 6; }
        item += (char)rand_no;
    }
    items.insert(item);
  }
  return items;
}
/*
void printNodes(SimpleNetwork& net) {
  auto_ptr<NodeIterator> si (net.getNodeIterator() );
  while(si->moveNext() ) {
    SimpleNode* c_node = dynamic_cast<SimpleNode*> (si->current() );
    cout << c_node->getAddress() << endl;
  }
}
*/
//int main(int argc, char *argv[])
int main(void) 
{
  int nodes = 100;
  int min_ttl = 2;
  int max_ttl = 4;
  int min_degree = 4;
  int max_degree = 6;
  //int ttl = 4;
  Ran1Random ran_no(-1);
  EventScheduler sched;
  double time = 0;
  auto_ptr<SimpleNetwork> Net_ptr (new SimpleNetwork(ran_no)); //empty network generation
  int node_ttl;
  int conn_limit;
  float q_ignore;
  bool g_id;
  
  //nodes generation
  for(int i = 0 ; i < nodes ; i++) {
      q_ignore = ran_no.getDouble01();
      node_ttl = ran_no.getInt(max_ttl, min_ttl);
      conn_limit = ran_no.getInt(max_degree, min_degree);
      g_id = 0;
      Net_ptr->create(1, node_ttl, conn_limit, q_ignore, g_id);
      //cout << q_ignore <<endl;
  }

  //Net_ptr->create(nodes); // nodes generation
  
  int k = 100; //number of items inserted into network
  
  int forl = 0; //for selection of leave and fail action

  int ctime, qtime, jtime, ftime, ltime;

  std::set<std::string> items = rstringGenerator(k, 10, ran_no);
  std::set<std::string>::const_iterator item_it;
  
  time += 10;   //start cache actions at 100 second after completing network creation. 
  
  //schedule caching for each item.
  UniformNodeSelector uns(ran_no);
  for (item_it = items.begin(); item_it != items.end(); item_it++) {
    std::string item = *item_it;
    ctime = time + ran_no.getExp(100.0);  
    //cout << "ctime : " << ctime << endl;
    Action* c_action = new CacheAction(sched, ran_no, uns, *Net_ptr.get(), item);
    sched.at(ctime, c_action);
    /*
    jtime = ctime + ran_no.getExp(720.0);
    //cout << "jtime : " << jtime << endl;
    Action* j_action = new JoinAction(sched, ran_no, *Net_ptr.get());
    sched.at(jtime, j_action);
*/
    //schedule query actions, join actions, and fail actions
    //UniformNodeSelector q_start(ran_no);
    for (int iter = 0; iter < 100; iter++) { //100 queries for each item
      bool join_ok = false;
      UniformNodeSelector q_start(ran_no);
      Action* q_action = new QueryAction(sched, ran_no, q_start, *Net_ptr.get(), item);
      qtime = ctime + ran_no.getExp(3600.0);
      //cout << "qtime : " << qtime << endl;
      sched.at(qtime, q_action);
    }
    /*
    forl++;
    forl=forl%2;
    //printNodes(*Net_ptr.get());
    if(forl == 0 ) {
      ftime = qtime + ran_no.getExp(1080.0);
      Action* f_action = new FailAction(sched, ran_no, *Net_ptr.get());
      sched.at(ftime, f_action);
    }
    else {
      ltime = qtime + ran_no.getExp(1080.0);
      //cout << "ftime : " << ftime << endl;
      Action* l_action = new LeaveAction(sched, ran_no, *Net_ptr.get());
      sched.at(ltime, l_action);
    }
      */
    time = ctime;
  } 
  //Run for 360,000 seconds (100 hours) of simulated time
  Action* stop = new StopAction(sched);
  sched.at(360000, stop);
  //std::cout << "#About to start" << std::endl;
  sched.start();
  //std::cout << "#Finished" << std::endl;
  auto_ptr<NodeIterator> totn (Net_ptr->getNodeIterator());
  //int a = 0;
  int total_hits = 0;
  while (totn->moveNext() ) {
    //a++;
    float hitrate;
    int cpu_cost;
    int disk_cost;
    float prob;
    float fitness;
    SimpleNode* inNode = dynamic_cast<SimpleNode*> (totn->current() );
    //cout << a << " Node : " << inNode->getAddress() << " Size after insertion : " << (inNode->getItem()).size() << endl;
    //cout << a << " Node : " << inNode->getAddress() << " The number of messages : " << inNode->getrxmessage() << endl;
    //cout << a << " Node : " << inNode->getAddress() << " The number of cache hits : " << inNode->getcachehits() << endl;
    //cout << " Node : " << inNode->getAddress() << " The probability : "<< inNode->getprob() << endl;
    //cout << a << " Node : " << inNode->getAddress() << " The number of processed message : " << inNode->getprmessage() << endl;
    //cout << "Node address : " << inNode -> getAddress() << "  Items size : " << (inNode -> getItem()).size() << "  Hits : " << inNode -> getHits() << "  prob : " << inNode -> getprob() << endl;
    //cout << " Node : " <<inNode->getAddress() << "The number of query hits : " <<inNode->getQueryhits() << endl;
    //cout <<  inNode->getQuerymessage() << endl;
    //cout <<  inNode->getQueryhits() << endl;
    //cout << inNode -> getprob() << endl;
    //float hitrate;
    //int cpu_cost;
    //int disk_cost;
    //float prob;
    //float fitness;
    //hitrate = (float)inNode->getCachehits()/(float)inNode->getrxmessage();
    hitrate = (float)inNode->getQueryhits()/(float)inNode->getQuerymessage();
    total_hits = total_hits + (inNode->getQueryhits());
    cpu_cost = inNode->getrxmessage()+(inNode->getprmessage())*(inNode->getItem()).size();
    disk_cost = (inNode->getItem()).size();
    //cout << cpu_cost << endl;
    //prob = inNode->getprob();
    //fitness = (float)(disk_cost/100);
    fitness = exp((10*hitrate)-((disk_cost/100)+(cpu_cost/50000)));
    //fitness = exp(7*hitrate);
    //fitness = hitrate - (disk_cost/100) - (cpu_cost/20000);
    //fitness = fitness * fitness;
    //cout << inNode->getrxmessage() << "\t" << inNode->getprmessage() << endl;
    //cout << inNode->getQueryhits() << "\t" << inNode->getQuerymessage() << endl;
    //cout<< fitness << "\t" << inNode->getprob() << "\t" << hitrate << "\t" << cpu_cost << "\t" << disk_cost << "\t" << inNode->getConnectionlimit() << "\t" << inNode->getTTL() << "\t" << inNode -> getQueryhits() << "\t" << inNode -> getQuerymessage() << endl;
    cout << fitness << "\t" <<  inNode->getprob() << "\t" << inNode->getConnectionlimit() << "\t" << inNode->getTTL() << endl; 
    //cout << hitrate << "\t" << disk_cost << "\t" << cpu_cost << "\t" << inNode->getprob() << "\t" << inNode->getQueryhits() << "\t" << inNode->getQuerymessage() << endl;
    //cout << inNode -> getQuerymessage() << endl;
    //cout << fitness << "\t" << inNode->getprob() << endl; 
    //cout << hitrate << "\t" << disk_cost << "\t" << cpu_cost << endl;

    }
  //cout << total_hits << endl;
  return 0;
  }
