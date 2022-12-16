push 1
pop rbx
push 2
pop rax

push 12
pop [rax+0]
push [rax+0]
out

hlt