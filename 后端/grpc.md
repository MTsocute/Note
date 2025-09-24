# GRPC

---

<br>

## `.proto` 文件

```protobuf
// test.proto

syntax = "proto3"; // 指定proto版本
package hello_grpc;     // 指定默认包名

// 指定golang包名
option go_package = "/hello_grpc";

//定义rpc服务
service HelloService {
  // 定义函数
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

### [编写风格](https://docs.fengfengzhidao.com/#/docs/grpc文档/4.proto文件?id=编写风格)

1. 文件名建议下划线，例如：`my_student.proto`
2. 包名和目录名对应
3. 服务名、方法名、消息名均为大驼峰
4. 字段名为下划线

### 基本数据类型

```protobuf
message Request {
  double a1 = 1;
  float a2 = 2;
  int32 a3 = 3;
  uint32 a4 = 4;
  uint64 a5 = 5;
  sint32 a6 = 6;
  sint64 a7 = 7;
  fixed32 a8 = 8;
  fixed64 a9 = 9;
  sfixed32 a10 = 10;
  sfixed64 a11 = 11;
  bool a12 = 12;
  string a13 = 13;
  bytes a14 = 14;
}
```

### 基本类型对应的 go 类型

```go
type Request struct {
  state         protoimpl.MessageState
  sizeCache     protoimpl.SizeCache
  unknownFields protoimpl.UnknownFields

  A1  float64 `protobuf:"fixed64,1,opt,name=a1,proto3" json:"a1,omitempty"`
  A2  float32 `protobuf:"fixed32,2,opt,name=a2,proto3" json:"a2,omitempty"`
  A3  int32   `protobuf:"varint,3,opt,name=a3,proto3" json:"a3,omitempty"`
  A4  uint32  `protobuf:"varint,4,opt,name=a4,proto3" json:"a4,omitempty"`
  A5  uint64  `protobuf:"varint,5,opt,name=a5,proto3" json:"a5,omitempty"`
  A6  int32   `protobuf:"zigzag32,6,opt,name=a6,proto3" json:"a6,omitempty"`
  A7  int64   `protobuf:"zigzag64,7,opt,name=a7,proto3" json:"a7,omitempty"`
  A8  uint32  `protobuf:"fixed32,8,opt,name=a8,proto3" json:"a8,omitempty"`
  A9  uint64  `protobuf:"fixed64,9,opt,name=a9,proto3" json:"a9,omitempty"`
  A10 int32   `protobuf:"fixed32,10,opt,name=a10,proto3" json:"a10,omitempty"`
  A11 int64   `protobuf:"fixed64,11,opt,name=a11,proto3" json:"a11,omitempty"`
  A12 bool    `protobuf:"varint,12,opt,name=a12,proto3" json:"a12,omitempty"`
  A13 string  `protobuf:"bytes,13,opt,name=a13,proto3" json:"a13,omitempty"`
  A14 []byte  `protobuf:"bytes,14,opt,name=a14,proto3" json:"a14,omitempty"`
}
```

### 标量类型

| .proto Type | 解释                                                         | Go Type |
| ----------- | ------------------------------------------------------------ | ------- |
| double      |                                                              | float64 |
| float       |                                                              | float32 |
| int32       | 使用变长编码，对于负值的效率很低，如果你的域有可能有负值，请使用sint64替代 | int32   |
| uint32      | 使用变长编码                                                 | uint32  |
| uint64      | 使用变长编码                                                 | uint64  |
| sint32      | 使用变长编码，这些编码在负值时比int32高效的多                | int32   |
| sint64      | 使用变长编码，有符号的整型值。编码时比通常的int64高效        | int64   |
| fixed32     | 总是4个字节，如果数值总是比总是比228大的话，这个类型会比uint32高效。 | uint32  |
| fixed64     | 总是8个字节，如果数值总是比总是比256大的话，这个类型会比uint64高效。 | uint64  |
| sfixed32    | 总是4个字节                                                  | int32   |
| sfixed64    | 总是8个字节                                                  | int64   |
| bool        |                                                              | bool    |
| string      | 一个字符串必须是UTF-8编码或者7-bit ASCII编码的文本           | string  |
| bytes       | 可能包含任意顺序的字节数据                                   | []byte  |

### [数组类型](https://docs.fengfengzhidao.com/#/docs/grpc文档/4.proto文件?id=数组类型)

```protobuf
message ArrayRequest {
  repeated int64 a1 = 1;
  repeated string a2 = 2;
  repeated Request request_list = 3;
}
```

```go
type ArrayRequest struct {
  A1          []int64 
  A2          []string   
  RequestList []*Request
}
```

### [map 类型](https://docs.fengfengzhidao.com/#/docs/grpc文档/4.proto文件?id=map类型)

> 键只能是基本类型

```protobuf
message MapRequest {
  map<int64, string> m_i_s = 1;
  map<string, bool> m_i_b = 2;
  map<string, ArrayRequest> m_i_arr = 3;
}

```

```go
type MapRequest struct {

  MIS   map[int64]string
  MIB   map[string]bool
  MIArr map[string]*ArrayRequest
}

```

### 嵌套类型

```protobuf
message Q1 {
  message Q2{
    string name2 = 2;
  }
  string name1 = 1;
  Q2 q2 = 2;
}
```

```go
// Q1 是你的外部消息
type Q1 struct {
	// 自动生成的字段，对应 proto 中的 name1
	Name1 string `protobuf:"bytes,1,opt,name=name1,proto3" json:"name1,omitempty"`
	// Q2 是嵌套消息，这里它是一个指针类型
	Q2 *Q1_Q2 `protobuf:"bytes,2,opt,name=q2,proto3" json:"q2,omitempty"`
	// protoc 内部用于处理未知字段的字段
	// 它的存在是为了保证将来 protobuf 版本兼容性
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields
}

// Q1_Q2 是嵌套消息 Q2
type Q1_Q2 struct {
	// 自动生成的字段，对应 proto 中的 name2
	Name2 string `protobuf:"bytes,1,opt,name=name2,proto3" json:"name2,omitempty"`
	// protoc 内部用于处理未知字段的字段
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields
}
```



## 生成对应的 go 文件

```bash
protoc --go_out=. --go_opt=paths=source_relative ^
       --go-grpc_out=. --go-grpc_opt=paths=source_relative ^
       test.proto
```

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

