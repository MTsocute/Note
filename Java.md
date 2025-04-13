# Java

---



## 1. 类

### 1. `this` 指针

> 特殊用法用**别的构造函数**来**构造函数**

```java
public class cat {
    public String name;
    public int age;
    public boolean gender;

    public cat(String name, int age, boolean gender) {
        this.name = name;
        this.age = age;
        this.gender = gender;
    }

    // 构造函数
    public cat(String name) {
        // 构造别的构造函数
        this(name, 10, true);	// 如果还有别的变量, 这个优先级也得是最高的
    }
}
```

> 用对象的函数方法调用函数类函数本身

```java
public class cat {
    // 参数是 this
	public void run(cat this) {
        System.out.printlin("Running!");
    }
}

// 那么我可以使用 run 调用自己来实现

/* 正常使用 */
cat cat1 = new cat();
cat1.run();

/* 类指针使用 */
run(cat1);
```

### 2. 封装

> 内部数据不被外部随意访问, 提供的 API 只可以保留自己的想法和限制条件

![image-20250225181622458](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250225181622458.png)

### 3. 继承

> 核心思想, 就是共有部分的减少重写
>
> 还得记得**重写函数方法**

![image-20250225181839707](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250225181839707.png)

```java
@Override
public void sayHello() {		// overwrite the method
    System.out.println("Ciallo!");
}
```

### 4. 多态

> 继承本来就是指代说变量和方法的继承, 但是多态的话, 就是指被继承的抽象基类代表一个类别

```java
class Animal {
  void makeSound() {
    System.out.println("The animal makes a sound");
  }
}

class Dog extends Animal {
    @Override
  void makeSound() {
    System.out.println("The dog barks");
  }
}

class Cat extends Animal {
    @Override
  void makeSound() {
    System.out.println("The cat meows");
  }
}

public class Main {
  public static void main(String[] args) {
    Animal myAnimal = new Animal(); // 创建 Animal 对象
    Animal myDog = new Dog(); // 创建 Dog 对象
    Animal myCat = new Cat(); // 创建 Cat 对象

    myAnimal.makeSound(); // 输出: The animal makes a sound
    myDog.makeSound(); // 输出: The dog barks
    myCat.makeSound(); // 输出: The cat meows
  }
}
```

<br>

## 2. 异常

> 本质就是如果出现了代码中的错误就不要让程序中断, 而是转到其他地方处理, 或者选择性跳过, 不要让程序崩溃
>
> `if ... else...` 是枚举不完错误的, 所以才有 `try... catch.. finally... throw.. throws` 的存在的意义

### 1. 异常的转交处理方法

```java
// 这个 throw Exception 会把错误提交给调用这个函数的 try catch 语句
public static void two_num(int num1, int num2) throws Exception {
    // 主动提出错误
    if (num1 < num2) {
        throw new Exception();
    }
}
```

## 3. Java 的 STL

---

### 1. `ArrayList`

