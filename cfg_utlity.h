#ifndef CFG_U
#define CFG_U

#include <ctype.h>
#include "graph.h"

using namespace std;
using namespace graph;

namespace cfg_u{
	
	enum ERROR {NOT_LINE_LABEL, MISSING_IF_CONDITIONS, OK};
	enum I_TYPE {BRANCH, JUMP};

	ERROR parser_wannabe(istream&, bool);
	void get_instruction(istream&, Graph&, string&, bool);
	Graph comput_gfc();
}

#endif