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

#ifndef starsky__SimpleNetwork_H
#define starsky__SimpleNetwork_H

#include "network.h"
#include "random.h"
#include "ran1random.h"
#include "simplenode.h"
#include "swedge.h"
#include <set>
#include <map>
#include <vector>
#include <math.h>

namespace Starsky {

    /**
     * John Kleinberg model for a 1-D small world.  
     * Nodes are arranged in a ring, 
     * and with probability p(~1/d) each node is also connected to a shortcut.
     */
class SimpleNetwork : public Network {

  public:
	SimpleNetwork(Ran1Random& r);
	//SimpleNetwork();
        SimpleNetwork(int nodes, Ran1Random& r);
	/**i
	 *make new empty network
	 */
	//virtual SimpleNetwork* newNetwork() const { 
	virtual SimpleNetwork* newNetwork() const { 
		return new SimpleNetwork(_r);}
		//return new Network();}
	/**
	 *@param address return Node by address
	 */
	//RandAddrNode* getNodeFromAddress(const int address) const;
	std::vector<SimpleNode*> node_vec;
    //void create(int n);
	void create(int n, int ttl, int connection_limit, float q_prob, bool g_id);
  protected:
	/** 
	 * @param n create new SWNetwork whose size is n
	 */
	//double _p;
	Ran1Random& _r;
	//std::map<int, RandAddrNode*> _node_map;
	//std::vector<RandAddrNode*> _node_vec;
  };
}

#endif
