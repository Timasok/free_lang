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

;calling function akkerman
push 2
pop [var_in_funccall+0]
push 3
pop [var_in_funccall+1]
call :akkerman
push ret_value
pop [var_in_def+0]
;calling function print
push [var_in_def+0]
out

push var_in_def
push 32
sub
pop var_in_def

ret

akkerman:

push var_in_def
push 32
add
pop var_in_def

push [var_in_funccall+0]
push [var_in_funccall+1]
pop [var_in_def+1]
pop [var_in_def+0]
push 0
clean
push [var_in_def+0] ;n
push 0
jaejbe :else_0
;calling function print_line
push 32
push 114
push 111
push 114
push 114
push 101
pop [0]
pop [1]
pop [2]
pop [3]
pop [4]
pop [5]
show
jmp :endif_0
else_0:
endif_0:
push 0
clean
push [var_in_def+1] ;m
push 0
jaejbe :else_1
;calling function print_line
push 32
push 114
push 111
push 114
push 114
push 101
pop [0]
pop [1]
pop [2]
pop [3]
pop [4]
pop [5]
show
jmp :endif_1
else_1:
endif_1:
push 0
push [var_in_def+0] ;n
je :else_2
jmp :endif_2
else_2:
push [var_in_def+1] ;m
push 1
ADD
pop [var_in_def+3]
;calling function return
push [var_in_def+3]
pop ret_value
ret
endif_2:
push 0
push [var_in_def+1] ;m
je :else_3
jmp :endif_3
else_3:
;calling function akkerman
pop [var_in_funccall+0]
push 1
pop [var_in_funccall+1]
call :akkerman
push ret_value
pop [var_in_def+4]
;calling function return
push [var_in_def+4]
pop ret_value
ret
endif_3:
;calling function akkerman
push [var_in_def+0]
pop [var_in_funccall+0]
pop [var_in_funccall+1]
push [var_in_def+1]
pop [var_in_funccall+2]
call :akkerman
push ret_value
pop [var_in_def+3]
;calling function akkerman
push [var_in_def+0]
pop [var_in_funccall+0]
push [var_in_def+3]
pop [var_in_funccall+1]
call :akkerman
push ret_value
pop [var_in_def+4]
;calling function return
push [var_in_def+4]
pop ret_value
ret

push var_in_def
push 32
sub
pop var_in_def

ret
