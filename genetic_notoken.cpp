#include <vector>
#include <fstream>
#include <iostream>
#include <string.h>
#include <memory>
#include <cmath>
#include <netmodeler.h>

using namespace Starsky;
using namespace std;

//random string generator
std::set<std::string> rstringGenerator (int howmany, int length, Random& r){
	std::set<std::string> items;
        for(int no = 0 ; no < howmany ; no++) {
		std::string item;
		for(int i = 0 ; i < length ; i++) {
			int rand_no = (int) (r.getDouble01()* 122);
			if ( rand_no < 65) { rand_no = 65 + rand_no % 56;}
			if ( (rand_no > 90) && (rand_no < 97) ) { rand_no += 6;}
			item += (char)rand_no;
		}
		items.insert(item);
	}
	return items;
}
//child message ignore probability generator
float ignore_prob_generator (float father, float mother, float mutation, Random& ran_no)
{
	float pure_cross_prob;
	float m_ignore;
	float mutation_value;
        //pure cross value
	pure_cross_prob = (father + mother) / 2;
	//mutation
	if (ran_no.getDouble01() < mutation) {
	        if (ran_no.getDouble01() > 0.5) {
		        m_ignore = pure_cross_prob + ran_no.getDouble(pure_cross_prob*0.05, 0);
		        if (m_ignore > 1) m_ignore = 0.9999999;
	        }
	        else {
		        m_ignore = pure_cross_prob - ran_no.getDouble(pure_cross_prob*0.05, 0);
		        if (m_ignore < 0) m_ignore = 0.0000001;
	        }
                return m_ignore;
	}
	else {
		return pure_cross_prob;
	}
}
//child ttl value generator
int ttl_generator (int father, int mother, float mutation, Random& ran_no)
{
	int pure_cross_ttl;
	int ttl;
	//float mutation_value;

	if (father > mother) pure_cross_ttl = ran_no.getInt(father, mother);
	else pure_cross_ttl = ran_no.getInt(mother, father);
	//mutation
	if (ran_no.getDouble01() < mutation) {
		if (ran_no.getDouble01() > 0.5) ttl = pure_cross_ttl + 1;
		else {
			ttl = pure_cross_ttl - 1;
			//check minimum value boundary
			if (ttl < 2) ttl = 2;
		}
	}
	else ttl = pure_cross_ttl;
	return ttl;
}
//child connection limit generator
int connection_limit_generator (int father, int mother, float mutation, Random& ran_no)
{
	int pure_cross_connection;
	int connection;
	float mutation_value;
	connection = father;
        
	if (father > mother) pure_cross_connection = ran_no.getInt(father, mother);
	else pure_cross_connection = ran_no.getInt(mother, father);
	//mutation
	mutation_value = mutation;
	if (ran_no.getDouble01() < mutation_value) {
		if (ran_no.getDouble01() > 0.5) connection = pure_cross_connection + 1;
		else {
			connection = pure_cross_connection - 1;
			//check minimum value boundary
			if (connection < 2) connection = 2;
		}
	}
	else connection = pure_cross_connection;
	return connection;
}
int main(int argc, char* argv[]) {
	int nodes = 100;	
	int seed = 11;
	int k = 100; //the nunber of items
        int generation = 1501;
	int address;
	float hit_th;
	/*
	int fitness_checking_window = 100;
	float fitness_checking[fitness_checking_window];
	float fitness_mean = 10000000;
	float fitness_std = 10000000;
	*/
	
	ifstream indata(argv[1],ios::in);
        ofstream datafile1(argv[2],ios::app);
        ofstream datafile2(argv[3],ios::app);
	hit_th = atof(argv[4]);
	
	//to calculate fitness
	float hitrate[nodes];
	float diskcost[nodes];
	float cpucost[nodes];
	float fitness[nodes];
	//to keep genes
	float msgignore[nodes];
	int ttl[nodes];
	int connection[nodes];
	//to calculate average value for each generation
	float avg_hit;
	float avg_disk;
	float avg_cpu;
	float avg_fit;
	float avg_prob;
	float avg_ttl;
	float avg_conn;
	
        Ran1Random ran_no1(-1);
	
        //arguments for group1
        float parent_fit[nodes]; //fitness 
        float parent_prob[nodes]; //probability
        int parent_connection[nodes]; //connection
        int parent_TTL[nodes]; //TTL
	int parent_priority[nodes];

        float mutation_ignore = 0.5;  // probability of giving mutation
        float mutation_other = 0.005;
        // file read from previous generation
        for (int i = 0; i < nodes; i++) {
		indata >> parent_fit[i] >> parent_prob[i] >> parent_TTL[i] >> parent_connection[i] >> parent_priority[i];
		hitrate[i] = 0;
		diskcost[i] = 0;
		cpucost[i] = 0;
		//fitness[i] = 0;
		//cout << parent_fit[i] << "\t" << parent_prob[i] << "\t" << parent_TTL[i] << "\t" << parent_connection[i] << "\t" << parent_priority[i] << endl;
	}
        
	for(int gen = 1 ; gen < generation ; gen++) {
	/*
	while ((generation < fitness_checking_window) || (fitness_std > (fitness_mean*0.025))) {
		generation++;
		fitness_mean = 0;
		fitness_std = 0;
	*/
		avg_hit = 0;
		avg_cpu = 0;
		avg_disk = 0;
		avg_fit = 0;
		avg_prob = 0;
		avg_ttl = 0;
		avg_conn = 0;
		address = 0;
                //temporary variable for group fitness sorting 
                float temp_fit; //for sorting
                float temp_prob; //for sorting
                int temp_connection; // for sorting
                int temp_TTL; // for sorting
		bool temp_priority;
		for(int i = 0; i < nodes-1 ; i++) {
			for(int j = i+1; j < nodes ; j++) {
				if (parent_priority[i] == false && parent_priority[j] == true) {
					//fitness sorting
                                        temp_fit = parent_fit[i];
                                        parent_fit[i] = parent_fit[j];
                                        parent_fit[j] = temp_fit;
                                        //probability sorting
                                        temp_prob = parent_prob[i];
                                        parent_prob[i] = parent_prob[j];
                                        parent_prob[j] = temp_prob;
                                        //connection sorting
					temp_connection = parent_connection[i];
                                        parent_connection[i] = parent_connection[j];
                                        parent_connection[j] = temp_connection;
                                        //TTL sorting
                                        temp_TTL = parent_TTL[i];
                                        parent_TTL[i] = parent_TTL[j];
                                        parent_TTL[j] = temp_TTL;
					//priority sorting
					temp_priority = parent_priority[i];
					parent_priority[i] = parent_priority[j];
					parent_priority[j] = temp_priority;
				}
				else if (parent_priority[i] == parent_priority[j]) {
					if (parent_fit[i] < parent_fit[j]) {
						//fitness sorting
                                        	temp_fit = parent_fit[i];
                                        	parent_fit[i] = parent_fit[j];
                                        	parent_fit[j] = temp_fit;
                                        	//probability sorting
                                        	temp_prob = parent_prob[i];
                                       	 	parent_prob[i] = parent_prob[j];
                                        	parent_prob[j] = temp_prob;
                                        	//connection sorting
						temp_connection = parent_connection[i];
                                        	parent_connection[i] = parent_connection[j];
                                        	parent_connection[j] = temp_connection;
                                        	//TTL sorting
                                        	temp_TTL = parent_TTL[i];
                                        	parent_TTL[i] = parent_TTL[j];
                                        	parent_TTL[j] = temp_TTL;
						//priority sorting
						temp_priority = parent_priority[i];
						parent_priority[i] = parent_priority[j];
						parent_priority[j] = temp_priority;

					}
				}
			}
		}
		for(int h = 0 ; h < nodes ; h++) {
			//cout << parent_fit[h] << "\t" << parent_prob[h] << "\t" << parent_TTL[h] << "\t" << parent_connection[h] << "\t" << parent_priority[h] << endl;
		}
		cout << "========================================" << endl;
		// child reproduction for next generation
		for(int i = 0 ; i < sqrt(nodes) ; i++) {
			for(int j = 0 ; j < sqrt(nodes) ; j++) {
				msgignore[address] = ignore_prob_generator(parent_prob[i], parent_prob[j], mutation_ignore, ran_no1);
		                ttl[address] = ttl_generator(parent_TTL[i], parent_TTL[j], mutation_other, ran_no1);
		                connection[address] = connection_limit_generator(parent_connection[i], parent_connection[j], mutation_other, ran_no1);
		                address++;
			}
		}
		for (int iter = 1; iter < seed ; iter++) {
			Ran1Random ran_no2(iter);
			auto_ptr<SimpleNetwork> Net_ptr (new SimpleNetwork(ran_no2));
                        //generate nodes
                        for (int addr = 0 ; addr < nodes ; addr++) {
				Net_ptr->create(addr, ttl[addr], connection[addr], msgignore[addr], false, ran_no2);
			} // for node generation
                        std::set<std::string> items = rstringGenerator(k, 10, ran_no1);
                	std::set<std::string>::const_iterator item_it;
                	UniformNodeSelector uns(ran_no1);
			for (item_it = items.begin(); item_it != items.end(); item_it++) {
				std::string item = *item_it;
				uns.selectFrom(Net_ptr.get());
				SimpleNode* cache_origin = dynamic_cast<SimpleNode*> (uns.select());
				auto_ptr<SimpleMessage> cache_m (new SimpleMessage(cache_origin->getTTL(), ran_no2));
                        	auto_ptr<SimpleNetwork> cache_net (cache_m->visit(cache_origin, *Net_ptr));
	                	auto_ptr<NodeIterator> ni (cache_net->getNodeIterator());
				while (ni->moveNext()) {
					SimpleNode* itemNode = dynamic_cast<SimpleNode*> (ni->current());
					itemNode->insertItem(item);
				}
                        }
			int max_query;
			bool hits;
			int diff;
			for (item_it = items.begin(); item_it != items.end(); item_it++) {
	                        std::string item = *item_it;
                                auto_ptr<NodeIterator> t_query (Net_ptr->getNodeIterator()); //Net_ptr is entire network
                                while (t_query->moveNext() ) {
        	                        SimpleNode* query_origin = dynamic_cast<SimpleNode*> (t_query->current() );
				        max_query = 10;
				        hits = false;
					diff = -1;
                                        query_origin->querycount();
				        while(hits != true && max_query > 0) {
						Ran1Random ran_no3(diff);
						
					        auto_ptr<SimpleMessage> query_m (new SimpleMessage(query_origin->getTTL(), ran_no3));
	                                        auto_ptr<SimpleNetwork> query_net (query_m->visit(query_origin, *Net_ptr));
	                                        auto_ptr<NodeIterator> ni (query_net->getNodeIterator());
	                                    	while (ni->moveNext()) {
					                SimpleNode* querynode = dynamic_cast<SimpleNode*> (ni->current());
		                                        querynode->countrxmessage();
			                                if(querynode->searchItem(item)) {
				                                hits = true;
				                        }
                                                }
					        max_query--;
						diff--;
				        }
				        if(hits == true) {
					        query_origin->hitcount();
				        }
			        }
		        }
			float tmp_hit = 0;
			int tmp_disk = 0;
			int tmp_cpu = 0;
			
			auto_ptr<NodeIterator> totn (Net_ptr->getNodeIterator());
                	while (totn->moveNext() ) { //
				SimpleNode* inNode = dynamic_cast<SimpleNode*> (totn->current() );	  
	                	tmp_hit = (float)inNode->getQueryhits()/(float)inNode->getQuerymessage();
	                	hitrate[inNode->getAddress()] = hitrate[inNode->getAddress()] + tmp_hit;
				tmp_disk = (inNode->getItem()).size() + (inNode->size_tokenTable()/100);
				diskcost[inNode->getAddress()] = diskcost[inNode->getAddress()] + tmp_disk;
				tmp_cpu = inNode->gettxmessage() + inNode->getrxmessage()+(inNode->getprmessage())*(inNode->getItem()).size();
	                	cpucost[inNode->getAddress()] = cpucost[inNode->getAddress()] + tmp_cpu;
			}
		} // for seed iteration
		for (int k = 0 ; k < nodes ; k++) {
		        hitrate[k] = hitrate[k] / (seed-1);
			diskcost[k] = diskcost[k] / (seed-1);
			cpucost[k] = cpucost[k] / (seed-1);
			if (hitrate[k] > hit_th) {
				fitness[k] = -(cpucost[k] + diskcost[k]);
				parent_priority[k] = true;
			}
			else {
				fitness[k] = -(cpucost[k] + diskcost[k]);
			}
			parent_fit[k] = fitness[k];
			parent_prob[k] = msgignore[k];
			parent_TTL[k] = ttl[k];
			parent_connection[k] = connection[k];	
		}
		//specific generation nodes printing
		if (gen == 1 || gen == 2 || gen == 5 || gen == 10 || gen == 50 || gen == 100 || gen == 200 || gen == 500 || gen == 1000 || gen == 1500) {
			for (int n = 0 ; n < nodes ; n++) {
				datafile1 << parent_fit[n] << "\t" << parent_prob[n] << "\t" << parent_TTL[n] << "\t" << parent_connection[n] << "\t" << hitrate[n] << "\t" << diskcost[n] << "\t" << cpucost[n] << endl;
			}
		}
		
		for (int m = 0 ; m < nodes ; m++) {
			avg_hit = avg_hit + hitrate[m];
			hitrate[m] = 0;
			avg_disk = avg_disk + diskcost[m];
			diskcost[m] = 0;
			avg_cpu = avg_cpu + cpucost[m];
			cpucost[m] = 0;
			avg_fit = avg_fit + fitness[m];
			fitness[m] = 0;
			avg_prob = avg_prob + msgignore[m];
			avg_ttl = avg_ttl + ttl[m];
			avg_conn = avg_conn + connection[m];
		}
		
		avg_hit = avg_hit / nodes;
		avg_disk = avg_disk / nodes;
		avg_cpu = avg_cpu / nodes;
		avg_fit = avg_fit / nodes;
		avg_prob = avg_prob / nodes;
		avg_ttl = avg_ttl / nodes;
		avg_conn = avg_conn / nodes;
		//for evoultion history printing
	        datafile2 << avg_hit << "\t" << avg_disk << "\t" << avg_cpu << "\t" << avg_fit << "\t" << avg_prob << "\t" << avg_ttl << "\t" << avg_conn << endl;
		/*
                for (int i = 0 ; i < fitness_checking_window-1 ; i++) {
			fitness_checking[i] = fitness_checking[i+1];
		}
		fitness_checking[fitness_checking_window-1] = avg_fit;
		for (int i = 0 ; i < fitness_checking_window; i++) {
			fitness_mean = fitness_mean + fitness_checking[i];
		}
		fitness_mean = fitness_mean / float(fitness_checking_window);
		for (int i = 0 ; i < fitness_checking_window ; i++) {
			fitness_std = fitness_std + (fitness_checking[i] - fitness_mean) * (fitness_checking[i]);
		}
		fitness_std = sqrt(fitness_std / float(fitness_checking_window - 1));
		//for (int i = 0 ; i< fitness_checking_window ; i++) {
		//	cout << "fitness : " << fitness_checking[i] << endl;
		//}
		//cout << " deviation : " << fitness_std << endl;
		//cout << avg_hit << endl;
		*/
	}
	datafile1.close();
	datafile2.close();
	return 0;
}
