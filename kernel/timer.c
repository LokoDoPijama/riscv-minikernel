#include "timer.h"

static uint64_t tick_interval = 100000;

static inline void sbi_set_timer(uint64_t time); // Função assembly definida em set_timer.S

void timer_next(void)
{
    uint64_t now;
    
    // Busca valor atual do timer interno e guarda em 'now'
    asm volatile (
        "csrr %0, time"
        : "r=" (now)
    );

    // Agenda o timer para disparar um interrupt no tempo 'now + tick_interval'
    sbi_set_timer(now + tick_interval);
}

void timer_init(uint64_t interval)
{
    if (interval != 0)
        tick_interval = interval;
    
    timer_next();

    // Habilitar STIE no CSR sie
    asm volatile("csrs sie, %0" : : "r"(0x20));

    // Habilitar SIE global em sstatus
    asm volatile("csrs sstatus, %0" : : "r"(0x02));
}