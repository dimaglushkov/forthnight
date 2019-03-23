%define STDIN 0
%define STDOUT 1

section .bss
user_dict: resq 65536

section .data
here:   dq user_dict
%define STATE_COMPILE 1 
%define STATE_INTERPRET 0 

state:  dq user_dict


const "forth-last-word", last_word
const "forth-here", here
const "forth-is-compiling", state


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


; ( -- 0 [addr of 0] )
colon "stack-cell", stack_cell
   dq xt_lit, 0, xt_sp, xt_exit 

; ( fd char -- )
colon "file-write-char", file_write_char
  dq xt_swap, xt_to_r, xt_to_r  ; (  , fd char )
  dq xt_stack_cell ; ( 0 addr , fd char )
  dq xt_dup, xt_from_r, xt_swap ; ( 0 addr char addr, fd )
  dq xt_write_char  ; ( char addr , fd )   c!
  dq xt_from_r, xt_swap , xt_lit, 1  ; ( char fd addr 1 )
  dq xt_sys_write, xt_drop, xt_drop, xt_exit

; ( fd -- 0 | char 1 ) 
colon "file-read-char", file_read_char
  dq xt_to_r, xt_stack_cell ; ( 0 addr , fd )
  dq xt_from_r, xt_swap, xt_lit, 1; ( 0 fd addr 1 ) 
  dq xt_sys_read ; ( 0 [0 or 1] )
  dq xt_branch0, .fail
  dq xt_lit, 1
.fail:
  dq xt_exit

colon "print-cr", print_cr
  dq xt_lit, STDOUT, xt_lit, 10, xt_file_write_char, xt_exit

colon "inc", inc
  dq xt_lit, 1, xt_plus, xt_exit

; ( addr - len )
colon "string-length", string_length 
  dq xt_dup ; ( start cur-addr)
.loop:
  dq xt_dup, xt_fetch_char ; (start cur-addr cur-char )
  dq xt_branch0, .exit
  dq xt_inc, xt_branch, .loop
.exit:
  dq xt_swap, xt_minus, xt_exit 

; ( str count - )
colon "print-string-counted", print_string_counted
  dq xt_to_r, xt_to_r
  dq xt_lit, STDOUT
  dq xt_from_r, xt_from_r ; ( STDOUT str count )
  dq xt_sys_write, xt_drop, xt_exit 

; ( str - )
colon "print-string", print_string
  dq xt_dup, xt_string_length, xt_print_string_counted, xt_exit

; ( char ) 
colon "char-is-space", char_is_space
  dq xt_to_r
  dq xt_r_fetch, xt_lit, ' ', xt_equals ; ( b1)
  dq xt_r_fetch, xt_lit, 10 , xt_equals ; ( b1 b2)
  dq xt_lor
  dq xt_r_fetch, xt_lit, 13 , xt_equals ; ( b1 b2)
  dq xt_lor
  dq xt_from_r, xt_lit, '\t' , xt_equals ; ( b1 b2)
  dq xt_lor
  dq xt_exit

; ( addr )
colon "string-terminate", string_terminate
  dq xt_lit, 0, xt_swap, xt_write_char, xt_exit

; ( fd buffer - length )
colon "file-read-word", file_read_word
    dq xt_dup, xt_to_r, xt_to_r

.read_space: ; ( fd , buf cur-buf)
    dq xt_dup, xt_file_read_char, xt_branch0, .eof
    ; ( fd char ) 
    dq xt_dup, xt_char_is_space, xt_branch0, .commit 
    dq xt_drop, xt_branch, .read_space


.read: ; ( fd , buf cur-buf )
    dq xt_dup, xt_file_read_char
    dq xt_branch0, .eof
    dq xt_dup, xt_char_is_space, xt_not, xt_branch0, .end


.commit:
    dq xt_r_fetch, xt_write_char
    dq xt_from_r, xt_inc, xt_to_r
    dq xt_branch, .read

.end: 
    dq xt_drop

