: here forth-here @ ; 

: IMMEDIATE forth-last-word @ forth-cfa 1 - dup c@ 1 or swap c! ;

: if ' 0branch , here 0 , ; IMMEDIATE
: else ' branch , here 0 , swap here swap ! ; IMMEDIATE
: then here swap ! ; IMMEDIATE

: while here ' 0branch , here 0 , swap ; IMMEDIATE
: elihw ' branch , here 0 , ! here swap ! ; IMMEDIATE

: for ' swap , ' >r , ' >r , here ' r> , ' r> , ' dup2 , ' >r , ' >r , ' < , ' 0branch , here 0 , swap ; IMMEDIATE
: rof ' r> , ' lit , 1 , ' + , ' >r , ' branch , , here swap ! ' r> , ' drop , ' r> , ' drop , ; IMMEDIATE
