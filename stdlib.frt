: IMMEDIATE forth-last-word @ forth-cfa 1 - dup c@ 1 or swap c! ;

: if ' 0branch , forth-here 0 , ; IMMEDIATE
: else ' branch , forth-here 0 , swap forth-here swap ! ; IMMEDIATE
: then forth-here swap ! ; IMMEDIATE

: while forth-here ' 0branch , forth-here 0 , swap ; IMMEDIATE
: elihw ' branch , forth-here 0 , ! forth-here swap ! ; IMMEDIATE

: for ' swap , ' >r , ' >r , forth-here ' r> , ' r> , ' dup2 , ' >r , ' >r , ' < , ' 0branch , forth-here 0 , swap ; IMMEDIATE
: rof ' r> , ' lit , 1 , ' + , ' >r , ' branch , , forth-here swap ! ' r> , ' drop , ' r> , ' drop , ; IMMEDIATE
