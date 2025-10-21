# 微服务

---

- [go-zero 参考文档](https://go-zero.dev/docs/tutorials)
- 


## 微服务架构模式

​	`微服务架构模式（Microservices Architecture Pattern）` 作为单一应用程序和面向服务架构的可行替代方案，正在行业中迅速普及。由于这种架构模式仍在不断发展，**因此业界对这种模式的全部内容及其实现方式存在很多困惑**。报告的这一部分将为您提供了解这一重要架构模式的好处（和权衡）以及它是否适合您的应用程序所必需的关键概念和基础知识。

​	理解此模式的最重要概念可能是服务组件的概念。与其考虑微服务架构中的服务，不如考虑服务组件，服务组件的粒度可以从单个模块到应用程序的大部分。服务组件包含一个或多个模块（例如 Java 类），**这些模块代表单一用途功能**（例如，提供特定城市或城镇的天气）**或大型业务应用程序的独立部分**（例如，股票交易配售或确定汽车保险费率）。`设计正确级别的服务组件粒度是微服务架构中最大的挑战之一`。

<img src="https://go-zero.dev/assets/images/basic-microservices-arch-6b59f6654d4b700f8105f6163a22d924.png" alt="Basic Microservices architecture pattern" style="zoom:70%;" />

​	微服务（Microservices）是一种软件架构风格，它是以专注于单一责任与功能的小型功能区块 (Small Building Blocks) 为基础，利用模块化的方式组合出复杂的大型应用程序，各功能区块使用与 `语言无关 (Language-Independent/Language agnostic）的API集` 相互通信。

​	微服务的另一个对比是单体式应用程序。单体式应用表示一个应用程序内包含了所有需要的业务功能，并且使用像主从式架构（Client/Server）或是多层次架构（英语：Multitier architecture）（N-tier）实现，虽然它也是能以分布式应用程序来实现，但是在单体式应用内，每一个业务功能是不可分割的。

## 创建一个模块

```bash
# 创建一个 api 模板
goctl api new user

# 编辑好其中的 .api 文件之后生成对应的逻辑代码
goctl api go -api user.api -dir ./ -style go_zero
```

> 譬如说我们的 `user.api` 结构如果是这样子的

```protobuf
syntax = "v1"

type UserLoginRequest {
    Username string `json:"username"`
    Password string `json:"password"`
}

type Response {
    Token string `json:"token"`
}

service user-api {
	// 注意看这里
    @handler UserLoginHandler
    get /user/login (UserLoginRequest) returns (Response)
}

```

- 我标注的哪个注释的位置，会在 `/user/logic/` 生成如下名字的文件 `user_logic_logic.go`
- 这个就是我们 API 业务逻辑的主要处理的地方

```go
func (l *UserLoginLogic) UserLogin(req *types.UserLoginRequest) (resp *types.UserLoginResponse, err error) {
	// todo: add your logic here and delete this line
	username := req.Username
	token, err := middleware.GenerateToken(username)
	if err != nil {
		return nil, err
	}
	return &types.UserLoginResponse{
		Token: token,
	}, nil
}
```

```bash
# 把服务给跑起来
go run user.go -f etc/user-api.yaml
```

## 一个模块内的数据共享

> 通过 goctl 创建一个项目的时候，有一个 `svc` 文件夹，里面的内容大致是这样子的

```go
package svc

import ...

type ServiceContext struct {
	Config config.Config
}

func NewServiceContext(c config.Config) *ServiceContext {
	return &ServiceContext{
		Config: c,
	}
}
```

![image-20250923152403711](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250923152403711.png)

​	这一层的主要作用就是把一些这个模块之间公用的数据和工具做分享，让模块的每一个部分都可以用对应的功能，譬如我上图里面这个数据库的共享

​	如此一来，统一模块下面的不同 `Handler` 创建的 `Logic` 就可以做到共享了

## gRPC 注册

```protobuf
syntax = "proto3";

package template;

option go_package = "./template";

service UserService {
  rpc GetUser (GetUserRequest) returns (GetUserResponse);
}

message GetUserRequest {
  string id = 1;
}

message GetUserResponse {
  string id = 1;
  string name = 2;
}

```

> 生成对应的 go-zero 模块

```bash
goctl rpc protoc xxx.proto --go_out=./types --go-grpc_out=./types --zrpc_out=. --style go_zero
```

```bash
rpc
├─etc
├─internal
│  ├─config
│  ├─logic
│  ├─server
│  └─svc
├─types
│  └─user
└─userservice
```

