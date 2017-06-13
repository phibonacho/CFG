#include "cfg_utlity.h"
using namespace cfg_u;

void get_instruction(istream&, Graph&, string&, bool);
ERROR parser_wannabe(istream&, bool);
Graph getVertex(Label, const Graph&);
list::List randomWalk_aux(const Graph&, string);

Graph cfg_u::compute_cfg(string file_name, string& start, string& end){
	string str(""), prevline("");
	ifstream in;
	Graph g = createEmptyGraph();
	in.open(file_name.c_str());
	for(int i=0; getline(in, str); i++){
		istringstream iss(str), to_check(str);
		if(parser_wannabe(to_check, false)!=OK)	return emptyGraph;
		get_instruction(iss, g, prevline, false);
		if(i==0) start = prevline;
	}
	end = prevline;
	return g;
}


list::List cfg_u::randomWalk(const Graph& g, string start, string end){
	Graph begin = getVertex(start, g);
	return randomWalk_aux(begin, end);
}

/*********************************************************************************************************************************/
Graph getVertex(Label l, const Graph& g){
	if(g==emptyGraph) return emptyGraph;
	if(g->payload == l) return g;
	return getVertex(l, g->nextVertex);
}

list::List randomWalk_aux(const Graph& begin, string end){
	list::List path=list::createEmpty();
	int degree, rindex;
	//add node to path:
	list::addBack(path, begin->payload);
	//obtain node adjoinments:
	graph::nodeDegree(begin->payload, degree,begin);
	rindex = rand()%degree;
	for(int i=0, graph::adjList tmp=begin->edges; i<rindex; i++, tmp=tmp->nextEdge);
	
}

void get_instruction(istream& in, Graph& g, string& prevline, bool is_subinstruction){
	string aux, line, nextline, auxline; 
	if(in.eof()) return;
	if(!is_subinstruction) in>>line;
	else{
		//compute line number:
		line=prevline;
		line+='A';
	}
	if(line=="")return;
	//add line label to graph:
	addVertex(line, g);
	//add edge between current label and previous:
	if(prevline!="") addEdge(prevline, line, 1, g);
	//parse type of instruction:
	in>>aux;
	if(aux=="IF"){
		in.ignore(256, ')');
		get_instruction(in, g, line, true);
	}
	if(aux=="GOTO"){
		in>>auxline;
		addVertex(auxline, g);
		addEdge(line, auxline, 1, g);
		if(!is_subinstruction) prevline = "";
	}
	if(!is_subinstruction) prevline = line;
	return;
}

ERROR parser_wannabe(istream& iss, bool is_subinstruction){
	I_TYPE instr_type;
	string tmp;
	//check line label:
	if(!is_subinstruction){
		iss>>tmp;
		for(unsigned int i=0; i<tmp.size(); i++ )if(!isdigit(tmp[i])) return NOT_LINE_LABEL;
	}
	//check instruction type:
	iss>>tmp;
	if(tmp=="IF") instr_type=BRANCH;
	if(tmp=="GOTO") instr_type=JUMP;
	if(tmp=="END") return OK;
	iss>>tmp;
	
	switch(instr_type){
		case BRANCH:
			if(tmp[0]!='(' || tmp[tmp.size()-1]!=')') return MISSING_IF_CONDITIONS;
			return parser_wannabe(iss, true);
		case JUMP:
			for(unsigned int i=0; i<tmp.size(); i++ )if(!isdigit(tmp[i])) return NOT_LINE_LABEL;
	}
	return OK;
}
