# RT-Thread

---

- [DSP 创建一个项目](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/tutorial/make-bsp/stm32-bsp/STM32%E7%B3%BB%E5%88%97BSP%E5%88%B6%E4%BD%9C%E6%95%99%E7%A8%8B?id=_1-bsp-%e6%a1%86%e6%9e%b6%e4%bb%8b%e7%bb%8d)
- [创建一个新的 FISH 可用的模块](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/application-note/setup/standard-project/an0017-standard-project?id=%e6%a0%87%e5%87%86%e5%b7%a5%e7%a8%8b%e7%ae%a1%e7%90%86)
- [可运行的 RT-Thread 版本安装](https://github.com/RT-Thread/rt-thread/archive/refs/tags/v5.1.0.zip)
- [RT-Thread 指南 PDF](D:\iCloud\iCloudDrive\Book\RT-Thread 编程指南.pdf )
- [线程应用示例](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/thread/thread?id=%e7%ba%bf%e7%a8%8b%e5%ba%94%e7%94%a8%e7%a4%ba%e4%be%8b)
- [官方文档](https://www.rt-thread.org/document/site/#/)
- [使用 LVGL](https://club.rt-thread.org/ask/article/3328bb2d6d54ea34.html)
- [视频教学搭建 LVGL + 模拟器环境](https://www.bilibili.com/video/BV1CV4y1m7nQ?vd_source=b47817c1aa0db593f452034d53d4273a&spm_id_from=333.788.videopod.sections)
- [RT-Thread 移植 U8g2](https://github.com/wuhanstudio/rt-u8g2)

<br>

## 1. 线程间的同步

> [!warning]
>
> 在多线程实时系统中，一项工作的完成往往可以通过多个线程协调的方式共同来完成，那么多个线程之间如何 “默契” 协作才能使这项工作无差错执行？

> `临界区`的概念：多个线程操作 / 访问同一块区域（代码），这块代码就称为临界区
>
> 只要有这种公共点，控制整体的流程，就是一种同步
>
> 线程的同步方式有很多种，其核心思想都是：在`访问临界区的时候只允许一个 (或一类) 线程运行`。

![image-20250722015617188](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250722015617188.png)

> 进入临界区的方式：

- 调用 `rt_hw_interrupt_disable()` 进入临界区，调用 `rt_hw_interrupt_enable()` 退出临界区
- 调用 `rt_enter_critical() 进入临界区`，调用 `rt_exit_critical() `退出临界区

<br>

### 1. 信号量

> 信号量是一种轻型的用于解决线程间同步问题的内核对象，线程可以获取或释放它，从而达到同步或互斥的目的

> [!note]
>
> 信号量工作示意图如下图所示，每个信号量对象都有一个`信号量值`和一个`线程等待队列`
>
> - 信号量的值对应了信号量对象的实例数目、资源数目，假如信号量值为 5，则表示共有 5 个信号量实例（资源）可以被使用
> - 当信号量实例数目为零时，再申请该信号量的线程就会被挂起在该信号量的等待队列上，等待可用的信号量实例（资源）

![image-20250722105218395](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250722105218395.png)

> [!note]
>
> 相关接口，对应的介绍看书的

![image-20250722110100923](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250722110100923.png)

<br>

### 2. CPP future && async 简易实现

---

> 假定我们的工作场景如下，有生产和消费者这个经典模型，然后我们的生产者包装要执行的函数到邮箱中，然后通过事件唤醒消费者，让消费者取执行函数。
>
> - 对于`压根不知道类型的函数返回值`要如何处理呢
> - 这种异步执行，啥时候线程执行完了
> - 在哪里读取到返回值呢

> 如果我们用的是 CPP，是有很好的解决思路的，那么如何在 C + RT-Thread 实现呢

#### 封装结构体

```c
typedef struct {
    void *result;
    rt_sem_t done;
} future_t;

typedef struct {
    void *(*wrapper_func)(void *);  // 统一接口（内部封装）
    void *arg;
    future_t *future;
} async_ctx_t;
```

#### 通用线程执行器

```c
static void async_task_entry(void *task) {
    async_ctx_t *ctx = (async_ctx_t *)task;
    ctx->future->result = ctx->wrapper_func(ctx->arg);
    rt_sem_release(ctx->future->done);
    rt_free(ctx);
}
```

<br>

## 错误处理

---

![image-20250722144825640](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250722144825640.png)

