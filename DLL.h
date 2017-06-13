#ifndef DLL
#define DLL
#include <iostream>
#include <string>
using namespace std;
/*
Libreria che implementa funzioni sulle Liste doppiamente collegate di Elem;
*/
namespace list{
	typedef string Elem;
	
	enum EXCEPTIONS {lenghtException, sizeException, alreadysetException, clearException};

	//struttura delle celle:
	struct Cell{
		Elem elem;
		Cell* next;
		Cell* prev;
	};

	typedef Cell* List;
	
	const Elem emptyElem = "$#$#$";
	
	//crea Lista vuota:
	List createEmpty();
	//aggiunge in cima:
	void addFront(List&, const Elem&);
	//aggiunge in coda:
	void addBack(List&, const Elem&);
	//aggiunge nella posizione n:
	void add(List&, const Elem&, unsigned int);
	//modifica elemento nella posizione data:
	void set(unsigned int, const Elem&, List&);
	//ottiene elemento in posizione n:
	Elem get(unsigned int, const List&);
	//elimina elemnto in cima:
	void popFront(List&);
	//elimina elemento in coda:
	void popBack(List&);
	//remove elem in position n:
	void removePos(List&, unsigned int);
	//verifica se la Lista è vuota:
	bool isEmpty(const List&);
	//return list size:
	unsigned int size(const List&);
	//destroy list:
	void clear(List&);
}

void printList(const list::List&);
#endif