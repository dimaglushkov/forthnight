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


( num mem_size - addr )
: prime-store 
    swap 
    prime
    swap

    allot
    dup rot swap 
    !
;

( outputs last 2 stack elements )
: .!
    
;

