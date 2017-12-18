M=main
L=DLL
G=graph
CU=cfg_utility
C=cfg
CL = $(OD)/libcfg.a
CFLAGS = -ansi -Wall -std=c++11 -o
OFLAGS = -Ilib -c $(CFLAGS)
LDFLAGS = -L$(OD) -lcfg -lrt
E=executable
OD= objs
BD = bin
LD = lib
OBJS = $(OD)/$(L).o $(OD)/$(G).o $(OD)/$(CU).o $(OD)/$(C).o $(OD)/$(M).o

# Common library
$(CL): $(OBJS) | $(OD)
	ar rcs $@ $(OD)/*.o

# Object files:
$(OD)/$(L).o: $(LD)/$(L).cpp $(LD)/$(L).h | $(OD)
	g++ $< $(OFLAGS) $@

$(OD)/$(G).o: $(LD)/$(G).cpp $(LD)/$(G).h | $(OD)
	g++ $< $(OFLAGS) $@	

$(OD)/$(CU).o: $(LD)/$(CU).cpp $(LD)/$(CU).h | $(OD)
	g++ $< $(OFLAGS) $@

$(OD)/$(C).o: $(LD)/$(C).cpp $(LD)/$(C).h | $(OD)
	g++ $< $(OFLAGS) $@

# Main object file:
$(OD)/$(M).o: $(M).cpp | $(OD) 
	g++ $< $(OFLAGS) $@

# Executable:	
$(BD)/$(E).out: $(CL) | $(BD) 
	g++ -o $@ $(LDFLAGS)

$(BD):
	mkdir -p $(BD)

$(OD):
	mkdir -p $(OD)

demo: $(BD)/$(E).out
	@cat README.md | less
	@./$(BD)/$(E).out samples/sample.txt 10 | less -R

clean:
	rm -rf $(OD) $(BD) $(L)$(R) $(G)$(R) $(CU)$(R) $(C)$(R) compilation$(R) 
