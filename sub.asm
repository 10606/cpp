                section         .text

                global          _start
_start:

                sub             rsp, 2 * 128 * 8 ; read rsi then rdi
                lea             rdi, [rsp + 128 * 8]
                mov             rcx, 128
                call            read_long
                mov             rdi, rsp
                call            read_long
                lea             rsi, [rsp + 128 * 8]

                mov             rbx, 10
                ; call            mul_long_long
                call            sub_long_long
                ; call            add_long_long

                call            write_long

                mov             al, 0x0a
                call            write_char

                jmp             exit

; adds two long number
;    rdi -- address of summand #1 (long number)
;    rsi -- address of summand #2 (long number)
;    rcx -- length of long numbers in qwords
; result:
;    sum is written to rdi
add_long_long:
                push            rdi
                push            rsi
                push            rcx

                clc
.loop:
                mov             rax, [rsi]
                lea             rsi, [rsi + 8]
                adc             [rdi], rax
                lea             rdi, [rdi + 8]
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rsi
                pop             rdi
                ret

; subs two long number rdi = rsi - rdi
;    rdi -- address of  #1 (long number)
;    rsi -- address of  #2 (long number)
;    rcx -- length of long numbers in qwords
; result:
;    sub is written to rdi
sub_long_long:
                push            rdi
                push            rsi
                push            rcx

                clc
.loop:
                mov             rax, [rsi]
                lea             rsi, [rsi + 8]
                sbb             rax, [rdi]
                mov             [rdi], rax
                lea             rdi, [rdi + 8]
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rsi
                pop             rdi
                ret

; << long number on 64
;    rdi -- address of (long number)
;    rcx -- length of long number in qwords
; result:
;    << is written to rdi
shl_long:
                push            rdi
                push            rcx
                
                xor             rbx, rbx
                ;lea             rdi, [rdi + 8 * rcx - 8]

.loop:
                xchg            rbx, [rdi]
                lea             rdi, [rdi + 8]
                dec             rcx
                ;cmp             rcx, 1
                ja             .loop

                pop             rcx
                pop             rdi
                ret

; rdi += rsi * rbx sultiplies long number by a short
;    rdi -- address of add #1 (long number)
;    rsi -- address of multiplier #2 (long number)
;    rbx -- multiplier #3 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    product is written to rdi
add_mul_long_short:
                push            rax
                push            rdi
                push            rsi
                push            rcx
                push            rbp
                
                xor             rbp, rbp            ; rbp = 0
.loop:
                mov             rax, [rsi] 
                mul             rbx                 ; rax = rbx*[rsi] young, rdx = rbx*[rsi] old  
                add             rax, rbp            ; rax = rbx*[rsi]+rbp
                adc             rdx, 0              ; rdx, rbp - перенос
                mov             rbp, rdx
                add             [rdi], rax          ; [rdi] += rbx*[rdi]+rbp
                adc             rbp, 0
                lea             rdi, [rdi + 8]
                lea             rsi, [rsi + 8]
                dec             rcx
                jnz             .loop
                
                pop             rbp
                pop             rcx
                pop             rsi
                pop             rdi
                pop             rax
                ret


; rsi = rdi rdi = zero
;    rdi -- from (long number)
;    rsi -- to
;    rcx -- length of long number in qwords
rewrite:
                push            rax
                push            rdi
                push            rsi
                push            rcx

.loop:
                mov             rax, [rdi]
                mov             [rsi], rax
                xor             rax, rax
                mov             [rdi], rax
                lea             rdi, [rdi + 8]
                lea             rsi, [rsi + 8]
                dec             rcx
                jnz             .loop 

                pop             rcx
                pop             rsi
                pop             rdi
                pop             rax
                ret


; rdi = rsi * rdi sultiplies long number by a short
;    rdi -- address of add #1 (long number)
;    rsi -- address of multiplier #2 (long number)
;    rcx -- length of long number in qwords
; result:
;    product is written to rdi
mul_long_long:
                push            rax
                push            rdi
                push            rsi
                push            rcx
                push            rbp
                push            rbx
                push            rsp
                
                mov             rax, 0x8
                mul             rcx

                sub             rsp, rax
                ;sub             rsp, 1024
                mov             rbp, rsp
                xchg            rsi, rbp
                call            rewrite
                ; rbp - new sequence = rdi, rdi = 0 !not ready! 
.loop:
                mov             rbx, [rbp]
                call            add_mul_long_short
                xchg            rdi, rsi
                call            shl_long
                xchg            rdi, rsi
                lea             rbp, [rbp + 8]
                dec             rcx
                jnz             .loop
                
                add             rsp, rax
                pop             rsp
                pop             rbx
                pop             rbp
                pop             rcx
                pop             rsi
                pop             rdi
                pop             rax
                ret



; adds 64-bit number to long number
;    rdi -- address of summand #1 (long number)
;    rax -- summand #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    sum is written to rdi
add_long_short:
                push            rdi
                push            rcx
                push            rdx

                xor             rdx,rdx
.loop:
                add             [rdi], rax
                adc             rdx, 0
                mov             rax, rdx
                xor             rdx, rdx
                add             rdi, 8
                dec             rcx
                jnz             .loop

                pop             rdx
                pop             rcx
                pop             rdi
                ret

