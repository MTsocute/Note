
# $FreeRTOS$

> 对于我们一开始学些的那些 c51 stm32 库, 本质上其实都是**裸机**开发, 直接管理的都是硬件, 操作逻辑都是线性的,但是有了操作系统之后,我们的任务都是系统管理,资源也有系统释放了,而且还支持了多任务的管理
>
> 但是单片机资源有限, 一般都还是并发而不是并行

## 1. 任务

### 1. 任务的经营方式

> `FreeRTOS` 默认使用固定优先级的抢占式调度策略，对同等优先级的任务执行时间片轮询调度：

1. **抢占式调度**：

   - FreeRTOS 采用抢占式调度方式，允许更高优先级的任务在任何时刻抢占正在执行的低优先级任务。
   - 这确保了高优先级任务能够及时响应，并提高了系统的实时性。

   ![image-20250220144117270](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250220144117270.png)
2. **时间片轮询**：

   - 在相同优先级的任务之间，FreeRTOS 采用时间片轮转策略。
   - 每个任务执行一个时间片，如果有其他同优先级的任务等待执行，则切换到下一个任务。
   - 这有助于公平地分配 CPU 时间。

<br>

### 2. 任务的状态

> FreeRTOS 中的任务共有四种状态：

1. **运行态（Running）**：
   - 当任务实际执行时，它处于运行状态。
   - 在单核处理器（如 STM32）上，同一时间只有一个任务可以处于运行状态。
2. **就绪态（Ready）**：
   - 准备就绪的任务是那些能够执行（即不处于阻塞或挂起状态）但目前没有执行的任务。
   - 这些任务等待执行是因为有同等或更高优先级的任务已经处于运行状态。
3. **阻塞态（Blocked）**：
   - 如果任务当前正在**等待延时或外部事件**（如等待信号量、消息队列等），则该任务被认为处于阻塞状态。
   - 在阻塞期间，任务不会消耗 CPU 时间。
4. **挂起态（Suspended）**：
   - 类似于暂停，任务调用 `vTaskSuspend()` 函数进入挂起态。
   - 需要调用解挂函数 `vTaskResume()` 才可以使任务进入就绪态。

---

> 只有就绪态的任务可以转变成运行态, 其他状态（阻塞态和挂起态）的任务想要运行，必须先转变成就绪态。
>
> 任务状态转换关系如下：

![image-20250220143546900](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250220143546900.png)

### 3. 任务堆栈

> 我们以 AB 两个任务为例来大致介绍任务堆栈的工作流程：

---

#### 任务 A 的保存

- **触发条件** ：当有外部事件或系统调度器决定切换到任务 B
- **保存内容** ：将任务 A 的寄存器值、程序计数器（记录程序执行的位置）、堆栈指针（标识当前堆栈的顶部）以及相关的上下文信息（如系统状态标志、权限级别等）保存到任务堆栈中。

---

#### 任务 B 的加载

- **加载内容** ：从任务 B 的堆栈中取出之前保存的寄存器值、程序计数器、堆栈指针等上下文信息。
- **恢复执行** ：将这些信息加载到相应的寄存器和系统控制单元中，使任务 B 进入可执行状态，从它之前暂停的地方开始继续运行。

---

#### 运行任务 B

- **执行逻辑** ：任务 B 根据自身的程序逻辑进行运算、读取和写入数据等操作。
- **响应事件** ：在运行过程中，任务 B 可能需要响应各种内部和外部事件，如处理用户输入、与其他任务通信、访问硬件设备（如存储器、I/O 设备等）。

---

#### 回到任务 A

- **再次保存任务 B 的状态** ：当系统决定切换回任务 A 时，需要保存当前任务 B 的寄存器值、程序计数器、堆栈指针等状态信息到任务堆栈。
- **重新加载任务 A 的状态** ：从任务 A 的堆栈中恢复之前保存的寄存器值、程序计数器等信息到相应的寄存器和系统控制单元。
- **继续执行任务 A** ：任务 A 从之前保存的中断点继续执行。

<br>

### 4. 任务 API 函数

![image-20250221091923407](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250221091923407.png)

> - 动态创建任务：任务的任务控制块以及任务的栈空间所需的内存，均由 FreeRTOS 从 FreeRTOS 管理的堆中分配。
> - 静态创建任务：任务的任务控制块以及任务的栈空间所需的内存，需用户分配提供。

#### 1. xTaskCreate()

> 任务堆栈的大小就是 `usStackDepth x 4 Byte(32位系统) / 2 Byte`

```cpp
BaseType_t xTaskCreate(
    TaskFunction_t pxTaskCode,       // 指向任务实现函数的指针
    const char * const pcName,       // 任务的名字，用于调试
    configSTACK_DEPTH_TYPE usStackDepth, /* 任务栈大小，以字为单位 */
    void *pvParameters,              // 传递给任务的参数
    UBaseType_t uxPriority,          // 任务的优先级
  
    // 区别
    TaskHandle_t *pxCreatedTask      // 任务的句柄，用于管理任务
);
```

