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
;calling function fly
push 1
pop [rbx+0]
call :fly

push rax
push 32
sub
pop rax

ret

fly:

push rax
push 32
add
pop rax

push [rbx+0]
pop [rax+0]
push [rax+0] ;z
push 1000
DIV
pop [rax+0]
;calling function print
push [rax+0]
out

push rax
push 32
sub
pop rax

ret
