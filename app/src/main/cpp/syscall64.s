    .text
    .global mySyscall
    .type mySyscall,@function

mySyscall:
    MOV x8, x0
    MOV x0 ,x1
    MOV x1 ,x2
    MOV x2 ,x3
    MOV x3 ,x4
    MOV x4 ,x5
    MOV x5 ,x6
    SVC 0
    RET