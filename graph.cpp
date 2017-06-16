#include "graph.h"

using namespace graph;

typedef struct edgeNode{
	//peso della cella;
	int weight;
	//puntatore al nodo di destinazione;
	vertexNode* dest;
	//puntatore alla prossima cella di adiacenza;
	edgeNode* nextEdge;
}* adjList;

const adjList emptyEdge = NULL;

struct graph::vertexNode{
	//nome del nodo;
	Label payload;
	//puntatore alla lista di adiacenza;
	adjList edges;
	//puntatore al nodo successivo;
	vertexNode* nextVertex;
	//
	bool visited;
};
/*******************************************************************************************************/
// Grafo
/*******************************************************************************************************/

/*Helper functions*/
bool memberNode(Label , const Graph&);
void reinit(Graph&);
Graph getVertex(Label , const Graph&);
int vertexIndex(Label, const Graph&);
Label* dijkstra(Graph&, Label, Label, int&);
int computeLenght(const list::List&, const Graph&, int);
void findPath_aux(Graph&, Label, Label, Label*, list::List&);
void printAdj(const adjList&);

// Restituisce il grafo vuoto
Graph graph::createEmptyGraph(){
  return emptyGraph;
}

// Aggiunge nuovo vertice con etichetta la stringa. Fallisce se gia' presente
bool graph::addVertex(Label l, Graph& g) {
	if(memberNode(l, g))return false;
	Graph tmp = new vertexNode;
	tmp->payload = l;
	tmp->edges = emptyEdge;
	tmp->nextVertex = g;
	tmp->visited = false;
	g = tmp;
	return true;
}

// Aggiunge un arco di peso "w" tra i nodi con etichetta "f" e "t". Fallisce se esiste gia' l'arco
// se i nodi non esistono nel grafo e se si tenta di inserire un arco tra un nodo ed esso stesso
bool graph::addEdge(Label from, Label to, Weight w, Graph& g) {
	if(from==to) return false;
	Graph vertexfrom = getVertex(from, g);
	Graph vertexto = getVertex(to, g);
	if(vertexfrom==emptyGraph || vertexto == emptyGraph) return false;
	//control of vertexfrom's adjacentment list:
	for(adjList tmp = vertexfrom->edges; tmp!=emptyEdge ; tmp = tmp->nextEdge) if(tmp->dest==vertexto) return false;
	//after all these damn controls, add the fucking edge:
	adjList aux = new edgeNode;
	aux->weight = w;
	aux->dest = vertexto;
	aux->nextEdge = vertexfrom->edges;
	vertexfrom->edges = aux;
#ifdef UNORIENTED
	//and then the other one:
	aux = new edgeNode;
	aux->weight = w;
	aux->dest = vertexfrom;
	aux->nextEdge = vertexto->edges;
	vertexto->edges = aux;
#endif
	return true;
}

// Restituisce true se il grafo e' vuoto, false altrimenti
bool graph::isEmpty(const Graph& g){
	return g==emptyGraph;
}

// Ritorna il numero di vertici del grafo
int graph::numVertices(const Graph& g){
 	if(g==emptyGraph)return 0;
 	return 1+numVertices(g->nextVertex);
}

// Ritorna il numero di archi del grafo
int graph::numEdges(const Graph& g){
	int numEdges = 0;
	for(Graph i = g; i!=emptyGraph; i=i->nextVertex)
		for(adjList j =i->edges; j!=emptyEdge; j=j->nextEdge) numEdges++;
#ifdef UNORIENTED
	return numEdges/2;
#else
	return numEdges;
#endif
}

// Calcola e ritorna (nel secondo parametro) il grado del nodo. Fallisce
// se il nodo non esiste
bool graph::nodeDegree(Label l, int& degree, const Graph& g) {
	Graph v = getVertex(l, g);
	degree = -1;
	if(v==emptyGraph) return false;
	degree++;
	for(adjList tmp=v->edges; tmp!=emptyEdge; tmp=tmp->nextEdge, degree++);
	return true;
}

// Verifica se i due vertici v1 e v2 sono adiacenti (ovvero se esiste un arco)
bool graph::areAdjacent(Label v1, Label v2, const Graph& g) {
	Graph vertex1 = getVertex(v1, g);
	Graph vertex2 = getVertex(v2, g);
	if(vertex1==emptyGraph || vertex2==emptyGraph) return false;
	for(adjList tmp = vertex1->edges; tmp!= emptyEdge; tmp = tmp->nextEdge) if(tmp->dest==vertex2) return true;
	return false;
}

// Restituisce la lista di adiacenza di un vertice
list::List graph::adjacentList(Label v1, const Graph& g) {
	adjList tmp;
	Graph vertex1 = getVertex(v1, g);
	list::List lst = list::createEmpty();
	for(tmp=vertex1->edges; tmp!=emptyEdge; tmp=tmp->nextEdge) list::addBack(lst, tmp->dest->payload);
	return lst;
}

// Ritorna un cammino tra una citta' ed un altra
// Il cammino da "v1" a "v2" alla fine sara' in "path"
// e la lunghezza sara' in "len".
// Si assume che il chiamante fornisca inizialmente un cammino vuoto.
//
// La funzione rappresenta una variante della visita DFS

