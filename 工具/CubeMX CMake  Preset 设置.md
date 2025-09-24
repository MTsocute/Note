# CubeMX CMake  Preset 设置

---

```json
    "configurePresets": [
        // default
        {
            "name": "default",
            "hidden": true,
            "generator": "Ninja",
            "binaryDir": "${sourceDir}/build/${presetName}",
		    "toolchainFile": "${sourceDir}/cmake/gcc-arm-none-eabi.cmake",
            "cacheVariables": {
            },
            // 在这里指定我们使用的工具链具体是哪一个
            "vendor": {
                "jetbrains.com/clion": {
                    "toolchain": "STM32-GNU-14.3"
                }
            }
        },
        // Debug
        {
            "name": "Debug",
            "inherits": "default",
            "cacheVariables": {
                "CMAKE_BUILD_TYPE": "Debug"
            }
        },
        // Release
        {
            "name": "Release",
            "inherits": "default",
            "cacheVariables": {
                "CMAKE_BUILD_TYPE": "Release"
            }
        }
    ],
```

