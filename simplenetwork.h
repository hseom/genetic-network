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
	void create(int address, int ttl, int connection_limit, float q_prob, bool g_id, Random& ran_no);
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
