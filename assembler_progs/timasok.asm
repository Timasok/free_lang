jmp :main 
hlt

main:
push 0
push 0
pop [var_in_def+1]
push [var_in_def+0]
push 1
ADD
push 5
push [var_in_def+1]
;calling function foo
pop [var_in_def+1]
dup
push [var_in_def+1]
push [var_in_funccall+0]
call :foo
ret

foo:
push [var_in_funccall+0]
push 0
pop [var_in_def+1]
push [var_in_def+0]
push 1
ADD
ret
