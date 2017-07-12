#include "cfg_utility.h"

using namespace cfg_u;

void get_instruction(istream&, Graph&, list::List&, bool);
void parser_wannabe(istream&, bool);
bool allVisited(string, list::List, const Graph&);
int visited_nodes(const Graph&);
int unvisited_nodes(const Graph&);

/******************************** Main utility Functions ****************************************/

// Turn a source code into a graph, or at least thi is the idea:
Graph cfg_u::compute_cfg(string file_name, string& start, string& end){
	string str(""), prevline("");
	list::List prevlines = list::createEmpty();
	ifstream in;
	Graph g = createEmptyGraph();
	in.open(file_name.c_str());
	for(int i=0; getline(in, str); i++){
		istringstream iss(str), to_check(str);
		parser_wannabe(to_check, false);
		get_instruction(iss, g, prevlines, false);
		// this line catch the start label:
		if(i==0) start = list::get(0, prevlines);
	}
	// this line catch end label:
	end = list::get(0, prevlines);
	return g;
}

// Compute randomWalk from start to end label in the given graph:
void cfg_u::randomWalk(const Graph& g, list::List& path, string curr, const string end, int& crossed_edges){
		if(list::size(path)>MAX_SIZE) throw STACK_LIMIT_REACHED;
		unsigned int rindex;
		string temp("");
		int degree;

		// aggiungo la label corrente al cammino:
		list::addBack(path, curr);
		if(curr==end) return;

		// inizializzo lista ausiliaria per le adiacenze:
		list::List aux_list = list::createEmpty();

		// creo lista label tra cui cercare il prossimo punto del cammino:
		aux_list=graph::adjacentList(curr, g);
		// se il nodo non eiste o ha 0 adiacenze (e non è il nodo finale), esco:
		if(!nodeDegree(curr, degree, g)||degree==0) throw GOTO_STOP_FLOW;

		//se già tutte le adiacenze sono state visitate allora ne prendo una casualmente(o voglio implementare la randomWalk esatta): 
#ifndef ACT_RW		
		if(allVisited(curr, aux_list, g)){
			rindex = rand()%degree;
			temp = list::get(rindex, aux_list);
		}

		// altrimenti continuo a cercare finché non ne trovo una non visitata:
		else{
			do{
				rindex = rand()%degree;
				temp = list::get(rindex, aux_list);
			}while(isCrossed(curr, temp, g));
			crossed_edges++;
			set_Crossed(curr, temp, g);
		}
#else
		rindex = rand()%degree;
		temp = list::get(rindex, aux_list);
		if(!isCrossed(curr, temp, g)){
			crossed_edges++;
			set_Crossed(curr, temp, g);
		}	
#endif
		randomWalk(g, path, temp, end, crossed_edges);
}

/*	Since I'm a lazy person, I decided to not delete 'weight' field in edgeNode structure(defined in: graph.cpp, line 5)
*	instead, I use it as a flag to know if that edge has already been visited, my laziness brought me to use 1 and 0 values
*	so it's easier and can be handled by boolean functions. This function reset all weights to 0.
*/
void cfg_u::reinit_edges(const Graph& g, string start){
	list::List tmp = list::createEmpty();
	list::List vertices = list::createEmpty();
	list::addBack(vertices, start);
	while(!list::isEmpty(vertices)){
		string temp = list::get(0, vertices);
		list::popFront(vertices);
		tmp = graph::adjacentList(temp, g);
		for(int i=0; i<list::size(tmp); i++){
			string to_add = list::get(0, tmp);
			list::popFront(tmp);
			set_uncrossed(temp, to_add, g);
			list::addBack(vertices, to_add);
		}
	}
}
/******************************** Utility Helper Functions **************************************/

// useless function I created to avoid madness during development:
void fprint(string s, int n){
#ifdef DEBUG
	cout<<"\e[1;"<<n<<"m"<<s<<"\e[0m";
#endif
}

// return true if all edge in graph has been crossed:
bool allVisited(string start, list::List adj, const Graph& g){
	for(unsigned int i=0; i<list::size(adj); i++) if(!isCrossed(start, list::get(i, adj), g)) return false;
	return true;
}

// Translate a line instruction into a graph label and computes possible adjacencies:
void get_instruction(istream& in, Graph& g, list::List& prevlines, bool is_subinstruction){
	string aux, line, nextline, auxline; 
	if(in.eof()) return;
	// if it is not a subinstruction, first token is line number:
	if(!is_subinstruction) in>>line;
	// else line number must be computed from prevline:
	else{
		int sizel = list::size(prevlines);
		line=list::get(sizel-1, prevlines);
		// if line caught is empty, program ends;
		if(line=="")return;	
		line+='A';
	}
	//add line label to graph:
	addVertex(line, g);
	//add edge between current label and previous:
	if(!is_subinstruction) {
		while(!list::isEmpty(prevlines)){
			graph::addEdge(list::get(0, prevlines), line, 0, g);
			list::popFront(prevlines);
		}
	}
	// if subinstruction, the only father vertex is previous one:
	else graph::addEdge(list::get(list::size(prevlines)-1, prevlines), line, 0, g);
	//adding current line to prevoiusline list:
	list::addBack(prevlines, line);
	//parse type of instruction:
	in>>aux;
	if(aux=="IF"){
		in.ignore(256, ')');
		get_instruction(in, g, prevlines, true);
		return;
	}
	if(aux=="GOTO"){
		in>>auxline;
		addVertex(auxline, g);
		addEdge(line, auxline, 0, g);
		if(!is_subinstruction) list::clear(prevlines);
		else list::popBack(prevlines);
		return;
	}
	return;
}

// This is not a parser, but it dreams of becoming one some day. Not today anyway.
void parser_wannabe(istream& iss, bool is_subinstruction){
	I_TYPE instr_type;
	string tmp;
	//check line label:
	if(!is_subinstruction){
		iss>>tmp;
		for(unsigned int i=0; i<tmp.size(); i++ )if(!isdigit(tmp[i])) throw NOT_LINE_LABEL;
	}
	//check instruction type:
	iss>>tmp;
	if(tmp=="IF") instr_type=BRANCH;
	if(tmp=="GOTO") instr_type=JUMP;
	else return;
	iss>>tmp;
	// check token after instruction:
	switch(instr_type){
		case BRANCH:
			if(tmp[0]!='(' || tmp[tmp.size()-1]!=')') throw MISSING_IF_CONDITIONS;
			return parser_wannabe(iss, true);
		case JUMP:
			for(unsigned int i=0; i<tmp.size(); i++ )if(!isdigit(tmp[i])) throw NOT_LINE_LABEL;
			break;
		default:
			return;
	}
}