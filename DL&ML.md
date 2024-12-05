# 深度学习和机器学习

---

> 李弘毅老师的课程为本笔记的参考来源

# 1. Python 数据分析的常用函数

---

<br>

## 1.1 Pandas

### 1.1.1 Pandas 读取 csv 文件并分析内容

> 对 `csv` 文件一些基本内容进行简要的分析

```py
import pandas as pd
df = pd.read_csv('文件路径.csv')  # 用 df 表示导入后的 DataFrame 对象

# front
print(df.head())  # 默认显示前5行
print(df.head(10))  # 显示前10行

# back
print(df.tail())  # 默认显示后5行
print(df.tail(10))  # 显示后10行


print(train_data.columns)  # 返回列名列表

print(train_data.shape)  # 返回 (行数, 列数)

print(train_data.info())  # 显示行数、列数、每列的数据类型和非空值数量
```

## 1.2 Matplotlib

<br>

## 1.3 Torch 

### 1.3.1 随机拆分数据为指定大小的 np.array

```py
from torch.utils.data import andom_split

# 第一个参数是要拆分的数据集
# 返回值个数是根据第二个参数 [size_1, size_2] 来定夺的
# 第三个参数是随机使用的生成器
train_set, valid_set = random_split(
    data_set, [train_set_size, valid_set_size], generator=torch.Generator().manual_seed(seed)
  )
```

## 1.4 类

### 1.4.1 super()

> ### **1. 方法重写后调用父类方法**
>
> ​	当子类重写了父类的方法，但仍然需要调用父类的方法时，`super()` 提供了一种 **更优雅、更安全** 的方式。

- **问题：直接调用父类方法，硬编码父类名**

```py
class Parent:
    def greet(self):
        print("Hello from Parent")

class Child(Parent):
    def greet(self):
        print("Hello from Child")
        Parent.greet(self)  # 直接用父类名调用

child = Child()
child.greet()
```

- 输出：

```python
Hello from Child
Hello from Parent
```

> 那么我们不声明父类，但是重写的同名函数还可以是父类的呢

- 解决

```python
class Parent:
    def greet(self):
        print("Hello from Parent")

class Child(Parent):
    def greet(self):
        print("Hello from Child")
        super().greet()  # 使用 super() 调用父类方法

child = Child()
child.greet()
```



> ### 2. 多继承下方法解析顺序（MRO）问题
>
> ​	多继承会引入复杂的父类调用关系，如果直接用父类名调用，可能会导致重复调用或遗漏调用。而 `super()` 会按照 **方法解析顺序（MRO, Method Resolution Order）** 自动查找和调用合适的父类方法。

- 关系图

```mermaid
graph TD;
  A-->C;
  A-->B;
  B-->D;
  C-->D

```



- 问题：手动调用父类方法复杂且容易出错

```py
class A:
    def do_something(self):
        print("Doing something in A")

class B(A):
    def do_something(self):
        print("Doing something in B")
        A.do_something(self)  # 手动调用父类方法

class C(B):
    def do_something(self):
        print("Doing something in C")
        B.do_something(self)  # 手动调用父类方法

c = C()
c.do_something()
```

```shell
Doing something in D
Doing something in B
Doing something in A
Doing something in C
Doing something in A  # 重复调用 A，因为两个都继承到了 A 所以三角回去的时候会两个调用父，这个有些时候是不需要的
```

- 解决：使用 `super()` 调用父类方法

```py
class A:
    def do_something(self):
        print("Doing something in A")

class B(A):
    def do_something(self):
        print("Doing something in B")
        super().do_something()

class C(A):
    def do_something(self):
        print("Doing something in C")
		# 显式的调用 super 和其他的差不多
        super(C, self).do_something()

class D(B, C):
    def do_something(self):
        print("Doing something in D")
        super().do_something()  # 按 MRO 调用

d = D()
d.do_something()
```

```sh
Doing something in D
Doing something in B
Doing something in C
Doing something in A  # super() 之后
```



## 1.5 Numpy

----

### 1.5.1 Numpy 获取一个二维数列的制定某些列

```python
# 这里是一个二维的 numpy 数据

import numpy as np

# 示例二维数据（列表形式）
data = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]

# 转换为 NumPy 数组
np_array = np.array(data)

# 获取二维数据我们想要的列
col_we_wanna = [0,2]

print(
    np_array[::, col_we_wanna]
)
```

<br>

# 2. 

# 作业

---

## 1.1 HW1

> 作业训练集的构成结构

![image-20241126113741612](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/uPic/image-20241126113741612.png)

> one-hot 数据的介绍

就是我们的如果是哪个地方的数据就是 1