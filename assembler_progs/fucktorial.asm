JMP :BEGIN

;------------------------------------------------------------------------ 
BEGIN:                          
; NUM IN RAX, RESULT IN RBX, CUR_NUM IN RCX

CALL :INTRO
CALL :CREATE_VARS

PUSH RAX
PUSH 0
JB :WRONG_NUM

CALL :FIND_FUCK
CALL :SHOW_ENDING

HLT

WRONG_NUM:
CALL :UNDEF_FUCK
HLT
;----------------------------------------------------------------------
FIND_FUCK:

PUSH RCX
PUSH 1
ADD
POP RCX

PUSH RCX
PUSH RAX
JA :STOP_FINDING

PUSH RBX
PUSH RCX
MUL

POP RBX

CALL :FIND_FUCK

STOP_FINDING:
RET
;-----------------------------------------------------------------------
SHOW_ENDING:
CALL :ENDING

PUSH RBX
OUT

RET
;-----------------------------------------------------------------------
CREATE_VARS:

PUSH 1
POP RBX

PUSH 1
POP RCX

RET
;------------------------------------------------------------------------
ENDING:

    PUSH 0      
    ;Fuck of your num is 
    PUSH 10
    PUSH 32
    PUSH 115
    PUSH 105
    PUSH 32
    PUSH 109
    PUSH 117
    PUSH 110
    PUSH 32
    PUSH 114
    PUSH 117
    PUSH 111
    PUSH 121
    PUSH 32
    PUSH 102
    PUSH 111
    PUSH 32
    PUSH 107
    PUSH 99
    PUSH 117
    PUSH 70
    POP [0]
    POP [1]
    POP [2]
    POP [3]
    POP [4]
    POP [5]
    POP [6]
    POP [7]
    POP [8]
    POP [9]
    POP [10]
    POP [11]
    POP [12]
    POP [13]
    POP [14]
    POP [15]
    POP [16]
    POP [17]
    POP [18]
    POP [19]
    POP [20]
    POP [21]
SHOW

RET
;------------------------------------------------------------------------
UNDEF_FUCK:
PUSH 0      
;CANT FIND FUCK OF NEGATIVE NUM
PUSH 10
PUSH 77
PUSH 85
PUSH 78
PUSH 32
PUSH 69
PUSH 86
PUSH 73
PUSH 84
PUSH 65
PUSH 71
PUSH 69
PUSH 78
PUSH 32
PUSH 70
PUSH 79
PUSH 32
PUSH 75
PUSH 67
PUSH 85
PUSH 70
PUSH 32
PUSH 68
PUSH 78
PUSH 73
PUSH 70
PUSH 32
PUSH 84
PUSH 78
PUSH 65
PUSH 67
POP [0]
POP [1]
POP [2]
POP [3]
POP [4]
POP [5]
POP [6]
POP [7]
POP [8]
POP [9]
POP [10]
POP [11]
POP [12]
POP [13]
POP [14]
POP [15]
POP [16]
POP [17]
POP [18]
POP [19]
POP [20]
POP [21]
POP [22]
POP [23]
POP [24]
POP [25]
POP [26]
POP [27]
POP [28]
POP [29]
POP [30]
POP [31]
SHOW
RET
;------------------------------------------------------------------------
INTRO:
;input your num
PUSH 0      
PUSH 10
PUSH 109
PUSH 117
PUSH 110
PUSH 32
PUSH 114
PUSH 117
PUSH 111
PUSH 121
PUSH 32
PUSH 116
PUSH 117
PUSH 112
PUSH 110
PUSH 105
POP [0]
POP [1]
POP [2]
POP [3]
POP [4]
POP [5]
POP [6]
POP [7]
POP [8]
POP [9]
POP [10]
POP [11]
POP [12]
POP [13]
POP [14]
POP [15]
SHOW

IN
POP RAX

RET