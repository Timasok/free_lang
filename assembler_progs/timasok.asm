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

;calling function input
in
pop [var_in_def+0]
push 0
clean
push [var_in_def+0] ;y
push 0
jbe :else_0
;calling function print_line
push 32
push 33
push 32
push 78
push 79
push 87
push 32
push 85
push 79
push 89
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
jmp :endif_0
else_0:
endif_0:

push var_in_def
push 32
sub
pop var_in_def

ret
