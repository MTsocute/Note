# Flutter

---

- [中文官方文档](https://docs.flutter.cn/tools/android-studio/#editing-code-and-viewing-issues)

## 配置项目

> [SDK 官方下载地址](https://flutter.dev/multi-platform)

### gradle 换成国内地址

```properties
distributionBase=GRADLE_USER_HOME
distributionPath=wrapper/dists
zipStoreBase=GRADLE_USER_HOME
zipStorePath=wrapper/dists
# 就是这个
distributionUrl=https\://mirrors.cloud.tencent.com/gradle/gradle-8.12-all.zip
# 原本地址 https\://services.gradle.org/distributions/gradle-8.12-all.zip

```

### kts

> [!note]
>
> 我是选择 kotlin 作为构建的语言，所以你可能有些不同
>
> `android/build.gradle.kts` 找到这个文件，然后修改里面内容为如下

```kotlin
allprojects {
    repositories {
        google()
        mavenCentral()
        // 添加下面的部分
        maven { url = uri("https://maven.aliyun.com/repository/google") }
        maven { url = uri("https://maven.aliyun.com/repository/jcenter") }
        maven { url = uri("https://maven.aliyun.com/repository/public") }
        maven { url = uri("https://storage.flutter-io.cn/download.flutter.io") }
    }
}
```



## Flutter 大致概念

---

### Widget

> [!note]
>
> **一切都是 Widget**：按钮、文字、图片、布局容器，全都是 Widget。
>
> **Widget 有两类**：
>
> - `StatelessWidget`：纯展示，不变的。
> - `StatefulWidget`：能存状态，会更新的。

```dart
class Hello extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Text("Hello Flutter!");
  }
}

```

### 布局

> [!note]
>
> Flutter 布局用组合的方式，而不是写死坐标。常用的：
>
> - `Row`（水平排布）
> - `Column`（垂直排布）
> - `Stack`（堆叠）
> - `Container`（万能盒子，可以调 padding、颜色、边框等）
>
> 比如写个水平按钮栏：

```dart
Row(
  mainAxisAlignment: MainAxisAlignment.spaceAround,
  children: [
    ElevatedButton(onPressed: () {}, child: Text("A")),
    ElevatedButton(onPressed: () {}, child: Text("B")),
  ],
)

```

<br>

## Wdiget

---

### StatelessWidget

```dart
import 'package:flutter/material.dart';

// 一个无状态页面（StatelessWidget），展示 "Hello xxx"
class HelloPage extends StatelessWidget {
  // 定义一个 name 变量，用来接收外部传进来的名字
  final String name;

  // 构造函数，required 表示必须传入 name
  const HelloPage({super.key, required this.name});

  // build 方法：描述这个页面的 UI 长什么样
  @override
  Widget build(BuildContext context) {
    // Scaffold 是一个页面的基础结构，提供 AppBar、body 等
    return Scaffold(
      appBar: AppBar(
        title: const Text("Hello Page"), // 页面上方的标题栏
      ),
      body: Center(
        // Center：让子组件居中
        child: Text(
          "Hello $name", // 文本内容，显示传进来的名字
          style: const TextStyle(fontSize: 30), // 设置字体大小为 30
        ),
      ),
    );
  }
}
```

### StatefulWidget

> 先要创建一个外壳，用于存放需要响应的对象

```dart
class MyPage extends StatefulWidget {
  const MyPage({super.key});

  @override
  State<MyPage> createState() => _MyPageState();
}
```

> 真正的内容

```dart
class _MyPageState extends State<MyPage> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text("Stateful 示例")),
      body: const Center(
        child: Text("这是一个 Stateful 页面"),
      ),
    );
  }
}

```



