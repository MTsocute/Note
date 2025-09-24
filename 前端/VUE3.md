# VUE 3

---

- [suspense 异步组件](https://www.bilibili.com/video/BV1dS4y1y7vd?spm_id_from=333.788.player.switch&vd_source=b47817c1aa0db593f452034d53d4273a&p=20)

> 这里涉及到了如何启动一个项目。我们有 `vite` 和 `vue` 两种，第一个比较轻量

```shell
npm init vite@latest		# 最基本的构成

npm init vue@latest		# 比较完整的
```

> 同时对于不同的 node 版本的管理，我们可以使用 `nvm` 实现

进入了项目之后，记得 `npm i` 下载依赖

> 大部分的 `npm` 可运行的命令都在一个 `package.json` 的 `script` 部分

<br>

## Vite 目录

> [具体介绍](https://www.bilibili.com/video/BV1dS4y1y7vd?spm_id_from=333.788.player.switch&vd_source=b47817c1aa0db593f452034d53d4273a&p=4)

```shell
my-vite-app/
├── index.html               # 项目入口 HTML，Vite 的核心入口文件
├── package.json             # 项目依赖和脚本定义
├── vite.config.ts|js        # Vite 配置文件（可选）
├── public/                  # 静态资源目录，不会被 Vite 打包处理，原样复制
│   └── favicon.svg
├── src/                     # 源码目录，主要开发工作在这里
│   ├── main.ts|js           # 程序主入口，挂载 app
│   ├── App.vue|jsx          # 根组件（如果使用 Vue 或 React）
│   ├── assets/              # 静态资源，如图片、字体等（会参与打包）
│   ├── components/          # 通用组件
│   ├── pages/               # 页面组件（常用于前端路由）
│   └── styles/              # 样式文件（CSS/SASS/Tailwind 等）
├── tsconfig.json            # TypeScript 配置（如果使用）
└── .gitignore               # Git 忽略项
```

> 其中我们注意到，`index.html` 这个才是游览器的中心架构，而 `vue` 是一个组件，挂载到页面当中去的

```html
<!DOCTYPE html>
<html lang="">
  <head>
    <meta charset="UTF-8">
    <link rel="icon" href="/favicon.ico">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Vite App</title>
  </head>
  <body>
    <!-- 这里就是 vue 的挂载位置 -->
    <div id="app"></div>
    <!-- 这个 ts 脚本让 vue 作为组件挂载到这个 div 当中去 -->
    <script type="module" src="/src/main.ts"></script>
  </body>
</html>

```

> [!important]
>
> 我们来看下 `main.ts` 是如何工作的，让 `vue` 作为组件挂载到其中去

```ts
import { createApp } from 'vue'
import { createPinia } from 'pinia'

// * css --> 用来管理 index.html 的 css
import '@/assets/style/base.css'

// 把 app.vue 作为组件
import App from './App.vue'
import router from './router'

// 让组件成为一个 #app 的 name 标签
const app = createApp(App)

app.use(createPinia())
app.use(router)

// 挂载 #app 到 index.html
app.mount('#app')
```

所以如果你创建了一个脚手架之后，删除了一些默认样式，发现如果浏览器出现了页边距，那就记得在这个 `css` 文件做修改

<br>

### 1. 环境变量

---

> 查看自带的环境

```js
console.log(import.meta.env)
```

![image-20250716153644111](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250716153644111.png)

> 我们先创建如下的两个文件用于切换环境
>
> - `.env.development`
> - `.env.production`  // 生产环境(build)默认读取这个

```.env.development
// .env.development
VITE_HTTP = http://www.momoSuperCool.com
```

> 然后再 `package.json` 文件修改脚本让我们的环境生效

```js
"scripts": {
"dev": "vite --mode .env.development",
// ...
},
```

![image-20250716160141477](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250716160141477.png)

### 2. 实现路径别名

> 为了让自己取得别名能够被找到，我们采用如下的配置方式
>
> 找到 `tsconfig.ts`

```ts
// tsconfig.ts
{
  "compilerOptions": {
    "target": "ESNext",                   // 支持最新 JS 特性
    "module": "ESNext",                   // 使用 ES 模块
    "moduleResolution": "bundler",        // 查找和解析模块
    "useDefineForClassFields": true,      // Vue3 推荐开启
    "strict": true,                       // 启用 TS 严格模式（更安全）
    "baseUrl": ".",                       // 相对路径基准
    "paths": {
      "@/*": ["src/*"]
    },
    "resolveJsonModule": true,            // 允许引入 .json 文件
    "isolatedModules": true,              // Vue3 + Vite 必开，否则 `<script setup>` 报错
    "esModuleInterop": true,              // ESM 与 CommonJS 兼容
    "allowSyntheticDefaultImports": true, // 配合上面那个
    "lib": ["ESNext", "DOM"],             // 支持最新浏览器 API
    "types": ["vite/client"]              // 提供 vite 全局类型提示
  },
  "include": ["src"],                     // 无需写冗余的 `src/**/*.ts`
  "exclude": ["node_modules", "dist"]
}

```

> 然后再我们的 `vite.config.ts` 实现如下的部分

```ts
  resolve: {
    alias: {
      '@': fileURLToPath(new URL('./src', import.meta.url)),
      '@styles': resolvePath('./src/assets/styles')
    },
  },
    
import path from 'path'
// 还有还有一个自定义找路径的函数
function resolvePath(paths: string) {
  return path.resolve(__dirname, paths)
}
```



<br>

## `vue 3` 语法

---

### v-if

```vue
<template>
  <p v-if="role === 'admin'">管理员页面</p>
  <p v-else-if="role === 'user'">用户页面</p>
  <p v-else>访客页面</p>
</template>

<script setup>
const role = 'user'  // 可切换为 'admin' / 'guest'
</script>
```

<br>

### v-on

---

#### 1. **点击事件**（最常用）

```vue
<button @click="handleClick">点击我</button>

<script setup>
function handleClick() {
  alert('按钮被点击了！')
}
</script>
```

------

#### 2. **输入事件**

```vue
<input @input="onInput" placeholder="输入点什么..." />

<script setup>
function onInput(e) {
  console.log('输入内容：', e.target.value)
}
</script>
```

------

#### 3. **提交事件**

```vue
<form @submit.prevent="onSubmit">
  <input />
  <button type="submit">提交</button>
</form>

<script setup>
function onSubmit() {
  alert('表单提交！')
}
</script>
```

> 注意 `.prevent` 是修饰符，用来阻止默认行为。

------

#### 4. **键盘事件**

```vue
<input @keyup.enter="onEnter" placeholder="按 Enter 触发" />

<script setup>
function onEnter() {
  alert('按下了 Enter')
}
</script>
```

------

#### 5. **鼠标进入 / 离开**

```vue
<div @mouseenter="onEnter" @mouseleave="onLeave">悬停我</div>

<script setup>
function onEnter() {
  console.log('鼠标进入')
}
function onLeave() {
  console.log('鼠标离开')
}
</script>
```

#### 6. 动态绑定

> @`[]` 这个部分就可以实现不同事件的绑定，不会写死了

```vue
<template>
  <button @[eventName]="handleEvent">点击我</button>
</template>

<script setup>
const eventName = 'click'

function handleEvent() {
  alert(`触发了 ${eventName} 事件！`)
}
</script>

```

<br>

### v-bind

> 就是一个 html 里面的标签属性，都可以通过 : 来实现绑定变量实现动态变化

```vue
<template>
  <img :src="imgUrl" :alt="imgDesc" />
  <button :disabled="isSubmitting">提交</button>
  <a :href="link" target="_blank">访问</a>
  <input :type="inputType" :placeholder="placeholderText" />
</template>

<script setup>
const imgUrl = 'https://example.com/logo.png'
const imgDesc = '网站 Logo'
const isSubmitting = true  // 此时按钮将被禁用
const link = 'https://vitejs.dev'
const inputType = 'email'
const placeholderText = '请输入邮箱'

</script>
```

<br>

### v-for

---

#### 1. 渲染数组

```vue
<ul>
  <li v-for="fruit in fruits">{{ fruit }}</li>
</ul>

<script setup>
const fruits = ['🍎 苹果', '🍌 香蕉', '🍇 葡萄']
</script>

```

#### 2. 获取索引

> ## 🔑 `:key` 的作用是什么？
>
> **作用**：帮助 Vue 精确、快速地识别每个元素或组件，使得在数据更新时能**复用 DOM**，避免重绘错误，提升性能。

```vue
<li v-for="(fruit, index) in fruits" :key="index">
  {{ index + 1 }}. {{ fruit }}
</li>

<script setup>
const fruits = ['🍎 苹果', '🍌 香蕉', '🍇 葡萄']
</script>
```

#### 3. 遍历对象

```vue
<div v-for="(value, key) in user" :key="key">
  {{ key }}: {{ value }}
</div>

<script setup>
const user = {
  name: 'Momo',
  age: 24,
  city: 'Taipei'
}
</script>
```

#### 4.嵌套循环

```vue
<div v-for="group in groups" :key="group.name">
  <h3>{{ group.name }}</h3>
  <ul>
    <li v-for="item in group.items" :key="item">{{ item }}</li>
  </ul>
</div>

<script setup>
const groups = [
  { name: '水果', items: ['🍎', '🍌'] },
  { name: '蔬菜', items: ['🥕', '🥬'] }
]
</script>
```

<br>

### v-model

---

> `v-model` 是一个 **双向绑定指令**，用来让视图和数据保持同步。

```vue
<template>
  <input v-model="name" />
  <p>你输入的是：{{ name }}</p>
</template>

<script setup>
import { ref } from 'vue'

const name = ref('')
</script>
```

> 使用场景

| 使用场景              | 用法示例                                      | 说明                         |
| --------------------- | --------------------------------------------- | ---------------------------- |
| 表单控件              | `<input v-model="name" />`                    | 绑定文本框、下拉框、复选框等 |
| `textarea` 编辑内容   | `<textarea v-model="desc"></textarea>`        | 多行文本编辑                 |
| `checkbox`、`radio`   | `<input type="checkbox" v-model="checked" />` | 控制布尔值                   |
| 组件通信（父 <-> 子） | `<MyInput v-model="title" />`                 | 自定义组件中传值回传         |

<br>

### Teleport

---

> [!warning]
>
> 假定我们有如下的一个场景，这里是一个 `A.vue` 组件，有如下的内容

```vue
<!--   A.vue   -->
<template>
  <div class="page-container">
    <div class="top-section">
      <!--   制作一个全屏居中的卡片   -->
      <teleport to="body"><login-card /></teleport>
    </div>
    <div class="bottom-section"></div>
  </div>
</template>
```

​	这里存在一个问题，就是我们的卡片其实是 `top-section` 的一部分，我们如何让他脱离但当前的组件，然后布局到正中间呢？于是就有了上面这个 `teleport` 的作用，所以现在创建出来的 `login-card` 其实和 `#app` 同级别

<br>

### Provide / Inject

---

> [!important]
>
> 在 Vue 中，`provide` 和 `inject` 是一对用于**跨级组件通信**的 API，可以让**父组件提供数据**，**后代组件（不限于子组件）注入数据使用**。

#### `父组件`

```vue
<template>
  <div>
    <h2>父组件：颜色控制</h2>
    <div class="buttons">
      <button v-for="c in colors" :key="c" @click="setColor(c)">
        {{ c }}
      </button>
    </div>

    <ColorBox />
  </div>
</template>

<script setup lang="ts">
import { ref, provide } from 'vue'
import ColorBox from './ColorBox.vue'

// 预定义颜色列表
const colors = ['red', 'green', 'blue', 'orange', 'purple']

// 颜色状态
const currentColor = ref<string>('red')

provide('color', currentColor)		// <== 将颜色通过 provide 传递

// 切换颜色函数
function setColor(newColor: string) {
  currentColor.value = newColor
}
</script>

<style scoped>
.buttons {
  margin-bottom: 1em;
}

button {
  margin-right: 8px;
  padding: 6px 12px;
  border: none;
  background-color: #ddd;
  border-radius: 4px;
  cursor: pointer;
  font-weight: 600;
  transition: background-color 0.3s ease;
}

button:hover {
  background-color: #bbb;
}
</style>

```

#### `子组件`

```vue
<template>
  <div class="color-box" :style="{ backgroundColor: color }">
    当前颜色：{{ color }}
  </div>
</template>

<script setup lang="ts">
import { inject } from 'vue'

const color = inject<string>('color', 'gray')		// <== 从父组件注入颜色，默认值为 'gray'
</script>

<style scoped>
.color-box {
  width: 150px;
  height: 150px;
  border-radius: 8px;
  color: white;
  display: flex;
  align-items: center;
  justify-content: center;
  font-weight: bold;
  user-select: none;
}
</style>

```

<br>

## 响应式

---

> 数据变化后，绑定的视图会自动更新

### 1. `ref`

```vue
<template>
  <button @click="increment">Count: {{ count }}</button>
</template>

<script setup>
import { ref } from 'vue'

// 创建一个响应式变量
const count = ref(0)

// 修改值时必须用 `.value`
function increment() {
  count.value++
}
</script>
```

<br>

### 2.`reactive`

```vue
<script setup>
import { reactive } from 'vue'
    
const state = reactive({
  count: 0,
  user: {
    name: 'Momo',
    age: 25,
  },
})

// 取值和赋值不需要 .value
state.count = 12

function increment() {
  state.count++
}
    
</script>
```

> 渲染到 template 的案例

```vue
<template>
  <form @submit.prevent="submitForm">
    <div>
      <label>姓名：
        <input v-model="state.name" type="text" />
      </label>
    </div>
    <div>
      <label>年龄：
        <input v-model.number="state.age" type="number" />
      </label>
    </div>
    <button type="submit">提交</button>
  </form>

  <div v-if="submitted">
    <p>提交的姓名：{{ state.name }}</p>
    <p>提交的年龄：{{ state.age }}</p>
  </div>
</template>

<script setup>
import { reactive, ref } from 'vue'

const state = reactive({
  name: '',
  age: null,
})

const submitted = ref(false)

function submitForm() {
  submitted.value = true
  console.log('提交数据:', state)
}
</script>

```

### 3. reactive 的赋值更新方法

> 直接的赋值不会更新，必须要使用 `push` 方法

```vue
<template>
  <div>
    <h3>演示 reactive 是 Proxy，不能直接赋值替换</h3>

    <p>数组内容: {{ state.items.join(', ') }}</p>

    <button @click="replaceArray">直接替换数组（错误示范）</button>
    <button @click="pushItem">用 push 添加元素（正确示范）</button>
  </div>
</template>

<script setup>
import { reactive } from 'vue'

const state = reactive({
  items: ['A', 'B', 'C'],
})

function replaceArray() {
  // 直接赋值替换整个数组（会破坏响应式，视图不更新）
  state.items = ['X', 'Y', 'Z']
}

function pushItem() {
  // 用 push 修改数组，保持响应式，视图会更新
  state.items.push('D')
}
</script>

```

### 4. `computed`

> `computed` 是计算属性，用来 **根据已有的数据计算出一个值**，并且具有 **缓存特性**（依赖的值不变时，computed 不会重新计算）
>
> [参考一下和 v-model 绑定为什么实现更新和显示](https://www.bilibili.com/video/BV1F5411g7qp/?spm_id_from=333.337.search-card.all.click&vd_source=b47817c1aa0db593f452034d53d4273a)

```vue
<template>
  <p>名字：{{ fullName }}</p>
</template>

<script setup>
import { ref, computed } from 'vue'

const firstName = ref('Momo')
const lastName = ref('Chan')

// 计算属性
const fullName = computed(() => {
  return firstName.value + ' ' + lastName.value
})
</script>
```

> 这种写法是不可以修改值的，我们可以用下面这种方式实现可写

```vue
<template>
  <div>
    <h3>分开输入：</h3>
    <input v-model="firstName" placeholder="姓" />
    <input v-model="lastName" placeholder="名" />

    <p>自动组合：{{ fullName }}</p>

    <h3>合并输入（自动拆分）：</h3>
    <input v-model="fullName" placeholder="姓 名" />
  </div>
</template>

<script setup>
import { ref, computed } from 'vue'

// 独立的姓和名
const firstName = ref('Momo')
const lastName = ref('Chan')

// 计算属性 fullName 自动组合和拆分
const fullName = computed({
  get() {
    return `${firstName.value} ${lastName.value}`
  },
  set(val) {
    const parts = val.trim().split(' ')
    firstName.value = parts[0] || ''
    lastName.value = parts[1] || ''
  }
})
</script>

```

### 5. watch

> `watch(要监听的值, 回调函数)`

```vue
<script setup>
import { ref, watch } from 'vue'

const count = ref(0)

watch(count, (newVal, oldVal) => {
  console.log(`count 从 ${oldVal} 变成了 ${newVal}`)
})
</script>
```

<br>

## PostCSS 全局样式控制

---

> 把 px 自动转换成 vw 单位，适配响应式视口宽度。

### 1. 配置文件

> [!warning]
>
> 为了适配 `vite + ts`，我们不可以直接写 js 文件，而要用 `cjs` 来写配置文件

<br>

### 2. 常见的需求

<br>

## SCSS

---

> **Sass 是一种 Css 预处理器（preprocessor）；**
>
> - 借助 Sass 语法，我们能更加高效地为页面编写样式
> - Sass 可以让我们使用一些 CSS 中没有实现的功能，例如 **变量、嵌套、组合、继承** 等等；
>
> 我们使用 Sass 语法来编写 `.scss` 文件，然后使用 Sass 预处理器将它们编译为 `.css` 文件，最后将编译产生的 `.css` 文件应用到我们的工程中。

| 功能名   | 例子                                                         |
| -------- | ------------------------------------------------------------ |
| **变量** | `$color: red;`                                               |
| **嵌套** | `.nav { ul { li { color: red }}}`                            |
| **继承** | `%btn { padding: 10px } .submit { @extend %btn }`            |
| **混合** | `@mixin shadow { box-shadow: 1px 1px 3px #000 }`             |
| **导入** | `@import 'xxx.scss'`（注意 `Vite`不支持 node_modules 里的旧 import） |
| **函数** | `lighten($color, 10%)`，颜色变浅                             |

<br>

### 1. `@mixin`

> 这个和它的名字一样，让我们定义的一些属性，作为一部分混入到我们某个装饰器中

```scss
// 命名空间（统一组件前缀）
$namespace: 'momo' !default;

// block 连接符，如：momo-button
$block-sel: '-' !default;

// element 连接符，如：momo-button__icon
$elem-sel: '__' !default;

// modifier 连接符，如：momo-button--primary
$mod-sel: '--' !default;


// app 样式
@mixin bfc {
  height: 100vh;
  overflow: hidden;
}

// Block 混入：.momo-button
@mixin b($block) {
  $B: #{$namespace + $block-sel + $block};
  .#{$B} {
    @content;
  }
}

// Element 混入：.momo-button__icon
@mixin e($el) {
  $selector: &;
  // 这个会自动加载夫修饰器在前面，但是其实也没必要
  @at-root {
    #{$selector + $elem-sel + $el} {
      @content;
    }
  }
}

// Modifier 混入：.momo-button--primary
@mixin m($mod) {
  $selector: &;
  @at-root {
    #{$selector + $mod-sel + $mod} {
      @content;
    }
  }
}

```

<br>

### 2. `@at-root`

> 我们使用这个时候，一般说明是要跳出前面父级标签来的
>
> 也就是说，我们创建出来的是一个新的类别

```css
.momo-login {
  font-size: 4rem !important;
  color: red;
}
.momo-login__inner {
  font-size: 2rem !important;
  color: blue;
}
.momo-login--success {
  font-size: 1rem !important;
  color: green;
}
```

<br>

### 3. 配置某个 sass 为全局通用文件

> 在 `vite.config.ts` 文件中，加入 `css` 字段来解决这个问题

```ts
import { fileURLToPath, URL } from 'node:url'
import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import vueJsx from '@vitejs/plugin-vue-jsx'
import vueDevTools from 'vite-plugin-vue-devtools'

export default defineConfig({
  plugins: [
    vue(),
    vueJsx(),
    vueDevTools(),
  ],
  resolve: {
    alias: {
      '@': fileURLToPath(new URL('./src', import.meta.url))
    },
  },
  // 设置为全局变量 
  css: {
    preprocessorOptions: {
      scss: {
        additionalData: `@use "@/assets/styles/bem.scss" as *;`
      }
    }
  }
})
```

<br>



## SLOT

> ### 📦 什么是 `slot`？
>
> **插槽（slot）是 Vue 提供的一种机制，用于让父组件向子组件传递 HTML 结构或内容。**
>
> 你可以把它理解成“占位符”：子组件先占个位置，父组件再决定填什么内容进去。

### 🌱 基本插槽：默认插槽

#### 子组件（`BaseCard.vue`）：

```vue
<template>
  <div class="card">
    <slot></slot>  <!-- 默认插槽，占位 -->
  </div>
</template>
```

#### 父组件使用：

```vue
<template>
  <BaseCard>
    <p>这是传进 BaseCard 的内容</p>
  </BaseCard>
</template>

<script setup>
import BaseCard from './BaseCard.vue'
</script>
```

💡 上面这段代码里，`<p>...</p>` 被作为“内容”插到了子组件 `<slot>` 的位置。

------

### 🌿 具名插槽（Named Slot）

> 当你想要多个插槽时，用 `name` 来区分。

#### 子组件（`Layout.vue`）：

```vue
<template>
  <header><slot name="header" /></header>
  <main><slot /></main> <!-- 默认插槽 -->
  <footer><slot name="footer" /></footer>
</template>
```

#### 父组件使用：

```vue
<template>
  <Layout>
    <template #header>
      <h1>我是头部</h1>
    </template>

    <p>我是主要内容</p>

    <template #footer>
      <p>我是底部</p>
    </template>
  </Layout>
</template>
```

------

### 🌳 作用域插槽（Scoped Slot）

> 这比较进阶：**子组件把数据传出来给父组件用。**

#### 子组件（`List.vue`）：

```vue
<template>
  <div>
    <slot v-for="item in items" :item="item" :key="item.id"></slot>
  </div>
</template>

<script setup>
const items = [
  { id: 1, name: 'Vue' },
  { id: 2, name: 'React' },
]
</script>
```

#### 父组件使用：

```vue
<template>
  <List>
    <template #default="{ item }">
      <p>{{ item.name }}</p>
    </template>
  </List>
</template>
```

🔍 重点：子组件用 `:item="item"` 把数据提供给 `slot`，父组件用 `#default="{ item }"` 接收。

------

### 📘 小总结

| 类型       | 用途                     | 写法示例               |
| ---------- | ------------------------ | ---------------------- |
| 默认插槽   | 插入默认内容             | `<slot></slot>`        |
| 具名插槽   | 插入特定区域             | `<slot name="footer">` |
| 作用域插槽 | 子组件把数据给父组件使用 | `<slot :item="item">`  |

<br>



