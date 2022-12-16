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
pop [rax+1]
push 1
pop [rax+1]
push 0
push 1
je :else_0
;calling function print
push 1
out
jmp :endif_0
else_0:
;calling function print
push 2
out
endif_0:

push rax
push 32
sub
pop rax

ret
