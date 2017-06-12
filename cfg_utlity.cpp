#include "cfg_utility.h"
using namespace cfg_u;

void cfg_u::get_instruction(istream& in, Graph& g, string& prevline, bool is_subinstruction){
	string aux, line, nextline, auxline; 
	if(in.eof()) return;
	if(!is_subinstruction) in>>line;
	else{
		//compute line number:
		line=prevline;
		line+='A';
	}
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

ERROR cfg_u::parser_wannabe(istream& iss, bool is_subinstruction){
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

Graph cfg_u::compute_cfg(string file_name){
	string str(""), prevline("");
	ifstream in;
	Graph g = createEmptyGraph();
	in.open(file_name.c_str());
	while(getline(in, str)){
		istringstream iss(str), to_check(str);
		if(parser_wannabe(to_check, false)!=OK){
			cout<<"sintax error\n";
			return emptyGraph;
		}
		get_instruction(iss, g, prevline, false);
	}
	return g;
}