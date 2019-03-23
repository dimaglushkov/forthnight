; ------------------------------------------------
; Forthress 2, a Forth dialect
;
; This file stores the core predefined words
; Check README.md in the root directory for
; word semantics
;
; Author: igorjirkov@gmail.com
; Date  : 08-03-2019
;
; ------------------------------------------------

native "drop", drop
    add rsp, 8
    jmp next

native "swap", swap
    pop rax
    pop rdx
    push rax
    push rdx
    jmp next

native "dup", dup
    push qword [rsp]
    jmp next

native "+", plus
    pop rax
    add [rsp], rax
    jmp next

native "*", mul
    pop rax
    pop rdx
    imul rdx
    push rax
    jmp next

native "/", div
    pop rcx
    pop rax
    cqo
    idiv rcx
    push rax
    jmp next

native "%", mod
    pop rcx
    pop rax
    cqo
    idiv rcx
    push rdx
    jmp next

native "-", minus
    pop rax
    sub [rsp], rax
    jmp next

native "not", not
    pop rax
    test rax, rax
    setz al
    movzx rax, al
    push rax
    jmp next

native "=", equals
    pop rax
    pop rdx
    cmp rax, rdx
    sete al
    movzx rax, al
    push rax
    jmp next

native "<", lt
    pop rdx
    pop rax
    cmp rax, rdx
    setl al
    movzx rax, al
    push rax
    jmp next

native "and", and
    pop rax
    and [rsp], rax
    jmp next

native "or", or
    pop rax
    or [rsp], rax
    jmp next

native "land", land
    pop rax
    pop rdx
    test rax, rax
    jz .no
    push rdx
    jmp next
.no:
    push rax
    jmp next

native "lor", lor
    pop rax
    pop rdx
    test rax, rax
    jnz .yes
    push rdx
    jmp next
.yes:
    push rax
    jmp next

native "exit", exit
    rpop pc
    jmp next

native ">r", to_r
    pop rax
    rpush rax
    jmp next

native "r>", from_r
    rpop rax
    push rax
    jmp next

native "r@", r_fetch
    push qword [rstack]
    jmp next

native "docol", docol
    rpush pc
    add w, 8
    mov pc, w
    jmp next

; ( call_num a1 a2 a3 a4 a5 a6 -- new_rax new_rdx )
; Executes syscall
; The following registers store arguments (according to ABI)
; rdi , rsi , rdx , r10 , r8 and r9
native "syscall", syscall
    pop r9
    pop r8
    pop r10
    pop rdx
    pop rsi
    pop rdi
    pop rax
    syscall
    push rax
    push rdx
    jmp next


native "branch", branch
    mov pc, [pc]
    jmp next

native "0branch", branch0
    pop rax
    test rax, rax
    jnz .skip
    mov pc, [pc]
    jmp next
    .skip:
    add pc, 8
    jmp next

native "lit", lit
    push qword [pc]
    add pc, 8
    jmp next

native "execute", execute
    pop rax
    mov w, rax
    jmp [rax]

native "@", fetch
    pop rax
    push qword[rax]
    jmp next

native "!", write
    pop rax
    pop rdx
    mov [rax], rdx
    jmp next

native "c!", write_char
    pop rax
    pop rdx
    mov [rax], dl
    jmp next

native "c@", fetch_char
    pop rax
    movzx rax, byte [rax]
    push rax
    jmp next

native "sp", sp
    push rsp
    jmp next

const "forth-dp", dp
