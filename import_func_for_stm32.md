# 几个重要的 STM 32 函数

# 1. 基础配置

---

> 单片机的型号

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

