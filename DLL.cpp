#include "DLL.h"
using namespace list;
/*
namespace dll{
	//struttura delle Celle:
	struct Cell{
		Elem elem;
		Cell* next;
		Cell* prev;
	};

	typedef Cell* List;

	//crea Lista vuota:
	List create_empty();
	//aggiunge in cima:
	void pushFront(List&, const Elem&);
	//aggiunge in coda:
	void pushBack(List&, const Elem&);
	//elimina elemnto in cima:
	Elem popFront(List&);
	//aggiunge nella posizione n:
	void insert_at(List&, const Elem&, unsigned int);
	//elimina elemento in coda:
	Elem popBack(List&);
	//restituisce elemento in cima:
	Elem top(List&);
	//verifica se la Lista è vuota:
	bool isEmpty(List&);
}
*/

bool member(const List& l, const Elem& e){
	List tmp = l->next;
	while(tmp!=l){
		if(tmp->elem==e) return true;
		tmp = tmp->next;
	}
	return false;
}

List list::createEmpty(){
	List aux = new Cell;
	aux->next = aux;
	aux->prev = aux;
	return aux;
}

void list::addFront(List& l, const Elem& e){
	if(member(l, e)) return;//throw alreadysetException;
	List aux = new Cell;
	aux->elem = emptyElem;
	aux->elem = e;
	aux->prev = l;
	aux->next = l->next;
	(l->next)->prev = aux;
	l->next = aux;
}

void list::addBack(List& l, const Elem& e){
	if(member(l, e)) return;//throw alreadysetException;
	List aux = new Cell;
	aux->elem = e;
	aux->next = l;
	aux->prev = l->prev;
	(l->prev)->next = aux;
	l->prev = aux;
		
}

void list::add(List& l, const Elem& e, unsigned int n){
	if(member(l, e)) return;//throw alreadysetException;
	List aux = l->next;
	for(unsigned int i = 0; i<n-1; i++){
		if(aux==l) throw lenghtException;
		aux = aux->next;
	}
	addFront(aux, e);
	return;
}

void list::set(unsigned int pos, const Elem& e,  List& l){
	List tmp = l->next; 
	for(unsigned int i=0; i<pos; i++ ){
		if(tmp==l) throw sizeException;
		tmp=tmp->next;
	}
	tmp->elem = e;
}

Elem list::get(unsigned int pos, const List& l){
	List tmp = l->next; 
	for(unsigned int i=0; i<pos; i++ ){
		if(tmp==l) throw sizeException;
		tmp=tmp->next;
	}
	return tmp->elem;
}

void list::popFront(List& l){
	if(isEmpty(l)) return;
	List aux = l->next;
	l->next = aux->next;
	aux->next->prev = l;
	delete aux;
}

void list::popBack(List& l){
	if(isEmpty(l)) return;
	List aux = l->prev;
	l->prev = aux->prev;
	aux->prev->next = l;
	delete aux;
}

void list::removePos(List& l, unsigned int n){
	List aux = l->next;
	for(unsigned int i = 0; i<n-1; i++){
		if(aux==l) return;
		aux = aux->next;
	}
	return popFront(aux);
}

bool list::isEmpty(const List& l){
	return l->next == l;
}

unsigned int list::size(const List& l){
	List tmp = l->next;
	unsigned int counter;
	for(counter = 0; tmp!=l; counter++) tmp = tmp->next;
	return counter;
}

void list::clear(List& l){
	if(l->next==l) return;
	List tmp = l->next;
	delete l;
	l = NULL;
	return clear(tmp);
}

void printList(const list::List& l){
	for(list::List tmp=l->next; tmp!=l; tmp=tmp->next) cout<<tmp->elem<<", ";
	cout<<"\b\b."<<endl;
}