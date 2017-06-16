#include "cfg.h"

using namespace cfg;

// definizione della struttura del cfg:
struct cfg::cfg_struct{
	string start, end;
	int vertices, edges, crossed_edges, total_paths;
	graph::Graph g;
	list::List* paths;
};

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
void cfg::initialize(string filename, cfg_type& cg, int budget){
	cfg_type aux = new cfg_struct;
	int i;
	aux->crossed_edges=0;
	aux->g=cfg_u::compute_cfg(filename, aux->start, aux->end);
	aux->vertices=graph::numVertices(aux->g);
	aux->edges=numEdges(aux->g);
	list::List* tmp = new list::List[budget], *tmp1;
	for(i=0; i<budget; i++) tmp[i]=list::createEmpty();
	for(i=0; i<budget; i++){
		if(aux->crossed_edges==aux->edges) break;
		cfg_u::randomWalk(aux->g, tmp[i], aux->start, aux->end, aux->crossed_edges);
	}
	aux->total_paths = i;
	tmp1 = new list::List[aux->total_paths];
	for(i=0; i<aux->total_paths; i++) tmp1[i]=list::createEmpty();
	for(i=0; i<aux->total_paths; i++) tmp1[i]=tmp[i];
	delete[] tmp;
	aux->paths = tmp1;
	cg = aux;
}

/*	In questo sorgente il campo "weight" del TDD GRAPH viene utilizzato per tenere traccia degli archi già attraversati(da vero genovese non butto via nulla);
*	quando viene creato il grafo tramite la funzione compute_graph, questa assegna ad ogni arco il valore 0, mentre la funzione randomWalk modifica tale campo
*	in 1, questa funzione è utile nel momento in cui l'utente volesse nuovamente calcolare dei cammini;
*/
void cfg::reset_paths(cfg_type& cg){
	// set all edges to unncrossed (0);
	cfg_u::reinit_edges(cg->g, cg->start);
	// delete all computed path:
	for(int i=0; i<cg->total_paths; i++) list::clear(cg->paths[i]);
	delete[] cg->paths;
}

// stampa il grafo ottenuto:
void cfg::display_graph(const cfg_type& cg){
	printGraph(cg->g);
}

// stampa tutti i cammini calcolati:
void cfg::display_paths(const cfg_type& cg){
	for(int i=0; i<cg->total_paths; i++){
		cout<<"path("<<i<<"): ";
		printList(cg->paths[i]);
	}
}

/* 	stampa le informazioni sul grafo, quali:
*	-numero di vertici;
*	-numero di archi;
*	-coverage ottenuto;
*/
void cfg::display_graph_info(const cfg_type& cg){
	cout<<"Computed graph has:\n";
	cout<<cg->vertices<<" vertices;\n";
	cout<<cg->edges<<" edges\n";
	cout<<"Coverage is: "<<setprecision(3)<<coverage(cg)<<" %\n";
}

// calcola il coverage:
float cfg::coverage(const cfg_type& cg){
	return static_cast<float>(cg->crossed_edges)/static_cast<float>(cg->edges)*100;
}