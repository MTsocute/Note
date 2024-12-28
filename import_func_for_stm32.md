<img src="https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/01a4e695a592ffe752d36e4ece118b1874a9adcee4.jpg" alt="01a4e695a592ffe752d36e4ece118b1874a9adcee4" style="zoom:50%;" />

# `STM 32` 杂项笔记

# 1. 基础配置

---

## 1. 烧录文件的配置内容

> 烧录文件配置：`stlink.cfg`

```cfg
# choose st-link/j-link/dap-link etc.
#adapter driver cmsis-dap
#transport select swd
source [find interface/stlink.cfg]
transport select hla_swd
source [find target/stm32f1x.cfg]
# download speed = 10MHz
adapter speed 10000
```

## 2. `STLink-V2` 的接线图

![2-1 工程模板](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/2-1%20%E5%B7%A5%E7%A8%8B%E6%A8%A1%E6%9D%BF.jpg)

## ==3. 基本 GIPO Pin 参数的介绍==

> 这段代码是STM32中使用HAL库配置GPIO（通用输入输出）引脚的例子
>
> ## `GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;`
>
> - Mode：设置GPIO引脚的工作模式。
>     - `GPIO_MODE_OUTPUT_OD`表示该引脚被配置为**开漏输出**（Open-Drain）。
>     - 开漏输出模式意味着引脚可以输出低电平或悬空状态，而不能输出高电平（在输出低电平时，外部电路需要提供拉高电平的电压）。这种模式常用于I2C等总线协议中，或者需要连接到外部电源的设备。
>     - 其他常见模式：
>         - `GPIO_MODE_INPUT`: 输入模式
>         - `GPIO_MODE_OUTPUT_PP`: 推挽输出模式（即可以输出高电平或低电平）
>         - `GPIO_MODE_AF`: 复用功能模式
>         - `GPIO_MODE_ANALOG`: 模拟模式
>
> ## `GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;`
>
> - Speed：设置GPIO引脚的输出速度（即信号的切换速率）。
>     - `GPIO_SPEED_FREQ_LOW`表示低速模式，适用于不需要频繁变化的信号。
>     - 其他常见选项：
>         - `GPIO_SPEED_FREQ_MEDIUM`: 中速模式
>         - `GPIO_SPEED_FREQ_HIGH`: 高速模式（适用于需要快速切换的信号，如高速通信）
>
> ## `GPIO_InitStruct.Pull = GPIO_NOPULL;`
>
> - Pull：设置引脚的**上拉/下拉**电阻配置。
>     - `GPIO_NOPULL`表示该引脚没有任何上拉或下拉电阻（即没有将引脚连接到电源或地）。
>     - `GPIO_PULLUP`: 启用上拉电阻，将引脚连接到VCC（电源）。
>     - `GPIO_PULLDOWN`: 启用下拉电阻，将引脚连接到GND（地）。
> - **上拉电阻 (PULLUP)**：将引脚默认设置为高电平，空闲状态逻辑 1。
> - **下拉电阻 (PULLDOWN)**：将引脚默认设置为低电平，空闲状态逻辑 0。

```cpp
/*Configure GPIO pin : PC13 */
GPIO_InitStruct.Pin = GPIO_PIN_13;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


/*Configure GPIO pin : PA9 */
GPIO_InitStruct.Pin = GPIO_PIN_9;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
```

## 4. GIPO 开漏和推挽模式的区别

> 在推挽模式下面，我们的高低电平都有驱动能力
>
> 但是开漏模式下面，不太一样，只有**低电平**有驱动能力
>
> **所以如果你希望有更好的控制，应该设置成开漏模式**

![image-20241201213834232](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241201213834232.png)

## 5.GPIO 脚针的 PIN 口的定义

