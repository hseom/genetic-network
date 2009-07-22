/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
Copyright (C) 2005  P. Oscar Boykin <boykin@pobox.com>, University of Florida

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

#include <simplemessage.h>

using namespace Starsky;
using namespace std;

SimpleMessage::SimpleMessage(int ttl) : _ttl(ttl)
{
}

SimpleNetwork* SimpleMessage::visit(Node* n, Network& net)
{
    map<int, Network::NodePSet> to_visit;
    map<int, Network::NodePSet >::iterator tv_it;
    Network::NodePSet::iterator a_it;
    //auto_ptr<SimpleNetwork> new_net (new SimpleNetwork());
    SimpleNetwork* new_net = dynamic_cast<SimpleNetwork*> (net.newNetwork() );
    SimpleNode* node = dynamic_cast<SimpleNode*> (n);
    to_visit[0].insert(node);
    new_net->add( node );
    int this_distance;
    //We loop through at each TTL:
    tv_it = to_visit.begin();
    while( tv_it != to_visit.end() && ( (tv_it->first < _ttl) || (_ttl == -1) ) ) {
        this_distance = tv_it->first + 1;
        //Here are all the nodes at this distance:
        for( a_it = tv_it->second.begin(); a_it != tv_it->second.end(); a_it++) {
            auto_ptr<EdgeIterator> ei( net.getEdgeIterator(*a_it) );
            while( ei->moveNext() ) {
	      Edge* this_edge = ei->current();
              SimpleNode* this_node = dynamic_cast<SimpleNode*> (this_edge->getOtherNode(*a_it) );
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
        }
        to_visit.erase(tv_it);
        tv_it = to_visit.begin();
    }
    //cout << "visit's return net size: " << new_net->getNodeSize() << endl;
    return new_net;
    //delete new_net;
}
