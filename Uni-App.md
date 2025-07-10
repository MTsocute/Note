# Uni-App

---

> [!important]
>
> 参考
>
> - [官方文档](https://uniapp.dcloud.net.cn/)
> - [路由控制](https://uniapp.dcloud.net.cn/collocation/pages.html)

<br>

## 1. 项目初始化

---

> 这里主要是如何通过 `npm` 初始化，我个人比较熟悉这样子的方案

```shell
npx degit dcloudio/uni-preset-vue#vite-ts mqtt_app
```

会创建一个 `mqtt_app` 的文件夹，然后我们进入之后，初始化这个项目

```shell
cd mqtt_app
npm install
```

完成上述操作之后，我们来运行一个 html5 的 demo 来验证是否成功了

```shell
# npm
npm run dev:h5
```

> [!important]
>
> 我们可执行的命令都在 `package.json` 的 `script` 部分

<br>

## 2. 打包发布

---

> 微信小程序的发布

```shell
npm run build:mp-weixin
npm run dev:mp-weixin
```

