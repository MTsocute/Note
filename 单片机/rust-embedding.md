# Rust 嵌入式开发

---

## 指令集安装

![image-20251005070009210](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20251005070009210.png)

## 调试器的安装（`Probe-rs`）

```bash
cargo install probe-rs-tools
```

这会安装一组工具：

- `probe-rs`（核心命令行工具）
- `cargo-flash`（刷写固件）
- `cargo-embed`（调试运行）
- `probe-rs-cli`（底层控制接口）

> 显示所有的调试器

```bash
probe-rs list
```

> 显示调试器连接的开发板

```bash
probe-rs info
```

<br>

## `Vscode Probe-rs` 配置

<br>

## 下拉 stm32f1 工程模板

- [stm32f1 参考](https://github.com/embassy-rs/embassy/tree/main/examples)
- [我的工程案例模板](https://github.com/embassy-rs/embassy/tree/main/examples)