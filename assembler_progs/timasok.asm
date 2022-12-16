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
push [rax+0] ;x
;calling function why
pop [rax+0]
dup
push [rax+0]
push [rbx+0]
call :why

push rax
push 32
sub
pop rax

ret

why:

push rax
push 32
add
pop rax

push 0
pop [rax+0]
push 9
pop [rax+0]

push rax
push 32
sub
pop rax

ret
