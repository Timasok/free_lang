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
push [var_in_def+0] ;y
push 3
SUB
ABS
pop [var_in_def+1]
push [var_in_def+2] ;print
pop [var_in_def+1]
push [var_in_def+1] ;x

push var_in_def
push 32
sub
pop var_in_def

ret
