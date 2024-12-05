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
/** @defgroup GPIO_pins_define GPIO pins define
  * @{
  */
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



#### 参数说明：

1. **`GPIOx`**:
    - 这是指向目标 GPIO 端口的指针，`GPIOx` 是端口的基地址。STM32 的每个 GPIO 端口都有一个不同的地址（如 `GPIOA`、`GPIOB`、`GPIOC` 等）。
    - 例如，`GPIOA` 表示 STM32 的 `GPIOA` 端口，`GPIOB` 表示 `GPIOB` 端口，等等。
2. **`GPIO_Pin`**:
    - 这是你要读取的具体引脚编号。STM32 的每个 GPIO 端口通常有多个引脚（例如，`GPIO_PIN_0`、`GPIO_PIN_1` 等），用来表示每个引脚的编号。
    - 例如，`GPIO_PIN_0` 表示端口中的第一个引脚，`GPIO_PIN_1` 表示第二个引脚，以此类推。

#### 返回值：

- **`GPIO_PIN_SET`**: 如果目标引脚的电平为高电平（1），则返回 `GPIO_PIN_SET`。
- **`GPIO_PIN_RESET`**: 如果目标引脚的电平为低电平（0），则返回 `GPIO_PIN_RESET`。

```cpp
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
```