#### 2. xTaskCreateStatic()

```cpp
TaskHandler_t xTaskCreateStatic(
    TaskFunction_t pxTaskCode,       // 指向任务实现函数的指针
    const char * const pcName,       // 任务的名字，用于调试
    configSTACK_DEPTH_TYPE ulStackDepth, /* 任务栈大小，以字为单位 */
    void * const pvParameters,       // 传递给任务的参数
    UBaseType_t uxPriority,          // 任务的优先级
  
    // 区别
    StackType_t * const puxStackBuffer, // 指向用户分配的栈空间
    StaticTask_t * const pxTaskBuffer  // 指向用户分配的任务控制块
);
```

#### 3. 任务控制块结构体成员介绍

```cpp
typedef struct tskTaskControlBlock
{
    volatile StackType_t * pxTopOfStack; /* 任务栈栈顶，必须为 TCB 的第一个成员 */
    ListItem_t xStateListItem;           /* 任务状态列表项 */
    ListItem_t xEventListItem;           /* 任务事件列表项 */
    UBaseType_t uxPriority;              /* 任务优先级，数值越大，优先级越大 */
    StackType_t * pxStack;               /* 任务栈起始地址 */
    char pcTaskName[ configMAX_TASK_NAME_LEN ]; /* 任务名字 */

    // 省略很多条件编译的成员
} tskTCB;
```

> `pxTopOfStack` 就是用于上下文切换的

![image-20250221093505671](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250221093505671.png)

#### 4. 任务状态查询

![image-20250223140928376](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250223140928376.png)

#### 5. 任务挂起

> - `vTaskSuspend()`: 挂起任务，类似暂停，可恢复
> - `vTaskResume()`: 恢复被挂起的任务

![image-20250221160228447](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250221160228447.png)

## 2. 中断

![image-20250221201823240](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250221201823240.png)

![image-20250221165217815](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250221165217815.png)

> 临界段代码: 必须在不被打断的情况下完整运行的代码段
>
> 进入临界段其实就是调用了中断, 然后切换任务和挂起的优先级别是 15, 这些等级低的就不可以中断打扰了

![image-20250221165443560](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250221165443560.png)

> 可以在这里配置进入系统中断的最大优先级, 让 TIM 可以提高级别

![image-20250221200213785](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250221200213785.png)

![image-20250221200243641](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250221200243641.png)

<br>

## 3. 消息队列

> 相当于进程的那个通信管道
>
> - **线程安全**：确保了在多任务环境中的数据完整性。
> - **阻塞和非阻塞操作**：任务可以选择等待直到有空间或数据可用，或者立即返回，增加了灵活性。
> - **优先级继承**：`FreeRTOS`支持基于优先级的消息传递，确保高优先级任务在队列操作期间不会被低优先级任务阻塞，这对于实时系统中的及时响应非常重要。
> - **可变长度项**：队列中的项可以是不同长度的数据块，增加了队列的灵活性，可以适应不同大小的数据。

**对于使用 STM32CUBEMX 来说我们可以去看 `cmsis_os.c`** 文件 封装的 API 函数

#### 1. 创建队列

![image-20250223142841356](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250223142841356.png)

#### 2. 写入消息

![image-20250223142903136](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250223142903136.png)

![image-20250223154212940](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250223154212940.png)

#### 3. 读出消息

![image-20250223143019556](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250223143019556.png)

![image-20250223154341847](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250223154341847.png)

## 4. 信号量

> 和多线程的锁特别的像

#### 1. 信号量和队列的区别和联系

![image-20250224102155793](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250224102155793.png)

#### 2. 二值信号量

![image-20250224102405254](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250224102405254.png)

##### 1. 二值信号量的函数 API

> 最好使用 `vSem...` 版本的, 因为会主动释放自己创建的资源

![image-20250224103402763](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250224103402763.png)

##### 2. 互斥信号量

> 优先级反转问题

![image-20250224151610896](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250224151610896.png)

> 强制抬搞任务的优先级

![image-20250224143338749](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250224143338749.png)

> 相关 API

![image-20250224143616462](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250224143616462.png)

<br>



## 5.事件组

---

### 1. 事件组介绍

![image-20250302134512446](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250302134512446.png)

### 2. 常用的 API

![image-20250302134334033](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250302134334033.png)

![image-20250302135452650](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250302135452650.png)

### 3. Demo

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#define NUM0_BIT BIT0
#define NUM1_BIT BIT1

static EventGroupHandle_t task_event;

