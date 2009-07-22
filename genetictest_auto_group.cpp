#include <vector>
#include <fstream>
#include <iostream>
#include <memory>
#include <cmath>
#include <netmodeler.h>

using namespace Starsky;
using namespace std;

//random string generator
std::set<std::string> rstringGenerator (int howmany, int length, Random& r){
    std::set<std::string> items;
    for(int no=0; no < howmany; no++) {
        std::string item;
        for(int i=0; i<length; i++) {
            int rand_no = (int) (r.getDouble01()* 122);
            if ( rand_no < 65) { rand_no = 65 + rand_no % 56;}
            if ( (rand_no > 90) && (rand_no < 97) ) { rand_no += 6;}
            item += (char)rand_no;
        }
        items.insert(item);
    }
    return items;
}

int main(void) {
  ifstream indata("final_test_modified.dat",ios::in);
  ofstream datafile1;
  ofstream datafile2;
  datafile1.open("small_group_5.dat");
  datafile2.open("big_group_5.dat");

  
  int nodes = 100;
  int node_ttl;
  int connection_limit;
  int k = 100; //the nunber of items
  //int forl = 0;
  int generation = 200;
  int group1_portion = 10;
  int group2_portion = 90;
  
  float group1_fitness_sum = 0;
  float group2_fitness_sum = 0;
  
  float a[nodes]; 
  float b[nodes]; //fitness
  float c[nodes]; //probability
  int d[nodes]; //connection
  int e[nodes]; //TTL

  float c_new[nodes]; // probability for child
  int d_new[nodes]; // connection for child
  int e_new[nodes]; // TTL for child
  
  Ran1Random ran_no(-1);
  EventScheduler sched;
  
  for(int i = 0; i < nodes ; i++) {
    indata >> b[i] >> c[i] >> d[i] >> e[i];
    a[i] = 0;
  }

  float select_father;
  float select_mother;

  float father_prob;
  int father_connection;
  int father_TTL;

  float mother_prob;
  int mother_connection;
  int mother_TTL;

  float pure_cross_prob;
  float pure_cross_ttl;
  float pure_cross_connection;

  //for crossover of query ignore probability
  for(int ge = 1; ge < generation; ge++) {
      datafile1 << "#-----------------------------" << endl;
      datafile2 << "#-----------------------------" << endl;
      int ctime, qtime;
      double time = 0;

      //empty network generation
      auto_ptr<SimpleNetwork> Net_ptr (new SimpleNetwork(ran_no));
      //for roulette selection method
      
      for(int i = 0 ; i < nodes; i++) {
          if(i < group1_portion) {
              group1_fitness_sum += b[i];
          }
          else {
              group2_fitness_sum += b[i];
          }
      }
      
      for(int i = 1; i < group1_portion; i++) {
          a[i] = b[i-1];
          b[i] += a[i];
      }

      for(int i = group1_portion+1; i < nodes; i++) {
          a[i] = b[i-1];
          b[i] += a[i];
      }
      //cout << 'a' << endl;
      for(int i = 0 ; i < group1_portion ; i++) {
          int name_parent;
          bool check_parent = false;
          //selection of father
          select_father = ran_no.getDouble01()*group1_fitness_sum;
          //select_father = ran_no.getDouble01()*fitness_sum;
          for(int f = 0 ; f < group1_portion ; f++) {
          //for(int f = 0 ; f < nodes ; f++) {
              if(a[f]<select_father && b[f]>select_father) {
                  father_prob = c[f];
                  father_connection = d[f];
                  father_TTL = e[f];
                  name_parent = f;
              }
          }
          while(!check_parent) {
          //seletion of mother 
              select_mother = ran_no.getDouble01()*group1_fitness_sum;
              for(int m = 0 ; m < group1_portion ; m++) {
                  if(a[m]<select_mother && b[m]>select_mother) {
                      if(name_parent != m) {
                          mother_prob = c[m];
                          mother_connection = d[m];
                          mother_TTL = e[m];
                          check_parent = true;
                      }
                  }
              }
          }
          pure_cross_prob = (father_prob + mother_prob)/2; //crossover without mutation
          //mutation process for query ignore probability
          //maximum mutation is 10% of crossover, minimum mutation is 1% of crossover
          if (ran_no.getDouble01() > 0.5) {
              c_new[i] = pure_cross_prob + ran_no.getDouble((pure_cross_prob)*0.001, 0); 
              if (c_new[i] > 1) {
              c_new[i] = 0.9999999;
              }
          }
          else {
              c_new[i] = pure_cross_prob - ran_no.getDouble((pure_cross_prob)*0.001, 0);
              if (c_new[i] < 0) {
                  c_new[i] = 0.0000001;
              }
          }
          //for TTL
          if (mother_TTL > father_TTL) {
              pure_cross_ttl = ran_no.getInt(mother_TTL, father_TTL);
              e_new[i] = pure_cross_ttl;
          }
          else {
              pure_cross_ttl = ran_no.getInt(father_TTL, mother_TTL);
              e_new[i] = pure_cross_ttl;
          }
/*
          if (ran_no.getDouble01() < 0.2) {
              if (ran_no.getDouble01() > 0.5) {
                  e_new[i] = pure_cross_ttl + 1;
              }
              else {
                  e_new[i] = pure_cross_ttl - 1;
                  if (e_new[i] < 1) {
                      e_new[i] = 1;
                  }
              }
          }
          else {
              e_new[i] = pure_cross_ttl;
          }
*/
          //for connection
          if (mother_connection > father_connection) {
              pure_cross_connection = ran_no.getInt(mother_connection, father_connection);
              d_new[i] = pure_cross_connection;
          }
          else {
              pure_cross_connection = ran_no.getInt(father_connection, mother_connection);
              d_new[i] = pure_cross_connection;
          }
          /*
          if (ran_no.getDouble01() < 0.2) {
              if (ran_no.getDouble01() > 0.5) {
                  d_new[i] = pure_cross_connection + 1;
              }
              else {
                  d_new[i] = pure_cross_connection - 1;
                  if (d_new[i] < 1) {
                      d_new[i] = 1;
                  }
              }
          }
          else {
              d_new[i] = pure_cross_connection;
          }
*/
          Net_ptr->create(1, e_new[i], d_new[i], c_new[i], true);
       }
       for(int i = group1_portion ; i < nodes ; i++) {
          int name_parent;
          bool check_parent = false;
          //selection of father
          select_father = ran_no.getDouble01()*group2_fitness_sum;
          //cout << "select_father " << "\t" << select_father << endl; 
          //cout << "group_fitness_sum" << "\t" << group2_fitness_sum << endl;
          //cout << a[99] << endl;
          for(int f = group1_portion ; f < nodes ; f++) {
              //cout << a[f] << endl;
              if(a[f]<select_father && b[f]>select_father) {
                  father_prob = c[f];
                  father_connection = d[f];
                  father_TTL = e[f];
                  name_parent = f;
                  //cout << 'd' << endl;
              }
          }
          while(!check_parent) {
          //seletion of mother
              select_mother = ran_no.getDouble01()*group2_fitness_sum;
              for(int m = group1_portion ; m < nodes ; m++) {
                  if(a[m]<select_mother && b[m]>select_mother) {
                      if(name_parent != m) {
                          mother_prob = c[m];
                          mother_connection = d[m];
                          mother_TTL = e[m];
                          check_parent = true;
                      }
                  }
              }
          }
          pure_cross_prob = (father_prob + mother_prob)/2; //crossover without mutation
          //mutation process for query ignore probability
          //maximum mutation is 10% of crossover, minimum mutation is 1% of crossover
          if (ran_no.getDouble01() > 0.5) {
              c_new[i] = pure_cross_prob + ran_no.getDouble((pure_cross_prob)*0.001, 0); 
              if (c_new[i] > 1) {
              c_new[i] = 0.9999999;
              }
          }
          else {
              c_new[i] = pure_cross_prob - ran_no.getDouble((pure_cross_prob)*0.001, 0);
              if (c_new[i] < 0) {
                  c_new[i] = 0.0000001;
              }
          }

          if (mother_TTL > father_TTL) {
              pure_cross_ttl = ran_no.getInt(mother_TTL, father_TTL);
              e_new[i] = pure_cross_ttl;
          }
          else {
              pure_cross_ttl = ran_no.getInt(father_TTL, mother_TTL);
              e_new[i] = pure_cross_ttl;
          }
/*
          if (ran_no.getDouble01() < 0.2) {
              if (ran_no.getDouble01() > 0.5) {
                  e_new[i] = pure_cross_ttl + 1;
              }
              else {
                  e_new[i] = pure_cross_ttl - 1;
                  if (e_new[i] < 1) {
                      e_new[i] = 1;
                  }
              }
          }
          else {
              e_new[i] = pure_cross_ttl;
          }
*/
          //for connection
          if (mother_connection > father_connection) {
              pure_cross_connection = ran_no.getInt(mother_connection, father_connection);
              d_new[i] = pure_cross_connection;
          }
          else {
              pure_cross_connection = ran_no.getInt(father_connection, mother_connection);
              d_new[i] = pure_cross_connection;
          }
          /*
          if (ran_no.getDouble01() < 0.2) {
              if (ran_no.getDouble01() > 0.5) {
                  d_new[i] = pure_cross_connection + 1;
              }
              else {
                  d_new[i] = pure_cross_connection - 1;
                  if (d_new[i] < 1) {
                      d_new[i] = 1;
                  }
              }
          }
          else {
              d_new[i] = pure_cross_connection;
          }
          */

          Net_ptr->create(1, e_new[i], d_new[i], c_new[i], false);
       }

       std::set<std::string> items = rstringGenerator(k, 10, ran_no);
       std::set<std::string>::const_iterator item_it;

       time += 10;

       UniformNodeSelector uns(ran_no);
       for (item_it = items.begin(); item_it != items.end(); item_it++) {
           std::string item = *item_it;
           ctime = time + ran_no.getExp(100.0);
           Action* c_action = new CacheAction(sched, ran_no, uns, *Net_ptr.get(), item);
           sched.at(ctime, c_action);
/*
           jtime = ctime + ran_no.getExp(720.0);
           Action* j_action = new JoinAction(sched, ran_no, *Net_ptr.get());
           sched.at(jtime, j_action);
*/
   
           UniformNodeSelector q_start(ran_no);
           for (int iter = 0; iter < 100; iter++) {
               Action* q_action = new QueryAction(sched, ran_no, q_start, *Net_ptr.get(), item);
               qtime = ctime +ran_no.getExp(3600.0);
               sched.at(qtime, q_action);
           }
/*
           forl++;
           forl=forl%2;

           if(forl == 0) {
               ftime = qtime + ran_no.getExp(1080.0);
               Action *f_action = new FailAction(sched, ran_no, *Net_ptr.get());
               sched.at(ftime, f_action);
           }
           else {
               ltime = qtime +ran_no.getExp(1080.0);
               Action* l_action = new LeaveAction(sched, ran_no, *Net_ptr.get());
               sched.at(ltime, l_action);
           }
  */ 
           time = ctime;
      }
      Action* stop = new StopAction(sched);
      sched.at(360000, stop);

      sched.start();

      auto_ptr<NodeIterator> totn (Net_ptr->getNodeIterator());
      
      float hitrate;
      int cpu_cost;
      int disk_cost;
      float prob;
      float fitness;
      int new_nodes = 0;

      int total_ttl = 0;
      int total_connection = 0;
      float total_fitness = 0;
      float total_prob = 0;
      float total_hitrate = 0;
      float total_cpu_cost = 0;
      float total_disk_cost = 0;
      
      float average_ttl;
      float average_connection;
      float average_fitness;
      float average_prob;
      float average_hitrate;
      float average_cpu_cost;
      float average_disk_cost;
      
      cout << "#fit \t P_ig \t P_s \t cost_cpu \t cost_d" << endl;
      while (totn->moveNext() ) {
          SimpleNode* inNode = dynamic_cast<SimpleNode*> (totn->current() );
          hitrate = (float)inNode->getQueryhits()/(float)inNode->getQuerymessage();
          cpu_cost = inNode->getrxmessage()+(inNode->getprmessage())*(inNode->getItem()).size();
          disk_cost = (inNode->getItem()).size();
          fitness = exp((10*hitrate)-((disk_cost/100)+(cpu_cost/50000)));
          //fitness = 10*(exp((0.5*hitrate)-((0.2*disk_cost/100)+(0.3*cpu_cost/500000))));
          //fitness = fitness * fitness;
          total_ttl += inNode->getTTL();
          total_connection += inNode->getConnectionlimit();
          total_hitrate += hitrate;
          total_cpu_cost += cpu_cost;
          total_disk_cost += disk_cost;
          total_fitness += fitness;
          total_prob += inNode->getprob();
          b[new_nodes] = fitness;
          //c[new_nodes] = inNode->getprob();
          //d[new_nodes] = inNode->getConnectionlimit();
          //e[new_nodes] = inNode->getTTL();
          c[new_nodes] = c_new[new_nodes];
          d[new_nodes] = d_new[new_nodes];
          e[new_nodes] = e_new[new_nodes];
          if (inNode -> getGroupid() == true){
              //datafile1 << fitness << endl;
              datafile1<<fitness<<"\t"<<inNode->getprob() << "\t" << hitrate << "\t" << cpu_cost << "\t" << disk_cost << "\t" << endl; 
          }
          else {
              //datafile2 << fitness << endl;
              datafile2<<fitness<<"\t"<<inNode->getprob() << "\t" << hitrate << "\t" << cpu_cost << "\t" << disk_cost << "\t" << endl; 
          }
              

          //cout<<fitness<<"\t"<<inNode->getprob() << "\t" << hitrate << "\t" << cpu_cost << "\t" << disk_cost << "\t" << endl; 
          //cout << hitrate << "\t" << disk_cost << "\t" << cpu_cost << endl;
          //cout << inNode->getQueryhits() << "\t" << inNode->getQuerymessage() << endl;
          //cout << fitness << "\t" << inNode->getprob() << "\t" << inNode->getTTL() << "\t" << inNode->getConnectionlimit() << endl;
          new_nodes++;
      }
      //cout << total_hits << endl;
      average_ttl = total_ttl / nodes;
      average_connection = total_connection / nodes;
      average_hitrate = total_hitrate / nodes;
      average_cpu_cost = total_cpu_cost / nodes;
      average_disk_cost = total_disk_cost / nodes;
      average_fitness = total_fitness / nodes;
      average_prob = total_prob / nodes; 
      //cout << average_hitrate << "\t" << average_cpu_cost << "\t" <<average_disk_cost << "\t" << average_fitness << "\t" << average_prob << "\t" << average_ttl << "\t" << average_connection << endl;
      //cout << average_fitness << "\t" << average_prob << endl;
      group1_fitness_sum = 0;
      group2_fitness_sum = 0;
      //fitness_sum = 0;
    }
    datafile1.close();
    datafile2.close();
    return 0;
}
