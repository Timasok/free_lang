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
;calling function tupok
push [var_in_def+0]
pop [var_in_funccall+0]
call :tupok
push ret_value
pop [var_in_def+1]
;calling function print
push [var_in_def+1]
out

push var_in_def
push 32
sub
pop var_in_def

ret

tupok:

push var_in_def
push 32
add
pop var_in_def

push [var_in_funccall+0]
pop [var_in_def+0]
push [var_in_def+0] ;x
push 1
ADD
pop [var_in_def+0]
;calling function return
push [var_in_def+0]
pop ret_value

push var_in_def
push 32
sub
pop var_in_def

ret
