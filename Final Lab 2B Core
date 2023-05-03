
.include "xc.inc"

.text                       ;BP (put the following data in ROM(program memory))

; This is a library, thus it can *not* contain a _main function: the C file will
; define main().  However, we will need a .global statement to make available ASM
; functions to C code.
; All functions utilized outside of this file will need to have a leading 
; underscore (_) and be included in a comment delimited list below.


.global _write_0, _write_1, _djo_wait_100us, _djo_wait_1ms

_write_0:
        
inc LATA	    ; 1
repeat #3	    ; 1	
nop		    ; 1 + 4
dec LATA	    ; 1
repeat	#6	    ; 1  
nop		    ; 1 + 9	
    
return
    
_write_1:

inc LATA	    ;1
repeat #6	    ;1
nop		    ;1 + 6
dec LATA	    ;1
repeat #3	    ;1 + 4
nop		    ;1		       
        
return






_djo_wait_100us:
    repeat #1600
    nop
    return
    
_djo_wait_1ms:
repeat #16000
nop
return
