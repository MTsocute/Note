# CMake 用法

---

## 1. 构建一个静态库的项目

> 注意，我们可以先创建，后续在添加内容的

```cmake
cmake_minimum_required(VERSION 3.31)

# 项目名称
project(ComplexProject VERSION 0.10 DESCRIPTION "Demo Project of Complex Number" LANGUAGES CXX)

# 添加可执行程序 target = demo
add_executable(demo)
target_sources(demo PRIVATE main.cpp)

# 添加库 target=complex
add_library(complex)
target_sources(complex PRIVATE complex.cpp complex.h)

# 链接库
target_link_libraries(demo PRIVATE complex)
```

## 2. 变量

---

### 2.1 普通变量

```cmake
set(myVar hello)		# myvar = "hello"
set(myVar hello World)	# myVar = list(hello, world)
# 如果想要一个 "hello World" 的变量就得
set(myVar "hello World")
```

> 如果想引用一个变量，就必须要

```cmake
${myVar}
# 可以使用下面的方法，让一个变量释放
unset(myVar)
```

> 如果想要在 `CMake` 编译的时候打印，就得这样子

```cmake
message("myvar = ${myVar}")
```

<br>

### 2.2 环境变量

> 比较常见的就是配置某个文件的位置作为这个程序的环境变量

```cmake
# 用于将某个路径添加到环境变量中
set(ENV{PATH} "$ENV{PATH}:/Path/to/somwhere")
```

<br>

### 2.3 缓存变量

> 可以在 build 目录下，使用 `cmake-gui` 打开查看到

```cmake
set(<variable_name> <value> CACHE <type> "<description>" [FORCE])
```

<br>

### 2.4 预定义变量

> 判断是那种编译器的用法

```cmake
if(MSVC)
    message("build with MSVC")
elseif(MINGW)
    message("build with MINGW")
elseif(XCODE)
    message("build with XCODE")
else()
    message("build with other compiler")
endif()
```

> 用于防止用户在源代码目录中直接构建项目
>
> `CMAKE_SOURCE_DIR`：表示项目的源代码所在的根目录。
>
> `CMAKE_BINARY_DIR`：表示你运行 `cmake` 命令时，当前使用的构建目录。

```cmake
# 譬如本该在 build/ 运行的命令，在 ../ 一层执行了
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "错误：禁止源码内编译。请为构建文件创建一个单独的目录。")
endif()
```


> 常见的有

| 变量名                     | 说明                                            |
| -------------------------- | ----------------------------------------------- |
| `CMAKE_SOURCE_DIR`         | 顶层 CMakeLists.txt 所在目录的绝对路径          |
| `CMAKE_BINARY_DIR`         | 顶层构建目录的绝对路径（通常是 `build` 文件夹） |
| `CMAKE_CURRENT_SOURCE_DIR` | 当前 CMakeLists.txt 所在目录的绝对路径          |
| `CMAKE_CURRENT_BINARY_DIR` | 当前目录的构建目录的绝对路径                    |

<br>

### 2.5 option 变量

```cmake
option(BUILD_MYLIB "构建MyLib目标")  # 定义一个选项，用户可以选择是否构建MyLib目标
if(BUILD_MYLIB)  # 如果用户选择了构建MyLib目标
    add_library(MyLib mylib.cpp)  # 添加一个名为MyLib的库，并指定源文件mylib.cpp
else()  # 如果用户没有选择构建MyLib目标
    message("忽略构建MyLib目标")  # 输出一条消息，表示忽略构建MyLib目标
endif()  # 结束条件判断
```

<br>

## 3. file

---

> file 就是创建一个文件列表，但是我们可以根据下面的特性来决定列表的内容

<img src="https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/uPic/image-20250529134327932.png" alt="image-20250529134327932" style="zoom:55%;" />

> 读取一个目录下的所有 cpp 文件

| 参数                           | 说明                                     |
| ------------------------------ | ---------------------------------------- |
| `varFileList`                  | 把找到的文件名放到这个变量里。           |
| `LIST_DIRECTORIES false`       | 不把目录夹名加进去，只找文件。           |
| `RELATIVE ${CMAKE_SOURCE_DIR}` | 输出的路径是**相对于源码根目录**的路径。 |
| `"*.cpp"`                      | 匹配当前目录下所有 `.cpp` 文件。         |

