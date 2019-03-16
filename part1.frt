( a - b: {1 if even, 0 otherwise} )
: even 
    2 % not dup
    if 
        ." 1" cr
    else 
        ." 0" cr
    then 
; 


( a - b: {lowest b: b*b >= a} )
: sqrt
    dup 2 / 1 +
    1
    for 
        dup dup

        r@ dup * swap
        < not
        swap
        r@ 1 + dup * swap 
        < not

        <
        if 
            r@ 1 + 
        then
    endfor  
    swap drop
; 


( a b - a b a b )
: dup2
    ( a b - b a - b a a - a a b - a a b b - a b b a - a b a b)
    swap
    dup
    rot
    dup
    rot
    swap
;


( a - b: {1 if prime, 0 otherwise} )
: prime  
    dup 1 = if exit then
    dup 2 = if exit then
    dup sqrt 
    1
    repeat
        1 +
        dup2
        <
        if 
            drop drop drop
            1 exit
        then

        rot dup2 swap
        % not
        if
            drop drop drop
            0 exit
        then
        rot rot
        0
    until
;


( Shows 2 stack elements. Does not pop them. )
: .!
    dup2 . cr . cr
;


( num - addr )
: prime-store  
    prime
    1 allot
    dup rot swap 
    c!
;


( addr1 addr2 - res_addr )
: concat
    dup2
    swap
    ( a1 a2 - a1 a2 l1 l2 - a1 a2 l3 - a1 a2 a3 )
    dup2 count swap count swap
    + 1 + 
    heap-alloc

    ( a1 a2 a3 - a2 a3 a3 a1 [l1] - a3' a3' a2 [l1] - a3' [l1] - a3 )
    dup rot dup count >r
    string-copy
    r@ + dup rot
    string-copy
    r> -

    rot heap-free
    swap heap-free
;

( a - b ) 
: collatz-conj
    dup . cr
    repeat
        dup 2 %
        if
            3 * 1 +
        else
            2 /
        then
        dup . cr
        dup 1 =        
    until
;
