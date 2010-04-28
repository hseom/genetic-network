#include <simplemessage.h>

using namespace Starsky;
using namespace std;

SimpleMessage::SimpleMessage(int ttl, Random& r) : _ttl(ttl), _r(r)
{
} 

SimpleNetwork* SimpleMessage::visit(Node* n, Network& net)
{
	map<int, Network::NodePSet> to_visit;
        map<int, Network::NodePSet >::iterator tv_it;
        Network::NodePSet::iterator a_it;
        SimpleNetwork* new_net = dynamic_cast<SimpleNetwork*> (net.newNetwork() );
        SimpleNode* node = dynamic_cast<SimpleNode*> (n);
	node->increaseMID();
        to_visit[0].insert(node); //query origin node
        new_net->add( node );
        int this_distance;
        int message_TTL;
	int messageID;
	messageID = node->getMID();
	bool first_tier = false;
        float message_ignore;
        //We loop through at each TTL:
        tv_it = to_visit.begin();
	while( tv_it != to_visit.end() && ((tv_it->first < _ttl) || (_ttl == -1)) ) {
		if(!first_tier) {
			//token code
			this_distance = tv_it->first + 1;
	                message_TTL = _ttl - this_distance;
                        //Here are all the nodes at this distance:
                        for( a_it = tv_it->second.begin(); a_it != tv_it->second.end(); a_it++) {
				auto_ptr<EdgeIterator> ei( net.getEdgeIterator(*a_it) );
                                while( ei->moveNext() ) {
					Edge* this_edge = ei->current();
                                        SimpleNode* this_node = dynamic_cast<SimpleNode*> (this_edge->getOtherNode(*a_it) );
					node->counttxmessage();
					if ((!this_node->messageIDcheck(node->getAddress(), messageID)) && (node->getAddress() != this_node->getAddress())) { 
						this_node->countrxmessage();
						//Do you have an information in the token table?
                                        	if (!node->token_there(this_node->getAddress())) {
							node->token_init(this_node->getAddress());
							node->freebieNumInit(this_node->getAddress());
							//cout << node->freebieNumGet(this_node->getAddress()) << endl;
                                       		}	
						if (!this_node->token_there(node->getAddress())) {
							this_node->token_init(node->getAddress());
							this_node->freebieNumInit(node->getAddress());
							//cout << this_node->freebieNumGet(node->getAddress()) << endl;
						}
						//freebie end?
						if(this_node->freebieNumGet(node->getAddress()) < 20) {
							this_node->freebieNumIncrease(node->getAddress());
							//cout << this_node->freebieNumGet(node->getAddress()) << endl;
	                                        	message_ignore = _r.getDouble01();
							//if freebie ok and message isn't ignored
	                                        	if (this_node->getprob() < message_ignore) {
						        	if (message_TTL < this_node->getTTL()) {
									//Make sure to consider potential directionality 
              	                                                	if( this_edge->connects(*a_it, this_node) ) {
										if( !new_net->has( this_node ) ) {
											//We have not seen this one yet.
                                                                                	to_visit[this_distance].insert( this_node );
		                                                                	new_net->add( this_node );
								        	}
		                                                        	//We have to cross it, only the other node knows if it should pass it
						                        	new_net->add( this_edge );
										//node->token_increase(this_node->getAddress());
							         	}
								}
					                	else if (message_TTL == this_node->getTTL()) {
									//Make sure to consider potential directionality 
              	                                                	if( this_edge->connects(*a_it, this_node) ) {
										if( !new_net->has( this_node ) ) {
											//We have not seen this one yet.
									        	to_visit[this_distance].insert( this_node );
		                                                                	new_net->add( this_node );
										}
		                                                        	//We have to cross it, only the other node knows if it should pass it
						                        	new_net->add( this_edge );
							                	this_node -> lastEdgechange(); //I am last node, so I don't want to forward the message to my neighbor.
										//node->token_increase(this_node->getAddress());
									}
								}
							}
							else {
								//do nothing because of freebie
							} 
						}
						else {
							//insert token method code
							this_node->freebieNumIncrease(node->getAddress());//why?
                                                	message_ignore = _r.getDouble01();
	                                        	if (this_node->getprob() < message_ignore) {
						        	if (message_TTL < this_node->getTTL()) {
									//Make sure to consider potential directionality 
              	                                                	if( this_edge->connects(*a_it, this_node) ) {
										if( !new_net->has( this_node ) ) {
											//We have not seen this one yet.
                                                                                	to_visit[this_distance].insert( this_node );
		                                                                	new_net->add( this_node );
								       		}
		                                                        	//We have to cross it, only the other node knows if it should pass it
						                        	new_net->add( this_edge );
										//node->token_increase(this_node->getAddress());
							         	}
								}
					                	else if (message_TTL == this_node->getTTL()) {
									//Make sure to consider potential directionality 
              	                                                	if( this_edge->connects(*a_it, this_node) ) {
										if( !new_net->has( this_node ) ) {
											//We have not seen this one yet.
									        	to_visit[this_distance].insert( this_node );
		                                                                	new_net->add( this_node );
										}
		                                                        	//We have to cross it, only the other node knows if it should pass it
						                        	new_net->add( this_edge );
							                	this_node -> lastEdgechange(); //I am last node, so I don't want to forward the message to my neighbor.
										//node->token_increase(this_node->getAddress());
									}
								}
							}
							else {
								node->token_decrease(this_node->getAddress());
							}
						}
					}
				}
			}
			first_tier = true;
		}
		else {
			//token code
                        this_distance = tv_it->first + 1;
	                message_TTL = _ttl - this_distance;
                        //Here are all the nodes at this distance:
                        for( a_it = tv_it->second.begin(); a_it != tv_it->second.end(); a_it++) {
				SimpleNode* pre_node = dynamic_cast<SimpleNode*> (*a_it);
			        if (!pre_node->lastEdgecheck()) { // check whether I am last node or not
					auto_ptr<EdgeIterator> ei( net.getEdgeIterator(*a_it) );
                                        while( ei->moveNext() ) {
						Edge* this_edge = ei->current();
                                                SimpleNode* this_node = dynamic_cast<SimpleNode*> (this_edge->getOtherNode(*a_it) );
						pre_node->counttxmessage();
						if ((!this_node->messageIDcheck(node->getAddress(), messageID)) && (node->getAddress() != this_node->getAddress())) { 
							this_node->countrxmessage();
                                                	if (!node->token_there(this_node->getAddress())) {
								node->token_init(this_node->getAddress());
								node->freebieNumInit(this_node->getAddress());
						        	//cout << node->freebieNumGet(this_node->getAddress()) << endl;
                                               		}	
					        	if (!this_node->token_there(node->getAddress())) {
						        	this_node->token_init(node->getAddress());
						        	this_node->freebieNumInit(node->getAddress());
						        	//cout << this_node->freebieNumGet(node->getAddress()) << endl;
					        	}
					        	//pre_node->counttxmessage();
                                         		if(this_node->freebieNumGet(node->getAddress()) < 10) {
								this_node->freebieNumIncrease(node->getAddress());
						        	//cout << this_node->freebieNumGet(node->getAddress()) << endl;
	                                                	message_ignore = _r.getDouble01();
	                                                	if (this_node->getprob() < message_ignore) {
									if (message_TTL < this_node->getTTL()) {
										//Make sure to consider potential directionality 
              	                                                        	if( this_edge->connects(*a_it, this_node) ) {
											if( !new_net->has( this_node ) ) {
												//We have not seen this one yet.
                                                                                        	to_visit[this_distance].insert( this_node );
		                                                                        	new_net->add( this_node );
									        	}
		                                                                	//We have to cross it, only the other node knows if it should pass it
						                                	new_net->add( this_edge );
										}
									}
					                        	else if (message_TTL == this_node->getTTL()) {
										//Make sure to consider potential directionality 
              	                                                        	if( this_edge->connects(*a_it, this_node) ) {
											if( !new_net->has( this_node ) ) {
												//We have not seen this one yet.
									                	to_visit[this_distance].insert( this_node );
		                                                                        	new_net->add( this_node );
									        	}
		                                                                	//We have to cross it, only the other node knows if it should pass it
						                                	new_net->add( this_edge );
							                        	this_node -> lastEdgechange(); //I am last node, so I don't want to forward the message to my neighbor.
										}
									}
								}
							}
							else {
						        	this_node->freebieNumIncrease(node->getAddress());
								message_ignore = _r.getDouble01();
	                                                	if (this_node->getprob() < message_ignore) {
									if (message_TTL < this_node->getTTL()) {
										//Make sure to consider potential directionality 
              	                                                        	if( this_edge->connects(*a_it, this_node) ) {
											if( !new_net->has( this_node ) ) {
												//We have not seen this one yet.
                                                                                        	to_visit[this_distance].insert( this_node );
		                                                                        	new_net->add( this_node );
									        	}
		                                                                	//We have to cross it, only the other node knows if it should pass it
						                                	new_net->add( this_edge );
										}
									}
					                        	else if (message_TTL == this_node->getTTL()) {
										//Make sure to consider potential directionality 
              	                                                        	if( this_edge->connects(*a_it, this_node) ) {
											if( !new_net->has( this_node ) ) {
												//We have not seen this one yet.
									                	to_visit[this_distance].insert( this_node );
		                                                                        	new_net->add( this_node );
									        	}
		                                                                	//We have to cross it, only the other node knows if it should pass it
						                                	new_net->add( this_edge );
							                        	this_node -> lastEdgechange(); //I am last node, so I don't want to forward the message to my neighbor.
										}
									}
								}
								else {
									node->token_decrease(this_node->getAddress());
								}
							}
						}
					}
				}
			}
		}
	        to_visit.erase(tv_it); //origin node erase
                tv_it = to_visit.begin(); //insert next node
	}
        return new_net;
        //delete new_net;
}
