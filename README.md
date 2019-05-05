```
███████╗ ██████╗ ██████╗ ████████╗██╗  ██╗███╗   ██╗██╗ ██████╗ ██╗  ██╗████████╗
██╔════╝██╔═══██╗██╔══██╗╚══██╔══╝██║  ██║████╗  ██║██║██╔════╝ ██║  ██║╚══██╔══╝
█████╗  ██║   ██║██████╔╝   ██║   ███████║██╔██╗ ██║██║██║  ███╗███████║   ██║   
██╔══╝  ██║   ██║██╔══██╗   ██║   ██╔══██║██║╚██╗██║██║██║   ██║██╔══██║   ██║   
██║     ╚██████╔╝██║  ██║   ██║   ██║  ██║██║ ╚████║██║╚██████╔╝██║  ██║   ██║   
╚═╝      ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝   
```                                                                                                                                                     
                                                                                                                                                                
**ForthNight is a simple forth dialect for a real memesters.**

Created as a part of 'System-software' course in ITMO University, which was created by [Igor Jirkov](https://github.com/sayon/) and this exact version was inspired by his [forthress dialect](https://github.com/sayon/forthress).


# Part 1
Includes some forth-words implementations, such as 
* ```even``` ( a - b: {1 if even, 0 otherwise} )
* ```prime``` ( a - b: {1 if prime, 0 otherwise} )
* ```dup2``` ( a b - a b a b ) 
* ```sqrt``` ( a - b: {lowest b: b*b >= a} )
* ```concat``` ( addr1 addr2 - res_addr )
and <a href="https://github.com/allacee/ssw-course-project/blob/master/part1.frt">more...</a>
