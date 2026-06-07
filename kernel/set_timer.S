.section .text
.global sbi_set_timer

sbi_set_timer:
    li a7, 0x54494D45
    li a6, 0
    ecall
    ret