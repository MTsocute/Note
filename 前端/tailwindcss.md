# TailWindCss

---

## 安装 Tailwind CSS 相关依赖
>在项目根目录下打开终端，运行以下命令：

```bash
npm install tailwindcss @tailwindcss/vite

npm install -D prettier prettier-plugin-tailwindcss
```

> Add the `@tailwindcss/vite` plugin to your Vite configuration.

```ts
// vite.config.ts
import { defineConfig } from 'vite'
import tailwindcss from '@tailwindcss/vite'

export default defineConfig({
  plugins: [
    tailwindcss(),
  ],
})
```



> [!important]
>
> `tailWind` 不支持支持 `vite` 6以上 版本，你可以用 `npm ls vite` 来查看当前安装的版本来变动
>
> 对于 4.0 以上的版本，还请[参考文档来配置](https://tailwindcss.com/docs/installation/using-vite)
>
> 这个插件实现的效果就是使用 `class="内置样式"` 来实现调整

```html
<!-- 直接用工具类组合 -->
<button class="bg-blue-500 text-white py-2 px-4 rounded">Click Me</button>
```

> [!important]
>
> 他甚至还可以在 `css` 中嵌套，减少 `class` 里面的长度

```css
/* 自定义按钮样式 */
.btn {
  /* import tailwind 样式 */
  @apply bg-blue-500 text-white py-2 px-4 rounded;
}
```

<br>

- [颜色对照表](https://tailwindcss.com/docs/colors)