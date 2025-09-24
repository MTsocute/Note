# CMake + Kconfig

---

- [环境和基本使用参考](https://zhuanlan.zhihu.com/p/669005906)

<br>

## 1. Python 辅助脚本

>Kconfiglib 生成的是 `.config` 文件，并非C语言文件，需要使用脚本生成，将以下代码复制到文件中并命名为`kconfig.py`

```py
import os
import sys
import logging
from kconfiglib import Kconfig, KconfigError

# 配置日志
logging.basicConfig(
    level=logging.INFO,
    format='%(levelname)s: %(message)s',
    handlers=[logging.StreamHandler(sys.stderr)]
)
logger = logging.getLogger(__name__)

def generate_config(kconfig_file, config_in, config_out, header_out):
    try:
        logger.info(f"Parsing Kconfig from: {kconfig_file}")
        kconf = Kconfig(
            kconfig_file,
            warn_to_stderr=False,
            suppress_traceback=True
        )

        # 加载配置文件
        logger.info(f"Loading config from: {config_in}")
        if not os.path.exists(config_in):
            logger.warning(f"Config file not found: {config_in}. Using default settings")

        kconf.load_config(config_in)

        # 写入合并后的配置
        logger.info(f"Writing merged config to: {config_out}")
        kconf.write_config(config_out)

        # 写入头文件
        logger.info(f"Generating header file: {header_out}")
        kconf.write_autoconf(header_out)

        # 处理头文件格式
        try:
            with open(header_out, 'r+') as header_file:
                content = header_file.read()

                # 处理CONFIG_前缀
                content = content.replace("#define CONFIG_", "#define ")

                # 重新写入带保护宏的内容
                header_file.seek(0)
                header_file.truncate()

                header_content = [
                    "#ifndef _CONFIG_H_",
                    "#define _CONFIG_H_\n",
                    "#ifdef __cplusplus",
                    'extern "C" {',
                    "#endif /* __cplusplus */\n\n",
                    content,
                    "\n#ifdef __cplusplus",
                    "}",
                    "#endif /* __cplusplus */\n\n",
                    "#endif /* _CONFIG_H_*/"
                ]

                header_file.write("\n".join(header_content))

            logger.info(f"Header file processed successfully")

        except IOError as e:
            logger.error(f"Failed to process header file: {str(e)}")
            sys.exit(1)

    except KconfigError as e:
        logger.error(f"Kconfig processing failed: {str(e)}")
        sys.exit(1)
    except Exception as e:
        logger.exception(f"Unexpected error during config generation")
        sys.exit(1)

def main():
    # 使用更安全的路径处理
    script_dir = os.path.dirname(os.path.abspath(__file__))
    kconfig_file = os.path.join(script_dir, 'Kconfig')
    config_in = os.path.join(script_dir, '.config')
    config_out = os.path.join(script_dir, '.config')
    header_out = os.path.join(script_dir, 'config.h')

    generate_config(kconfig_file, config_in, config_out, header_out)
    logger.info("Configuration generation completed successfully")

if __name__ == "__main__":
    try:
        main()
        sys.exit(0)
    except SystemExit as e:
        # 已经处理过的退出
        raise
    except Exception as e:
        logger.exception("Unhandled exception in main")
        sys.exit(1)
```

## 2. 创建 Kconfig

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
