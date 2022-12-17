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
push 0
COS
pop [rax+0]
push 10
pop [rax+1]
push 15
pop [rax+2]
;calling function yo
push [rax+0]
pop [rbx+0]
push [rax+1]
pop [rbx+1]
push [rax+2]
pop [rbx+2]
call :yo

push rax
push 32
sub
pop rax

ret

yo:

push rax
push 32
add
pop rax

push [rbx+0]
push [rbx+1]
push [rbx+2]
pop [rax+2]
pop [rax+1]
pop [rax+0]
;calling function print
push [rax+2]
out
push [rax+1]
out
push [rax+0]
out

push rax
push 32
sub
pop rax

ret
