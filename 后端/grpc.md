# GRPC

---

<br>

## `.proto` 文件

```protobuf
// test.proto

// 语法版本
syntax = "proto3";

// 包定义，但是对 go 无效
package hello_grpc;     

// 指定golang module 名
option go_package = "/hello_grpc";

//定义rpc服务
service HelloService {
  // 服务 API 名字
  rpc SayHello (HelloRequest) returns (HelloResponse) {}
}

// HelloRequest 请求内容
message HelloRequest {
  string name = 1;
  string message = 2;
}

// HelloResponse 响应内容
message HelloResponse{
  string name = 1;
  string message = 2;
}
```

<br>

## 生成对应的 go 文件

```bash
protoc --go_out=. --go_opt=paths=source_relative ^
       --go-grpc_out=. --go-grpc_opt=paths=source_relative ^
       test.proto
```

>  使用名之后，就会生成两个对应的文件夹
>
> `greeter.pb.go`：包含 Go 语言的消息结构体（`HelloRequest`, `HelloReply`）以及它们的序列化/反序列化代码。
>
> `greeter_grpc.pb.go`：包含 Go 语言的 `gRPC 客户端`和`服务端接口`代码。

### 服务端接口代码

```go
package main

import (
	"context"
	"fmt"
	pb "grpc_study/grpc_test/test"
	"net"

	"google.golang.org/grpc"
	"google.golang.org/grpc/grpclog"
)

type HelloServer1 struct {
	//! 嵌入这个结构体可以保证向后兼容性，避免 future-proof errors
	pb.UnimplementedHelloServiceServer
}

func (HelloServer1) SayHello(ctx context.Context, request *pb.HelloRequest) (pd *pb.HelloResponse, err error) {
	fmt.Printf("Param: {%v} & {%v}\n", request.Name, request.Message)

	// 更简洁的写法，直接返回一个指针
	return &pb.HelloResponse{
		Name:    "你好",
		Message: "ok",
	}, nil // 注意：如果方法成功执行，必须返回 nil 作为错误
}

func main() {
	// 监听端口
	listen, err := net.Listen("tcp", ":8080")
	if err != nil {
		grpclog.Fatalf("Failed to listen: %v", err)
	}

	// 创建一个gRPC服务器实例。
	s := grpc.NewServer()
	server := HelloServer1{}
	// 将server结构体注册为gRPC服务。
	pb.RegisterHelloServiceServer(s, &server)
	fmt.Println("grpc server running<Demo2> :8080")
	// 开始处理客户端请求。
	err = s.Serve(listen)
}
```

### 客户端接口代码

```go
package main

import (
	"context"
	"log"
	"time"

	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"

	pb "grpc_study/grpc_test/test"
)

const (
	address = "localhost:8080"
)

func main() {
	// 创建一个与gRPC服务器的连接。NewClient是非阻塞的。
	// 连接过程将在第一次调用RPC时懒惰地进行。
	conn, err := grpc.NewClient(address,
		grpc.WithTransportCredentials(insecure.NewCredentials()))
	if err != nil {
		// NewClient在配置错误时返回错误，而不是连接错误
		log.Fatalf("配置客户端失败: %v", err)
	}
	defer conn.Close()

	//! 创建客户端存根
	c := pb.NewHelloServiceClient(conn)

	// 设置带有超时的上下文，这很重要，因为它为RPC调用本身设置了超时
	ctx, cancel := context.WithTimeout(context.Background(), time.Second)
	defer cancel()

	// 构造请求消息
	req := &pb.HelloRequest{
		Name:    "Alice",
		Message: "你好，gRPC！",
	}

	//! 调用RPC方法。此时，连接实际建立。
	res, err := c.SayHello(ctx, req)
	if err != nil {
		// 如果连接失败，错误也会在这里返回
		log.Fatalf("调用SayHello失败: %v", err)
	}

	// 处理服务器的响应
	log.Printf("收到来自服务器的问候: Name=%s, Message=%s",
		res.GetName(), res.GetMessage())
}
```

