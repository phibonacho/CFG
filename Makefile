I=instruction
L=DLL
G=graph
CU=cfg_utlity
C=cfg
$(I).out: $(I).cpp $(G)* $(L)* $(C)* $(CU)*
	g++ -std=c++11 -o $(I).out -ansi -Wall $(I).cpp $(G).cpp $(L).cpp $(C).cpp $(CU).cpp 2> compilation_result.txt