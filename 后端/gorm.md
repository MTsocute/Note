# Gorm-GEN

---

> 官网是这么说的，所以我才用
>
> - Idiomatic & Reusable API from Dynamic Raw SQL
> - 100% Type-safe DAO API without `interface{}`
> - Database To Struct follows GORM conventions
> - GORM under the hood, supports all features, plugins, DBMS that GORM supports

```bash
go get -u gorm.io/gen
```

> 使用的数据库结构

```sql
-- ======================================
-- IoT 平台数据库初始化 SQL
-- ======================================

-- 建数据库
CREATE DATABASE IF NOT EXISTS iot_platform CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

-- 切换数据库
USE iot_platform;

-- USERS 表：用户
CREATE TABLE `users` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `username` VARCHAR(100) NOT NULL UNIQUE,
  `password_hash` VARCHAR(255) NOT NULL,
  `email` VARCHAR(255),
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- PRODUCTS 表：产品（或产品模型，用来区分设备种类、功能等）
CREATE TABLE `products` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `product_key` VARCHAR(100) NOT NULL UNIQUE,
  `name` VARCHAR(255) NOT NULL,
  `description` TEXT,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- DEVICES 表：设备
CREATE TABLE `devices` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `device_key` VARCHAR(100) NOT NULL UNIQUE,
  `product_id` BIGINT UNSIGNED NOT NULL,  -- 外键指向 products
  `secret` VARCHAR(255) NOT NULL,         -- 用于认证或 mqtt 密码之类
  `status` VARCHAR(50),                  -- 比如 online/offline
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_devices_product` (`product_id`),
  CONSTRAINT `fk_device_product` FOREIGN KEY (`product_id`) REFERENCES `products`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- DEVICE_STATUS 表：设备状态快照/状态日志
CREATE TABLE `device_statuses` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `device_id` BIGINT UNSIGNED NOT NULL,
  `online` BOOLEAN NOT NULL DEFAULT FALSE,
  `last_seen` TIMESTAMP NULL,
  `properties` JSON,  -- 若有额外属性用 JSON 存
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_ds_device_id` (`device_id`),
  CONSTRAINT `fk_ds_device` FOREIGN KEY (`device_id`) REFERENCES `devices`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- DEVICE_DATA 或 MESSAGES 表：设备上报的数据
CREATE TABLE `device_messages` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `device_id` BIGINT UNSIGNED NOT NULL,
  `topic` VARCHAR(255),
  `payload` TEXT,  -- 若大数据可改 LONGTEXT
  `qos` TINYINT,
  `timestamp` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_dm_device_topic` (`device_id`,`topic`),
  CONSTRAINT `fk_dm_device` FOREIGN KEY (`device_id`) REFERENCES `devices`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;


-- 可选：后台管理员表或权限表等
CREATE TABLE `admins` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `username` VARCHAR(100) NOT NULL UNIQUE,
  `password_hash` VARCHAR(255) NOT NULL,
  `role` VARCHAR(50),
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
```

<br>

## 通过 GEN 生成 models

```go
package main

import (
	"fmt"
	"os"

	"github.com/joho/godotenv"
	"gorm.io/driver/mysql"
	"gorm.io/gen"
	"gorm.io/gorm"
)

var is_env_loaded = false

func init() {
	// 读取 .env 文件
	err := godotenv.Load(".env")
	if err != nil {
		fmt.Println("Error loading .env file")
		is_env_loaded = false
		return
	}
	is_env_loaded = true
}

func main() {
    // 配置文件参考这里：https://gorm.io/zh_CN/gen/dao.html
	g := gen.NewGenerator(gen.Config{
		OutPath:       "./query",                                 // gen代码的输出目录
		ModelPkgPath:  "models",                                  // 模型代码的输出目录
		Mode:          gen.WithDefaultQuery | gen.WithoutContext, // 启用默认查询和链式接口
		FieldNullable: true,                                      // 允许 NULL 的字段生成指针类型
	})

	if !is_env_loaded {
		fmt.Println("Error loading .env file")
		return
	}

	user := os.Getenv("DB_USER")
	pass := os.Getenv("DB_PWD")
	dia := mysql.Open(fmt.Sprintf("%s:%s@tcp(127.0.0.1:3306)/iot_platform?charset=utf8mb4&parseTime=True&loc=Local", user, pass))

	db, err := gorm.Open(dia, &gorm.Config{})
	if err != nil {
		fmt.Println(err)
		return
	}
	g.UseDB(db)
	g.GenerateAllTable() // 通过sql表生成gorm模型
    // g.GenerateModel("users") // 生成某一张表的 gorm 而不是全部，名字要和表名一样
	g.Execute()          // 执行
}

