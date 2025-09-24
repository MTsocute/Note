# Pinia 笔记

> 直奔主题，干货优先 — Pinia 最常用的用法、类型化、持久化、最佳实践以及常见坑。适合 Vue 3 + TypeScript / JS 项目快速查阅。

<br>

## 1. 简介
Pinia 是 Vue 官方推荐的状态管理库（用于 Vue 3）。API 简洁、首级 TypeScript 支持、体积小、与 devtools 集成良好。概念更贴近“组合式 store”。

---

## 安装
~~~bash
# npm
npm install pinia

# 或 pnpm
pnpm add pinia

# 可选：持久化插件
pnpm add pinia-plugin-persistedstate
~~~

在主文件挂载：
~~~js
// main.ts
import { createApp } from 'vue'
import { createPinia } from 'pinia'
import App from './App.vue'

const app = createApp(App)
const pinia = createPinia()
app.use(pinia)
app.mount('#app')
~~~

---

## 基本用法（Composition API）
~~~ts
// stores/counter.ts
import { defineStore } from 'pinia'

export const useCounterStore = defineStore('counter', () => {
  // state
  const count = ref(0)

  // getters (computed)
  const double = computed(() => count.value * 2)

  // actions
  function increment() {
    count.value++
  }
  async function incrementAsync() {
    await new Promise(r => setTimeout(r, 200))
    count.value++
  }

  return { count, double, increment, incrementAsync }
})
~~~

使用：
~~~vue
<script setup lang="ts">
import { useCounterStore } from '@/stores/counter'
const store = useCounterStore()
store.increment()
</script>
~~~

---

## 选项式写法（Options API）
~~~ts
// stores/todo.ts
import { defineStore } from 'pinia'

export const useTodoStore = defineStore('todo', {
  state: () => ({
    list: [] as { id: number; title: string; done: boolean }[]
  }),
  getters: {
    remaining: (state) => state.list.filter(i => !i.done).length
  },
  actions: {
    add(item: { id: number; title: string }) {
      this.list.push({ ...item, done: false })
    },
    toggle(id: number) {
      const t = this.list.find(i => i.id === id)
      if (t) t.done = !t.done
    }
  }
})
~~~

---

## TypeScript 常用范式
- 对于 Composition API，尽量把 `ref`/`computed` 的类型写清楚或通过返回值推断。
- 对于 Options API，state 的类型可用接口或 `as` 断言。

示例（带接口）：
~~~ts
interface User { id: string; name: string }

export const useUserStore = defineStore('user', {
  state: () => ({
    me: null as User | null
  }),
  actions: {
    setUser(u: User) { this.me = u }
  }
})
~~~

**注意**：不要在 state 中放大对象（binary、DOM 节点）；store 用来放纯数据。

---

## 异步操作：actions 与组合
- 把副作用／异步请求放入 actions。
- actions 可以返回 Promise，组件中可 `await store.someAction()`。

示例：
~~~ts
export const useAuthStore = defineStore('auth', {
  state: () => ({ token: '' }),
  actions: {
    async login(username: string, pwd: string) {
      const res = await fetch('/api/login', { method: 'POST', body: JSON.stringify({ username, pwd }) })
      const data = await res.json()
      this.token = data.token
      return data
    }
  }
})
~~~

---

## 7. getters 与 computed 等价
- Pinia 的 getters 在内部就是 `computed`。
- getters 可以接收 `state`（Options API）或直接使用 `computed`（Composition API）。

**注意**：不要在 getters 中做副作用（异步、修改 state）。

---

## 插件与持久化
示例：`pinia-plugin-persistedstate`
~~~ts
import { createPinia } from 'pinia'
import piniaPluginPersistedstate from 'pinia-plugin-persistedstate'

const pinia = createPinia()
pinia.use(piniaPluginPersistedstate)
app.use(pinia)
~~~

在 store 中启用：
~~~ts
export const useCounterStore = defineStore('counter', {
  state: () => ({ count: 0 }),
  persist: true // 或者配置对象：{ storage: sessionStorage, paths: ['count'] }
})
~~~

**坑**：持久化复杂对象或类实例会丢失方法，优先序列化纯数据。

---

##  在组件中使用
- `setup()` 内直接使用 `useXxxStore()`。
- 在 Options API 的组件里，用 `this.$pinia` 不常见，推荐使用 `useXxxStore()` 并放在 `created()` 或 `mounted()` 中。

示例（非 setup）：
~~~vue
<script lang="ts">
import { defineComponent } from 'vue'
import { useCounterStore } from '@/stores/counter'

export default defineComponent({
  setup() {
    const counter = useCounterStore()
    return { counter }
  }
})
</script>
~~~

**注意**：不要在组件外（模块顶层）调用 `useXxxStore()`，那样会在未挂载 Pinia 时出问题（SSR / 测试）。

---

## 10. SSR 注意点
- 在服务器端渲染时每次请求应创建新的 Pinia 实例（避免共享状态）。
- 服务端创建 Pinia：`const pinia = createPinia(); app.use(pinia)`，然后将 state 序列化注入到 HTML，再在客户端 `hydrate`。
- 使用 `pinia.state.value` 获取/注入 state。

简要流程：
1. 服务端创建 store，填充数据。
2. 使用 `pinia.state.value` 获取状态并注入 `<script>`。
3. 客户端创建新的 Pinia，并用注入的 statehydrate。

---

##  测试要点
- 测试 store 本身时，直接 import store 并在每个测试里创建新的 pinia（例如 use createPinia + setActivePinia）。
- 推荐使用 `setActivePinia(createPinia())`（来自 pinia 测试文档）。
- 给 store mock 网络请求（axios/fetch）。

示例：
~~~ts
import { setActivePinia, createPinia } from 'pinia'
beforeEach(() => setActivePinia(createPinia()))
~~~

---

## 从 Vuex 迁移要点
- state → state
- getters → getters（或 computed）
- mutations → 取消（直接在 actions 里 `this.x = y`）
- actions → actions（保持异步）
- 更轻量，移除大量模板代码。

迁移技巧：把 mutation 合并到 action，逐步替换组件里 `mapState/mapGetters` 为 `useStore()`。

---

## 附录：常用代码片段速览
~~~ts
// defineStore（composition）
export const useExample = defineStore('example', () => {
  const items = ref<string[]>([])
  const count = computed(() => items.value.length)
  function add(v: string) { items.value.push(v) }
  return { items, count, add }
})

// defineStore（options）
export const useExampleOpt = defineStore('exampleOpt', {
  state: () => ({ items: [] as string[] }),
  getters: { count: s => s.items.length },
  actions: { add(v: string) { this.items.push(v) } }
})
~~~

