#include "scheduler.h"
#include "task.h"
#include <stdint.h>

extern void uart_print(const char*);

extern void context_switch(void*, void*);

static int current = 0;

/*   Round-Robin padrão   */

static int round_robin()
{
    return (current + 1) % task_count;
}

/*   Algoritmo atual   */

static sched_algo_t current_algo = round_robin;

void scheduler_set_algorithm(sched_algo_t algo)
{
    if (algo)
        current_algo = algo;
}

/*   Yield   */

void yield()
{
    int prev = current;
    int next = current_algo();

    current = next;

    context_switch(tasks[prev].regs,
                   tasks[next].regs);
}
 
/*   Início   */

void scheduler_start()
{
    if (task_count == 0)
        return;

    tasks[0].entry();
}

void schedule_from_trap(uint64_t *frame) // frame = array na stack com registradores da task interrompida
{
    if (task_count < 2) return; // Só escalona se houver mais de uma task

    int prev = current;
    int next = current_algo();

    for (int i = 0; i < 31; i++) {
        tasks[prev].regs[i] = frame[i];
    }

    // Lê o valor do CSR sepc para o TCB da task anterior
    asm volatile("csrr %0, sepc" : "=r"(tasks[prev].sepc));

    current = next;

    // TODO: Copiar tasks[next].regs -> frame
    for (int i = 0; i < 31; i++) {
        frame[i] = tasks[next].regs[i];
    }

    // Escreve o valor salvo no TCB da nova task de volta no CSR sepc
    asm volatile("csrw sepc, %0" : : "r"(tasks[next].sepc));
}
