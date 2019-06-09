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


native ".", dot
	push rdi
	call print_tab
	pop rdi
	pop rdi
	call print_int
	call print_newline
    jmp next

native ".S", show_stack
	mov rcx, rsp
	.loop:
        cmp rcx, [stack_start]
		jae next
		mov rdi, [rcx]
		push rcx
     	push rdi
		call print_tab
		pop rdi
		call print_int
		call print_newline
		pop rcx
        add rcx, 8
        jmp .loop


section .rodata
msg:
  .no_such_word: db "! No such word: ", 0

section .bss
word_buf:  resq 512
user_dict:  resq 65536
section .data
here:  dq user_dict
fd: dq 0

%define STATE_COMPILE 1
%define STATE_INTERPRET 0
state: dq STATE_INTERPRET

const "forth-here", here
const "forth-last-word", last_word
const "forth-is-compiling", state
const "forth-input-fd", fd
const "forth-stack-start", stack_start


colon "bye", bye
    dq xt_lit, 60
    TIMES 6 dq xt_lit, 0
    dq xt_syscall

%define sys_read_no 0
%define sys_write_no 1

colon "sys-read", sys_read
TIMES 3 dq xt_to_r
dq xt_lit, sys_read_no
TIMES 3 dq xt_from_r
TIMES 3 dq xt_lit, 0
dq xt_syscall , xt_drop, xt_exit

colon "sys-write", sys_write
TIMES 3 dq xt_to_r
dq xt_lit, sys_write_no
TIMES 3 dq xt_from_r
TIMES 3 dq xt_lit, 0
dq xt_syscall , xt_drop, xt_exit


colon  "stack-cell", stack_cell
  dq xt_lit, 0, xt_sp, xt_exit

; ( fd - char 1) or ( - 0 )
colon "file-read-char", file_read_char
  dq xt_to_r, xt_stack_cell, xt_from_r, xt_swap, xt_lit, 1, xt_sys_read
	branch0 .fail
  dq xt_lit, 1
.fail :
	dq xt_exit

  ; ( fd char - )
colon "file-write-char", file_write_char
  dq xt_swap, xt_to_r, xt_to_r
  dq xt_stack_cell, xt_dup, xt_from_r, xt_swap, xt_write_char
  dq xt_from_r, xt_swap, xt_lit, 1, xt_sys_write, xt_drop, xt_drop, xt_exit

colon "string-length", string_length
      dq xt_dup
.loop:
      dq xt_dup, xt_fetch_char
      branch0 .exit
      dq xt_inc
      branch .loop
.exit:
  dq xt_swap, xt_minus, xt_exit

colon "print-cr", print_cr
  dq xt_lit, 1, xt_lit, 10, xt_file_write_char, xt_exit

;  ( str count - )
colon "print-string-counted", print_string_counted
  dq xt_to_r, xt_to_r, xt_lit, 1, xt_from_r, xt_from_r, xt_sys_write, xt_drop, xt_exit

; ( str -- )
colon "print-string", print_string
  dq xt_dup, xt_string_length, xt_print_string_counted, xt_exit

colon "char-is-space", char_is_space
  dq xt_to_r
  dq xt_r_fetch, xt_lit,  ' ', xt_equals
  dq xt_r_fetch, xt_lit, 10, xt_equals, xt_lor
  dq xt_r_fetch, xt_lit, 13, xt_equals, xt_lor
  dq xt_from_r,  xt_lit, '\t', xt_equals, xt_lor, xt_exit

  ; ( addr - )
colon "string-terminate", string_terminate
  dq xt_lit, 0, xt_swap, xt_write_char, xt_exit

;  ( fd buffer -  )
colon "file-read-word", file_read_word
  dq xt_dup, xt_to_r, xt_to_r

.read_space:
  dq xt_dup, xt_file_read_char
  branch0 .eof

  dq xt_dup, xt_char_is_space
  branch0 .commit

  dq xt_drop
  branch .read_space

.read:
  dq xt_dup, xt_file_read_char
  branch0 .eof

  dq xt_dup, xt_char_is_space, xt_not
  branch0 .end

.commit:
  dq xt_r_fetch, xt_write_char
  dq xt_from_r, xt_inc, xt_to_r
  branch .read

.end:
  dq xt_drop
.eof:
  dq xt_drop, xt_r_fetch, xt_string_terminate
  dq xt_from_r, xt_from_r, xt_minus, xt_exit



