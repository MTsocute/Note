# `STM 32` 杂项笔记

---

- 笔记本非常的杂乱，前期是了别翻阅文件拷贝相同代码而用的，所以有点乱，现在主要分成三块

1. 实现某功能如何配置 GPIO 
2. 实现所需功能使用对应的 HAL 的库
3.  [单片机]() 实现这个功能所需要的基本原理，主要是为了告诉参数是对应什么，不会太深入

- 参考资料

1. [`stm32 + freeRTOS`](https://www.bilibili.com/video/BV1sE411d7x7/?spm_id_from=333.337.search-card.all.click&vd_source=b47817c1aa0db593f452034d53d4273a)
2. [`stm32 如何使用 C++ 开发`](https://www.bilibili.com/video/BV1KZ4y1B7DH/?spm_id_from=333.1387.favlist.content.click)
3. [`smt32 openOCD 开发环境`](https://blog.csdn.net/qq_46226142/article/details/142715291?spm=1001.2014.3001.5501)
4. [高版本的 `arm-none-gcc` 编译器](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)

# 1. 基础配置

---

## 1. 烧录文件的配置内容

> 烧录文件配置：`stlink.cfg`
>
> 适用于 `stm32f103c8t6`

```ruby
# choose st-link/j-link/dap-link etc.
# adapter driver cmsis-dap
# transport select swd
source [find interface/stlink.cfg]
transport select hla_swd
source [find target/stm32f1x.cfg]
# download speed = 10MHz
adapter speed 10000
```

## 2. `STLink-V2` 的接线图

![2-1 工程模板](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/2-1%20%E5%B7%A5%E7%A8%8B%E6%A8%A1%E6%9D%BF.jpg)

## 3. 基本 GIPO Pin 参数的介绍

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
>     - /其他常见选项：
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

## 4. PlatformIO 配置

---

### 1. `pio` 安装 stm32f103c8 的库

> Board: STM32F103C8 (20k RAM, 64k Flash) (Generic)
>
> 先用 CUBEMX 生成文件再用 PIO

### 2. `ini` 文件配置

```ini
[env:genericSTM32F103C8]
platform = ststm32
board = genericSTM32F103C8
framework = stm32cube
upload_protocol = stlink
debug_tool = stlink

[platformio]
include_dir = ./Core/Inc
src_dir = ./Core/Src
```

## 3.GPIO 脚针的 PIN 口的定义

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

![image-20250221171858866](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250221171858866.png)

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

> 开启定时器中断

![image-20250221171933230](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250221171933230.png)

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

<br>

## 13. AD 转换

---

### 1. 引脚的配置

![image-20241228164159722](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241228164159722.png)

> `ADC Settings` 的部分
>
> - Data Alignment：数据的对齐
> - Scan Conversion：是否支持多通道
> - Continuous Conversion：转换后是否自动转换
>
> `ADC_Regular_ConversionMode` 的部分
>
> - Number of Conversion：读取数据的通道的数目

![image-20241228165147384](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241228165147384.png)

### 2. 时钟频率的设置

> ![image-20241228165425521](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241228165425521.png)

![image-20241228165317275](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241228165317275.png)

> 所以我们需要分频器，让我们的频率 < 14 MHz

## 14. DMA 配置

### 1. M2M

> 内存和内存之间的传递数据

![image-20241229204621455](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241229204621455.png)

### 2. 多通道

> ADC 多通道设置

![image-20241229213129801](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241229213129801.png)

> 记得不同的通道指定不同哈，不然不会读取的

![image-20241229235024273](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241229235024273.png)

### 3. P2M

> 搭配 DMA 使用，ADC 1 触发

![image-20241229214212382](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241229214212382.png)



## 15. USART 配置

> `Parity` 的意思是校验码
>
> 对于接受部分记得打开，NVIC 中断，然后重写 `void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);` 这个部分

![image-20250104212812859](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250104212812859.png)

![image-20250104183845273](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250104183845273.png)

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

### 3.  获取外部按钮

> 按钮默认上拉模式

```c
// 按一下，返回一次的模式
uint8_t getButton(void) {
    static uint8_t last_button_state = GPIO_PIN_SET;
    uint8_t current_button_state = HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin);

    // 这行代码表示检测按钮的按下瞬间
    if (last_button_state == GPIO_PIN_SET && current_button_state == GPIO_PIN_RESET) {
        HAL_Delay(50);  // 消抖
        // 消除抖动后的确认是否按下
        if (HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin) == GPIO_PIN_RESET) {
            last_button_state = GPIO_PIN_RESET;		// 确认是按下了，那么上一时刻为按下
            return 1;  // 按钮按下时返回 1
        }
    }
    // 如果按钮未按下或已经松开，更新 last_button_state，使其保持与当前状态一致，返回 0
    last_button_state = current_button_state;
    return 0;  // 未按下返回 0
}
```

<br>

## 2.2 定时器

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

### 4. EXIT 外部中断 和 TIM 外部时钟的区别

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

#### 1. 使用 HAL 复现一个 Arduino 的 `pulseIn(pin, level, timeout)`

> - 我们设置 PSC = 72，然后 ARR 设最大
>- 那么我们 ARR 完成一次 + 1 的时间就是 1 us
> - 然后我们就利用`htim6`记录了多少数，来判断从开始计数到停止开销的时间

```c
extern TIM_HandleTypeDef htim6;
// 获取定时器当前的记数
static uint16_t micros(void) { return __HAL_TIM_GET_COUNTER(&htim6); }

static void delay_us(uint16_t us){
    uint16_t t0 = micros();
    while ((uint16_t)(micros() - t0) < us) ;
}

/**
 * @brief  等待并测量指定引脚出现某电平的持续时间
 *
 * 1. 先等到引脚电平 == level（超时直接返回 0）
 * 2. 再测量该电平保持多久，直到电平翻转或超时
 *
 * @param port_type  GPIO 端口（如 GPIOA）
 * @param pin        引脚号（如 GPIO_PIN_0）
 * @param level      要等待的电平：HAL_GPIO_SET | RESET
 * @param timeout_us 最大等待时间（微秒）
 *
 * @retval 0          超时
 * @retval > 0        指定电平实际持续的时间（微秒）
 */
static uint16_t pulseIn(GPIO_TypeDef *port_type, uint16_t pin, uint8_t level, uint16_t timeout_us)
{
    uint16_t t0;

    /* 等待与 level 相同的电平出现（或超时） */
    t0 = micros();
    while (HAL_GPIO_ReadPin(port_type, pin) != level) {
        if ((uint16_t)((micros() - t0) > timeout_us)) return 0;
    }

    /* 测量该电平持续的时间（或超时） */
    t0 = micros();
    while (HAL_GPIO_ReadPin(port_type, pin) == level) {
        if ((uint16_t)((micros() - t0) > timeout_us)) return 0;
    }

    return micros() - t0;
}
```

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

<br>

> 实现下面四种定时器（或者别的功能）的GPIO重分配
>
> [具体改变和更内在的看看这个视频](https://www.bilibili.com/video/BV1pZ421W7hY/?spm_id_from=333.337.search-card.all.click&vd_source=b47817c1aa0db593f452034d53d4273a)
>
> 我们的 STM32CUBE 配置更简单，甚至都不需要代码中实现，只需要开启 TIM，然后选对应引脚打开就好
>
> 更多的内容在表的 $8.3.7$ 这个部分

![image-20241215215859532](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241215215859532.png)

> [!warning]
>
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

## 2.3 ADC 

> 校正 ADC

```cpp
HAL_ADCEx_Calibration_Start(&hadc1);
```

> 读取 ADC 的函数（）

### 1. 单次非扫描

```cpp
uint16_t Ret;
HAL_StatusTypeDef HalState;
uint16_t StartAndGetOneResult() {
    HAL_ADC_Start(&hadc1);                          	// 1. 启动 ADC，在 whlie 部分不断再启动
    HalState = HAL_ADC_PollForConversion(&hadc1, 10); 	// 2. 等待 ADC 转换完成 (最多等待10ms)

    if (HalState == HAL_OK) {                         // 3. 如果转换成功
        Ret = HAL_ADC_GetValue(&hadc1);               // 4. 读取 ADC 转换结果
    } else {
        Ret = 0;                                      // 5. 转换失败，返回 0
    }
    return Ret;                                       // 6. 返回 ADC 结果
} 
```

### 2. 连续非扫描

> 只需要开始一次，也不需要判断转换完成与否，因为我们不需要等待了，转换之后就马上下一次了，判断的逻辑就完全去掉
>
> 甚至都不需要下面的函数的

```cpp
HAL_ADC_Start(&hadc1);      // 执行一次就可以了，后面会自动来的
while (1) {
    ADValue = HAL_ADC_GetValue(&hadc1); // 获取读取到的 AD 值
}
```

### 3. 单次非扫描多通道

> 你可会好奇，为什么非扫描可以实现多通道
>
> 我们知道**多通道扫描是要扫描完全部通道之后，才会触发 EOC 表示 AD 转换完成**，但是没有 DMA的存在，当你读完最后一个通道的时候，前面的数据早就被覆盖了
>
> 想单一的多通道读取，他也不会完成一个扫描告诉你读完了，所以我们采用单次，读完一个扫描一个新的通道读取数据

```cpp
uint16_t StartAndGetOneResult(uint32_t adc_channel) {
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = adc_channel; // 转换我们指定的管道
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
    // 重新分配 channel
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        return 0;
    }

    HAL_ADC_Start(&hadc1);
    const HAL_StatusTypeDef result = HAL_ADC_PollForConversion(&hadc1, 10);

    if (result == HAL_OK) {
        return HAL_ADC_GetValue(&hadc1);
    }
    return 0;
}

// 读取
const uint16_t AD1 = StartAndGetOneResult(ADC_CHANNEL_0);
const uint16_t AD2 = StartAndGetOneResult(ADC_CHANNEL_1);
const uint16_t AD3 = StartAndGetOneResult(ADC_CHANNEL_2);
```

### 4. DMA 多通道

> 内存到内存之间的转运

```cpp
// 启动 DMA
HAL_DMA_Start(&hdma_memtomem_dma1_channel1, (uint32_t)DataA, (uint32_t)DataB, sizeof(DataA));

// 等待 DMA 完成
HAL_DMA_PollForTransfer(&hdma_memtomem_dma1_channel1, HAL_DMA_FULL_TRANSFER, HAL_Delay_MAX);
```

> 外设到内存的转运
>
> 在 `adc.h` 中写入 `extern DMA_HandleTypeDef hdma_adc1;`

```cpp
uint16_t AD_Value[4] = {0, 0, 0, 0};

extern DMA_HandleTypeDef hdma_adc1;		// DMA

HAL_ADCEx_Calibration_Start(&hadc1);    // 校正 ADC

HAL_StatusTypeDef status = HAL_ADC_Start_DMA(&hadc1, AD_Value, 4);
if (status != HAL_OK) {
    for (int i = 0; i < 4; ++i) { AD_Value[i] = 0; }
}
else {
    while (hdma_adc1.State != HAL_ADC_STATE_READY);     // 等待 DMA 数据转换完成
    HAL_ADC_Stop_DMA(&hadc1);
}
```

## 2.4 USART

> 发送数据
>
> ```c
> void HAL_UART_Transmit(&huart1, &Byte, ByteSize, HAL_MAX_DELAY);
> ```

### 1. 格式化重写函数

```c
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



### 2. 重写  `printf` USART 输出

> 对于 `clion` 可以使用下面的做法，但是如果你使用了 `freeRtos` 那就对 `xTask` 多分配点空间，不然 `print` 输出不出来

```cpp
#include <stdio.h>

int __io_putchar(int ch) {
    HAL_UART_Transmit(&huart1, (uint8_t *) &ch, 1, 200);
    return ch;
}
```

# 3. 硬件编码原理

---

> 这里提供主要的驱动模块，而是每一个驱动的编写原理

## 3.1 PWM 波形

> 看最右边的图，一个周期的时间就是： $T_S = T_{ON} + T_{OFF}$，就是这个周期中控制 `ON` 和 `OFF` 所占时间的多少
>
> ON 占用一个周期的时间就叫作为占空比，如果占空比越大，那么高电平再这个周期的时间就越大
>
> 举个例子：占空比有1%，2%，3%......那么它的分辨率就是 `1%`

![image-20241213191610824](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241213191610824.png)

### 3.0 PWM 模式

![image-20241213192614929](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241213192614929.png)

> 我们只需要掌握两个模式
>
> - 第一个就是翻转，这个可以输出**高低电频**花销时间都一样的波形
>
> - PWM，1 2 两个模式唯一的不同就是 1  0 相反

<img src="https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250303132554211.png" alt="image-20250303132554211" style="zoom:66%;" />

- 参数和原理图

> 红线之前都是 1 之后都是 0，直到遇到黄线

<img src="https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241213192832139.png" alt="image-20241213192832139" style="zoom:67%;" />

1. 占空比：总时间为 99，高电平占用的计数是 30，所以是 30 / (99 + 1)

2. 频率：

    - 假定计数器的时钟频率是：$f_{clk} = 72\, \text{MHz}$，`PSC =71`是预分频系数

    - `PWM` 的频率就是完成一次 `ARR` 所需要的频率单位时间

    - $$
        Freq = \frac{\frac{f_{clk}}{PSC + 1}}{ARR + 1}
        $$
    
3. 分辨率: 就是一个周期的计数的倒数: 1/99



### 3.1  按参数生成 PWM 波形

> 这里一个周期是 `1ms` 哈

<img src="https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241215202802169.png" alt="image-20241215202802169" style="zoom:122%;" />

### 3.2 舵机 PWM 控制原理

> 舵机的先对应的接口关系

![image-20241216133327340](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241216133327340.png)

> 占空比和旋转角度的关系

<div style="display: flex; justify-content: space-between; align-items: center; width: 100%;">
    <img src="https://i-blog.csdnimg.cn/blog_migrate/8487efa01529a2b26ce8e2aadd58f7f3.png" alt="img" style="width: 49%;"/>
    <img src="https://i-blog.csdnimg.cn/blog_migrate/318656994c810caec4dbade7ec889493.gif" alt="img" style="width: 49%;"/>
</div>


#### 1. 参数配置

> 舵机的 PWM 必须是 `20 ms`（`50Hz`），高平电的宽度必须在 `0.5~2.5 ms (2.5%~12.5%)` 之间，所以这个就直接的确定了我们的占空比和频率，由此我们计算出对应的参数，`CK_PSC = 72M`
>
> 1. $Freq = \frac{CK\_PSC}{PSC + 1} \times (ARR+1) = 50Hz$，也可以直接算时间来算 ARR
> 2. $Duty = \frac{CRR}{ARR + 1} \times100 = 2.5 \to 12.5$
>
> 确定我们参数 ARR = 20000，CRR = 500 ~ 2500（也可以再代码中调试）
>
> 同时我们没有 IO 复用所以看对应的 PWM 5.1 表，打开对应的 PA1 开启通道 2

![image-20241216142104604](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241216142104604.png)

## 3.2  输入捕获

> 因为第一时间存储到的是cache，所以程序读取时间是存储的，我们不需要那么着急去马上读取计数器里面的值

<img src="https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241225092204464.png" alt="image-20241225092204464" style="zoom:67%;" />

### 1. 频率测量的方法

<img src="https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241225101839273.png" alt="image-20241225101839273" style="zoom:60%;" />

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

## 3.3 程序下载电路

---

- [实现程序一键下载](https://www.bilibili.com/video/BV1TZ4y1h7S5/?spm_id_from=333.337.search-card.all.click&vd_source=b47817c1aa0db593f452034d53d4273a)

![image-20250731140824577](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250731140824577.png)

<br>

## 3.5  GIPO 开漏和推挽模式的区别

> 在推挽模式下面，我们的高低电平都有驱动能力
>
> 但是开漏模式下面，不太一样，只有**低电平**有驱动能力
>
> **所以如果你希望有更好的控制，应该设置成开漏模式**

![image-20241201213834232](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241201213834232.png)

## 3.6 时间的计算

> 执行 72 次任务，然后我们的最大时钟是 `72MHz`，所以执行 72 次任务就是需要 $1\mu s = 10^{-6} s$ 时间
>
> **最大定时时间的计算**：`f = 72M / 65536` 代表每一个数字的频率，然后取倒数就得到计数每一个数的时间。

## 3.7 AD 转换

> 模拟信号，他是连续的型号，这些信号就比如说电压的变化，温度的变化，这种自然现象转换到我们用数值取衡量的过程，对于如何测，不是我们考虑的，我们考虑的是如何把测出来的数据读出来
>
> DA 转换这个一般使用场景比较少，PWM 就可以做到，就是用数字信号生成模拟信号的样子，就是用数值取模拟波的形式

![image-20241228160734602](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241228160734602.png)

### 1. 数字信号转换的模式和存储方式

![image-20241228160228142](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241228160228142.png)

> 左右是因为，我们读取存进来的数据站不满 16，所以总要空 4 位，要么左，要么右

### 2. STM 的 ADC 通道对应的引脚

> 非常遗憾的是，我们的 STM32，好像只有 ADC 1、2

| **通道** |   **ADC1**   | **ADC2** | **ADC3** |
| :------: | :----------: | :------: | :------: |
|  通道0   |     PA0      |   PA0    |   PA0    |
|  通道1   |     PA1      |   PA1    |   PA1    |
|  通道2   |     PA2      |   PA2    |   PA2    |
|  通道3   |     PA3      |   PA3    |   PA3    |
|  通道4   |     PA4      |   PA4    |   PF6    |
|  通道5   |     PA5      |   PA5    |   PF7    |
|  通道6   |     PA6      |   PA6    |   PF8    |
|  通道7   |     PA7      |   PA7    |   PF9    |
|  通道8   |     PB0      |   PB0    |   PF10   |
|  通道9   |     PB1      |   PB1    |          |
|  通道10  |     PC0      |   PC0    |   PC0    |
|  通道11  |     PC1      |   PC1    |   PC1    |
|  通道12  |     PC2      |   PC2    |   PC2    |
|  通道13  |     PC3      |   PC3    |   PC3    |
|  通道14  |     PC4      |   PC4    |          |
|  通道15  |     PC5      |   PC5    |          |
|  通道16  |  温度传感器  |          |          |
|  通道17  | 内部参考电压 |          |          |

### 3. 硬件电路

![image-20241228162126363](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241228162126363.png)

> 简图

![image-20241228162152945](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241228162152945.png)

- 看图你就会发现如果通过注入通道读取到的数据是可以存在4个16位的寄存器中的
- 规则通道只有一个寄存器，所以如果转换的数据不想被覆盖，那么就需要 DMA

### 4. AD 数据转换的模式

> 分两个维度：
>
> - 单次/连续：转换后是否自动转换
> - 扫描/非扫描：是否支持多通道
>
> 上面四个选两个组合起来，一共四种模式，别 $C_4^2$ 哈，是 $C^1_2 \times C^1_2$

### 5. ADC 扫描 + DMA

> 知道了为啥有被覆盖的说法了吧，所以需要 DMA 马上把转换到 DR 的数据移动到内存，真等到 EOC，早都给盖完了
>
> 这里也 call back 了为什么外设寄存器不自增，内部需要自增

![image-20241229202423317](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241229202423317.png)

### 6. ADC的分辨率

> [!note]
>
> 在ADC（模数转换器）中，分辨率是用来权衡模拟数据的数字数据。这个数字数据的大小由ADC的**位数（bit depth）**决定，计算公式是：

$$
分辨率=2^{ADC 位数} - 1
$$

- 如果是**8位ADC**，分辨率 = 28−1=255
- 如果是**10位ADC**，分辨率 = 210−1=1023
- 如果是**12位ADC**，分辨率 = 212−1=4095

> 我们用一个程序案例来说明：假定我们的程序是 10 位的 ADC，输入电压最大 5V，那么换算电压的时候，就要这么做

```cpp
 float volts =  analogRead(TEMT6000) * 5 / 1024.0; // Convert reading to VOLTS
```

- 这个在电池用电压恒定点量的时候，也不失为一种方案

<br>

## 3.8 DMA

> - DMA 可以提供**外设和存储器**或者**存储器和存储器**之间的高速数据传输，无须CPU干预，节省了CPU的资源
> - 12个独立可配置的通道： DMA1（7个通道）， DMA2（5个通道）
> - 每个通道都支持软件触发和**特定的**硬件触发

### 1. 存储器映射

| **类型** | **起始地址** |    **存储器**    |                  **用途**                   |
| :------: | :----------: | :--------------: | :-----------------------------------------: |
|   ROM    | 0x0800 0000  | 程序存储器 Flash | 存储 C 语言编译后的程序代码 (烧录的，const) |
|          | 0x1FFF F000  |    系统存储器    |       存储`BootLoader`，用于串口下载        |
|          | 0x1FFF F800  |     选项字节     |      存储一些独立于程序代码的配置参数       |
|   RAM    | 0x2000 0000  |   运行内存SRAM   |          存储运行过程中的临时变量           |
|          | 0x4000 0000  |    外设寄存器    |           存储各个外设的配置参数            |
|          | 0xE000 0000  |  内核外设寄存器  |         存储内核各个外设的配置参数          |

### 2. DMA 基本结构

> 起始地址：我们知道外设寄存器和存储器是可以互通的，所以这个参数是决定从哪里到哪里的
>
> 数据宽度：单次转运的一个数据的大小
>
> 1. `Byte`：字节，8 位
> 2. `HalfWord`：半字，16 位
> 3. `word`：字，32 位
>
> 地址是否自增：就是指针指向数组，然后指针 ++ 操作
>
> - 外设寄存器一般比较小，所以不需要自增
> - 但是内部的存储器比较大，就是为了存储外部变动对应的结果，需要自增存储外边变化的所有过程
>
> 传输计数器：告知 DMA 我们需要转运几次
>
> 自动重装器：转运结束后，我们是否重新开始
>
> `M2M`：决定是硬件还是软件触发
>
> - 软件触发：不是通过函数控制，而是尽快的触发 DMA
> - 硬件触发：这个真的是外部硬件信号触发

![image-20241229195144415](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241229195144415.png)

> 注意下：写传输计数器的时候必须关闭 DMA

### 3. DMA 触发规定的对应通道

> 软件触法啥通道都行，但是硬件触发必须对应的通道

![image-20241229201236234](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241229201236234.png)

> 对应的表格形式

![image-20241229201626702](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241229201626702.png)

- DMA 2 的就自己查手册

<br>

## 3.9 OLED

---

### 1. 位置

> 页地址和列地址，页指定第x行灯排，列指定的灯牌的第x列指定 8 个灯中哪些的亮灭

![image-20250114152246379](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250114152246379.png)

### 2. 指令和数据

> 0x00 这个地址发送数据就代表是指令，0x40 发送数据就是数据
>
> 发送的数据会按照发送过去的地址一个个的递增把数据填充完整

![image-20250114160216901](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250114160216901.png)

其中的列地址，我们要说下，就是他是 0A，这个 0 代表着数据的低4位，15 这个 1 是高位，组合起来就是 5A，所以发送数据要发送两次的

```c
void OLED_SendCmd(uint8_t cmd) {
    static uint8_t sendBuf[2];

    sendBuf[0] = 0x00;
    sendBuf[1] = cmd;

    HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, sendBuf, 2, HAL_MAX_DELAY);
}

// page 0 第 0 行 0 列
OLED_SendCmd(0xB0);
OLED_SendCmd(0x00);
OLED_SendCmd(0x10);
```

### 3. 缓存

> 我们把数据用缓存机制，用一个数组表示要存储的数据，然后再统一的把这个数据写入到我们的屏幕中

```c
uint8_t GRAM[OLED_PAGE][OLED_COLUMN];	// 缓存数组
```

> 然后为了在这个数组上画东西，我们设置如下的坐标系，$x\in[0, 127]$ && $y\in [0, 63]$

![image-20250114171042040](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250114171042040.png)

### 4. 代码下载

> [这个代码修改一下再使用](https://led.baud-dance.com/)



## 3.10 PWR

---

> 低功耗模式包括 **睡眠模式（Sleep）、停机模式（Stop）和待机模式（Standby）**，可在系统空闲时，降低STM32的功耗，延长设备使用时间

### 1. 低功耗模式

> 从上到下是越来越省电的，但是同时也是越来越难唤醒的

![image-20250130192455849](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250130192455849.png)

### 2. 模式选择的判断流程图

![{8EE8F416-63E5-4EF2-8692-30093D55FF23}](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/%7B8EE8F416-63E5-4EF2-8692-30093D55FF23%7D.png)

### 3. 不同模式的不同特性

| **特性**          | **Sleep 模式** | **Stop 模式**                   | **Standby 模式**               |
| ----------------- | -------------- | ------------------------------- | ------------------------------ |
| **功耗**          | 较高           | 较低                            | 最低                           |
| **唤醒时间**      | 最短           | 较短                            | 较长                           |
| **CPU 状态**      | 停止运行       | 停止运行                        | 停止运行                       |
| **外设状态**      | 外设继续运行   | 外设时钟停止                    | 外设时钟停止                   |
| **SRAM 和寄存器** | 保持           | 保持                            | 丢失（备份寄存器保持）         |
| **唤醒源**        | 任意中断或事件 | 外部中断、RTC 闹钟、WKUP 引脚等 | WKUP 引脚、RTC 闹钟、NRST 复位 |
| **时钟配置**      | 保持           | 需重新配置                      | 需重新配置                     |
| **备份域**        | 保持           | 保持                            | 保持                           |
| **典型应用场景**  | 短时低功耗     | 中等低功耗                      | 深度低功耗                     |

<br>