> [API 调用接口](https://www.w3schools.com/java/java_arraylist.asp)

## 4. IO 流

> 体系结构, 虽然看起来复杂, 但是实际上只需要知道一组输入输出流就可以掌握其他的了

![image-20250225195653608](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250225195653608.png)

### 1. 逐字读取

```java
import java.io.File;
import java.io.FileReader;

public class Main {
    public static void main(String[] args) {
        // 创建 File 对象以表示 PC 上的真实文件
        File data_txt = new File("FILEPATH");
    
        try {
            // 使用 FileReader 读取数据
            FileReader fr = new FileReader(data_txt);
        
            // 逐字节读取并输出
            int c;
            while ((c = fr.read()) != -1) { // 当读取到文件末尾时，read() 返回 -1
                System.out.print((char) c); // 将读取的字节转换为字符并输出
            }
        
            fr.close();
        } catch (IOException  e) {
            e.printStackTrace();
        }
    }
}
```

### 2. 逐行读取

```java
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.File;

public class Main {
    public static void main(String[] args) {
        // 创建 File 对象以表示 PC 上的真实文件
        File data_txt = new File("FILEPATH");
    
        try {
            // 使用 BufferedReader 读取数据
            FileReader fr = new FileReader(data_txt);
            BufferedReader br = new BufferedReader(fr);
        
            /* 逐行读取并输出 */ 
            String line;
            // 当读取到文件末尾时，readLine() 返回 null
            while ((line = br.readLine()) != null) { 
                System.out.println(line); // 输出整行
            }
        
            br.close(); // 关闭 BufferedReader
            fr.close(); // 关闭 FileReader
        } catch (IOException  e) {
            e.printStackTrace();
        }
    }
}
```

### 3.写入数据

> `PrintWriter` 是一个方便的类，用于向文件或其他输出流写入各种数据类型，如字符串、整数、浮点数等。它通常与 `FileWriter` 或 `BufferedWriter` 一起使用。
>
> `PrintWriter` **默认不会抛出 I/O 异常，而是将错误记录下来**。如果需要捕获异常，可以通过 `checkError()` 方法检查错误。

```java
import java.io.PrintWriter;
import java.io.FileWriter;
import java.io.IOException;

public class PrintWriterExample {
    public static void main(String[] args) {
        // 指定文件路径
        String filePath = "FILEPATH";

        try {
            // 创建 FileWriter 和 PrintWriter 对象
            FileWriter writer = new FileWriter(filePath, true);
            PrintWriter printWriter = new PrintWriter(writer); // 包装 FileWriter

            // 写入内容
            printWriter.println("Hello, World!");
            printWriter.println("This is a test.");

            // 关闭流
            printWriter.close();
            writer.close();
            System.out.println("文件写入成功。");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```

### 4. 写入二进制数据

```java
import java.io.DataOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;

public class BinaryFileWriter {
    public static void main(String[] args) {
        // 文件名
        String fileName = "example.bin";

        // 要写入的二进制数据
        byte[] data = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        // 使用 FileOutputStream 创建文件并写入数据
        try (FileOutputStream fos = new FileOutputStream(fileName)) {
            fos.write(data); // 写入字节数组
            System.out.println("Data has been written to " + fileName);
        } catch (IOException e) {
            e.printStackTrace();
        }

        // 使用 DataOutputStream 写入更多结构化的数据
        try (DataOutputStream dos = new DataOutputStream(new FileOutputStream("example2.bin"))) {
            dos.writeInt(12345);  // 写入一个整数
            dos.writeDouble(3.14159);  // 写入一个浮动点数
            dos.writeUTF("Hello, Binary!");  // 写入一个字符串
            System.out.println("Structured data has been written to example2.bin");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```

## 5. 线程

---

![image-20250225204156832](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250225204156832.png)

```java

// 创建一个线程类
// 继承 Thread 类, 具有多线程的能力
public class TestThread extends Thread {
    @Override
    public void run() {
        for (int i = 0; i < 10; i++) {
            System.out.println(i);
        }
    }
}

// 启动方式
TestThread t = new TestThread();
t.start();
```

## 6. Maven

> 相当于是一个项目管理工具, 就不需要手动的都如 jar 包了

### 1. SQL

> 数据库连接格式: 
>
> ```java
> String url="jdbc:mysql://127.0.0.1:3306/databaseName?useSSL=false&useUnicode=true&characterEncoding=UTF-8&serverTimezone=Asia/Shanghai&allowPublicKeyRetrieval=true";
> ```

![image-20250226140616712](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250226140616712.png)

> 基本步骤

```java
import java.sql.*;

public class JDBC_Learing01 {
    public static void main(String[] args) throws ClassNotFoundException, SQLException {
        try {
            // 1. loading Driver
            Class.forName("com.mysql.jdbc.Driver");

            // 2. Get URL link
            String url = "jdbc:mysql://localhost:3306/ts_demo";
            String user = "root";
            String password = "wenshuheng";
            Connection conn = DriverManager.getConnection(url, user, password);

            // 3. Create Session
            Statement stmt = conn.createStatement();

            // 4. Send SQL
            stmt.execute("select * from employee");
            
            // 5. Print the Data
            ResultSet rs = stmt.getResultSet();
            System.out.println(rs);
            
            // 6. Close the source
            System.out.println("Success");
            stmt.close();
            conn.close();
        } catch (Exception e) {
            System.out.println("Failed to execute");
            e.printStackTrace();
        }
    }
}
```

<br>

# MyBatis

---

![image-20250226151258310](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250226151258310.png)



## 1. `MyBatis` 如何配置

> **ORM(Object/Relation Mapping)**，中文名称：对象/关系 映射。是一种解决**数据库发展和面向对象编程语言发展不匹配**问题而出现的技术。

![image-20250226152237377](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250226152237377.png)

> 这三部就是我们大部分时间做的事情

![image-20250226184109156](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250226184109156.png)

### 1. 配置全局文件

> 注意 `DOCTYPE` 是 Configuration
>
> 还有就是我们的 `environments` 的 `default` 必须要是 `environment id` 其中的一个

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
        "https://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <!-- default should at least be one of the id of environment -->
    <environments default="mysql">
        <!-- mysql -->
        <environment id="mysql">
            <transactionManager type="JDBC"/>
            <dataSource type="POOLED">
                <property name="driver" value="com.mysql.jdbc.Driver"/>
                <property name="url" value="jdbc:mysql://localhost:3306/ts_demo"/>
                <property name="username" value="root"/>
                <property name="password" value="wenshuheng"/>
            </dataSource>
        </environment>

    </environments>
    <mappers>
        <mapper resource="mapper/deprImport.xml"/>
    </mappers>
</configuration>
```

### 2. 配置映射文件

> 这个最好在全局资源文件夹 `source` 下建一个 `mapper` 这样子全局文件好找映射文件
>
> 注意 `DOCTYPE` 是 mapper

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
        "https://mybatis.org/dtd/mybatis-3-mapper.dtd">

<mapper namespace="mysql_area">
    <select id="selectAllDepartment" resultType="com.momo.entity.department">
        select * from department
    </select>
</mapper>
```

### 3. 实体文件

> 这个部分是用于承载接收到的数据, 里面的数据类型最好是跟着你的数据库里面而定
>
> **实体的变量名必须和数据库的键名一致**

![image-20250226184441188](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250226184441188.png)

```java
package com.momo.entity;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class department {
    private int id;
    private String name;
}
```

### 4. 交互数据的中间层的构建

```java
package com.momo.test;

import com.momo.entity.department;
import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;

import java.io.IOException;
import java.io.InputStream;
import java.util.List;


public class Test {
    public static void main(String[] args) throws IOException {
        String resource = "mybatis-config.xml";
        InputStream inputStream = Resources.getResourceAsStream(resource);
        SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
        SqlSession sqlSession = sqlSessionFactory.openSession();
        List<department> list = sqlSession.selectList("mysql_area.selectAllDepartment");

        for (department object : list) {
            System.out.println(object);
        }

        sqlSession.close();
    }
}
```

<br>

## 2. `Mybatis` 配置简化

> 在全局的 xml 文件简化这个 java 路径, 然后再需要的文件中直接用别名

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
        "https://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <!-- It should be init as the beginning of the config -->
    <typeAliases>
        <typeAlias type="com.momo.entity.department" alias="department" />
    </typeAliases>

	<!-- ... -->
</configuration>
```

> 解耦变量到外部文件

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
        "https://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
     <!-- It should be init as the beginning of the project -->
    <properties resource="db.properties"/>

</configuration>
```

![image-20250226194620386](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250226194620386.png)

## 3. `Mybatis` 导出日志

> 使用外部依赖 `Log4j`
>
> ![image-20250226195010331](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250226195010331.png)
>
> 打印级别越低, 输出的内容就越多

![image-20250226195333537](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250226195333537.png)

```properties
# Set the root logger level to DEBUG and its appender to console
log4j.rootLogger= trace, console

# Console appender configuration - output logs to console
log4j.appender.console=org.apache.log4j.ConsoleAppender
log4j.appender.console.Target =System.out
log4j.appender.console.layout=org.apache.log4j.PatternLayout
log4j.appender.console.layout.ConversionPattern=%d{yyyy-MM-dd HH:mm:ss} [%t] %-5p %c{1}:%L - %m%n
```

> 当然也可以改变一下打印方式, 让我们获取那些有用的信息
>
> 比如我们只关心这个部分的数据获取情况和 Error, 那么我们就可以针对性的获取数据

![image-20250226195803919](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250226195803919.png)

```properties
# Set the root logger level to DEBUG and its appender to console
log4j.rootLogger=error, console

log4j.logger.mysql_area=TRACE

# Console appender configuration - output logs to console
log4j.appender.console=org.apache.log4j.ConsoleAppender
log4j.appender.console.Target =System.out
log4j.appender.console.layout=org.apache.log4j.PatternLayout
log4j.appender.console.layout.ConversionPattern=%d{yyyy-MM-dd HH:mm:ss} [%t] %-5p %c{1}:%L - %m%n

```

## 4. `Mybatis` 接口
