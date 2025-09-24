# Python

---

## 1. 报错处理

```python
while True:
    try:
        # 提示用户选择功能选项
        choose = int(input("..."))

    except ValueError:
        # 捕获输入不是整数时的错误
        print("输入无效！请输入有效的数字选项。")

    except Exception as e:
        # 捕获其他未知错误，并将其输出到终端
        print(f"发生未知错误: {e}")
        
    else:
        # 处理预期输入的逻辑
		...
```

### 1.1 finally 

> `finally` 块无论是否遇到异常都会被执行。它通常用于确保某些操作总是执行，比如关闭文件、释放资源等。在你这个例子中，`finally` 确保了函数总是返回 `i`，无论在 `try` 和 `except` 块中发生了什么。

```python
import re

def get_id():
    # 获取外部用户输入的 ID 或 URL
    id = input("请输入 id 或 URL: ")
    try:
        # 如果输入的是数字 ID，直接返回
        int(id)
    except ValueError:
        # 如果不是数字，则假定为 URL 并用正则表达式提取 ID
        pattern = r'id=(\d+)'
        match = re.search(pattern, id)
        if match:
            id = int(match.group(1))
        else:
            print(f"错误 URL，找不到 URL 中存在的 ID")
            return None  # 错误情况下返回 None
    except Exception as e:
        print(f"无法识别，出现未知错误：{e}")
        return None  # 遇到未知错误时返回 None
    finally:
        return id  # 在所有情况下返回 id
```



## 2. 正则

> 一个 `re.Match` 对象的所有的一般函数

```py
import re

url = "https://music.163.com/#/song?id=29771146"		# 这些规则可以交给 AI 没必要自己写
pattern = r'id=(\d+)'

match = re.search(pattern, url)
if match:
    print(f"Match object: {match}")
    print(f"Matched group: {match.group(0)}")   # 匹配的整个子串 "id=29771146"
    print(f"Captured ID: {match.group(1)}")     # 捕获的 ID "29771146"
    print(f"Start position: {match.start(1)}")  # 捕获组的起始位置
    print(f"End position: {match.end(1)}")      # 捕获组的结束位置
    print(f"Span: {match.span(1)}")             # 捕获组的起始和结束位置
else:
    print("No match found")
```

```shell
Match object: <re.Match object; span=(27, 39), match='id=29771146'>
Matched group: id=29771146
Captured ID: 29771146
Start position: 30
End position: 38
Span: (30, 38)
```

## 3. 网络

---

### 3.1 `requests.get()`

> 参数介绍

- `url`: 这是你要请求的目标 URL。

- `headers` (可选): 可以提供一个字典，指定 HTTP 请求的头部信息，比如 User-Agent、Referer 等。

- `params` (可选): 可以提供一个字典，指定 URL 的查询参数，像 `?id=123` 这样的查询参数。

- `timeout` (可选): 设置请求的超时时间（单位：秒），防止请求卡住太长时间。

- `cookies` (可选): 可传递一个字典，用于在请求中携带 cookies。

> 返回值

- `response.status_code`: 返回状态码，200 表示成功，404 表示找不到资源。
- `response.text`: 返回响应内容的文本形式（字符串）。
- `response.content`: 返回响应的二进制内容（适用于图片、音频等非文本文件）。
- `response.json()`: 返回 JSON 格式的数据（如果服务器返回的内容是 JSON），否则会抛出错误。
- `response.headers`: 返回服务器的响应头信息（字典形式）。

#### 3.1.1 带请求头的请求

```py
def get_lyrics(id):
    url = f"http://music.163.com/api/song/lyric?id={id}&lv=1&kv=1&tv=-1"
    headers = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36',
        'Referer': f'https://music.163.com/song?id={id}'
    }

    # 发起请求
    response = get(url, headers=headers)

    # 检查请求是否成功
    if response.status_code == 200:
        try:
            # 解析为 JSON
            lyrics_json = response.json()
            return lyrics_json
        except ValueError:
            print("无法解析 JSON 数据")
            return None
    else:
        print(f"请求失败，状态码: {response.status_code}")
        return None
```

#### 3.1.2 处理超时

```python
try:
    response = requests.get('https://example.com', timeout=5)
except requests.Timeout:
    print("请求超时")
```

<br>

## 4. 类

---

### 1. 子类必须赋予父类的写法（纯虚）

```python
class Base:
    def __init__(self):
        # 子类必须在 super().__init__() 之前给 self.x 赋值
        if getattr(self, 'x', None) is None:
            raise ValueError('子类必须给 x 赋值')
        # 其他初始化 …

class Child(Base):
    def __init__(self):
        self.x = 42          # 如果这行漏掉，父类会抛错
        super().__init__()
```

> 第二种写法更加 python 一点

```python
from abc import ABC, abstractmethod

class Base(ABC):
    @property
    @abstractmethod
    def x(self):
        """子类必须实现同名属性或同名方法并返回非 None 值"""
        ...

class Child(Base):
    def __init__(self):
        self._x = 42   # 漏掉这一行会在实例化时抛 TypeError

    @property
    def x(self):
        return self._x
```

### 5. 修饰器

---

### 1. @property

> [!note]
>
> - `@property` 把**一个方法伪装成属性**，外部可以像读写普通属性那样读写它，而背后却运行你写的函数。
> - `@xxx.setter` 把方法变成可写属性
>
> 其实这个部分就是简化了传统意义的 `getter` 和 `setter`

```python
class Circle:
    def __init__(self, radius):
        self._radius = radius

    @property           # 读
    def radius(self):
        return self._radius

    @radius.setter      # 写
    def radius(self, value):
        if value < 0:
            raise ValueError("半径不能为负")
        self._radius = value

    @property           # 只读派生属性
    def area(self):
        return 3.1416 * self._radius ** 2
```

> 外部直接当成员变量一样来访问

```python
c = Circle(5)
print(c.radius)   # 5          （自动调用 getter）
print(c.area)     # 78.54      （只读）
c.radius = -1     # ValueError （自动调用 setter 并校验）
```

<br>

## Ex. 其他配置

---

### 1. uv 管理 python 环境

> [解决 Python 烦人的环境管理分裂问题](https://www.youtube.com/watch?v=aVXs8lb7i9U)

<br>