colon ",", comma
  dq xt_here, xt_fetch, xt_write
  dq xt_here, xt_fetch, xt_lit, 8, xt_plus
  dq xt_here, xt_write, xt_exit

colon "c,", char_comma
  dq xt_here, xt_fetch, xt_write_char
  dq xt_here, xt_fetch, xt_inc
  dq xt_here, xt_write, xt_exit

colon "2dup", ddup
  dq xt_to_r, xt_dup, xt_r_fetch, xt_swap, xt_from_r, xt_exit

colon "2inc", dinc
  dq xt_to_r, xt_inc, xt_from_r, xt_inc, xt_exit

colon "string-equals", string_equals
.start:
  dq xt_ddup, xt_fetch_char, xt_swap, xt_fetch_char, xt_equals
  branch0 .no
  dq xt_dup, xt_fetch_char
  branch0 .yes
  dq xt_dinc
  branch .start
.yes:
  dq xt_drop, xt_drop, xt_lit, 1, xt_exit
.no:
  dq xt_drop, xt_drop, xt_lit, 0, xt_exit

colon "forth-dict-word-name", forth_dict_word_name
  dq xt_lit, 9, xt_plus, xt_exit

colon "forth-find-word", forth_find_word
  dq xt_to_r, xt_last_word, xt_fetch
  dq xt_state, xt_fetch
  branch0 .loop
  dq xt_fetch
.loop:
  dq xt_dup
  branch0 .not_found

  dq xt_dup, xt_forth_dict_word_name, xt_r_fetch, xt_string_equals, xt_not
  branch0 .found

  dq xt_fetch
  branch .loop
.found:
  dq xt_lit, 1
.not_found:
  dq xt_from_r, xt_drop, xt_exit

colon "'", forth_tick, 1
  dq xt_fd, xt_fetch, xt_lit, word_buf, xt_file_read_word
  branch0 .eof
  dq xt_lit, word_buf, xt_forth_find_word
  branch0 .no_word
  dq xt_forth_cfa, xt_state
  branch0 .interp
  dq xt_lit, xt_lit, xt_comma, xt_comma
.interp:
  dq xt_exit
.no_word:
  dq xt_lit, msg.no_such_word, xt_print_string, xt_print_string, xt_print_cr, xt_exit
  .eof:
  dq xt_drop, xt_exit

colon "forth-cfa", forth_cfa
  dq xt_lit, 9, xt_plus
  dq xt_dup, xt_string_length, xt_plus
  dq xt_lit, 2, xt_plus, xt_exit

; ( dest source - )
colon "string-copy", string_copy
.loop:
dq xt_ddup, xt_fetch_char, xt_to_r, xt_r_fetch, xt_swap, xt_write_char
dq xt_dinc, xt_from_r, xt_not
  branch0 .loop
  dq xt_drop, xt_drop, xt_exit

; ( flags name - )
colon "forth-create", forth_create
  dq xt_here, xt_fetch                ; word start
  dq xt_last_word, xt_fetch, xt_comma ; fill `link`
  dq xt_last_word, xt_write           ; update `last_word`
  dq xt_lit, 0, xt_char_comma
  dq xt_dup, xt_here, xt_fetch, xt_swap, xt_string_copy
  dq xt_string_length, xt_here, xt_fetch, xt_plus, xt_inc, xt_here, xt_write
  dq xt_char_comma
  dq xt_exit

colon ":", colon
  .restart:
    dq xt_fd, xt_fetch, xt_lit, word_buf, xt_file_read_word
    branch0 .restart
    dq xt_lit, 0, xt_lit, word_buf, xt_forth_create
    dq xt_lit, STATE_COMPILE, xt_state, xt_write
    dq xt_lit, i_docol, xt_comma, xt_exit

colon ";", semicolon, 1
    dq xt_lit, STATE_INTERPRET, xt_state, xt_write
    dq xt_lit, xt_exit, xt_comma, xt_exit

colon "char-is-digit", char_is_digit
  dq xt_to_r
  dq xt_r_fetch, xt_lit, '9'+1, xt_lt
  dq xt_lit, '0'-1, xt_r_fetch, xt_lt, xt_land
  dq xt_from_r, xt_drop, xt_exit

colon "drop0", drop0
  dq xt_dup
  branch0 .is_zero
  dq xt_exit
  .is_zero:
  dq xt_drop, xt_exit

