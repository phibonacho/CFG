/*
Autore:				Andrea Storace
Ultima modifica:	16/06/2017
Descrizione:		Libreria principale del tipo di dato CONTROL FLOW GRAPH;
*/
#ifndef CFG
#define CFG

#include "cfg_utlity.h"
#include <iomanip>
using namespace std;	

namespace cfg{
	// struttura del control flow graph:
	typedef struct cfg_struct* cfg_type;
	
	// costante cfg vuoto:
	const cfg_type emptyCfg = NULL;

	// costruttori:
	cfg_type createEmpty();
	void initialize(string, cfg_type&, int);
	
	// reset dei cammini calcolati:
	void reset_paths(cfg_type&);

	// funzioni informative:
	void display_graph(const cfg_type&);
	void display_paths(const cfg_type&);
	void display_graph_info(const cfg_type&);

	float coverage(const cfg_type&);
}
#endif