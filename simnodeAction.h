/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2008  Taewoong Choi <twchoi@ufl.edu>, University of Florida

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

//Only implement the cache and query action

#ifndef starsky__nodeAction_h
#define starsky__nodeAction_h

#include "event.h"
#include "netmodeler.h"
#include "node.h"
#include "edge.h"
#include <map>
#include <vector>
#include <iostream>
#include <cmath>
#include <memory>
#include <simplemessage.h>

namespace Starsky {

  //action for node fails
  class FailAction : public Action {
    public:
      FailAction(EventScheduler& sched, Random& r, SimpleNetwork& sn);
      void Execute();
    protected:
      EventScheduler& _sched;
      Random& _r;
      SimpleNetwork& _sn;
  };

  // action for node leaves
  class LeaveAction : public Action {
    public:
      LeaveAction(EventScheduler& sched, Random& r, SimpleNetwork& sn);
      void Execute();
      void getNewConnection(SimpleNetwork& sn, SimpleNode* ns, SimpleNode* nei, std::set<Edge*> add_edge);
    protected:
      EventScheduler& _sched;
      Random& _r;
      SimpleNetwork& _sn;
  };

  //action for node joins
  class JoinAction : public Action {
    public:
      JoinAction(EventScheduler& sched, Random& r, SimpleNetwork& sn);
      void Execute();
      void copyItems(SimpleNode* ns, SimpleNode* nei);
      //void getConnection(SimpleNode* ns);
    protected:
      EventScheduler& _sched;
      Random& _r;
      SimpleNetwork& _sn;
  };
  // action for caching objects in the network
  class CacheAction : public Action { 
    public:
      CacheAction(EventScheduler& sched, Random& r, INodeSelector& ns, SimpleNetwork& sn, std::string item);
      void Execute();
      void copyItems(SimpleNode* ns, SimpleNode* nei);

    protected:
      EventScheduler& _sched;
      Random& _r;
      SimpleNetwork& _sn;
      INodeSelector& _ns;
      std::string _item;
      int _ttl;
  };
  // action for querying items in the network
  class QueryAction : public Action {
    public:
      int sum_hits;
      QueryAction(EventScheduler& sched, Random& r, INodeSelector& ns, SimpleNetwork& sn, std::string item);
      void Execute();
    protected:
      EventScheduler& _sched;
      Random& _r;
      SimpleNetwork& _sn;
      INodeSelector& _ns;
      std::string _item;
      int _ttl;
  };
}
#endif


