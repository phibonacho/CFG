/*
Autore:				Andrea Storace
Ultima modifica:	16/06/2017
Descrizione:		Libreria Ausiliaria al tipo di dato Control Flow Graph;
*/
#ifndef CFG_U
#define CFG_U

// Uncomment following line to enter debug mode:
// #define DEBUG
// Uncomment following line to implement actual randomWalk(default is 'smart' randomWalk):
// #define ACT_RW

#include <cstdlib>
#include <ctype.h>
#include "graph.h"

using namespace std;
using namespace graph;

namespace cfg_u{	
	// A bunch of useless enum 'cause I'm a paranoid student:
	enum ERROR {NOT_LINE_LABEL, MISSING_IF_CONDITIONS, GOTO_STOP_FLOW, EMPTY_SOURCE_CODE, STACK_LIMIT_REACHED, BLACKMAGIC_OCCURRED};
	enum I_TYPE {BRANCH, JUMP};
	const int MAX_SIZE = 16000;
	// compute control flow graph of a program(return start and end labels):
	Graph compute_cfg(string, string&, string&);
	
	// return a random walk in a graph:
	void randomWalk(const Graph&, list::List& path, string,const string, int&);

	// reinitialize edges:
	void reinit_edges(const Graph&, string);
}

void fprint(string, int);

#endif