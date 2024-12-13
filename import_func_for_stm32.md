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

## 8. `OLED `时 `GPIO` 的配置

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

> 注意配置之后是 `PA0`，还有时钟频率就是 $8MHz$ 就可以了

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

1. 占空比：总时间为 99，然后我们变化到低电平的时间是 30，所以是 30 / (99 + 1)

2. 频率：

    - 假定计数器的时钟频率是：$f_{clk} = 72\, \text{MHz}$，$T_{wave}$ 完成一个波形花费的时间

    - PWM 的频率就是计数器的更新频率（完成一次高低变换需要的时间）

    - $$
        T_{wave} = \frac{一个波段周期数}{f_{clk}} = \frac{100次}{f_{clk}} \mu s \\
        f_{PWM} = \frac{1}{T_{wave}}
        $$

3. 分辨率就是直接 30 作为 1 $\rightarrow$ 1 / (99+1)