```cmake
file(GLOB varFileList LIST_DIRECTORIES false RELATIVE ${CMAKE_SOURCE_DIR} "*.cpp")
message("varFileList: ${varFileList}")
```

| 部分                           | 作用                               | 补充说明               |
| ------------------------------ | ---------------------------------- | ---------------------- |
| `GLOB_RECURSE`                 | 表示**递归查找**文件（包括子目录） | 递归搜索所有子文件夹   |
| `SRC_LIST`                     | 找到的文件列表将保存到这个变量中   | 可替换为你喜欢的变量名 |
| `RELATIVE ${CMAKE_SOURCE_DIR}` | 输出路径是**相对于**`cmakelist`的  | 输出更简洁，避免长路径 |

>自动添加source文件

```cmake
file(GLOB_RECURSE SRC_LIST
     RELATIVE ${CMAKE_SOURCE_DIR}
     "${CMAKE_SOURCE_DIR}/src/*.cpp" 
     "${CMAKE_SOURCE_DIR}/src/*.c")

add_executable(${PROJECT_NAME} ${SRC_LIST})
```

<br>

## 4. 目录分层管理

---

> 注意这个头文件的引用是不可以 `private` 的，不然的话，其他程序是找不到头文件

![image-20250529141055894](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/uPic/image-20250529141055894.png)

### 4.1 目标属性的作用域

> 所谓`properties`就是`target`的属性，以下这些方式都存在：
>
> - 编译标志：使用 `target_complie_option`
> - 预处理宏标志：使用 `target_compile_definitions`
> - 头文件目录：使用 `target_include_directories`
> - 链接库：使用 `target_link_libraries`
> - 链接标志：使用 `target_link_options`
>
> `PUBLIC`选项使得链接的库对提供方和使用方都可见，`PRIVATE`选项仅对提供方可见，`INTERFACE`选项仅对使用方可见。

![image-20250529141341527](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/uPic/image-20250529141341527.png)

- **提供方**：指的是创建或定义了某个库（或目标）的一方。这个库可以提供接口（如头文件）和/或实现（如源文件），供其他库或应用程序使用
- **使用方**：指的是依赖于某个库（或目标）的一方。这个库或应用程序需要链接到提供方的库，以便使用其提供的接口和/或实现。

<br>

### 4.2 第三方库查找

> 这里建议使用 `vcpkg` 包管理器统一管理第三方库，`vcpkg` 安装的第三方库是根据当前使用的编译器来下载库的类型的
>
> 这里以下面的库为例

![image-20250611092407242](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250611092407242.png)

> 我正在使用的编译器是 `mingw` 所以他会根据我的编译器，自动下载 `sdl2:x64-mingw-dynamic`，我们使用 `vcpkg` 配合 `cmake` 的 `find_package` 的大前提是都是 `x64-windows`，因为用的 `MSVC`，他的 `cmake` 命令如下

```shell
# msvc
cmake -DCMAKE_TOOLCHAIN_FILE=D:/Develop_Tool/vcpkg/scripts/buildsystems/vcpkg.cmake
```

> 所以对于 `mingw` 还请添加指定第三方库类型，才可以找到对应的库

```shell
# mingw32
cmake -DCMAKE_TOOLCHAIN_FILE=D:/Develop_Tool/vcpkg/scripts/buildsystems/vcpkg.cmake
-DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic
```

<br>

## 5. 模版

---

> 可以根据下面的代码创建一个模版文件，然后通过这个模版文件来构造出对应的 `config.h` 在 `build` 文件夹内

````cmake
cmake_minimum_required(VERSION 2.6)
project(Tutorial)
# The version number.
set(Tutorial_VERSION_MAJOR 1)
set(Tutorial_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
    "${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"		# 模版文件在在源码区
    "${PROJECT_BINARY_DIR}/TutorialConfig.h"		# 
)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_executable(Tutorial tutorial.cxx)
````

