# Router

---

##  前置路由

> `router` 有一个控制的函数叫做`beforeEach` 可以控制我们跳转的行为，所以如果我们在里面加入一个白名单和token校验的话，实际上我们就可以实现一个简单的前端拦截功能

```ts
import useRouter from 'vue-router'
import router from '@/router/index.ts'

const whiteList = ['/', '/404']

router.beforeEach(async (to, from, next) => {
  const isTokenValidity: boolean = await validateToken()
  if (whiteList.includes(to.path) || isTokenValidity) {
    next()
  } else {
      // next("/")
    next({name: "home"})
  }
})

async function validateToken(): Promise<boolean> {
  return new Promise((resolve, reject) => {
    // resolve(true)
    // 具体逻辑自己写哈，反正就是往后端发送一个请求，然后获取看下对不对
  })
}

```

> 同时为了简化，以免我们的 `router` 页面过于冗长占据内容，我们可以单独在外面创建一个 router 然后放进去

```ts
const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  // 就是这个部分，我们在外面用 ts 文件单独写一个
  routes: [
    {
      path: '/',
      name: 'home',
      component: HomeView,
    },
    {
      path: '/about',
      name: 'about',
      component: () => import('../views/AboutView.vue'),
    },
  ],
})
```

> 我们在外面实现一个 `staticRouter`

```ts
import type { RouteRecordRaw } from 'vue-router'
import HomeView from '@/views/HomeView.vue'

export const staticRouter: RouteRecordRaw[] = [
  {
    path: '/',
    name: 'home',
    component: HomeView,
  },
  {
    path: '/about',
    name: 'about',
    component: () => import('@/views/AboutView.vue'),
  },
]
```

> 然后简化这个 `routes` 部分

```ts
import { createRouter, createWebHistory } from 'vue-router'
import { staticRouter } from '@/router/routers/staticRouter.ts'

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: staticRouter,
})

export default router
```

<br>

##  后置路由

> 实现页面跳转的进度条，这里，我们配合第三方库实现
>
> >  [!important]
> >
> >  [解决 ts 中使用 nprogress 类型报错的方法](https://blog.csdn.net/weixin_45686265/article/details/125024253)

```bash
# 安装
npm i nprogress

# 在需要的位置导包
import NProgress from 'nprogress'
import 'nprogress/nprogress.css'
```

- 因为 nprogress 是 `js` ，在 `vite + ts` 所以还需要在 `env.d.ts` 声明这个三方库才不会报错

```ts
declare module 'nprogress'
```

主要就是两个函数：

- `NProgress.start()`
- `NProgress.done()`

> 进度条的配置可以做成一个函数，外部启用

```ts
/** 顶部进度条配置 */
export const configureNProgress = () => {
  NProgress.configure({
    easing: 'ease',
    speed: 600,
    showSpinner: false,
    trickleSpeed: 200,
    parent: 'body',
  })
}
```

<br>

## 路由元信息

> 我们可以在静态的路由的里面加入一个 `meta` 标签

```ts
export const staticRoute: RouteRecordRaw[] = [
  {
    path: '/',
    name: 'home',
    component: HomeView,
    // 就是这里
    meta: {
      title: 'Home'
    }
  },
]
```

> 然后我们肯定是要在前端路由里面做处理的

![image-20250717110749298](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250717110749298.png)

> [!important]
>
> 你会发现有一个报错，这里是因为 meta 数据的类型是不知道的，所以需要自己声明才可以，那么我们在静态路由里面多补充一个声明类型

```ts
declare module 'vue-router' {
  interface RouteMeta {
    title: string
  }
}
```

<br>

### 4. 动态路由

---

> 假定后端发送给过来的 `json` 数据如下：

```json
// 后端返回的 menus
{
    "name": "order",
    "path": "/order/list",
    "component": "OrderList",
    "meta": { "title": "订单管理", "icon": "order" }
}
```

> 动态路由


```ts
import { createRouter, createWebHistory } from 'vue-router'
const staticRoutes = [
  { path: '/login', component: () => import('@/views/Login.vue') }
]
const router = createRouter({
  history: createWebHistory(),
  routes: staticRoutes
})

// 路由守卫
router.beforeEach(async (to) => {
  if (!router.hasRoute('dashboard') && localStore.token) {
    const { menus } = await getUserMenus() // 调后端接口，更具命令去往对应的页面
    menus.forEach(msg => {
      // 动态路由 
      router.addRoute('root', {
        path: m.path,
        name: m.name,
        component: () => import(`@/views/${msg.component}.vue`)
      })
    })
    // 关键：重新触发一次跳转
    return to.fullPath
  }
})
export default router
```

### 路由递归所有路径

```ts
/**
 * 将菜单数据转换为路由配置
 * @param route 菜单数据对象
 * @param parentPath 父级路径
 * @returns 处理后的路由配置
 */
export const menuDataToRouter = (route: AppRouteRecord, parentPath = ''): AppRouteRecord => {
  const fullPath = buildRoutePath(route, parentPath)
  // console.log(fullPath)
  return {
    ...route,
    path: fullPath,
    children: processChildren(route.children || [], fullPath)
  }
}

/**
 * 构建路由完整路径
 * @param route 菜单数据对象
 * @param parentPath 父级路径
 * @returns 构建后的完整路径
 */
const buildRoutePath = (route: AppRouteRecord, parentPath: string): string => {
  if (!route.path) return ''

  // iframe 类型路由直接使用原始路径
  if (route.meta?.isIframe) return route.path

  // 拼接并规范化路径
  return parentPath ? `${parentPath}/${route.path}`.replace(/\/+/g, '/') : route.path
}

/**
 * 处理子路由
 * @param children 子路由数组
 * @param parentPath 父级路径
 * @returns 处理后的子路由数组
 */
const processChildren = (children: AppRouteRecord[], parentPath: string): AppRouteRecord[] => {
  if (!Array.isArray(children) || children.length === 0) return []

  return children.map((child) => menuDataToRouter(child, parentPath))
}
```

<br>