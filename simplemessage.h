#ifndef starsky__simplemessage
#define starsky__simplemessage

#include <message.h>
#include <simplenode.h>
#include <simplenetwork.h>

namespace Starsky {

/**
 * Message that does a breadth first broadcast.  Each node rebroadcasts
 * the message to its neighbors unless it has seen it already or the TTL=0.
 * If the TTL is -1, the broadcast goes forever.
 *
 * This code is basically identical to the Starsky::PercolationMessage class
 * except that the percolation probability is 1.
 * 
 */
	
class SimpleMessage : public Message {

  public:
    SimpleMessage(int ttl, Random& r);
    /**
     * This will return all the nodes and edges in the
     * out component of a particular Node within a number of hops
     * @param anode the node to start the message at
     * @param aNet the network that the message will travel on
     * @return a network consisting of all the nodes and edges crossed in a broadcast.
     */
    virtual SimpleNetwork* visit(Node* anode, Network& aNet);	
  protected:
    int _ttl;
    Random& _r;
};
	
}

#endif
