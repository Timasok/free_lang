call :yo
hlt

yo:

push 12
pop rax

in
pop [rax+0]
push [rax+0]
out

ret