```cpp
// 0000 0000 0000 0001
#define GPIO_PIN_0                 ((uint16_t)0x0001)  /* Pin 0 selected    */
// 0000 0000 0000 0010
#define GPIO_PIN_1                 ((uint16_t)0x0002)  /* Pin 1 selected    */
#define GPIO_PIN_2                 ((uint16_t)0x0004)  /* Pin 2 selected    */
#define GPIO_PIN_3                 ((uint16_t)0x0008)  /* Pin 3 selected    */
#define GPIO_PIN_4                 ((uint16_t)0x0010)  /* Pin 4 selected    */
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
#define GPIO_PIN_6                 ((uint16_t)0x0040)  /* Pin 6 selected    */
#define GPIO_PIN_7                 ((uint16_t)0x0080)  /* Pin 7 selected    */
#define GPIO_PIN_8                 ((uint16_t)0x0100)  /* Pin 8 selected    */
#define GPIO_PIN_9                 ((uint16_t)0x0200)  /* Pin 9 selected    */
#define GPIO_PIN_10                ((uint16_t)0x0400)  /* Pin 10 selected   */
#define GPIO_PIN_11                ((uint16_t)0x0800)  /* Pin 11 selected   */
#define GPIO_PIN_12                ((uint16_t)0x1000)  /* Pin 12 selected   */
#define GPIO_PIN_13                ((uint16_t)0x2000)  /* Pin 13 selected   */
#define GPIO_PIN_14                ((uint16_t)0x4000)  /* Pin 14 selected   */
#define GPIO_PIN_15                ((uint16_t)0x8000)  /* Pin 15 selected   */
#define GPIO_PIN_All               ((uint16_t)0xFFFF)  /* All pins selected */
```

## 6. Debug 文件配置

![image-20241206180242134](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241206180242134.png)

## 7. 事件的计算

> 执行 72 次任务，然后我们的最大时钟是 $72MHz$，所以执行 72 次任务就是需要 $1\mu s = 10^{-6} s$ 时间
>
> 最大定时时间的计算
>
> 72M / 65536 代表每一个数字的频率，共65536个，每个频率就是72M/65536，然后取倒数就得到计数每一个数的时间。（f=1/T）再乘以 65536 个数字，就得到计数的总时间。

## 8. `OLED` 时 `GPIO` 的配置

```c
/*Configure GPIO pins : SCL_Pin SDA_Pin */
GPIO_InitStruct.Pin = SCL_Pin | SDA_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;		// 主要是这里是开漏
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
```

## 9. `TIM` 内部中断配置介绍

**`Prescaler = 7200 - 1`:**

- 定时器时钟的分频系数。

- 假设 STM32 的主时钟频率为 72 MHz，那么定时器的时钟频率为： 

- 
    $$
    f_{\text{TIM}}= \frac{f_{clock}}{Prescaler+1} = \frac{72 MHz}{(7200-1)+1} = 10 \, \text{kHz}
    $$

- 即定时器的计数频率为 `10 kHz`，每个计数周期为 `0.1 毫秒`。

**`Period = 10000 - 1`:**

- 定时器的自动重装值（ARR）。

- 当计数器值到达 `Period` 时，计数器会溢出并触发中断。

- 因此，定时器的溢出周期为： 

- 

- $$
    T_{\text{overflow}} = \frac{\text{Period} + 1}{f_{\text{TIM}}} = \frac{(10000-1)+1}{10 \, \text{kHz}} = 1 \, \text{秒}
    $$

```c
htim2.Instance = TIM2;
htim2.Init.Prescaler = 7200 - 1;		// 最重要的部分
htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
htim2.Init.Period = 10000 - 1;			// *同上
htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
```

## 10. 外部 TIM 配置

> 首先需要打开外部时钟的配置

![image-20241213165229717](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241213165229717.png)

> 然后配置两个关键参数，就是 `Prescale` 和 `Period`
>
> 第一个参数我们配置成为 0，因为外面触发一次我们就该记录一次，然后 10 个我们算一组 设置 `Period `为 10

![image-20241213165636780](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241213165636780.png)

## 11. PWM 配置

> 使用内部时钟来生成 `PWM` 波
>
> 记得不要开启 NVIC Setting

<img src="https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241215204320479.png" alt="image-20241215204320479" style="zoom:77%;" />

> 具体的波形设置，参数为什么这么取可以参照 3.2 部分
>
> $PSC: 720-1$
>
> $ARR : 100 - 1$
>
> $CCR : 20$，这个可以在代码中用参数调整也可以的

![image-20241215204629235](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241215204629235.png)

> `PWM Generation Channel 1` 的参数，这里具体再说明一下

- `Counter Mode` 这个参数影响`CNT`的计数方式
- `PWM Mode` 这个参数影响比较`CNT`和`Pulse`的比较方式
    - `mode = 1`时，当 `CNT < Pulse` 时，比较结果为 1、反之为 0
    - `mode = 2`时，差不多就是比较结果反一反
