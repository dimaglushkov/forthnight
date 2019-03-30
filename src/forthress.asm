; ------------------------------------------------
; Forthress 2, a Forth dialect 
;
; Author: igorjirkov@gmail.com
; Date  : 08-03-2019
;
; This is the main Forthress 2 file which defines the entry point
; Please define words inside "words.inc"
; ------------------------------------------------

global _start
%include "util.inc"
%include "macro.inc"

%define pc r15
%define w r14
%define rstack r13

section .text

%include "kernel.inc"   ;  Minimal word set is here
%include "words.inc"    ;  Predefined words are here

section .bss

; return stack end-----;
resq 1023              ;
rstack_start: resq 1   ;
; return stack start---;

user_mem: resq 65536   ; global data for user

section .data

last_word: dq _lw      ; stores a pointer to the last word in dictionary
                       ; should be placed after all words are defined
dp: dq user_mem        ; current global data pointer
stack_start:  dq 0     ; stores a saved address of data stack
 
section .text

_start: 
    mov rstack, rstack_start
    mov [stack_start], rsp

    ;;  push to stack to set up initial stack state
    mov pc, forth_init

next:                  
    mov w, pc
    add pc, 8
    mov w, [w]
    jmp [w]

