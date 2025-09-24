# Xmake

---

- [官方](https://xmake.io/)
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





## 配置

```bash
xmake f --menu		# 图形化配置界面
```

```bash
# 生成compile_commands.json后，Clion可以使用，但是没法 debug
```

### 智能提示 - vscode 插件

> xmake-vscode will generate `.vscode/compile_commands.json` file, so you need only add it to `.vscode/c_cpp_properties.json` to enable IntelliSense.
>
> for example (`.vscode/c_cpp_properties.json`):

```bash
xmake project -k compile_commands
```

```json
// c_cpp_properties.json

{
    "configurations": [
        {
            "compileCommands": ".vscode/compile_commands.json"
        }
    ],
    "version": 4
}
```

#### How can I generate c_cpp_properties.json ?

> in VS Code, select C/C++: Edit Configurations (UI) from the Command Palette
>
> [here for more](https://code.visualstudio.com/docs/cpp/configure-intellisense-crosscompilation)

![img](https://camo.githubusercontent.com/a9bfd93def37e6f7c62772964f7632c4a421eca052f09b34e4552f7e77959c00/68747470733a2f2f636f64652e76697375616c73747564696f2e636f6d2f6173736574732f646f63732f6370702f6370702f636f6d6d616e642d70616c657474652e706e67)

### Type

| Type       | Description            | Output Files                              |
| :--------- | :--------------------- | :---------------------------------------- |
| binary     | Executable program     | app.exe, app                              |
| static     | Static library         | libapp.a, app.lib                         |
| shared     | Dynamic library        | libapp.so, app.dll                        |
| object     | Object file collection | *.o, *.obj                                |
| headeronly | Header-only library    | No compilation output                     |
| phony      | Virtual target         | No output, only for dependency management |
