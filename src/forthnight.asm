; forthnight start point
global _start
%include "macro.inc"
%include "utilities.inc"

; next command pointer
%define pc r15 
; sub commands pointer
%define w r14 
; return stack pointer
%define rstack r13


section .text
; predefined words here
%include "words.inc"


section .bss
; return stack end
resq 1023              
rstack_start: resq 1   
; return stack start
user_mem: resq 65536


section .data
last_word: dq _last_word 
dp: dq user_mem        
stack_start:  dq 0    
 

section .text
_start: 
    mov rstack, rstack_start
    mov [stack_start], rsp
    mov pc, forth_init

next:                  
    mov w, pc
    add pc, 8
    mov w, [w]
    jmp [w]