#include "trap.h"
#include "timer.h"
#include "scheduler.h"
#include "uart.h"

void trap_handler(uint64_t *frame)
{
    uint64_t scause;
    
    /* TODO:
    * Ler CSR scause.
    * Verificar se interrupo.
    * Identificar Supervisor Timer Interrupt.
    * Chamar timer_next().
    * Chamar schedule_from_trap(frame).
    */

    uart_print("Unhandled trap\n");
    while (1)
        ;
}