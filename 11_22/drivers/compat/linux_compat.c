#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include "linux_compat.h"

TickType_t msecs_to_jiffies(unsigned int ms)
{
	TickType_t delay = (TickType_t)(ms / portTICK_RATE_MS);
	delay = delay > 0 ? delay : 1;
	return delay;
}

void msleep(unsigned int ms)
{
	   TickType_t delay = (TickType_t)(ms / portTICK_RATE_MS);
	   delay = delay > 0 ? delay : 1;
	   vTaskDelay(delay);
}

/*
 * Save the current interrupt enable state & disable IRQs
 */
unsigned long local_irq_save(void)
{
	unsigned long flags, temp;

	asm volatile(
		"mrs    %0, cpsr    @ arch_local_irq_save\n"
		"orr    %1, %0, #128\n"
		"msr    cpsr_c, %1"
		: "=r" (flags), "=r" (temp)
		:
		: "memory", "cc");
	return flags;
}

void local_irq_restore(unsigned long flags)
{
	asm volatile(
		"msr    cpsr_c, %0  @ local_irq_restore"
		:
		: "r" (flags)
		: "memory", "cc");
}

FRTOS_completion_t FRTOS_completion_init(void)
{
	return FRTOS_sem_create(0);
}

void FRTOS_completion_reinit(FRTOS_completion_t x)
{
	while (xSemaphoreTake(x, 0) == pdPASS);
}

void FRTOS_complete(FRTOS_completion_t x)
{
	FRTOS_sem_signal_from_isr(x);
}

//timeout : how many ticks, 0 is wait forever
unsigned int FRTOS_completion_wait(FRTOS_completion_t x, unsigned int timeout)
{
	return FRTOS_sem_wait(x, timeout);
}

void FRTOS_mutex_create( FRTOS_mutex * mutex_ptr )
{
    *mutex_ptr = xSemaphoreCreateRecursiveMutex();
}

void FRTOS_mutex_free( FRTOS_mutex * mutex_ptr )
{
    if( ( *mutex_ptr ) != NULL )
    {
        vSemaphoreDelete( *mutex_ptr );
    }
}

void FRTOS_mutex_lock( FRTOS_mutex mutex )
{
    if( ( mutex ) != NULL )
    {
        while( xSemaphoreTakeRecursive( mutex, portMAX_DELAY ) != pdTRUE )
        {
        }
    }
}

void FRTOS_mutex_unlock( FRTOS_mutex mutex )
{
    if( ( mutex ) != NULL )
    {
        xSemaphoreGiveRecursive( mutex );
    }
}

FRTOS_sem_t  FRTOS_sem_create(unsigned char count)
{
	SemaphoreHandle_t  xSemaphore;

	portENTER_CRITICAL();
	vSemaphoreCreateBinary( xSemaphore );
	if(count == 0)	// Means it can't be taken
	{
		xSemaphoreTake(xSemaphore,1);
	}
	portEXIT_CRITICAL();

	if( xSemaphore == NULL )
	{
		return NULL;	// TBD need assert
	}
	else
	{
		return xSemaphore;
	}
}

/*
	timeout : how many ticks, 0 is wait forever
*/
unsigned int  FRTOS_sem_wait(FRTOS_sem_t sem, unsigned int timeout)
{
	if(	timeout != 0)
	{
		if( xSemaphoreTake( sem, timeout ) == pdTRUE )
			return 0;
		else
			return portMAX_DELAY;
	}
	else // must block without a timeout
	{
		while( xSemaphoreTake( sem, 10000 ) != pdTRUE )
		{
			;
		}
		return 0;
	}
}

/*-----------------------------------------------------------------------------------*/
void  FRTOS_sem_signal(FRTOS_sem_t sem)
{
	xSemaphoreGive(sem);
}

void  FRTOS_sem_signal_from_isr(FRTOS_sem_t sem)
{
	long xHigherPriorityTaskWoken = pdFALSE;

	xSemaphoreGiveFromISR(sem, &xHigherPriorityTaskWoken);

	if (xHigherPriorityTaskWoken == pdTRUE) {
		portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	}
}


/*-----------------------------------------------------------------------------------*/
void  FRTOS_sem_free(FRTOS_sem_t sem)
{
	vQueueDelete( sem );
}


int ffs(int x)
{
	int r = 1;
	if (!x)
		return 0;
	if (!(x & 0xffff)) {
		x >>= 16;
		r += 16;
	}
	if (!(x & 0xff)) {
		x >>= 8;
		r += 8;
	}
	if (!(x & 0xf)) {
		x >>= 4;
		r += 4;
	}
	if (!(x & 3)) {
		x >>= 2;
		r += 2;
	}
	if (!(x &1)) {
		x >>= 1;
		r += 1;
	}
	return r;
}

inline void mutex_lock(struct mutex *lock)
{
	FRTOS_mutex_lock(lock->fmutex);
}

inline void mutex_unlock(struct mutex *lock)
{
	FRTOS_mutex_unlock(lock->fmutex);
}

inline void mutex_init(struct mutex *lock)
{
	FRTOS_mutex_create(&lock->fmutex);
}
