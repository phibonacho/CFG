#ifndef CFG_U
#define CFG_U

#include <cstdlib>
#include <ctype.h>
#include "graph.h"

using namespace std;
using namespace graph;

namespace cfg_u{	
	enum ERROR {NOT_LINE_LABEL, MISSING_IF_CONDITIONS, OK};
	enum I_TYPE {BRANCH, JUMP};
	//compute control flow graph of a program(return start and end labels):
	Graph compute_cfg(string, string&, string&);
	//return a random walk of a graph:
	list::List randomWalk(const Graph&, string, string);
	//

}
#endif