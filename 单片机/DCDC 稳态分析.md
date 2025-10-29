# DCDC 稳态分析

---

## 从电感公式出发

电感基本方程：
$$
v_L = L \frac{di_L}{dt}
$$
对一个完整开关周期 $T_s$（含导通时间 $t_{on}$ 和关断时间 $t_{off}$）积分：
$$
\int_0^{T_s} v_L \, dt = L \int_0^{T_s} \frac{di_L}{dt} \, dt = L [i_L(T_s) - i_L(0)]
$$

<br>

## 稳态条件（Steady State）

> 稳态意味着：每个周期电感`电流波形重复`，电流开始和结束时值一样，即
> $$
> i_L(T_s) = i_L(0)
> $$

![image-20251028204048937](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20251028204048937.png)

代回上式：
$$
\int_0^{T_s} v_L \, dt = 0
$$


> [!important]
>
> 这就是 **伏秒平衡（Volt-Second Balance）** 条件。

<br>

## 套入 DCDC 的工作周期

> 以 **Buck 降压电路** 为例（升压、反激同理）：

###  导通阶段（ON）：

- 开关管导通；

<img src="https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20251028205705630.png" alt="image-20251028205705630" style="zoom:80%;" />

- `电感`两端电压：
  $$
  v_{L_{on}} = V_{in} - V_{out}
  $$

- 电感电流增加：
  $$
  \Delta i_{on} = \frac{v_{L_{on}} \cdot t_{on}}{L} = \frac{(V_{in} - V_{out}) D \cdot T_s}{L}
  $$
  $$
  D = \frac{t_{on}}{T_s} \\
  $$

------

### 关断阶段（OFF）：

- 开关管关断，电感通过二极管或同步管放能；

![image-20251028205847180](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20251028205847180.png)

- `电感`两端电压：
  $$
  v_{L_{off}} = 0 -V_{out}
  $$

- 电感电流减小：
  $$
  \Delta i_{off} = \frac{v_{L_{off}} \cdot t_{off}}{L} = \frac{(-V_{out})(1 - D)T_s}{L}
  $$

<br>

## 稳态条件下电流波形平衡

> 稳态时，一周期内`电感电流的增量与减量相等`，即：
> $$
> \Delta i_{on} + \Delta i_{off} = 0
> $$

<img src="https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20251028211256596.png" alt="image-20251028211256596" style="zoom:67%;" />

> [!note]
>
> `正半轴的三角就是电压增量，负半轴的三角就是电压减量`，因为稳态条件是 $i_L(T_s) = i_L(0)$，所以起点和终点是相同的，且顶点相同，所以增量和减量是相同的

代入上面两个式子：
$$
(V_{in} - V_{out}) D T_s + (-V_{out})(1 - D)T_s = 0
$$
约掉 $T_s$：
$$
(V_{in} - V_{out})D - V_{out}(1 - D) = 0
$$
整理得出熟悉的 **Buck 电压关系式**
$$
V_{out} = D V_{in}
$$

<br>

## 电容电压的计算

<img src="https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20251028211256596.png" alt="image-20251028211256596" style="zoom:67%;" />

> 我们这里选用放电的时刻，用来计算出这个区间内电容的瞬时电压

$$
\Delta i_{off}  = \frac{(-V_{out})(1 - D)T_s}{L}
$$

> 且已知

$$
v_L = L \frac{di_L}{dt} \to \Delta v_L = L\int i(t) dt \\
L = \frac{1}{C} \\

v_L = \frac{1}{C} \int i(t) dt
$$

> 积分的结果就是放电那一小段三角形的面积

$$
\Delta v_L =
\frac{1}{C} \int^{T_s}_{T_s/2} i(t) dt = \frac{1}{C} \cdot \frac{1}{2} \cdot \frac{T_s}{2} \cdot \frac{\Delta i}{2} = 
\frac{T_s \cdot \Delta i_{off}}{8C}
$$

> 代入我们的 $\Delta i_{off}$

$$
\Delta v_L = -\frac{V_{out} \cdot (1 - D) \cdot T_s^2}{8CL}
$$

