push 1032
pop var_in_funccall
push 1064
pop var_in_def
call :main 
hlt

main:

push var_in_def
push 32
add
pop var_in_def

;calling function print_line
push 32
push 83
push 70
push 70
push 69
push 79
push 67
push 32
push 69
push 82
push 65
push 85
push 81
push 83
push 32
push 82
push 69
push 84
push 78
push 69
pop [0]
pop [1]
pop [2]
pop [3]
pop [4]
pop [5]
pop [6]
pop [7]
pop [8]
pop [9]
pop [10]
pop [11]
pop [12]
pop [13]
pop [14]
pop [15]
pop [16]
pop [17]
pop [18]
pop [19]
show
;calling function input
in
pop [var_in_def+3]
;calling function input
in
pop [var_in_def+4]
;calling function input
in
pop [var_in_def+5]
push 0
push [var_in_def+3] ;a
je :else_0
;calling function solve_square
push [var_in_def+3]
pop [var_in_funccall+0]
push [var_in_def+4]
pop [var_in_funccall+1]
push [var_in_def+5]
pop [var_in_funccall+2]
call :solve_square
jmp :endif_0
else_0:
;calling function solve_linear
push [var_in_def+4]
pop [var_in_funccall+0]
push [var_in_def+5]
pop [var_in_funccall+1]
call :solve_linear
endif_0:

push var_in_def
push 32
sub
pop var_in_def

ret

solve_square:

push var_in_def
push 32
add
pop var_in_def

push [var_in_funccall+0]
push [var_in_funccall+1]
push [var_in_funccall+2]
pop [var_in_def+2]
pop [var_in_def+1]
pop [var_in_def+0]
push [var_in_def+1] ;b
push [var_in_def+1] ;b
MUL
push 4
push [var_in_def+0] ;a
MUL
push [var_in_def+2] ;c
MUL
SUB
pop [var_in_def+3]
push 0
push [var_in_def+3] ;D
je :else_1
push 0
clean
push [var_in_def+3] ;D
push 0
jbe :else_2
push 0
push [var_in_def+3] ;D
SQRT
push [var_in_def+1] ;b
SUB
push 2
push [var_in_def+0] ;a
MUL
DIV
pop [var_in_def+4]
push 0
push 0
push [var_in_def+3] ;D
SQRT
SUB
push [var_in_def+1] ;b
SUB
push 2
push [var_in_def+0] ;a
MUL
DIV
pop [var_in_def+5]
;calling function print_line
push 32
push 69
push 82
push 65
push 32
push 83
push 84
push 79
push 79
push 82
pop [0]
pop [1]
pop [2]
pop [3]
pop [4]
pop [5]
pop [6]
pop [7]
pop [8]
pop [9]
show
;calling function print
push [var_in_def+4]
out
push [var_in_def+5]
out
jmp :endif_2
else_2:
;calling function print_line
push 32
push 83
push 84
push 79
push 79
push 82
push 32
push 79
push 78
pop [0]
pop [1]
pop [2]
pop [3]
pop [4]
pop [5]
pop [6]
pop [7]
pop [8]
show
endif_2:
jmp :endif_1
else_1:
push 0
push [var_in_def+1] ;b
SUB
push 2
push [var_in_def+0] ;a
MUL
DIV
pop [var_in_def+10]
;calling function print_line
push 32
push 83
push 73
push 32
push 84
push 79
push 79
push 82
pop [0]
pop [1]
pop [2]
pop [3]
pop [4]
pop [5]
pop [6]
pop [7]
show
;calling function print
push [var_in_def+10]
out
endif_1:

push var_in_def
push 32
sub
pop var_in_def

ret

solve_linear:

push var_in_def
push 32
add
pop var_in_def

push [var_in_funccall+0]
push [var_in_funccall+1]
pop [var_in_def+1]
pop [var_in_def+0]
push 0
push [var_in_def+1] ;b
je :else_3
push 0
push [var_in_def+0] ;a
je :else_4
push 0
push [var_in_def+1] ;b
SUB
push [var_in_def+0] ;a
DIV
pop [var_in_def+2]
;calling function print_line
push 32
push 83
push 73
push 32
push 84
push 79
push 79
push 82
pop [0]
pop [1]
pop [2]
pop [3]
pop [4]
pop [5]
pop [6]
pop [7]
show
;calling function print
push [var_in_def+2]
out
jmp :endif_4
else_4:
;calling function print_line
push 32
push 83
push 84
push 79
push 79
push 82
push 32
push 79
push 78
pop [0]
pop [1]
pop [2]
pop [3]
pop [4]
pop [5]
pop [6]
pop [7]
pop [8]
show
endif_4:
jmp :endif_3
else_3:
push 0
push [var_in_def+0] ;a
je :else_5
push 0
pop [var_in_def+2]
;calling function print_line
push 32
push 83
push 73
push 32
push 84
push 79
push 79
push 82
pop [0]
pop [1]
pop [2]
pop [3]
pop [4]
pop [5]
pop [6]
pop [7]
show
;calling function print
push [var_in_def+2]
out
jmp :endif_5
else_5:
;calling function print_line
push 32
push 83
push 84
push 79
push 79
push 82
push 32
push 70
push 78
push 73
pop [0]
pop [1]
pop [2]
pop [3]
pop [4]
pop [5]
pop [6]
pop [7]
pop [8]
pop [9]
show
endif_5:
endif_3:

push var_in_def
push 32
sub
pop var_in_def

ret
