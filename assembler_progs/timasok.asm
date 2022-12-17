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

;calling function input
in
pop [rax+0]
push [rax+0] ;x
push 3
POW
pop [rax+1]
;calling function print
push [rax+1]
out

push rax
push 32
sub
pop rax

ret
