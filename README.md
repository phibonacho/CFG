# CFG
Bad Implementation of a Control Flow Graph. Obviously.

This new repository rely on [Graph](https://github.com/rollingflamingo/Graph) repository, and it try to implement a very **very** **VERY** simple [control flow graph](https://en.wikipedia.org/wiki/Control_flow_graph) for programs written in the tiny language showned below:
```
Program ::= Label ‘END’ | StmtList Label ‘END’
StmtList ::= LabelledStmt | LabelledStmt StmtList
LabelledStmt ::= Label Stmt
Stmt ::= AssignStmt | IfStmt
AssignStmt ::= Var ‘=’ Expr
IfStmt ::= ‘IF’ ‘(’BoolExpr‘)’ AssignStmt | ‘IF’ ‘(’BoolExpr‘)’ GotoStmt
GotoStmt ::= ‘GOTO’ Label
Expr ::= Var | Num | Expr ‘+’ Expr
BoolExpr ::= Var ‘==’ Var | Var ‘==’ Num | Num ‘==’ Num
Var ::= ‘a’ | ... | ‘z’
```
