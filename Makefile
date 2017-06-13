I=instruction
L=DLL
G=graph
$(I).out: $(I).cpp $(G)* $(L)*
	g++ -std=c++11 -o $(I).out -ansi -Wall $(I).cpp $(G).cpp $(L).cpp 