colon "char-to-digit", char_to_digit
  dq xt_lit, '0', xt_minus, xt_exit

; ( buf -- num? len )
colon "string-parse-unsigned-number", string_parse_unsigned_number
  dq xt_dup, xt_to_r
  dq xt_lit, 0, xt_to_r
.loop:
  dq xt_dup, xt_fetch_char, xt_dup, xt_char_is_digit
  branch0 .end
  dq xt_char_to_digit
  dq xt_lit, 10, xt_from_r, xt_mul, xt_plus, xt_to_r
  dq xt_inc
  branch .loop
  .end:
  dq xt_drop, xt_from_r, xt_swap, xt_from_r, xt_minus, xt_drop0, xt_exit

colon "neg", neg
  dq xt_lit, 0, xt_swap, xt_minus, xt_exit

colon "inc", inc
  dq xt_lit, 1, xt_plus, xt_exit

; ( buf -- num? len )
colon "string-parse-number", string_parse_number
  dq xt_dup, xt_fetch_char, xt_dup, xt_char_is_digit, xt_not
  branch0 .unsigned
  dq xt_dup, xt_lit, '-', xt_equals, xt_not
  branch0 .negative

  dq xt_dup, xt_lit, '+', xt_equals, xt_not
  branch0 .positive

.negative:
  dq xt_drop
  dq xt_inc, xt_string_parse_unsigned_number
  dq xt_dup
  branch0 .error
  dq xt_inc
  dq xt_swap, xt_neg, xt_swap, xt_exit
.positive:
  dq xt_drop
  dq xt_inc, xt_string_parse_unsigned_number
  dq xt_dup
  branch0 .error
  dq xt_inc, xt_exit
.unsigned:
  dq xt_drop, xt_string_parse_unsigned_number, xt_exit
.error:
  dq xt_drop, xt_lit, 0, xt_exit

colon "forth-follows-branch", forth_follows_branch
    dq xt_here, xt_fetch, xt_lit, 8, xt_minus, xt_fetch
    dq xt_dup, xt_lit, xt_branch0, xt_equals, xt_swap
    dq xt_lit, xt_branch, xt_equals, xt_lor, xt_exit

; (  fd -- )
colon "forth-interpret-fd", forth_interpret_fd
    dq xt_lit, xt_fd, xt_fetch, xt_to_r
    dq xt_fd, xt_write

    .loop:
    dq xt_fd, xt_fetch, xt_lit, word_buf, xt_file_read_word
    branch0 .eof

    dq xt_lit, word_buf, xt_forth_interpret

    branch .loop

    .eof:
    dq xt_from_r, xt_fd, xt_write, xt_exit

colon "forth-xt-is-immediate", forth_xt_is_immediate
    dq xt_lit, 1, xt_minus, xt_fetch_char, xt_exit

; ( str - )
colon "forth-interpret", forth_interpret
.start:
    ; ( inbuf )
    dq xt_dup, xt_forth_find_word         ; find in dict
    branch0 .number                       ; ( inbuf dictaddr? 0/1 )
    dq xt_swap, xt_drop, xt_forth_cfa     ; ( xt )

    ; if not compiling
    dq xt_state, xt_fetch
    branch0 .interpreting

    dq xt_dup, xt_forth_xt_is_immediate, xt_not
    branch0 .interpreting

.compiling:
    dq xt_comma, xt_exit
.interpreting:
    dq xt_execute, xt_exit

.number: ; ( inbuf )
    dq xt_dup, xt_string_parse_number      ; ( inbuf num? 0/len )
    branch0 .no_word

    ; ( inbuf num )
    dq xt_swap, xt_drop
    dq xt_state, xt_fetch                  ; if interpreting, that's it
    branch0 .exit

    dq xt_forth_follows_branch, xt_not
    branch0 .is_branch

.not_branch:
    dq xt_lit, xt_lit, xt_comma
.is_branch: ; ( num prev_xt )
    dq xt_comma, xt_exit

.no_word: ; ( inbuf )
  dq xt_lit, msg.no_such_word, xt_print_string, xt_print_string, xt_print_cr

.exit:
    dq xt_exit

native "forth-stack-restore", forth_stack_restore
  mov rsp, [stack_start]
  jmp next

colon "trap", trap
    dq xt_lit, wh_trap + 9, xt_print_string, xt_print_cr, xt_exit

forth_init:
    dq xt_lit, 0, xt_forth_interpret_fd, xt_bye


