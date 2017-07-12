M=main
L=DLL
G=graph
CU=cfg_utility
C=cfg
R=_result.txt
E=executable
ZNAME=StoraceA
DIRNAME=Lab10

$(L).o: $(L).cpp $(L).h
	g++ -c $(L).cpp -ansi -Wall -std=c++11 2>$(L)$(R)

$(G).o: $(G).h $(G).cpp
	g++ -c $(G).cpp -ansi -Wall -std=c++11 2>$(G)$(R)	

$(CU).o: $(CU).h $(CU).cpp
	g++ -c $(CU).cpp -ansi -Wall -std=c++11 2>$(CU)$(R)

$(C).o: $(C).h $(C).cpp
	g++ -c $(C).cpp -ansi -Wall -std=c++11 2>$(C)$(R)

$(E).out: $(M).cpp $(C).o $(G).o $(L).o $(CU).o
	@cat README.md | less
	g++ $(M).cpp $(C).o $(CU).o $(G).o $(L).o -o $(E).out -ansi -Wall -std=c++11 2> compilation$(CR)

launch: $(E).out
	@./$(E).out ${FILE_NAME} ${BUDGET} | less -R

clean :
	/bin/rm -f *.o *.out $(L)$(R) $(G)$(R) $(CU)$(R) $(C)$(R) compilation$(R) 