```

> 结果

```log
2025/09/22 21:15:49 find 6 table from db: [admins device_messages device_statuses devices products users]
2025/09/22 21:15:49 got 6 columns from table <admins>
2025/09/22 21:15:49 got 7 columns from table <device_messages>
2025/09/22 21:15:49 got 6 columns from table <device_statuses>
2025/09/22 21:15:49 got 7 columns from table <devices>
2025/09/22 21:15:49 got 6 columns from table <products>
2025/09/22 21:15:49 got 6 columns from table <users>
2025/09/22 21:15:49 Start generating code.
2025/09/22 21:15:49 generate model file(table <device_statuses> -> {models.DeviceStatus}): D:\GitHub\IOT_Backend\test\generate_models\models\device_statuses.gen.go
2025/09/22 21:15:49 generate model file(table <devices> -> {models.Device}): D:\GitHub\IOT_Backend\test\generate_models\models\devices.gen.go
2025/09/22 21:15:49 generate model file(table <users> -> {models.User}): D:\GitHub\IOT_Backend\test\generate_models\models\users.gen.go
2025/09/22 21:15:49 generate model file(table <products> -> {models.Product}): D:\GitHub\IOT_Backend\test\generate_models\models\products.gen.go
2025/09/22 21:15:49 generate model file(table <admins> -> {models.Admin}): D:\GitHub\IOT_Backend\test\generate_models\models\admins.gen.go
2025/09/22 21:15:49 generate model file(table <device_messages> -> {models.DeviceMessage}): D:\GitHub\IOT_Backend\test\generate_models\models\device_messages.gen.go
2025/09/22 21:15:49 Generate code done.
```

### 对比创建的 `gorm model` 和 `sql`

> sql table

```sql
-- 可选：后台管理员表或权限表等
CREATE TABLE `admins` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `username` VARCHAR(100) NOT NULL UNIQUE,
  `password_hash` VARCHAR(255) NOT NULL,
  `role` VARCHAR(50),
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
```

> go

```go
package models

import (
	"time"
)

const TableNameAdmin = "admins"

// Admin mapped from table <admins>
type Admin struct {
	ID           int64     `gorm:"column:id;primaryKey;autoIncrement:true" json:"id"`
	Username     string    `gorm:"column:username;not null" json:"username"`
	PasswordHash string    `gorm:"column:password_hash;not null" json:"password_hash"`
	Role         *string   `gorm:"column:role" json:"role"`
	CreatedAt    time.Time `gorm:"column:created_at;not null;default:CURRENT_TIMESTAMP" json:"created_at"`
	UpdatedAt    time.Time `gorm:"column:updated_at;not null;default:CURRENT_TIMESTAMP" json:"updated_at"`
}

// TableName Admin's table name
func (*Admin) TableName() string {
	return TableNameAdmin
}

```

### 生成更小的 API

> `interface` 用于只生成某个表的某些键的 API
>
> ```go
> // Dynamic SQL
> type Querier interface {
>   // SELECT * FROM @@table WHERE name = @name{{if role !=""}} AND role = @role{{end}}
>   FilterWithNameAndRole(name, role string) ([]gen.T, error)
> }
> ```

```go
func main() {
  g := gen.NewGenerator(gen.Config{
    OutPath: "../query",
    Mode: gen.WithoutContext|gen.WithDefaultQuery|gen.WithQueryInterface, // generate mode
  })

  // gormdb, _ := gorm.Open(mysql.Open("root:@(127.0.0.1:3306)/demo?charset=utf8mb4&parseTime=True&loc=Local"))
  g.UseDB(gormdb) // reuse your gorm db

  // Generate basic type-safe DAO API for struct `model.User` following conventions
  g.ApplyBasic(model.User{})

  // Generate Type Safe API with Dynamic SQL defined on Querier interface for `model.User` and `model.Company`
  g.ApplyInterface(func(Querier){}, model.User{}, model.Company{})

  // Generate the code
  g.Execute()
}
```



## 通过 model 生成 gen 对象

> 因为我们指定了 model，也就是 `ModelPkgPath:  "models"`，所以不会创建 `query` 文件，必须要自己手动创建。如果注释掉了，就可以自动创建了，我们主要是为了看到模型的结构，这点对于初步的学习还是有点重要的
>
> 创建过程如下

```go
package main

import (
	"IOT_Backend/test/generate_models/models"		// 对应的模型的包

	"gorm.io/gen"
)

func main() {
	// 1. 创建生成器
	g := gen.NewGenerator(gen.Config{
		OutPath:       "./query",                                 // 生成的查询代码目录
		Mode:          gen.WithDefaultQuery | gen.WithoutContext, // 启用默认查询和链式接口
		FieldNullable: true,                                      // 允许 NULL 的字段生成指针类型
	})

	// 2. 为指定模型生成查询代码
	g.ApplyBasic(
		// 在这里添加您的模型
		models.Admin{},
	)
    
    // 没写数据库，所以没有和数据库交互的能力 ...

	// 3. 执行代码生成
	g.Execute()
}

```

> 然后就会生成 query 包了，有了生成的gen代码，对于增删改查操作会非常简单
>
> ```bash
> # cmd
> D:\GitHub\IOT_Backend\test\generate_models > go run .\generate_gen_obj.go
> 
> # result
> 2025/09/22 21:48:15 Start generating code.
> 2025/09/22 21:48:15 generate query file: D:\GitHub\IOT_Backend\test\generate_models\query\admins.gen.go
> 2025/09/22 21:48:15 generate query file: D:\GitHub\IOT_Backend\test\generate_models\query\gen.go
> ```