- `CH Polarity` 这个参数影响信号输出电平
    - 设置为 `High` 时，当比较结果为 1 时，输出高电平，结果为 0 时，输出低电平
    - 设置为 `Low` 时则输出电平反一反

## 12. IC 模式配置

> 我们这里使用 TIM 3 来捕获外部输入(IC)
>
> 顺带一提 IO 复用表，我们使用 Channel 1，也就是 PA6，捕获上升沿

![image-20241225113746700](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241225113746700.png)

> 我们要周期重置计数，所以我们需要利用信号和槽的机制，也就是主从模式，于是 CUBE 配置如下
>
> 从机模式的打开通道用 FP1，从机模式用于重置计数，选择计数高电平，捕获上升沿（Rising Edge），默认配置就是

![image-20241225120239533](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241225120239533.png)

> 捕获是需要打开全局中断的

![image-20241225120305878](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241225120305878.png)

> **对于 `Counter Period` ，我们这里解释下为什么是最大值，我们知道，这个是 ARR，我们一次计数的事件是 $1\mu s$，所以这个 ARR 相当决定了我们最大的计数时间，防止外部周期时间过长，等不到下一次的上升沿**
>
> **对应的最大时间的倒数，就是频率了，也就是说，我们 $f = \frac{1MHz}{65536}$ 就是我们可以测得最大的频率了，更小就不可以了**
>
> 所以你可以降低 PSC，然后 1MHz 会变小（除数变大了嘛），然后就可以测更小的频率了

### 1. PWMI 配置

> 为实现我们的 3.4 的 2 PWMI 效果，我们要打开同一个定时器的交叉捕获，存储多个数值

![image-20241225133322284](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241225133322284.png)

### 2. 编码器 配置



# 2. HAL 库函数

---

## 2.1 GPIO

### 1. `GPIO_Write_Pin()`

<img src="https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241201203842396.png" alt="image-20241201203842396" style="zoom:70%;" />

```cpp
/*Configure GPIO pin Output Level */
HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

/*Configure GPIO pin Output Level */
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
```

### 2. `HAL_GPIO_ReadPin()`

> 用于读取某个 GPIO 引脚的输入状态。它通常用于检测外部信号的电平（高电平或低电平）
>
> - **`GPIO_PIN_SET`** 表示引脚的电平为高，通常对应外部电压为逻辑高电平（如 3.3V 或 5V）。
> - **`GPIO_PIN_RESET`** 表示引脚的电平为低，通常对应外部电压为逻辑低电平（如 0V）。
>
> 如果你需要设置接受输出的，记得弄成下面的**INPUT**模式才可以读取外部的数据
>
> 上拉输入：若GPIO引脚配置为上拉输入模式，在默认情况下(GPIO引脚无输入)，读取 得的GPIO引脚数据位1，高电平。

```cpp
GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;       // 输入模式
GPIO_InitStruct.Pull = GPIO_PULLUP;           // 上拉，这模式下灯亮为 1
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
```

参数说明：

1. **`GPIOx`**:
    - 这是指向目标 GPIO 端口的指针，`GPIOx` 是端口的基地址。STM32 的每个 GPIO 端口都有一个不同的地址（如 `GPIOA`、`GPIOB`、`GPIOC` 等）。
    - 例如，`GPIOA` 表示 STM32 的 `GPIOA` 端口，`GPIOB` 表示 `GPIOB` 端口，等等。
2. **`GPIO_Pin`**:
    - 这是你要读取的具体引脚编号。STM32 的每个 GPIO 端口通常有多个引脚（例如，`GPIO_PIN_0`、`GPIO_PIN_1` 等），用来表示每个引脚的编号。
    - 例如，`GPIO_PIN_0` 表示端口中的第一个引脚，`GPIO_PIN_1` 表示第二个引脚，以此类推。

返回值：

- **`GPIO_PIN_SET`**: 如果目标引脚的电平为高电平（1），则返回 `GPIO_PIN_SET`。
- **`GPIO_PIN_RESET`**: 如果目标引脚的电平为低电平（0），则返回 `GPIO_PIN_RESET`。

```cpp
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
```

<br>

## 2.2 中断

---

### 1. EXIT 外部中断函数

![image-20241213171321715](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241213171321715.png)

> **`__HAL_GPIO_EXTI_GET_IT(Sensor_Count_Pin)`**: 用来检查外部中断的标志位是否被触发
>
> **`HAL_GPIO_EXTI_IRQHandler(Sensor_Count_Pin)`**: 清除外部中断的挂起标志

