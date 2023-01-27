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

push 1
pop [var_in_def+0]
while_0:
push 0
clean
push 10
jbe :end_while_0
;calling function print
push [var_in_def+0]
out
push [var_in_def+0] ;x
push 1
ADD
pop [var_in_def+0]
jmp :while_0
end_while_0:

push var_in_def
push 32
sub
pop var_in_def

ret
