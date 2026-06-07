#include "trap.h"
#include "timer.h"
#include "scheduler.h"
#include "uart.h"

void trap_handler(uint64_t *frame)
{
    uint64_t scause;
    
    asm volatile (
        "csrr %0, scause"
        : "r=" (scause)
    );

    // scause negativo (bit mais significativo = 0) significa que é um interrupt, não uma exceção
    if ((int64_t) scause < 0) {
        // Verifica se é um Supervisor Timer Interrupt (código 5)
        if ((scause & 0xff) == 5) {
            timer_next();
            schedule_from_trap(frame);
            return;
        }
    }

    uart_print("Unhandled trap\n");
    while (1)
        ;
}