```c
void EXTI9_5_IRQHandler(void) {
    // 如果发生中断的话
    if (__HAL_GPIO_EXTI_GET_IT(Sensor_Count_Pin)) {
        HAL_GPIO_EXTI_IRQHandler(Sensor_Count_Pin);
        IR_Sensing_Count ++;	// 不重要
    }
}
```

### 2. TIM 内部时钟中断

> 定时器的开启语句，使用内部时钟计时
>
> ```c
> HAL_TIM_Base_Start_IT(&htim2); // 初始化 TIMER2 定时器
> ```
>
> 找到我们 TIM 文件中的内部中断函数，并在 `main.c` 中重构它

![image-20241213121049477](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241213121049477.png)

> `main.c` 重构

![image-20241213121445243](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241213121445243.png)

> - `htim` 是中断触发时传入的参数，表示当前触发中断的定时器的句柄
> -  所以这里的意识是 htim2 这个地址的定时器完成了一个周期，触发了中断

```c
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim2) {
        // ...
    }
}
```

### 3. TIM 外部时钟中断

> 注意配置之后是 `PA0`，还有时钟频率就是 8MHz 就可以了

![image-20241213172427731](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241213172427731.png)

> 因为外部时钟接受的引脚是 `PA0`，所以 `DO` 就接到上面

![image-20241213172804137](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241213172804137.png)

### ==4. `EXIT` 外部中断 和 `TIM` 外部时钟的区别==

> **EXTI：** 按键检测，按下触发中断。
>
> **TIM 外部时钟：** 测量外部信号频率，统计脉冲个数。

|   特性   |      EXTI（外部中断）      |          TIM 外部时钟          |
| :------: | :------------------------: | :----------------------------: |
|   功能   | 检测引脚状态变化，触发中断 |  使用外部信号作为定时器时钟源  |
| 触发方式 |   上升沿、下降沿或双边沿   |          外部脉冲信号          |
| 应用场景 |     按键检测、事件响应     | 频率测量、脉冲计数、编码器测速 |
| 是否计数 |     不计数，只响应事件     |    可计数，基于外部信号驱动    |
|  优缺点  |   响应快，适合一次性事件   |  配置复杂，适合周期性信号处理  |

<br>
### 5. PWM 

> 我们在 `1.基础配置` 的 `11.PWM 配置` 中又说，我们可以把占空比的设置交给代码中实现，我们可以手动要设置

![image-20241215211112025](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241215211112025.png)

配置之后，我们的代码就会生成在对应的 `stm321xx_hal_tim.c` 文件里面，有一个对应的 `HAL_TIM_Start/Stop(...)` 函数了

```c
// main.c 调用的方法
HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_1);	// TIM_CHANNEL for IO remap

// while 部分自己定义占空比
while {
   // 使用代码来控制占空比，实现呼吸灯的效果
    for (int i = 0; i < 100; ++i) {
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, i); 	// 让低电平越来越少
        HAL_Delay(10);
    }

    for (int i = 0; i < 100; ++i) {
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 100-i);
        HAL_Delay(10);
    }
}
```

> 顺便说一下为什么要延迟 `10 ms` 就是我们完成一个高低变换的周期是 1ms，如果你按照我的配置，这个灯的变换的整个过程 100 也才 `100 ms`，所以变化是非常快的，有延迟的的话，每一次改变占空比的结果至让你看个 `10 ms`，就不会太快

#### 5.1 引脚重分配表

