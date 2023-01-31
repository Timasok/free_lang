push 1032
pop var_in_funccall
push 1096
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
in
pop [var_in_def+1]
;calling function ackerman
push [var_in_def+0]
pop [var_in_funccall+0]
push [var_in_def+1]
pop [var_in_funccall+1]
call :ackerman
push ret_value
pop [var_in_def+2]
;calling function print
push [var_in_def+2]
out

push var_in_def
push 32
sub
pop var_in_def

ret

ackerman:

push var_in_def
push 32
add
pop var_in_def

push [var_in_funccall+0]
push [var_in_funccall+1]
pop [var_in_def+1]
pop [var_in_def+0]
push 0
push [var_in_def+0] ;n
je :else_0
push 0
push [var_in_def+1] ;m
je :else_1
push [var_in_def+1] ;m
push 1
SUB
pop [var_in_def+2]
;calling function ackerman
push [var_in_def+0]
pop [var_in_funccall+0]
push [var_in_def+2]
pop [var_in_funccall+1]
call :ackerman
push ret_value
pop [var_in_def+2]
push [var_in_def+0] ;n
push 1
SUB
pop [var_in_def+3]
;calling function ackerman
push [var_in_def+3]
pop [var_in_funccall+0]
push [var_in_def+2]
pop [var_in_funccall+1]
call :ackerman
push ret_value
pop [var_in_def+4]
;calling function return
push [var_in_def+4]
pop ret_value

push var_in_def
push 32
sub
pop var_in_def

ret
jmp :endif_1
else_1:
push [var_in_def+0] ;n
push 1
SUB
pop [var_in_def+3]
;calling function ackerman
push [var_in_def+3]
pop [var_in_funccall+0]
push 1
pop [var_in_funccall+1]
call :ackerman
push ret_value
pop [var_in_def+4]
;calling function return
push [var_in_def+4]
pop ret_value

push var_in_def
push 32
sub
pop var_in_def

ret
endif_1:
jmp :endif_0
else_0:
push [var_in_def+1] ;m
push 1
ADD
pop [var_in_def+4]
;calling function return
push [var_in_def+4]
pop ret_value

push var_in_def
push 32
sub
pop var_in_def

ret
endif_0:

push var_in_def
push 32
sub
pop var_in_def

ret
