#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include <inttypes.h>
#include <stdio.h>
#include <sys/time.h>

static uint32_t uxLastTaskID = -1u;
static uint32_t uxLastTaskID_old = -1u;

// compare the last task id with the current task id
void printState(void)
{
    if (uxLastTaskID != uxLastTaskID_old)
    {
        uxLastTaskID_old = uxLastTaskID;

        // print out the current time and tick count and task id
        struct timeval tv_now;
        gettimeofday(&tv_now, NULL);
        printf("%ld.%06ld, Tick: %04u, ID: %u\n", tv_now.tv_sec, tv_now.tv_usec, xTaskGetTickCount(), uxLastTaskID);
    }
}

static TaskHandle_t task1 = NULL;
static TaskHandle_t task2 = NULL;

void vTask(void *param)
{
    while (1)
    {
        uxLastTaskID = (uint32_t)param;
        printState();
    }

    vTaskDelete(NULL);
}

void app_main(void)
{
    vTaskPrioritySet(NULL, 10);

    xTaskCreate(vTask, "Task 1", 2048, (void *)1u, 1, &task1);
    xTaskCreate(vTask, "Task 2", 2048, (void *)2u, 2, &task2);

    vTaskDelete(NULL);
}