void taskA(void *parm)
{
    while (1)
    {
        xEventGroupSetBits(task_event, NUM0_BIT);
        vTaskDelay(pdMS_TO_TICKS(1000));
        xEventGroupSetBits(task_event, NUM1_BIT);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void taskB(void *parm)
{
    EventBits_t ev;
    while (1)
    {
        ev = xEventGroupWaitBits(
            task_event,
            NUM0_BIT | NUM1_BIT,
            pdTRUE,
            pdFALSE,
            pdMS_TO_TICKS(5000));

        if (ev & NUM0_BIT)
        {
            ESP_LOGI("ev", "get Num0");
        }
        if (ev & NUM1_BIT)
        {
            ESP_LOGI("ev", "get Num1");
        }
    }
}

void app_main(void)
{
    task_event = xEventGroupCreate();
    xTaskCreatePinnedToCore(taskA, "Task A", 2048, NULL, 3, NULL, 1);
    xTaskCreatePinnedToCore(taskB, "Task B", 2048, NULL, 3, NULL, 1);
}

```



## 6. 定时器

---

![image-20250303125309327](C:\Users\shuhe\AppData\Roaming\Typora\typora-user-images\image-20250303125309327.png)

### 1. API

![image-20250303125353663](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250303125353663.png)

# Ex. 快速工程配置

---

## 1. 快速配置

> `stm32`

```cpp
#include "FreeRTOS.h"
#include "main.h"
#include "task.h"

#include <stdio.h>
#include <strings.h>

#include "queue.h"

#define STACK_TASK_STACK 128
#define STACK_TASK_PRIORITY 1


// region init
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */
void start_task(void);
TaskHandle_t start_task_handler;
// endregion


// region task

/* config for task one */
#define TASK_ONE_STACK 128
#define TASK_ONE_PRIORITY 1
void task_one(void *pvParam);
TaskHandle_t task_one_handler;


/* config for task tow */
#define TASK_TWO_STACK 128
#define TASK_TWO_PRIORITY 2
void task_two(void *pvParam);
TaskHandle_t task_two_handler;


/* config for task three */
#define TASK_Three_STACK 128
#define TASK_Three_PRIORITY 2
void task_three(void *pvParam);
TaskHandle_t task_three_handler;
// endregion


/* 队列句柄 */
QueueHandle_t queue1;
QueueHandle_t big_queue;

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
    // 创建好队列
    queue1 = xQueueCreate(2, sizeof(uint8_t));
    if (queue1 == NULL) {
        printf("Error creating queue\r\n");
    } else {
        printf("Created queue\r\n");
    }
    big_queue = xQueueCreate(1, sizeof(char *));
    if (big_queue == NULL) {
        printf("Error creating queue\r\n");
    } else {
        printf("Created queue\r\n");
    }

    // 创建一个启动任务
    xTaskCreate((TaskFunction_t) start_task, (const char *const) "start_task",
                (configSTACK_DEPTH_TYPE) STACK_TASK_STACK, (void *) NULL,
                (UBaseType_t) STACK_TASK_PRIORITY, (TaskHandle_t *) &start_task_handler);
    // 开启任务调度器
    vTaskStartScheduler();
}

void start_task(void) {
    xTaskCreate((TaskFunction_t) task_one, (const char *const) "task_one",
                (configSTACK_DEPTH_TYPE) TASK_ONE_STACK, (void *) NULL,
                (UBaseType_t) TASK_ONE_PRIORITY, (TaskHandle_t *) &task_one_handler);
    xTaskCreate((TaskFunction_t) task_two, (const char *const) "task_two",
                (configSTACK_DEPTH_TYPE) TASK_TWO_STACK, (void *) NULL,
                (UBaseType_t) TASK_TWO_PRIORITY, (TaskHandle_t *) &task_two_handler);
    xTaskCreate((TaskFunction_t) task_three, (const char *const) "task_three",
                (configSTACK_DEPTH_TYPE) TASK_Three_STACK, (void *) NULL,
                (UBaseType_t) TASK_Three_PRIORITY, (TaskHandle_t *) &task_three_handler);
    // 创建任务之后删除多余创建任务这个任务
    vTaskDelete(NULL);
}


void task_one(void *pvParam) {
    while (1) {
        printf("Hello World1\r\n");
        vTaskDelay(500);
    }
}
void task_two(void *pvParam) {
    while (1) {
        printf("Hello World2\r\n");
        vTaskDelay(500);
    }
}
void task_three(void *pvParam) {
    while (1) {
        printf("Hello World3\r\n");
        vTaskDelay(500);
    }
}

```

<br>

## 2. 转发 `printf `到 `usart `输出

```cpp
#include <stdarg.h>

void USART_printf(char *format, ...) {
  char buffer[256]; // 缓冲区，可以根据需要调整大小

  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args); // 格式化字符串
  va_end(args);

  // 发送格式化后的字符串
  HAL_StatusTypeDef status =
      HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 100);

  if (status != HAL_OK) {
    char waring[] = "Unable to transmit data to Uart\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t *)waring, strlen(waring), 100);
  }
}
```

## 3. 参考文献

1. [`FreeRTOS API` 参考](https://www.freertos.org/zh-cn-cmn-s/Documentation/02-Kernel/04-API-references/06-Queues/00-QueueManagement)
2. [网络串口助手 + 库 (波特律动)](https://docs.baud-dance.com/)
3. [信号量的优先级反转问题](https://www.bilibili.com/video/BV1vUpje9Ej2?spm_id_from=333.788.player.switch&vd_source=b47817c1aa0db593f452034d53d4273a&p=36)