; multiplies long number by a short
;    rdi -- address of multiplier #1 (long number)
;    rbx -- multiplier #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    product is written to rdi
mul_long_short:
                push            rax
                push            rdi
                push            rcx

                xor             rsi, rsi            ; rsi = 0
.loop:
                mov             rax, [rdi] 
                mul             rbx                 ; rax = rbx*[rdi] young, rdx = rbx*[rdi] old  
                add             rax, rsi            ; rax = rbx*[rdi]+rsi
                adc             rdx, 0              ; rdx, rsi - перенос
                mov             [rdi], rax          ; [rdi] = rbx*[rdi]+rsi
                add             rdi, 8
                mov             rsi, rdx
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rdi
                pop             rax
                ret

; divides long number by a short
;    rdi -- address of dividend (long number)
;    rbx -- divisor (64-bit unsigned)
;    rcx -- length of long number in qwords (rdx = rdi / rbx)
; result:
;    quotient is written to rdi
;    rdx -- remainder
div_long_short:
                push            rdi
                push            rax
                push            rcx

                lea             rdi, [rdi + 8 * rcx - 8] ; from end to begin
                xor             rdx, rdx

.loop:
                mov             rax, [rdi]
                div             rbx         ; rax = rdx[rdi]/rbx res, rdx=rdx[rdi]/rbx remainder
                mov             [rdi], rax  ; genius!!!!
                sub             rdi, 8
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rax
                pop             rdi
                ret

; assigns a zero to long number
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
set_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep stosq   ; while (rcx!=0) {[rdi]=rax; rdi+=8;} 

                pop             rcx
                pop             rdi
                pop             rax
                ret

; checks if a long number is a zero
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
; result:
;    ZF=1 if zero
is_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep scasq   ; while (rcx!=0 && cf==0) {[rdi]!=rax -> cf; rdi+=8;}

                pop             rcx
                pop             rdi
                pop             rax
                ret

; read long number from stdin
;    rdi -- location for output (long number)
;    rcx -- length of long number in qwords
read_long:
                push            rcx
                push            rdi

                call            set_zero
.loop:
                call            read_char
                or              rax, rax        ; sf=1 if code return of read_char==-1 error
                js              exit
                cmp             rax, 0x0a       ; \n
                je              .done
                cmp             rax, '0'        ; < 0
                jb              .invalid_char
                cmp             rax, '9'        ; > 9
                ja              .invalid_char

                sub             rax, '0'        ; rax-='0' 
                mov             rbx, 10         ; rbx=10
                call            mul_long_short  ; [rdi]*=10
                call            add_long_short  ; [rdi]+=rax-'0'
                jmp             .loop

.done:
                pop             rdi
                pop             rcx
                ret

.invalid_char:
                mov             rsi, invalid_char_msg
                mov             rdx, invalid_char_msg_size
                call            print_string
                call            write_char
                mov             al, 0x0a
                call            write_char

.skip_loop:
                call            read_char
                or              rax, rax
                js              exit
                cmp             rax, 0x0a
                je              exit
                jmp             .skip_loop

; write long number to stdout
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
write_long:
                push            rax
                push            rcx

                mov             rax, 20
                mul             rcx             ; rdx,rax=20*rcx 
                mov             rbp, rsp        ; rbp=rsp
                sub             rsp, rax        ; rsp-=20*rcx

                mov             rsi, rbp        ; rsi=rsp old val
                                                ; увеличили в 20 раз 

.loop:
                mov             rbx, 10
                call            div_long_short  ; [rdi]/=10
                add             rdx, '0'
                dec             rsi
                mov             [rsi], dl       ; [rsi] decimal char of [rdi]
                call            is_zero
                jnz             .loop

                mov             rdx, rbp        ; вывод [rsi]
                sub             rdx, rsi
                call            print_string

                mov             rsp, rbp
                pop             rcx
                pop             rax
                ret

; read one char from stdin
; result:
;    rax == -1 if error occurs
;    rax \in [0; 255] if OK
read_char:
                push            rcx
                push            rdi

                sub             rsp, 1 ; for syscall
                xor             rax, rax
                xor             rdi, rdi
                mov             rsi, rsp
                mov             rdx, 1
                syscall

                cmp             rax, 1
                jne             .error
                xor             rax, rax
                mov             al, [rsp] ; al young part of rax
                add             rsp, 1 ; for syscall

                pop             rdi
                pop             rcx
                ret
.error:
                mov             rax, -1
                add             rsp, 1
                pop             rdi
                pop             rcx
                ret

; write one char to stdout, errors are ignored
;    al -- char
write_char:
                sub             rsp, 1
                mov             [rsp], al

                mov             rax, 1
                mov             rdi, 1
                mov             rsi, rsp
                mov             rdx, 1
                syscall
                add             rsp, 1
                ret

exit:
                mov             rax, 60
                xor             rdi, rdi
                syscall

; print string to stdout
;    rsi -- string
;    rdx -- size
print_string:
                push            rax

                mov             rax, 1
                mov             rdi, 1
                syscall

                pop             rax
                ret


                section         .rodata
invalid_char_msg:
                db              "Invalid character: "
invalid_char_msg_size: equ             $ - invalid_char_msg
