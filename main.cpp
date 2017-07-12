#include <stdlib.h>
#include <iomanip>
#include "cfg.h"

using namespace std;
using namespace cfg;

int main(int argc, char* argv[]){
	string filename;
	int budget=0;
	switch(argc){
		case 3:
			budget=atoi(argv[2]);
		case 2:
			filename = argv[1];
			break;
		default:
			cerr<<"filename field missing, ending program.\a\n";
			return 1;
			break;
	}
try{
	srand(time(NULL));
	if(budget==0){
		cout<<"define budget: \e[95m";
		cin>>budget;
	}
	cout<<"\e[0m";
	
	cfg_type new_graph = createEmpty();
	cout<<"initializing graph: ";
	initialize(filename, new_graph);
	cout<<"\e[1;91mdone\e[0m\n";

	cout<<"computing paths: ";
	compute_paths(new_graph, budget);
	cout<<"\e[1;91mdone\e[0m\n";	
	
	display_graph(new_graph);
	display_paths(new_graph);
	display_graph_info(new_graph);
	reset_paths(new_graph);
}
catch(cfg_u::ERROR n){
	cout<<"\e[1;93mFAILED\e[0m\n";
	switch(n){
		case cfg_u::NOT_LINE_LABEL:
			cerr<<"\e[1;91mThe program terminated due to a GOTO statement followed by non-numerical label, or a line starting with a non-numerica label\e[0m\n\a";
			break;
		case cfg_u::MISSING_IF_CONDITIONS:
			cerr<<"\e[1;91mThe program terminated due to a IF statement without condition\e[0m\n\a";
			break;
		case cfg_u::GOTO_STOP_FLOW: 
			cerr<<"\e[1;91mThe programs terminated due to a GOTO statement that leads to a non-existent node or cuts off the end node\e[0m\n\a";
			break;
		case cfg_u::EMPTY_SOURCE_CODE:
			cerr<<"\e[1;91mThe program terminated due to an empty or non-existent source code\e[0m\n\a";
			break;			
		case cfg_u::BLACKMAGIC_OCCURRED:
			cerr<<"\e[1;91mThis is a shame, but I don't actually know what happened, sorry...\e[0m\n";
	}
	return 1;
}
	return 0;
}