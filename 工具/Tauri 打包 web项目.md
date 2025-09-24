# Vue 3 + Tauri 打包筆記 (Windows)

---

## 前置準備 (Prerequisites)

> [!important]
>
> 在開始前，請確保你的開發環境已安裝以下工具：

1. **Rust**: 前往 [Rust 官網](https://www.rust-lang.org/tools/install) 安裝 `rustup`。
2. **Node.js**: 確保已安裝。
3. **Tauri CLI**: 在終端執行 `npm install -g @tauri-apps/cli`。

------



## 步驟一：初始化 Tauri

進入你的 Vue 專案根目錄，執行以下指令，將 Tauri 的配置檔案添加到專案中。

```bash
tauri init
```

- **`What is the URL of your dev server?`**: 輸入你的 Vue 開發伺服器網址，通常是 `http://localhost:5173`。
- **`What is the path to your dist dir?`**: 輸入你的 Vue 專案建置後產物的目錄，通常是 `../dist`。

完成後，你的專案會多出一個 `src-tauri` 資料夾。

------



## 步驟二：配置 `tauri.conf.json`

> 打開 `src-tauri/tauri.conf.json` 檔案，進行以下關鍵修改。

**1. 更改唯一識別碼 (identifier)** 這是打包失敗最常見的原因。將預設值修改為一個獨特的名稱。

```json
{
  // ... 其他配置
  "identifier": "com.yourcompany.yourapp", // 必須修改！
  // ... 其他配置
}
```

**2. 檢查圖示檔案 (icon)** 確保 `icons` 資料夾中的圖示檔案存在，尤其是 `.ico` 格式。

```json
{
  // ... 其他配置
  "bundle": {
    "icon": [
      "icons/32x32.png",
      "icons/128x128.png",
      "icons/icon.ico" // 確保這些檔案存在
    ]
  }
}
```

------



### 步驟三：打包專案 (Tauri Build)

> 在終端機中，按順序執行以下兩個指令：

1. **建置前端專案**

```bash
npm run build
```

   這個指令會將你的 Vue 專案編譯成靜態檔案，並放入 `dist` 目錄。

2. **執行 Tauri 打包**

```bash
tauri build
```

   這個指令會啟動 Tauri 的打包流程。如果一切順利，它會將前端檔案和 Rust 後端程式碼打包成一個可執行的 `.msi` 或 `.exe` 檔案。

打包完成後，你可以在 **`src-tauri/target/release/bundle`** 目錄中找到生成的安裝包。

------



## 常見問題排查

---

- **`Error: Failed to parse version...`**:
  - **原因**: `Cargo.toml` 中插件的版本號格式不對。
  - **解決**: 打開 `src-tauri/Cargo.toml`，將類似 `tauri-plugin-log = "2"` 的版本號，更改為完整的格式，例如 `tauri-plugin-log = "2.*"`。
- **`Error: failed to bundle project: timeout: global`**:
  - **原因**: 打包過程超時，通常是網路或電腦效能問題。
  - **解決**: 把代理切换到全局模式，或者尝试开启 TUN 模式

<br>

# Tauri 操作

---

- [官方文档](https://v2.tauri.app/start/)
- [官方案例 - example](https://github.com/tauri-apps/tauri/tree/dev/examples)

## 基本操作

> [!warning]
>
> 如果运行 `tauri run dev` 却发现，没有弹出窗口界面只有
>
> ```json
> Warn [tauri_cli::dev] Waiting for your frontend dev server to start on http://localhost:5137/...
> ```
>
> 这个是因为我们的端口和 vue3 启动的端口不一致
>
> - 记得在 `vite.config.ts` 指定 `serer {port : xxx}` 
> - 保持和 `tauri.conf.json` 的 `  "devUrl": "http://localhost:xxx",` 一致

```bash
# 桌面端直接运行的操作
tauri run dev 
```

<br>

## 如何自定义窗口

- [官方文档](https://v2.tauri.app/learn/window-customization/)
- [视频参考](https://www.bilibili.com/video/BV1VG411b7sk/?spm_id_from=333.337.search-card.all.click&vd_source=b47817c1aa0db593f452034d53d4273a)
- [vue3 的参考](https://zhuanlan.zhihu.com/p/681161178)

>  在你的 `tauri.conf.json` 文件中将 `decorations` 设置为 `false`

```json
{
  "windows": [
    {
      "decorations": false
    }
  ]
}
```

> 在这个文件中，`src-tauri/capabilities/default.json` 添加窗口权限
>
> [更多操作请参考这里](https://v2.tauri.app/zh-cn/reference/acl/)

```json
{
  "$schema": "../gen/schemas/desktop-schema.json",
  "identifier": "default",
  "description": "enables the default permissions",
  "windows": ["main"],
  "permissions": [
    "core:window:default",
    "core:window:allow-start-dragging",
      // 下面的部分就是关键部分 --> 对应窗口的三个操作
    "core:window:allow-close",
    "core:window:allow-toggle-maximize",
    "core:window:allow-minimize"
  ]
}

```

> 安装下面这个 npm 包

```bash
import { Window } from '@tauri-apps/api/window';
```

>  创建一个默认的标题栏

```vue
<template>
    <!-- data-tauri-drag-region is required to make the titlebar letting window draggable -->
    <div class="titlebar" data-tauri-drag-region>
        <div class="titlebar-button" data-tauri-drag-region="false" @click="minimize">
            <img src="https://api.iconify.design/mdi:window-minimize.svg" alt="minimize" />
        </div>
        <div class="titlebar-button" data-tauri-drag-region="false" @click="toggleMaximize">
            <img src="https://api.iconify.design/mdi:window-maximize.svg" alt="maximize" />
        </div>
        <div class="titlebar-button" data-tauri-drag-region="false" @click="close">
            <img src="https://api.iconify.design/mdi:close.svg" alt="close" />
        </div>
    </div>
</template>

<script setup lang="ts">
import { Window } from '@tauri-apps/api/window';	// npm 下载的
const appWindow = new Window("main");

/* 操作的部分, 就是我们 capabilities/default.json */
const minimize = () => {
    appWindow.minimize();
};

const toggleMaximize = () => {
    appWindow.toggleMaximize();
};

const close = () => {
    appWindow.close();
};
</script>

<style scoped>
.titlebar {
    height: 30px;
    background: #3F384C;
    user-select: none;
    display: flex;
    justify-content: flex-end;
    position: fixed;
    top: 0;
    left: 0;
    right: 0;
}

.titlebar-button {
    display: inline-flex;
    justify-content: center;
    align-items: center;
    width: 30px;
    height: 30px;
    user-select: none;
}

.titlebar-button:hover {
    background: #5bbec3;
}
</style>
```

## 直接创建一个工程

```bash
npm create tauri-app@latest my-tauri-app
```

> `Template created`! To get started run:
>
> ```bash
> cd my-tauri-app
> npm install
> npm run tauri init
> ```


> For `Desktop` development, run:
> ```bash
> npm run tauri dev
> ```


> For `Android` development, run:
>
> ```bash
> npm run tauri android init
> npm run tauri android dev
> ```

## 如何绑定创建一个快捷键操作

- [官方教程](https://v2.tauri.app/plugin/global-shortcut/)

```bash
npm run tauri add global-shortcut
```



## 资源文件管理

- [CSDN 参考](https://blog.csdn.net/xiaoyuanbaobao/article/details/148617514)

> [!warning]
>
> 我们在如软件中访问的哪些文件，都需要考虑到打包，所以就有了这个部分

```json
{
  "bundle": {
    // 这里会把需要的文件全部都打包到我们的指定资源文件位置
    "resources": [
      "/absolute/path/to/textfile.txt",
      "relative/path/to/jsonfile.json",
      "resources/*"
    ]
  }
}
```

> 譬如，你看我的写法，对应的文件所在位置就是：`D:\Study\Rust\my-tauri-app\src-tauri\target\debug\rs_resources`
>
> 我们暂且称呼 `resources` 里面列举的文件都叫做`资源文件`

```json
"bundle": {
    "active": true,
    "targets": "all",
    "icon": [
      "icons/32x32.png",
      "icons/128x128.png",
      "icons/128x128@2x.png",
      "icons/icon.icns",
      "icons/icon.ico"
    ],
    "resources": [
      "rs_resources/img/*",
      "rs_resources/res/*"
    ]
}
```

### plugin-fs

> [!note]
>
> 通常为了考虑到我们`文件的操作`和`安全管理`，我们会使用 `fs` 这个[插件](https://v2.tauri.app/zh-cn/plugin/file-system/)
>
> - 这个模块阻止路径遍历，不允许绝对路径或父目录组件
> - 使用这个 API 访问的路径必须相对于一个基本目录（`BaseDirectory.AppData`）
> - 这个插件提供了前端大量的相对于`资源文件`的操作能力

```ts
import { exists, BaseDirectory } from '@tauri-apps/plugin-fs';

// 检查 `$APPDATA/avatar.png` 文件是否存在
await exists('avatar.png', { baseDir: BaseDirectory.AppData });
```

- 为了开放我们对资源文件的读取能力，我们必须给出一下的配置

```json
// default.json
  "permissions": [
    "fs:default",
    "core:default",
    "core:window:default",
    "core:window:allow-start-dragging",
    "global-shortcut:allow-is-registered",
    "global-shortcut:allow-register",
    "global-shortcut:allow-unregister",
+   "fs:allow-read-file",
+   "fs:allow-read-text-file",
+   "fs:allow-resource-read-recursive"
  ]
```

### 转换文件为 url 连接到页面

> [!warning]
>
> 一定要开启 `tauri.conf.json` 里面的配置

```json
"app": {
	// ...
    // 这些内容一定要写进去
    "security": {
      "csp": {
        "default-src": "'self' customprotocol: asset:",
        "connect-src": "ipc: http://ipc.localhost",
        "font-src": ["https://fonts.gstatic.com"],
        "img-src": "'self' asset: http://asset.localhost blob: data:",
        "style-src": "'unsafe-inline' 'self' https://fonts.googleapis.com"
      },
      "assetProtocol": {
        "enable" : true,
        "scope": {
          "allow": ["$APPDATA/db/**", "$RESOURCE/**"],
          "deny": ["$APPDATA/db/*.stronghold"]
        }
      }
    }
},
```

