#include "cfg_utlity.h"
using namespace cfg_u;

int main(){
	string start, end;
	list::List path=list::createEmpty();
	int len;
	cout<<"Computing graph:";
	Graph g = compute_cfg("sample.txt", start, end);
	cout<<"done\nComputing path: ";
	graph::findPath(start, end, path, len, g);
	cout<<"done\nPath: "<<start<<", ";
	printList(path);
	return 0;
} 