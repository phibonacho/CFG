/*
Autore:				Andrea Storace
Ultima modifica:	16/06/2017
Descrizione:		Libreria Ausiliaria al tipo di dato Control Flow Graph;
*/
#ifndef CFG_U
#define CFG_U

//#define DEBUG

#include <cstdlib>
#include <ctype.h>
#include "graph.h"

using namespace std;
using namespace graph;

namespace cfg_u{	
	// A bunch of useless enum 'cause I'm a paranoid student:
	enum ERROR {NOT_LINE_LABEL, MISSING_IF_CONDITIONS, OK, GOTO_STOP_FLOW, BLACKMAGIC_OCCURRED};
	enum I_TYPE {BRANCH, JUMP};

	// compute control flow graph of a program(return start and end labels):
	Graph compute_cfg(string, string&, string&);
	
	// return a random walk of a graph:
	void randomWalk(const Graph&, list::List& path, string,const string, int&);

	// reinitialize edges:
	void reinit_edges(const Graph&, string);
}
#endif