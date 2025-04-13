# CMake + Kconfig

---

- [环境和基本使用参考](https://zhuanlan.zhihu.com/p/669005906)

<br>

## 1. Win 环境搭建

<br>

### 1. MSVC `output` 输出 乱码问题

---

> MSVC 使用了 GBK 编码, 然后大多数的终端都是 UTF-8 然后就会导致输出不正常
>
> 所以我们可以再 cmake 配置如下命令解决乱码问题

```cmake
# CMake的最低版本要求
cmake_minimum_required(VERSION 3.22)

# 项目名称
project(MyProject)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)

# 判断是否使用 MSVC 编译器
if (MSVC)
    # 设置 C 标志为 utf-8
    set(CMAKE_C_FLAGS /utf-8)
    # 设置 C++ 标志为 utf-8
    set(CMAKE_CXX_FLAGS /utf-8)
    # 启用 Boost 异常处理
    set(CMAKE_CXX_FLAGS "/EHsc ${CMAKE_CXX_FLAGS}")
endif ()

# 添加可执行文件
add_executable(MyProject demo.cpp)
```

## 2. Python 辅助脚本

>Kconfiglib 生成的是 `.config` 文件，并非C语言文件，需要使用脚本生成，将以下代码复制到文件中并命名为`kconfig.py`

```py
import os
import re
import sys
import textwrap

from kconfiglib import Kconfig, split_expr, expr_value, expr_str, BOOL, \
                       TRISTATE, TRI_TO_STR, AND, OR

def generate_config(kconfig_file,config_in, config_out, header_out):

    print("Parsing " + kconfig_file)
    kconf = Kconfig(kconfig_file, warn_to_stderr=False, 
                    suppress_traceback=True)

    # Load config files
    print(kconf.load_config(config_in))

    # Write merged config        
    print(kconf.write_config(config_out))

    # Write headers
    print(kconf.write_autoconf(header_out))

    with open(header_out, 'r+') as header_file:

        content = header_file.read()
        header_file.truncate(0)
        header_file.seek(0)

        # Remove CONFIG_ and MR_USING_XXX following number
        content = content.replace("#define CONFIG_", "#define ")

        # Add the micro
        header_file.write("#ifndef _CONFIG_H_\n") 
        header_file.write("#define _CONFIG_H_\n\n")

        header_file.write("#ifdef __cplusplus\n")
        header_file.write("extern \"C\" {\n") 
        header_file.write("#endif /* __cplusplus */\n\n")

        # Write back the original data
        header_file.write(content)

        # Add the micro
        header_file.write("\n#ifdef __cplusplus\n")
        header_file.write("}\n")
        header_file.write("#endif /* __cplusplus */\n\n")
        header_file.write("#endif /* _CONFIG_H_*/\n")

def main():
    kconfig_file = 'Kconfig' 
    config_in = '.config'
    config_out = '.config'
    header_out = 'config.h'
    generate_config(kconfig_file, config_in, config_out, header_out)

if __name__ == "__main__":
    main()
```

## 3. 创建 Kconfig

> 新建一个名字为 `Kconfig` 的文件（注意没有后缀名），和上一步中中创建的 `kconfig.py` 的文件放在一起。

```kotlin
// Kconfg Content
mainmenu "Demo"

menu "Args configure"
config CFG_ARG1
    bool "Enable arg1"
    default n

config CFG_ARG2
    int "Arg2 number"
    default 8
    range 0 64

endmenu
```

> 在当前目录下调用命令行，输入：

```bash
menuconfig
```

- 配置完的内容就会有一个 `.config` 的文件, 然后 运行 python 脚本解析这个文件就会有 `config.h` 的头文件

```bash
Parsing Kconfig
Loaded configuration '.config'
No change to configuration in '.config'
Kconfig header saved to 'config.h'
```

