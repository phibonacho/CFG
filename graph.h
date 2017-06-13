#ifndef GRAPH
#define GRAPH

//uncomment the following line to implement unoriented graph:
//#define UNORIENTED

#include <cstddef>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector> 
#include <fstream> 
#include <string>         
#include <limits.h>

#include "DLL.h"

using namespace std;

namespace graph {

typedef string Label;
typedef int Weight;

struct vertexNode; // da definire nel file graph.cpp

typedef vertexNode* Graph; // un grafo e' identificato dal puntatore al primo vertice inserito

const Graph emptyGraph = NULL; 

// createEmptyGraph restituisce il grafo vuoto 
Graph createEmptyGraph();

// Aggiunge nuovo vertice con etichetta la stringa. Fallisce se gia' presente
bool addVertex(Label, Graph&);

// Aggiunge nuovo arco tra i due nodi con etichette le due stringe e peso
// l'intero. Fallisce se non sono presenti tutti e due i nodi o se l'arco
// tra i due e' gia' presente.
bool addEdge(Label, Label, Weight, Graph&);

// Restituisce true se il grafo e' vuoto, false altrimenti
bool isEmpty(const Graph&);

// Ritorna il numero di vertici del grafo
int numVertices(const Graph&);

// Ritorna il numero di archi del grafo
int numEdges(const Graph&);

// Calcola e ritorna (nel secondo parametro) il grado del nodo.
// Fallisce se il nodo non esiste
bool nodeDegree(Label, int&, const Graph&);

// Verifica se due vertici sono adiacenti (ovvero se esiste un arco)
bool areAdjacent(Label, Label, const Graph&); 

// Ritorna la lista di adiacenza di un vertice
list::List adjacentList(Label, const Graph&);

// Calcola, se esiste, un cammino tra due vertici
void findPath(Label, Label, list::List &, int &, const Graph& g);

// Calcola, se esiste, un cammino tra due vertici(in modo casuale)
void randomWalk(Label, Label, list::List&, const Graph&);
}


/* Funzioni che non caratterizzano il TDD Graph, ma che servono per input/output */
void printGraph(const graph::Graph&);
#endif