call :main

;------------------------------------------
check_for_special_cases:

dup
push 2
je :equals_two

dup
push 0
jb :is_negative

dup
push 1
jbe :is_unit

dup
push 16
jae :cannot_calculate

ret

is_unit:
push 1
out
hlt

is_negative:
cannot_calculate:
dmp
hlt

equals_two:
push 2
out
hlt
;------------------------------------------
mul_all:

pop rbx

for2:
push rbx
mul
pop rbx
dup
push rax
dup
pop rax
jne :for2

push rbx
mul
out

ret
;------------------------------------------
main:

in

call :check_for_special_cases

dup
pop rax

for1:
dup
push 1
sub
dup
push 1
ja :for1

call :mul_all

hlt