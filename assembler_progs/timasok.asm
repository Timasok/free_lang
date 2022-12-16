push 128
pop rbx
push 256
pop rax
call :main 
hlt

main:

push rax
push 32
add
pop rax

push 0
pop [rax+0]
push 1
pop [rax+0]
;calling function print
pop [rax+0]
dup
push [rax+0]
out
push 0
push [rax+0] ;x
push 1
SUB
je :else_0
;calling function print
push 1
out
else_0:
;calling function print
push 2
out

push rax
push 32
sub
pop rax

ret
