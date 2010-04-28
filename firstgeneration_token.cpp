#include <netmodeler.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <memory>
//#include <time>

using namespace Starsky;
using namespace std;

//random string generator
std::set<std::string> rstringGenerator (int howmany, int length, Random& r ){
	std::set<std::string> items;
	for(int no = 0 ; no < howmany ; no++){
		std::string item;
		for(int i = 0 ; i < length ; i++){
			int rand_no = (int) (r.getDouble01() * 122);
			if ( rand_no < 65 ) { rand_no = 65 + rand_no % 56;}
			if ( (rand_no > 90) && (rand_no < 97) ) { rand_no += 6; }
			item += (char)rand_no;
		}
		items.insert(item);
	}
	return items;
}

int main(int argc, char* argv[]) {
        int nodes = 100;
        int seed = 11;
        int min_ttl = 4;
        int max_ttl = 10;
        int min_degree = 6;
        int max_degree = 8;
	int k = 100;
	float hit_th;
        
	ofstream datafile1(argv[1],ios::app);
	hit_th = atof(argv[2]);


        Ran1Random ran_no1(-1);
        
	//To parse the result after network operation
	float hitrate[nodes];
	float diskcost[nodes];
	float cpucost[nodes];
	float fitness[nodes];
	bool priority[nodes];

	//For genes 
	float msgignore[nodes];
	int ttl[nodes];
	int connection[nodes];

	for(int i = 0; i < nodes ; i++) {
		//initiate variable 
		hitrate[i] = 0;
		diskcost[i] = 0;
		cpucost[i] = 0;
		fitness[i] = 0;
		priority[i] = false;
		//gene generation
		msgignore[i] = ran_no1.getDouble01();
		ttl[i] = ran_no1.getInt(max_ttl, min_ttl);
		connection[i] = ran_no1.getInt(max_degree, min_degree);
	}
  
        int addr;
        bool group = false;
	std::set<std::string> items = rstringGenerator(k, 10, ran_no1); //item generation
        std::set<std::string>::const_iterator item_it; //item iteration generation

        for (int iter = 1 ; iter < seed ; iter++) {
		Ran1Random ran_no2(iter); //seed generation
                auto_ptr<SimpleNetwork> Net_ptr (new SimpleNetwork(ran_no2)); //empty network generation
		//nodes generation
		for(int i = 0 ; i < nodes ; i++) {
			addr = i;
                        Net_ptr->create(addr, ttl[i], connection[i], msgignore[i], group, ran_no2);
		}
		// network generation finished
		// item caching
		//std::set<std::string> items = rstringGenerator(k, 10, ran_no1); //item generation
                //std::set<std::string>::const_iterator item_it; //item iteration generation
                UniformNodeSelector uns(ran_no2);
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
		//item querying
		int max_query;
		bool hits;
		int diff; // for different query network
                for (item_it = items.begin(); item_it != items.end(); item_it++) {
			std::string item = *item_it;
                        auto_ptr<NodeIterator> t_query (Net_ptr->getNodeIterator()); //Net_ptr is entire network
                        while (t_query->moveNext() ) {
				SimpleNode* query_origin = dynamic_cast<SimpleNode*> (t_query->current());
				max_query = 10;
                                hits = false;
				query_origin->querycount();
				diff = -1;
			    	while(hits != true && max_query > 0) {
					//cout << "max_query: " << "\t" << max_query << ", hits: " << "\t" << hits << endl; 
					/*
					srand(time(0));
					diff = rand()%100 + 1;
					*/
					Ran1Random ran_no3(diff); //seed generation
	                    		auto_ptr<SimpleMessage> query_m (new SimpleMessage(query_origin->getTTL(), ran_no3));
	                    		auto_ptr<SimpleNetwork> query_net (query_m->visit(query_origin, *Net_ptr));
	                   	 	auto_ptr<NodeIterator> ni (query_net->getNodeIterator());
	                    		while (ni->moveNext()) {
		                  		SimpleNode* querynode = dynamic_cast<SimpleNode*> (ni->current());
		                  		querynode->countrxmessage();
						if (querynode->token_valuecheck(query_origin->getAddress())) {
			                		if(querynode->searchItem(item)) {
								//cout << " hits good" << endl;
				              			hits = true;
								//for (int t = 0 ; t < 2 ; t++) {

									query_origin->token_increase(querynode->getAddress());
								//}
							}
							else {

								//cout << " hits bad" << endl;
							}
				        	}
						else {
							//do nothing
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

		// fitness calculation
		float tmp_hit = 0;
		float tmp_disk = 0;
		float tmp_cpu = 0;

		auto_ptr<NodeIterator> totn (Net_ptr->getNodeIterator());
		while (totn->moveNext()){
                        SimpleNode* inNode = dynamic_cast<SimpleNode*> (totn->current() );
			tmp_hit = (float)inNode->getQueryhits()/(float)inNode->getQuerymessage();
			hitrate[inNode->getAddress()] = hitrate[inNode->getAddress()] + tmp_hit;
			tmp_disk = inNode->getItem().size() + (inNode->size_tokenTable()/100); 
                        diskcost[inNode->getAddress()] = diskcost[inNode->getAddress()] + tmp_disk;
                        tmp_cpu = inNode->getrxmessage() + inNode->gettxmessage() + (inNode->getprmessage()*(inNode->getItem()).size());
			cpucost[inNode->getAddress()] = cpucost[inNode->getAddress()] + tmp_cpu;
		}
	}
	for (int k = 0 ; k < nodes ; k++) {
		hitrate[k] = hitrate[k] / (seed-1);
		diskcost[k] = diskcost[k] / (seed-1);
		cpucost[k] = cpucost[k] / (seed-1);
		if (hitrate[k] > hit_th) {
			fitness[k] = -(cpucost[k] + diskcost[k]);
			priority[k] = true;

		}
		else {
			fitness[k] = -(cpucost[k] + diskcost[k]);
			priority[k] = false;
		}
		//datafile1 << fitness[k] << "\t" << msgignore[k] << "\t" << ttl[k] << "\t" << connection[k] << "\t" << priority[k] << endl;
		datafile1 << fitness[k] << "\t" << hitrate[k] << "\t" <<diskcost[k] << "\t" << cpucost[k] << "\t" << msgignore[k] << "\t" << ttl[k] << "\t" << connection[k] << "\t" << priority[k] << endl;
	}
	return 0;
}
