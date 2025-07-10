# VUE

---

<br>

## 1. 工程的创建

> 导入 `elements-ui`

```shell
npm install element-plus --save
```

> 自动导入样式

```shell
npm install -D unplugin-vue-components unplugin-auto-import
```

> 把后续的放到 vite 里面

```ts
// vite.config.ts
import AutoImport from 'unplugin-auto-import/vite'
import Components from 'unplugin-vue-components/vite'
import { ElementPlusResolver } from 'unplugin-vue-components/resolvers'

export default defineConfig({
  // ...
  plugins: [
    // ...
    AutoImport({
      resolvers: [ElementPlusResolver()],
    }),
    Components({
      resolvers: [ElementPlusResolver()],
    }),
  ],
})
```

<br>

> 然后我们在 `main.ts` 中导入

```ts
import { createApp } from 'vue'
import ElementPlus from 'element-plus'		// <---
import 'element-plus/dist/index.css'		
import App from './App.vue'

const app = createApp(App)

app.use(ElementPlus)		// <---
app.mount('#app')
```

<br>

## 2.router 的基本使用

> 在我们的 `src/router/index.ts` 可以规定路由的跳转和对应的基本信息

```ts
import { createRouter, createWebHistory } from 'vue-router'
import HomeView from '../views/HomeView.vue'

const routes = [
  {
    path: '/',
    name: 'home',
    component: HomeView,
    meta: {
      title: '首页',
      icon: '/logo.png', 
    },
  },
  {
    path: '/doc',
    name: 'doc',
    component: () => import('../views/Document.vue'),
    meta: {
      title: '文档',
      icon: '/logo.png', 
    },
  },
]
```

> 但是就是你补充了基本信息也还是不会管用的，必须要自己设置，让设置的消息生效

```ts
// 创建 router 对象
const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes,
})

router.beforeEach((to, from, next) => {
  // 设置网页标题
  document.title = (to.meta.title as string) || '默认标题'

  // 设置网页图标 favicon
  if (to.meta.icon) {
    document.getElementsByTagName('link')[0].setAttribute("href", "" + to.meta.icon)
  }
  else {
    // 没有就设置默认图标 
    document.getElementsByTagName('link')[0].setAttribute("href", "/public/favicon.ico")
  }
  next()
})
```

> 404 页面的捕获

```ts
const routes = [
  // ...
 	
  // ⚠️ 最后一项：通配符匹配 404
  {
    path: '/:pathMatch(.*)*', // 匹配任意路径
    name: '404',
    meta: {
      title: '找不到页面',
      icon: '/logo.png',
    },
    component: () => import('../views/NOTFOUND.vue'),
  }
]
```

> 实现记住密码的方法

> Router-view 自己实现

```txt
/dashboard
  └── /settings
```



<br>

# GO

---

>  使用`sqlite` 创建一个数据库

```shell
sqlite3 database_name.db
```

> 使用下面两个库，作为驱动

```go
"github.com/glebarez/sqlite" // 纯 go 实现的 SQLite 驱动
"gorm.io/gorm"
```

```go
```



> 创建 ini 配置文件读取的部分

```go
import (
	"github.com/go-ini/ini"
)

var (
	AppMode  string
	HttpPort string
	LogPath  string
)


func init() {
	cfg_file, err := ini.Load("/config/config.ini")
	if (err != nil) {
		// TODO logurs 记录一切
		panic(err)
	}
	LoadServerConfig(cfg_file)
}


func LoadServerConfig(cfg_file * ini.File) {
	AppMode = cfg_file.Section("server").Key("AppMode").String()
	HttpPort = cfg_file.Section("server").Key("HttpPort").String()
	LogPath = cfg_file.Section("server").Key("LogPath").String()
}
```

中间件创建一个日志管理的系统

> 运行一个 gin 的最小构成

```go
func InitRouter() {
	gin.SetMode(utils.AppMode)
	r := gin.Default()
	r.Run(utils.HttpPort)
}
```

## 1. 日志系统

> 主要解决的问题有下面几个方面
>
> 1. 我们要配置多个输出器，以便于存储不同的格式到终端、文本
> 2. 文本自带的输出非常的难看，所以我们希望自定义格式

```go

```

> 接受第三方错误

> 保存 gin 崩溃错误的记录

> 校验用户输入数据的错误与否

## 2. 数据库

### 3. 解决跨域问题

> 'http://localhost:5173' has been blocked by CORS policy: No 'Access-Control-Allow-Origin' 

<br>

$2a$10$rjo/qph54V/adj1EKLU8UemnDfasnrCc.piMkSDHt4LfQMKfeVm4q



