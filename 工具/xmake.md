# XMake

---

- [官方网站](https://xmake.io/)
- [包管理器仓库](https://xmake.microblock.cc/)
- [官方包管理工具 - Xrepo](https://github.com/xmake-io/xrepo)

<br>

## Xrepo 包管理工具

```bash
$ xrepo install zlib tbox
```

```bash
# 避免手动确认
xrepo install -y xxx
```

```bash
# Install the specified platform package

$ xrepo install -p iphoneos -a arm64 zlib
$ xrepo install -p android [--ndk=/xxx] zlib
$ xrepo install -p mingw [--mingw=/xxx] zlib
$ xrepo install -p cross --sdk=/xxx/arm-linux-musleabi-cross zlib
```

```bash
# Install the debug package
$ xrepo install -m debug zlib

# Install the package with dynamic library
$ xrepo install -k shared zlib

# Install the specified configuration package
## Tips: https://xmake.microblock.cc/ --> check for Configuration Options
$ xrepo install -f "vs_runtime='MD'" zlib
$ xrepo install -f "regex=true,thread=true" boost
```

### 使用一个库

- [包配置相关信息](https://xmake.io/zh/guide/package-management/using-official-packages.html#%E5%8F%AF%E9%80%89%E5%8C%85%E8%AE%BE%E7%BD%AE)

```lua
add_requires("tbox 1.6.*", "libpng ~1.16", "zlib")

target("test")
    set_kind("binary")
    add_files("src/*.c")
    add_packages("tbox", "libpng", "zlib")
```

<br>

## 配置

```bash
xmake f --menu		# 图形化配置界面
```

```bash
# 生成compile_commands.json后，Clion可以使用，但是没法 debug
```

### 智能提示 - vscode

> 手动生成 vs 配置

```bash
xmake project -k vsxmake
xmake project -k compile_commands --lsp=clangd
```

> 或者把这个规则，写到 `xmake.lua` 里面，也可以生成配置文件

```lua
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})
```

> 下载 `clangd` 插件，然后再下载 `LSP` 本体

```json
# setting.json 加入下面这一条
"clangd.arguments": [
    "--compile-commands-dir=${workspaceFolder}/.vscode",
    // "--completion-style=detailed",
    // "--header-insertion=never"
],
```

> [!note]
>
> 如果代码提示不是最新的，手动使用 `xmake project -k compile_commands --lsp=clangd` 生成的配置文件，并移动到 `.vscode` 文件夹下面，然后通过下面的命令更新一下，启动代码提示

![image-20251013094432664](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20251013094432664.png)

### 包的类型

| Type       | Description            | Output Files                              |
| :--------- | :--------------------- | :---------------------------------------- |
| binary     | Executable program     | app.exe, app                              |
| static     | Static library         | libapp.a, app.lib                         |
| shared     | Dynamic library        | libapp.so, app.dll                        |
| object     | Object file collection | *.o, *.obj                                |
| headeronly | Header-only library    | No compilation output                     |
| phony      | Virtual target         | No output, only for dependency management |

```lua
add_rules("mode.debug", "mode.release")
set_languages("c++20")

-- 好处就是可以 debug 也可以生成 release
add_requires("fmt", {configs = {shared = true}})

add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

target("big_little_endian")
    set_kind("binary")
    add_files("src/main.cpp")
    set_encodings("utf-8")		-- fmt 的需求
    add_packages("fmt")
```



### DEBUG

> LLVM debug，[下载地址](https://github.com/llvm/llvm-project/releases/tag/llvmorg-21.1.2)
>
> vscode 里面搜插件，然后把这个换成 codelldb

![image-20251003085821723](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20251003085821723.png)
