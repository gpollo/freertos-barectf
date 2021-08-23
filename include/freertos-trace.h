#ifndef INCLUDE_TRACE_FREERTOS_H
#define INCLUDE_TRACE_FREERTOS_H

#include "barectf.h"

extern struct barectf_freertos_ctx* trace_freertos_ctx;

/* 
 * Tracepoints can be manually deactivated by defining an empty
 * macro before the inclusion or this file.
 *
 * TODO: Idealy, we shouldn't cast types here since a port may
 *       have different type aliases.
 */

#define __MASK_CONCAT(x, y) x ## y
#define MASK_CONCAT(x, y) __MASK_CONCAT(x, y)
#define MASK MASK_CONCAT(mask, __LINE__)

#ifndef traceSTART
    #define traceSTART()              \
        taskENTER_CRITICAL();         \
        barectf_freertos_trace_start( \
            trace_freertos_ctx,       \
        );                            \
        taskEXIT_CRITICAL();
#endif /* traceSTART */

#ifndef traceEND
    #define traceEND()              \
        taskENTER_CRITICAL();       \
        barectf_freertos_trace_end( \
            trace_freertos_ctx,     \
        );                          \
        taskEXIT_CRITICAL();
#endif /* traceEND */

#ifndef traceTASK_SWITCHED_IN
    #define traceTASK_SWITCHED_IN()               \
        int MASK = taskENTER_CRITICAL_FROM_ISR(); \
        barectf_freertos_trace_task_switched_in(  \
            trace_freertos_ctx,                   \
            (uint32_t) pxCurrentTCB               \
        );                                        \
        taskEXIT_CRITICAL_FROM_ISR(MASK);
#endif /* traceTASK_SWITCHED_IN */

#ifndef traceTASK_SWITCHED_OUT
    #define traceTASK_SWITCHED_OUT()              \
        int MASK = taskENTER_CRITICAL_FROM_ISR(); \
        barectf_freertos_trace_task_switched_out( \
            trace_freertos_ctx,                   \
            (uint32_t) pxCurrentTCB,              \
            (uint32_t) pxCurrentTCB->pxTopOfStack \
        );                                        \
        taskEXIT_CRITICAL_FROM_ISR(MASK);
#endif /* traceTASK_SWITCHED_OUT */

#ifndef traceMOVED_TASK_TO_READY_STATE
    #define traceMOVED_TASK_TO_READY_STATE(tcb)           \
        int MASK = taskENTER_CRITICAL_FROM_ISR();         \
        barectf_freertos_trace_moved_task_to_ready_state( \
            trace_freertos_ctx,                           \
            (uint32_t) tcb                                \
        );                                                \
        taskEXIT_CRITICAL_FROM_ISR(MASK);
#endif /* traceMOVED_TASK_TO_READY_STATE */

#ifndef traceTASK_CREATE
    #define traceTASK_CREATE(tcb)           \
        taskENTER_CRITICAL();               \
        barectf_freertos_trace_task_create( \
            trace_freertos_ctx,             \
            tcb->pcTaskName,                \
            (uint32_t) tcb,                 \
            (uint32_t) tcb->uxPriority,     \
            (uint32_t) tcb->pxStack,        \
            (uint32_t) tcb->pxEndOfStack    \
        );                                  \
        taskEXIT_CRITICAL();
#endif /* traceTASK_CREATE */

#ifndef traceTASK_DELAY_UNTIL
    #define traceTASK_DELAY_UNTIL(tcb)           \
        taskENTER_CRITICAL();                    \
        barectf_freertos_trace_task_delay_until( \
            trace_freertos_ctx,                  \
            (uint32_t) pxPreviousWakeTime,       \
            (uint32_t) xTimeIncrement,           \
            (uint32_t) xTimeToWake               \
        );                                       \
        taskEXIT_CRITICAL();
#endif /* traceTASK_DELAY_UNTIL */

#ifndef traceTASK_DELAY
    #define traceTASK_DELAY(tcb)           \
        taskENTER_CRITICAL();              \
        barectf_freertos_trace_task_delay( \
            trace_freertos_ctx,            \
            (uint32_t) xTicksToDelay       \
        );                                 \
        taskEXIT_CRITICAL();
#endif /* traceTASK_DELAY */

#ifndef traceTASK_SUSPEND
    #define traceTASK_SUSPEND(tcb)           \
        taskENTER_CRITICAL();                \
        barectf_freertos_trace_task_suspend( \
            trace_freertos_ctx,              \
            (uint32_t) tcb                   \
        );                                   \
        taskEXIT_CRITICAL();
#endif /* traceTASK_SUSPEND */

#ifndef traceTASK_RESUME
    #define traceTASK_RESUME(tcb)           \
        taskENTER_CRITICAL();               \
        barectf_freertos_trace_task_resume( \
            trace_freertos_ctx,             \
            (uint32_t) tcb                  \
        );                                  \
        taskEXIT_CRITICAL();
#endif /* traceTASK_RESUME */

#ifndef traceTASK_RESUME_FROM_ISR
    #define traceTASK_RESUME_FROM_ISR(tcb)           \
        int MASK = taskENTER_CRITICAL_FROM_ISR();    \
        barectf_freertos_trace_task_resume_from_isr( \
            trace_freertos_ctx,                      \
            (uint32_t) tcb                           \
        );                                           \
        taskEXIT_CRITICAL_FROM_ISR(MASK);
#endif /* traceTASK_RESUME_FROM_ISR */

#ifndef traceTASK_INCREMENT_TICK
    #define traceTASK_INCREMENT_TICK(tick_count)    \
        int MASK = taskENTER_CRITICAL_FROM_ISR();   \
        barectf_freertos_trace_task_increment_tick( \
            trace_freertos_ctx,                     \
            (uint32_t) tick_count                   \
        );                                          \
        taskEXIT_CRITICAL_FROM_ISR(MASK);
#endif /* traceTASK_INCREMENT_TICK */

#ifndef traceTASK_NOTIFY_TAKE_BLOCK
    #define traceTASK_NOTIFY_TAKE_BLOCK(index_to_wait) \
        int MASK = taskENTER_CRITICAL_FROM_ISR();      \
        barectf_freertos_trace_task_notify_take_block( \
            trace_freertos_ctx,                        \
            (uint32_t) index_to_wait                   \
        );                                             \
        taskEXIT_CRITICAL_FROM_ISR(MASK);
#endif /* traceTASK_NOTIFY_TAKE_BLOCK */

#ifndef traceTASK_NOTIFY_TAKE
    #define traceTASK_NOTIFY_TAKE(index_to_wait)  \
        int MASK = taskENTER_CRITICAL_FROM_ISR(); \
        barectf_freertos_trace_task_notify_take(  \
            trace_freertos_ctx,                   \
            (uint32_t) index_to_wait              \
        );                                        \
        taskEXIT_CRITICAL_FROM_ISR(MASK);
#endif /* traceTASK_NOTIFY_TAKE */

#endif /* INCLUDE_TRACE_FREERTOS_H */
