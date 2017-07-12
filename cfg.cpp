#include "cfg.h"

using namespace cfg;

// definizione della struttura del cfg:
struct cfg::cfg_struct{
	string start, end;
	int vertices, edges, crossed_edges, total_paths;
	graph::Graph g;
	list::List* paths;
};

float coverage(const cfg_type&);

// crea cfg vuoto:
cfg_type cfg::createEmpty(){
	return emptyCfg;
}

/* inizializza la struttura calcolando il grafo da un file sorgente, salva le informazioni quali:
*	-numero di archi;
*	-numero di vertici;
*	-Label di inizio e fine;
*	-numero di archi attraversati;
*	-numero di cammini calcolati;
*/
void cfg::initialize(string filename, cfg_type& cg){
	cfg_type aux = new cfg_struct;
	int vertices, edges;
	string start, end;

	// Inizializza grafo:
	graph::Graph tmpg = graph::createEmptyGraph(); 
	tmpg = cfg_u::compute_cfg(filename, start, end);

	if(tmpg==graph::emptyGraph) throw cfg_u::EMPTY_SOURCE_CODE;

	// informazioni su vertici e archi:
	vertices = graph::numVertices(tmpg);
	edges = graph::numEdges(tmpg);

	aux->start = start;
	aux->end = end;
	aux->g = tmpg;
	aux->vertices=vertices;
	aux->edges=edges;
	aux->crossed_edges = 0;
	cg = aux;
}

void cfg::compute_paths(cfg_type& cg, int budget){
	int i;
	list::List* tmp = new list::List[budget], *tmp1;

	// inizializza i cammini:
	for(i=0; i<budget; i++) tmp[i]=list::createEmpty();
	
	// calcola tanti cammini quanto è il budget (se coverage raggiunge 100% interrompe):
	for(i=0; i<budget; i++){
		if(cg->crossed_edges==cg->edges) break;
try{cfg_u::randomWalk(cg->g, tmp[i], cg->start, cg->end, cg->crossed_edges);}
catch(cfg_u::ERROR n){
	if(n==cfg_u::STACK_LIMIT_REACHED){
		list::clear(tmp[i]);
		i--;
	}
	else throw n;
}
	}
	// numero dei cammini totali:
	cg->total_paths = i;
	
	if(cg->total_paths!=budget){
		// crea lista finale:
		tmp1 = new list::List[cg->total_paths];
		for(i=0; i<cg->total_paths; i++) tmp1[i]=list::createEmpty();
		// copia lista:
		for(i=0; i<cg->total_paths; i++) tmp1[i]=tmp[i];
		delete[] tmp;
		cg->paths = tmp1;
	}
	else cg->paths=tmp;
}

/*	In questo sorgente il campo "weight" del TDD GRAPH viene utilizzato per tenere traccia degli archi già attraversati(da vero genovese non butto via nulla);
*	quando viene creato il grafo tramite la funzione compute_graph, questa assegna ad ogni arco il valore 0, mentre la funzione randomWalk modifica tale campo
*	in 1, questa funzione è utile nel momento in cui l'utente volesse nuovamente calcolare dei cammini;
*/
void cfg::reset_paths(cfg_type& cg){
	// set all edges to uncrossed (0);
	cfg_u::reinit_edges(cg->g, cg->start);
	// delete all computed path:
	for(int i=0; i<cg->total_paths; i++) list::clear(cg->paths[i]);
	delete[] cg->paths;
}

// stampa il grafo ottenuto:
void cfg::display_graph(const cfg_type& cg){
	cout<<"\nComputed graph:\n\n";
	printGraph(cg->g);
}

// stampa tutti i cammini calcolati:
void cfg::display_paths(const cfg_type& cg){
	cout<<"\nComputed paths:\n\n";
	for(int i=0; i<cg->total_paths; i++){
		cout<<"\e[1;94mpath(\e[0m"<<i<<"\e[1;94m): \e[0m";
		printList(cg->paths[i]);
	}
}

/* 	stampa le informazioni sul grafo, quali:
*	-numero di vertici;
*	-numero di archi;
*	-coverage ottenuto;
*/
void cfg::display_graph_info(const cfg_type& cg){
	cout<<"\nComputed graph has:\n\n";
	cout<<cg->vertices<<" vertices;\n";
	cout<<cg->edges<<" edges\n";
	cout<<"Coverage is: "<<setprecision(3)<<coverage(cg)<<" %\n";
}

// calcola il coverage:
float coverage(const cfg_type& cg){
	return static_cast<float>(cg->crossed_edges)/static_cast<float>(cg->edges)*100;
}