void graph::findPath(Label v1, Label v2, list::List &path, int &len, const Graph& g) {
	Graph tmp = g;
	Label* prev = dijkstra(tmp, v1, v2, len);
	findPath_aux(tmp, v1, v2, prev, path);
	delete[] prev;
}

/*******************************************************************************************************/
// Stampa il grafo

void printGraph(const graph::Graph& g) {
	if(g==emptyGraph) return;
	cout<<g->payload<<": ";
	printAdj(g->edges);
	cout<<endl;
	return printGraph(g->nextVertex);
}

bool isCrossed(Label v1, Label v2, const Graph& g){
	Graph tmp = getVertex(v1, g);
	adjList aux;
	for(aux=tmp->edges; aux!=emptyEdge; aux=aux->nextEdge) if((aux->dest)->payload==v2) return aux->weight;
	return false;
}

void set_Crossed(Label v1, Label v2,const graph::Graph& g){
	graph::Graph tmp = getVertex(v1, g);
	adjList aux;
	for(aux=tmp->edges; aux!=emptyEdge; aux=aux->nextEdge) if((aux->dest)->payload==v2) break;
	aux->weight=1;	
}

void set_uncrossed(graph::Label v1, graph::Label v2, const graph::Graph& g){
	graph::Graph tmp = getVertex(v1, g);
	adjList aux;
	for(aux=tmp->edges; aux!=emptyEdge; aux=aux->nextEdge) if((aux->dest)->payload==v2) break;
	aux->weight=0;
}

/***************************************FUNZIONI AUSILIARIE******************************/
bool memberNode(Label l, const Graph& g){
	if(g==emptyGraph)return false;
	if(g->payload==l) return true;
	return memberNode(l, g->nextVertex);
}

void printAdj(const adjList& e){
	if(e==emptyEdge){
		cout<<"\b\b.";
		return;	
	} 
	cout<<e->dest->payload;
#ifdef SHOW_WEIGHT
	cout<<"("<<e->weight<<")";
#endif
	cout<<", ";
	return printAdj(e->nextEdge);
}


void reinit(Graph& g){
	if(g==emptyGraph) return;
	g->visited = false;
	return reinit(g->nextVertex);
}

Graph getVertex(Label l, const Graph& g){
	if(g==emptyGraph) return emptyGraph;
	if(g->payload == l) return g;
	return getVertex(l, g->nextVertex);
}

int vertexIndex(Label v1, const Graph& g){
	int count = 0;
	for(Graph tmp=g; tmp!=emptyGraph; tmp=tmp->nextVertex, count++) if(tmp->payload==v1) return count;
	count = -1;
	return count;
}

Label* dijkstra(Graph& g, Label from, Label to, int& len){
	reinit(g);
	//creazione dell'array di dimensione del numero di nodi:
	Label temp_Label;
	int dsize = numVertices(g), index, temp_len;
	int* distance = new int[dsize];
	Label* previous = new Label[dsize];
	//creazione della lista per le chiamate in ampiezza:
	list::List queue = list::createEmpty();
	//aggiungo l'etichetta di partenza:
	list::addBack(queue, from);
	//inizializzo tutte le distanze al massimo:
	for(int i = 0; i<dsize; i++) distance[i] = INT_MAX;
	//pongo la distanza del nodo di partenza a zero;
	index = vertexIndex(from, g);
	if(index!=-1) distance[index] = 0;
	previous[index] = "";
	do{
		//ottengo label di cui fare la visita:
		temp_Label = list::get(0, queue);
		//rimuovo il nodo dalla lista:
		list::popFront(queue);
		//chiamo sul nodo:
		Graph aux = getVertex(temp_Label, g);
		//lo marco come visitato:
		aux->visited = true;
		//aggiorno la lunghezza corrente(quella fino al nodo che sto visitando):
		index = vertexIndex(temp_Label, g);
		temp_len = distance[index];
		//per ogni elemento della lista di adiacenza:
		for(adjList tmp=aux->edges;tmp!=emptyEdge; tmp=tmp->nextEdge){
			//se l'elemento non è stato visitato:
			if(!tmp->dest->visited){
				//lo aggiungo alla lista per la prossima chiamata:
				list::addBack(queue, (tmp->dest)->payload);
				//lo marco come visitato:
				tmp->dest->visited = true;
			}
			index = vertexIndex(tmp->dest->payload, g);
			if(distance[index]>temp_len+tmp->weight){
				distance[index] = temp_len+tmp->weight;
				previous[index] = aux->payload;
			}
		}
	}while(!list::isEmpty(queue));
	index = vertexIndex(to, g);
	len = distance[index];
	delete[] distance;
	return previous;
}

void findPath_aux(Graph& g, Label from, Label to, Label* previous, list::List& path){
	int tmp_index;
	Label temp_label=to;
	while(true){
		if(temp_label==from) return;
		list::addFront(path, temp_label);
		tmp_index = vertexIndex(temp_label, g);
		temp_label = previous[tmp_index];
	}
}