> 实现下面四种定时器（或者别的功能）的GPIO重分配
>
> [具体改变和更内在的看看这个视频](https://www.bilibili.com/video/BV1pZ421W7hY/?spm_id_from=333.337.search-card.all.click&vd_source=b47817c1aa0db593f452034d53d4273a)
>
> 我们的 STM32CUBE 配置更简单，甚至都不需要代码中实现，只需要开启 TIM，然后选对应引脚打开就好
>
> 更多的内容在表的 $8.3.7$ 这个部分

![image-20241215215859532](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241215215859532.png)

> 你可以在这个启用部分通过 `Channel` 来改变

```cpp
/**
  * @brief  Starts the PWM signal generation.
  * @param  htim TIM handle
  * @param  Channel TIM Channels to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
```

### 6. IC 

> `HAL_TIM_IC_CaptureCallback()` 这个函数会在每次捕获事件之后进行操作，也就是我们的槽函数本体，对于其内部的执行的逻辑，我们可以重写来重定义我们想要的结果
>
> 其捕捉的信号，我们在 CUBE 也配置过了，就是上升沿
>
> 由于我们想要计算出捕获信号的频率，所以我们需要重写这个部分

```cpp
int32_t freq;
// 这个函数的定义在 `stm32f1xx_hal_tim.h` 中
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    // 只捕捉 TIM 3 的上升沿
    if (htim->Instance == TIM3) {
        uint32_t capture = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
        freq = 1e6 / capture;
    }
}
```

> 初始化开启 IC

```cpp
HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);        // Initialize IC
```

> 多个通道记得打开其他的通道的中断

```cpp
// HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);        // Initialize IC
// 更好的方案，如果有多个的话
HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_ALL);
```

- 对于高电平的部分，是否转到低电平，我们可以根据哪个通道激活来判断
- 通道的激活本身就设立了对高低沿检测，所以正好

```cpp
uint32_t freq, Duty, highCNT, period;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM3) {
        // 只捕捉 TIM 3 的上升沿
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
            period = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1) + 1;
            freq = 1e6 / period;
        }
        // 只捕捉 TIM 3 下降沿
        else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
            highCNT = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2) + 1;
            // 算出占空比
            Duty = highCNT * 100 / period;
        }
    }
}
```

### 7. Encode

> 其实没啥好说的，主要就是这个部分

```cpp
HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1); // Initialize IC
HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_2); // Initialize IC

OLED_ShowString(2, 1, "CNT:");

while (1) {
    OLED_ShowSignedNum(2, 5, (int16_t) __HAL_TIM_GET_COUNTER(&htim3), 5);
    HAL_Delay(100);
}
```




# 3. 硬件编码原理

---

> 这里提供主要的驱动模块，而是每一个驱动的编写原理

## 3.1 PWM 波形

> 看最右边的图，一个周期的时间就是： $T_S = T_{ON} + T_{OFF}$，我们需要做的就是这个周期中控制 ON 和 OFF 所占时间的多少
>
> ON 占用一个周期的时间就叫作为占空比，如果占空比越大，那么高电平再这个周期的时间就越大
>
> 举个例子：占空比有1%，2%，3%......那么它的分辨率就是1%

![image-20241213191610824](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241213191610824.png)

> 输出比较模式的选择

![image-20241213192614929](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241213192614929.png)

> 我们只需要掌握两个模式
>
> - 第一个就是翻转，这个可以输出**高低电频**花销时间都一样的波形
>
> - PWM，1 2 两个模式唯一的不同就是 1  0 相反

- 参数和原理图

> 红线之前都是 1 之后都是 0，直到遇到黄线

<img src="https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241213192832139.png" alt="image-20241213192832139" style="zoom:67%;" />

1. 占空比：总时间为 99，高电平占用的计数是 30，所以是 30 / (99 + 1)

2. 频率：

    - 假定计数器的时钟频率是：$f_{clk} = 72\, \text{MHz}$，`PSC =71`用于化整频率

    - PWM 的频率就是完成一次 ARR 所需要的频率单位时间

    - $$
        Freq = \frac{\frac{f_{clk}}{PSC + 1}}{ARR + 1}
        $$
    
3. 分辨率就是直接 30 作为 1 $\rightarrow$ 1 / (99+1)



## 3.2  按参数生成 PWM 波形

> 这里一个周期是 `1ms` 哈

<img src="https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241215202802169.png" alt="image-20241215202802169" style="zoom:122%;" />

## 3.3 舵机 PWM 控制原理

> 舵机的先对应的接口关系

![image-20241216133327340](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241216133327340.png)

> 占空比和旋转角度的关系

<div style="display: flex; justify-content: space-between; align-items: center; width: 100%;">
    <img src="https://i-blog.csdnimg.cn/blog_migrate/8487efa01529a2b26ce8e2aadd58f7f3.png" alt="img" style="width: 49%;"/>
    <img src="https://i-blog.csdnimg.cn/blog_migrate/318656994c810caec4dbade7ec889493.gif" alt="img" style="width: 49%;"/>
</div>


### 1. 参数配置

> 舵机的 PWM 必须是 `20 ms`（`50Hz`），高平电的宽度必须在 `0.5~2.5 ms (2.5%~12.5%)` 之间，所以这个就直接的确定了我们的占空比和频率，由此我们计算出对应的参数，`CK_PSC = 72M`
>
> 1. $Freq = \frac{CK\_PSC}{PSC + 1} \times (ARR+1) = 50Hz$，也可以直接算时间来算 ARR
> 2. $Duty = \frac{CRR}{ARR + 1} \times100 = 2.5 \to 12.5$
>
> 确定我们参数 ARR = 20000，CRR = 500 ~ 2500（也可以再代码中调试）
>
> 同时我们没有 IO 复用所以看对应的 PWM 5.1 表，打开对应的 PA1 开启通道 2

![image-20241216142104604](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241216142104604.png)

## 3.4  输入捕获

> 因为第一时间存储到的是cache，所以程序读取时间是存储的，我们不需要那么着急去马上读取计数器里面的值

<img src="https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241225092204464.png" alt="image-20241225092204464" style="zoom:67%;" />

### 1. 频率测量的方法

![image-20241225101839273](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241225101839273.png)

- 第一种方法就是左边的部分，我们指定一个时间长度 T，测这个时间内的高频次数，然后用次数除以对应 T，然后就可以得到频率了
- 第二种方法就是两个上升沿之间持续的时间，然后取倒数，然后因为，我们的计算持续时间的方法就是计数器，所以我们单位时间其实是 $\frac{1}{f_c}$，知道多少次计数（N），我们就可以知道持续时间为 $\frac{N}{f_c}$，取倒数就是频率

> 上面两个信号试用的范围比较狭隘，一个对于周期短(高频)的好，一个对于周期长(低频)的好，对于周期不定长的
>
> 这就涉及到一个问题，对于不同的情况，我们选用不同的方法，**可是多高算高算低啊？**
>
> 于是就有上面第三个算法

- 第三种方法的推导：
    $$
    \begin{align*}
    & \frac{N}{T} = \frac{f_c}{N} \\
    & N^2 = f_c \times T \\
    & f_m = \frac{N}{T} = \sqrt{\frac{f_c}{T}} 
    \end{align*}
    $$
    

### 2. PWMI

> 我们需要存储两个数值，占空比和频率，但是一个 TIM 的 cache 只有一个
>
> 要存储两个数据必须要有两个存储器，那么我们可以借用别人的存储器，来存储数据，所以就有了 FP 这个线路来用别人的存储器
>
> 对应的我们需要在代码中配置这个交叉模式，具体看配置部分

![image-20241225111727135](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241225111727135.png)

> 我们实现占空比的原理就是计一个周期内的高平计数，然后到低平马上保留那个数到寄存器，也就是这里的 CCR2 = CNT

![image-20241225133506086](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241225133506086.png)

### ==3. 主从模式==

> 先解释下，为什么我们要使用这个模式，首先我们的测量使用的周测法，所以我们希望记录下一个周期需要清空计数器
>
> 就是下面的 CNT = 0

![image-20241225114304544](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241225114304544.png)

> 那我们是如何自动实现这个清楚的呢？就是依靠下面这个电路。
>
> 也不需要知道太多，就是知道有这个通道所以有主从模式就行，后面主从模式是啥才比较重要些
>
> 还有那个 `T1FP1`

<img src="https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241225114653307.png" alt="image-20241225114653307" style="zoom:80%;" />

> 先看下主从模式的图表
>
> 这个和 QT 的信号和槽机制如出一辙，主模式的信号通过 TRGO 传递出去，然后从模式相当于就是槽函数执行具体的操作 

![image-20241225114756972](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241225114756972.png)

> 比较有趣的是，Channel 3 和 4 还没有从模式，他们没有 XOR

![image-20241225132549566](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241225132549566.png)

### 4. 编码器

> 先来看如何开启编码器，是的我们还是可以用这两个触发器来触发，然后我们再使用两个通道来使用 TIM 就可以获取两个数值的变化
>
> [具体详细看这个视频](https://www.bilibili.com/video/BV1im42137c2/?spm_id_from=333.337.search-card.all.click&vd_source=b47817c1aa0db593f452034d53d4273a)

![image-20241225140451822](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241225140451822.png)

> 我们记录到的两个数据就是下面的两个图，在介绍这个图相位对应的实际含义之前，我们先看下硬件的部分

<img src="https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241225140302288.png" alt="image-20241225140302288" style="zoom:67%;" />

> 旋钮数据输出的接口

![image-20241225140806956](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241225140806956.png)

<br>