.eof: ; ( fd , buf cur-buf )
    dq xt_drop, xt_r_fetch, xt_string_terminate ; ( , buf cur-buf )
    dq xt_from_r,  xt_from_r,   xt_minus, xt_exit

native ".", dot
    pop rdi
    call print_int
    jmp next

native ".S", show_stack
    mov rcx, rsp
    .loop:
        cmp rcx, [stack_start]
        jae next
        mov rdi, [rcx]
        push rcx
        call print_int
        call print_newline
        pop rcx
        add rcx, 8
        jmp .loop

; ( a b - a b a b)
colon "2dup", ddup
    dq xt_to_r, xt_dup, xt_r_fetch, xt_swap, xt_from_r, xt_exit

; ( a b - a+1 b+1 ) 
colon "2inc", dinc
    dq xt_to_r, xt_inc, xt_from_r, xt_inc, xt_exit

; ( str1 str2 - 0|1 )
colon "string-equals", string_equals
.start:
  dq xt_ddup, xt_fetch_char, xt_swap, xt_fetch_char 
  ; ( str1 str2 c1 c2 )
  dq xt_equals, xt_branch0, .no
  dq xt_dup, xt_fetch_char, xt_branch0, .yes
  dq xt_dinc, xt_branch, .start
.yes:
  dq xt_drop, xt_drop, xt_lit, 1, xt_exit
.no:
  dq xt_drop, xt_drop, xt_lit, 0, xt_exit

colon "forth-dict-word-name", forth_dict_word_name
  dq xt_lit, 9, xt_plus, xt_exit

; ( str - addr 1 ) 
colon "forth-find-word", forth_find_word
  dq xt_to_r, xt_last_word, xt_fetch

  ; ( cur-word , name ) 
.loop:
  dq xt_dup, xt_branch0, .not_found

  dq xt_dup, xt_forth_dict_word_name
  ; ( cur-word-header cur-word-name , name  ) 
  dq xt_r_fetch, xt_string_equals, xt_not, xt_branch0, .found

  ; ( cur-word-header )
  dq xt_fetch, xt_branch, .loop 

.found: ; ( cur-word-header, name)
  dq xt_lit, 1
.not_found:
  dq xt_from_r, xt_drop, xt_exit
 

; ( header - xt )
colon "forth-cfa", forth_cfa
  dq xt_lit, 9, xt_plus
  dq xt_dup, xt_string_length, xt_plus,
  dq xt_lit, 1+1, xt_plus, xt_exit

; ( a - ) 
colon ",", comma
  dq xt_here, xt_fetch, xt_write
  dq xt_here, xt_fetch, xt_lit, 8, xt_plus
  dq xt_here, xt_write, xt_exit

; ( c - ) 
colon "c,", char_comma
  dq xt_here, xt_fetch, xt_write_char
  dq xt_here, xt_fetch, xt_lit, 8, xt_plus
  dq xt_here, xt_write, xt_exit

; ( dest source ) 
colon "string-copy", string_copy
.loop
   dq xt_ddup, xt_fetch_char, xt_to_r, xt_r_fetch, xt_swap, xt_write_char
   dq xt_dinc, xt_from_r, xt_not, xt_branch0, .loop

   dq xt_drop, xt_drop, xt_exit

; ( flags name - )
colon "forth-create", forth_create
   dq xt_here, xt_fetch

   dq xt_last_word, xt_fetch, xt_comma  ; [here] <- last_word
   dq xt_last_word, xt_write
   dq xt_lit, 0, xt_char_comma 
   dq xt_dup, xt_here, xt_fetch, xt_swap, xt_string_copy
   dq xt_string_length, xt_here, xt_fetch, xt_plus, xt_inc
   dq xt_here, xt_write
   dq xt_char_comma
   dq xt_exit



section .data
str1: db "999999999999", 0
str2: db "Dima", 0

program:
    dq xt_lit, str1
    dq xt_lit, str2
    dq xt_string_copy
    dq xt_lit, str1
    dq xt_print_string 
    dq xt_bye
