# Redis

---

<br>

##  Docker 配置 Redis

> [!note]
>
> 这里需要一些 Docker 的前置知识，但是如果不知道也没关系，照下面的步骤走，也是可以运行的，我们主要是跑一个 redis 服务，主要讲的是 redis 其他不知道也没关系。

<br>

### Redis 的 Docker 挂载

---

```yaml
services:
  redis:
    image: redis
    container_name: redis_server
    ports:
      - "6379:6379"  # 暴露端口：宿主机6379 -> 容器6379
    volumes:
      - redis-data:/data  # 用 volume 挂载数据目录
    restart: unless-stopped  # 自动重启策略
    # 如果你有配置文件：
    # volumes:
    #   - ./redis.conf:/usr/local/etc/redis/redis.conf
    # command: ["redis-server", "/usr/local/etc/redis/redis.conf"]

volumes:
  redis-data:
    name: redis_data_volume  # 你可以为 volume 指定名字

```

> 运行 redis 服务

```bash
docker-compose up -d
```

<br>

### Redis Docker 交互

---

```bash
docker exec -it redis_container_name redis-cli
```

<br>

###  redis.conf

---

> [!important]
>
> 常用的一些配置

```md
################################## NETWORK #####################################
# Redis 监听端口，默认 6379
port 6379

# 绑定地址：默认仅本地。Docker / 公网需改为 0.0.0.0，但生产环境务必配合防火墙
# bind 127.0.0.1 -::1
bind 0.0.0.0

# 保护模式：当没有密码且监听公网时拒绝远程写操作；建议始终保留 yes
protected-mode yes

# 监听 TCP-backlog 长度
tcp-backlog 511

# 客户端空闲 N 秒后关闭；0 表示永不超时
timeout 0

# TCP keepalive：检测死连接，秒
tcp-keepalive 300

################################# SECURITY #####################################
# 访问密码（AUTH），强烈建议设置复杂随机串
requirepass momo123

# ACL 文件，细粒度用户权限；比 requirepass 更灵活
# aclfile /etc/redis/users.acl

# 重命名或禁用高危命令
# rename-command FLUSHDB ""
# rename-command CONFIG CONFIG_b9f2e3

################################## GENERAL #####################################
# 以守护进程方式运行：容器里必须为 no
daemonize no

# 托管模式：systemd/docker 可设为 "systemd"
supervised no

# PID 文件路径
pidfile /var/run/redis_6379.pid

# 日志级别：debug / verbose / notice / warning
loglevel notice

# 日志文件："" 表示输出到标准输出（Docker 推荐）
logfile ""

# 默认 16 个逻辑库
databases 16

# 启动时显示 ASCII Logo（可关闭）
always-show-logo yes

################################ SNAPSHOTTING ##################################
# RDB 保存策略：秒 修改键数
save 900 1
save 300 10
save 60 10000

# 持久化出错后是否停止写操作
stop-writes-on-bgsave-error yes

# RDB 压缩、校验
rdbcompression yes
rdbchecksum yes

# RDB 文件名
dbfilename dump.rdb

# 工作目录，RDB/AOF 都放这里；Docker 卷挂载常用 /data
dir /data

################################# APPEND ONLY ##################################
# AOF 持久化，默认关闭；生产建议 yes
appendonly yes
appendfilename "appendonly.aof"

# AOF 刷盘策略：always / everysec / no
appendfsync everysec

# 重写 AOF 时是否继续 fsync
no-appendfsync-on-rewrite no

# 自动触发 AOF 重写的增长率/最小尺寸
auto-aof-rewrite-percentage 100
auto-aof-rewrite-min-size 64mb

# 启动时加载 AOF 出错是否继续
aof-load-truncated yes

# 使用 RDB + AOF 混合格式（Redis 7 默认开启）
aof-use-rdb-preamble yes

################################## LIMITS ######################################
# 最大客户端连接数
maxclients 10000

# 最大内存；0 表示不限制。生产常用 2gb / 4gb …
maxmemory 0

# 内存淘汰策略：noeviction / allkeys-lru / volatile-lru / allkeys-lfu / ...
maxmemory-policy noeviction

# 内存使用采样数量（LRU/LFU）
maxmemory-samples 5

################################## SLOW LOG ####################################
# 超过多少微秒算慢查询
slowlog-log-slower-than 10000

# 慢查询队列长度
slowlog-max-len 128

################################ LATENCY MONITOR ###############################
# 延迟监控阈值，0 表示禁用
latency-monitor-threshold 0

################################ EVENT NOTIFICATION ############################
# Keyspace 事件通知；常用 Ex / Kx / AKE 等
notify-keyspace-events ""

################################## CLIENT ######################################
# 客户端输出缓冲区限制：normal / replica / pubsub
client-output-buffer-limit normal 0 0 0
client-output-buffer-limit replica 256mb 64mb 60
client-output-buffer-limit pubsub 32mb 8mb 60

############################### REPLICATION ###################################
# 主从复制：本机作为从库时配置
# replicaof <masterip> <masterport>

# 从库只读
replica-read-only yes

# 主库连接密码
# masterauth <master-password>

################################## CLUSTER #####################################
# 启用集群模式
# cluster-enabled yes
# cluster-config-file nodes-6379.conf
# cluster-node-timeout 15000
# cluster-require-full-coverage yes

################################# TLS/SSL ######################################
# 生产如需加密可启用 TLS
# port 0
# tls-port 6379
# tls-cert-file /tls/redis.crt
# tls-key-file /tls/redis.key
# tls-ca-cert-file /tls/ca.crt

################################## INCLUDES ####################################
# 引入额外配置，方便不同环境覆盖
# include /etc/redis/local.conf
```

<br>

##  Redis 通用命令

---

![image-20250803184830302](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250803184830302.png)



##  常用数据类型的操作

---

### String 操作

![image-20250803191227194](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250803191227194.png)

<br>

### Hash 类型的操作

---

![image-20250803192355260](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250803192355260.png)

> 数据结构

![image-20250803192558541](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250803192558541.png)



### List 的操作

---

> 双向链表
>
> - LRANGE 的下标从 0 开始

![image-20250804113531392](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250804113531392.png)

<br>

### Set 的操作

---

![image-20250804134650229](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250804134650229.png)

### SortedSet 的操作

---

![image-20250804135505850](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250804135505850.png)

<br>

## Redis 常用操作命令表

---

| 分类            | 命令                                       | 说明               | 示例                                   |
| --------------- | ------------------------------------------ | ------------------ | -------------------------------------- |
| **连接操作**    | `PING`                                     | 测试连接           | `PING`                                 |
|                 | `AUTH password`                            | 密码认证           | `AUTH mypassword`                      |
|                 | `SELECT index`                             | 选择数据库         | `SELECT 1`                             |
|                 | `QUIT`                                     | 关闭连接           | `QUIT`                                 |
| **键操作**      | `KEYS pattern`                             | 查找键             | `KEYS user:*`                          |
|                 | `EXISTS key`                               | 检查键是否存在     | `EXISTS mykey`                         |
|                 | `TYPE key`                                 | 查看键类型         | `TYPE mykey`                           |
|                 | `TTL key`                                  | 查看过期时间(秒)   | `TTL mykey`                            |
|                 | `PTTL key`                                 | 查看过期时间(毫秒) | `PTTL mykey`                           |
|                 | `EXPIRE key seconds`                       | 设置过期时间       | `EXPIRE mykey 300`                     |
|                 | `PERSIST key`                              | 移除过期时间       | `PERSIST mykey`                        |
|                 | `DEL key [key ...]`                        | 删除键             | `DEL key1 key2`                        |
|                 | `RENAME key newkey`                        | 重命名键           | `RENAME oldkey newkey`                 |
| **String 操作** | `SET key value`                            | 设置值             | `SET name "Redis"`                     |
|                 | `GET key`                                  | 获取值             | `GET name`                             |
|                 | `MSET key value [key value ...]`           | 批量设置           | `MSET k1 v1 k2 v2`                     |
|                 | `MGET key [key ...]`                       | 批量获取           | `MGET k1 k2`                           |
|                 | `INCR key`                                 | 原子递增           | `INCR counter`                         |
|                 | `INCRBY key increment`                     | 指定增量           | `INCRBY counter 5`                     |
|                 | `DECR key`                                 | 原子递减           | `DECR counter`                         |
|                 | `APPEND key value`                         | 追加字符串         | `APPEND name "DB"`                     |
|                 | `STRLEN key`                               | 获取长度           | `STRLEN name`                          |
| **Hash 操作**   | `HSET key field value`                     | 设置字段           | `HSET user:1 name Tom`                 |
|                 | `HGET key field`                           | 获取字段值         | `HGET user:1 name`                     |
|                 | `HMSET key field value [field value ...]`  | 批量设置字段       | `HMSET user:1 name Tom age 25`         |
|                 | `HMGET key field [field ...]`              | 批量获取字段       | `HMGET user:1 name age`                |
|                 | `HGETALL key`                              | 获取所有字段       | `HGETALL user:1`                       |
|                 | `HDEL key field [field ...]`               | 删除字段           | `HDEL user:1 age`                      |
|                 | `HEXISTS key field`                        | 检查字段是否存在   | `HEXISTS user:1 name`                  |
|                 | `HKEYS key`                                | 获取所有字段名     | `HKEYS user:1`                         |
|                 | `HVALS key`                                | 获取所有字段值     | `HVALS user:1`                         |
|                 | `HLEN key`                                 | 获取字段数量       | `HLEN user:1`                          |
| **List 操作**   | `LPUSH key element [element ...]`          | 左侧插入           | `LPUSH mylist a b c`                   |
|                 | `RPUSH key element [element ...]`          | 右侧插入           | `RPUSH mylist x y z`                   |
|                 | `LPOP key`                                 | 左侧弹出           | `LPOP mylist`                          |
|                 | `RPOP key`                                 | 右侧弹出           | `RPOP mylist`                          |
|                 | `LRANGE key start stop`                    | 获取范围元素       | `LRANGE mylist 0 -1`                   |
|                 | `LLEN key`                                 | 获取列表长度       | `LLEN mylist`                          |
|                 | `LINDEX key index`                         | 获取指定位置元素   | `LINDEX mylist 0`                      |
|                 | `LSET key index element`                   | 设置指定位置元素   | `LSET mylist 0 newvalue`               |
|                 | `LTRIM key start stop`                     | 保留指定范围       | `LTRIM mylist 0 2`                     |
| **Set 操作**    | `SADD key member [member ...]`             | 添加成员           | `SADD myset a b c`                     |
|                 | `SMEMBERS key`                             | 获取所有成员       | `SMEMBERS myset`                       |
|                 | `SISMEMBER key member`                     | 检查成员是否存在   | `SISMEMBER myset a`                    |
|                 | `SCARD key`                                | 获取成员数量       | `SCARD myset`                          |
|                 | `SREM key member [member ...]`             | 删除成员           | `SREM myset a`                         |
|                 | `SPOP key [count]`                         | 随机弹出成员       | `SPOP myset 2`                         |
|                 | `SRANDMEMBER key [count]`                  | 随机获取成员       | `SRANDMEMBER myset 2`                  |
|                 | `SUNION key [key ...]`                     | 集合并集           | `SUNION set1 set2`                     |
|                 | `SINTER key [key ...]`                     | 集合交集           | `SINTER set1 set2`                     |
|                 | `SDIFF key [key ...]`                      | 集合差集           | `SDIFF set1 set2`                      |
| **ZSet 操作**   | `ZADD key score member [score member ...]` | 添加成员           | `ZADD leaderboard 100 user1`           |
|                 | `ZRANGE key start stop [WITHSCORES]`       | 按分数升序获取     | `ZRANGE leaderboard 0 -1 WITHSCORES`   |
|                 | `ZREVRANGE key start stop [WITHSCORES]`    | 按分数降序获取     | `ZREVRANGE leaderboard 0 2 WITHSCORES` |
|                 | `ZSCORE key member`                        | 获取成员分数       | `ZSCORE leaderboard user1`             |
|                 | `ZRANK key member`                         | 获取成员排名(升序) | `ZRANK leaderboard user1`              |
|                 | `ZREVRANK key member`                      | 获取成员排名(降序) | `ZREVRANK leaderboard user1`           |
|                 | `ZCARD key`                                | 获取成员数量       | `ZCARD leaderboard`                    |
|                 | `ZREM key member [member ...]`             | 删除成员           | `ZREM leaderboard user1`               |
|                 | `ZINCRBY key increment member`             | 增加成员分数       | `ZINCRBY leaderboard 10 user1`         |
| **数据库操作**  | `FLUSHDB`                                  | 清空当前数据库     | `FLUSHDB`                              |
|                 | `FLUSHALL`                                 | 清空所有数据库     | `FLUSHALL`                             |
|                 | `DBSIZE`                                   | 获取键数量         | `DBSIZE`                               |
|                 | `INFO`                                     | 获取服务器信息     | `INFO`                                 |
|                 | `CONFIG GET parameter`                     | 获取配置           | `CONFIG GET maxmemory`                 |
|                 | `CONFIG SET parameter value`               | 设置配置           | `CONFIG SET maxmemory 2gb`             |



##  Redis 的应用方向

---

![image-20250804135902157](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250804135902157.png)

<br>

# Redis 短信登录实现

---

## 1. Redis 的关键技术点

> Session 其实并不一定要用，可以用 token 来替代

![image-20250804141955711](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250804141955711.png)

> [!note]
>
> Redis 在短信登录中主要是**临时存验证码 + 限流防刷 + 过期自动清理**，提供了快速、高效、安全的登录支持。
>
> 主要实现功能：
>
> 1. 临时存储短信验证码
> 2. 限流防刷

<br>

### 1. 临时存储短信验证码

> 我这里提供 go 实现思路
>
> - Redis 设计结构
>
> ```ini
> SMS:LOGIN:CODE:<手机号> => 验证码（6位数字），设置过期时间 5 分钟
> ```

```go
import (
    "fmt"
    "math/rand"
    "net/http"
    "time"

    "github.com/gin-gonic/gin"
    "github.com/redis/go-redis/v9"
    "context"
)

var ctx = context.Background()

var rdb = redis.NewClient(&redis.Options{
    Addr: "localhost:6379", // 你的 Redis 地址
})

// 生成 6 位验证码
func generateCode() string {
    rand.Seed(time.Now().UnixNano())
    return fmt.Sprintf("%06d", rand.Intn(1000000))
}

// 发送验证码接口
func sendCode(c *gin.Context) {
    phone := c.Query("phone")
    if phone == "" {
        c.JSON(http.StatusBadRequest, gin.H{"error": "手机号不能为空"})
        return
    }

    code := generateCode()
    redisKey := fmt.Sprintf("SMS:LOGIN:CODE:%s", phone)

    // 存入 Redis，设置 5 分钟过期
    err := rdb.Set(ctx, redisKey, code, 5*time.Minute).Err()
    if err != nil {
        c.JSON(http.StatusInternalServerError, gin.H{"error": "Redis 存储失败"})
        return
    }

    // 实际项目应通过短信服务发送验证码
    c.JSON(http.StatusOK, gin.H{"message": "验证码发送成功", "debug_code": code})
}

// 验证验证码接口
func verifyCode(c *gin.Context) {
    phone := c.PostForm("phone")
    inputCode := c.PostForm("code")
    if phone == "" || inputCode == "" {
        c.JSON(http.StatusBadRequest, gin.H{"error": "手机号或验证码不能为空"})
        return
    }

    redisKey := fmt.Sprintf("SMS:LOGIN:CODE:%s", phone)
    storedCode, err := rdb.Get(ctx, redisKey).Result()
    if err == redis.Nil {
        c.JSON(http.StatusBadRequest, gin.H{"error": "验证码已过期或不存在"})
        return
    } else if err != nil {
        c.JSON(http.StatusInternalServerError, gin.H{"error": "Redis 获取失败"})
        return
    }

    if storedCode != inputCode {
        c.JSON(http.StatusBadRequest, gin.H{"error": "验证码错误"})
        return
    }

    c.JSON(http.StatusOK, gin.H{"message": "验证成功"})
}

```

<br>

### 2. 限流防刷

> ```ini
> SMS:LIMIT:PHONE:<手机号> => 1（存在表示冷却中，60秒过期）
> SMS:LIMIT:IP:<ip地址>   => 计数（每次 +1，1小时过期）
> ```

```go
// 判断是否可以发送验证码（限流逻辑）
func canSendCode(phone, ip string) (bool, string) {
    phoneKey := fmt.Sprintf("SMS:LIMIT:PHONE:%s", phone)
    ipKey := fmt.Sprintf("SMS:LIMIT:IP:%s", ip)

    // 限制手机号 60s 一次
    if rdb.Exists(ctx, phoneKey).Val() == 1 {
        return false, "请勿频繁请求验证码"
    }

    // 限制 IP 每小时最多 10 次
    ipCount := rdb.Incr(ctx, ipKey).Val()
    if ipCount == 1 {
        rdb.Expire(ctx, ipKey, time.Hour)
    }
    if ipCount > 10 {
        return false, "当前 IP 请求过于频繁"
    }

    return true, ""
}

// 发送验证码接口（添加限流）
func sendCodeWithLimit(c *gin.Context) {
    phone := c.Query("phone")
    ip := c.ClientIP()

    ok, reason := canSendCode(phone, ip)
    if !ok {
        c.JSON(http.StatusTooManyRequests, gin.H{"error": reason})
        return
    }

    code := generateCode()
    redisCodeKey := fmt.Sprintf("SMS:LOGIN:CODE:%s", phone)
    redisPhoneLimitKey := fmt.Sprintf("SMS:LIMIT:PHONE:%s", phone)

    // 设置验证码和限流标记
    err := rdb.Set(ctx, redisCodeKey, code, 5*time.Minute).Err()
    if err != nil {
        c.JSON(http.StatusInternalServerError, gin.H{"error": "验证码存储失败"})
        return
    }
    rdb.Set(ctx, redisPhoneLimitKey, 1, 60*time.Second) // 标记冷却期

    c.JSON(http.StatusOK, gin.H{"message": "验证码已发送", "debug_code": code})
}

```

### 3. Token 的刷新机制

---

> Token 特别合适存储到 Redis，存储，再续 Token 时长，防止多次申请重复的 Token，而且还可以通过指定 ID 关联到 SQL 的对应用户，简直不二人选，关键还有缓存。
>
> - accessToken 在客户端
>- freshToken 在服务器

```yaml
# 目录结构
/internal/token/token.go         // 封装生成、解析逻辑
/internal/middleware/auth.go     // 中间件，调用 token 模块验证
```

```go
package main

import (
    "errors"
    "time"
    "github.com/golang-jwt/jwt/v5"
)

var jwtSecret = []byte("your-secret")

type Claims struct {
    UserID uint `json:"uid"`
    jwt.RegisteredClaims
}

// 解析但不校验签名
func parseUnverified(tokenString string) (*Claims, error) {
    token, _, err := jwt.NewParser().ParseUnverified(tokenString, &Claims{})
    if err != nil {
        return nil, err
    }
    if claims, ok := token.Claims.(*Claims); ok {
        return claims, nil
    }
    return nil, errors.New("invalid claims")
}

// 生成新的双 token
func genTokenPair(uid uint) (access, refresh string, err error) {
    // access
    access, err = jwt.NewWithClaims(jwt.SigningMethodHS256, Claims{
        UserID: uid,
        RegisteredClaims: jwt.RegisteredClaims{
            ExpiresAt: jwt.NewNumericDate(time.Now().Add(15 * time.Minute)),	// 15 min
        },
    }).SignedString(jwtSecret)

    // refresh
    refresh, err = jwt.NewWithClaims(jwt.SigningMethodHS256, jwt.RegisteredClaims{
        ExpiresAt: jwt.NewNumericDate(time.Now().Add(7 * 24 * time.Hour)),	    // 7 days
    }).SignedString(jwtSecret)
    return access, refresh, err
}
```

> 中间件部分

```go
package middleware

import (
    "net/http"
    "strings"
    "time"
    "github.com/gin-gonic/gin"
    "github.com/redis/go-redis/v9"
)

const (
    refreshThreshold = 0.3               // 剩余 <30% 触发刷新
    accessTTL        = 15 * time.Minute  // 与生成时保持一致 [建议写环境变量哈]
)

func TokenAuthMiddleWare(rdb *redis.Client) gin.HandlerFunc {
    return func(c *gin.Context) {
        auth := c.GetHeader("Authorization")
        if !strings.HasPrefix(auth, "Bearer ") {
            c.Next()
            return
        }
        accessToken := strings.TrimPrefix(auth, "Bearer ")

        // 1. 转换 token 为 claims，用于解析数据
        claims, err := parseUnverified(accessToken)
        if err != nil {
            c.AbortWithStatus(http.StatusUnauthorized)
            return
        }

        exp := time.Unix(claims.ExpiresAt.Unix(), 0)
        remain := time.Until(exp)
        ratio := remain.Seconds() / accessTTL.Seconds()

        if ratio < refreshThreshold { // 需要滑动刷新
            uid := claims.UserID

            // 2. 校验 refresh_token 是否仍有效
            refreshKey := "refresh:" + accessToken // 简单示例：存 access 本身做 key
            uidStr, _ := rdb.Get(c, refreshKey).Result()	// 通过 uid 查询 freshToken
            if uidStr == "" { // refresh_token 已被撤销
                c.AbortWithStatus(http.StatusUnauthorized)
                return
            }

            // 3. 生成新的双 token
            newAccess, newRefresh, err := genTokenPair(uid)
            if err != nil {
                c.AbortWithStatus(http.StatusInternalServerError)
                return
            }

            // 4. 更新 Redis
            rdb.Set(c, "refresh:"+newAccess, uid, 7*24*time.Hour)
            rdb.Del(c, refreshKey) // 旧 access 对应的 refresh 失效

            // 5. 把新 token 塞回响应头
            c.Header("X-New-Access-Token", newAccess)
            c.Header("X-New-Refresh-Token", newRefresh)
            c.Header("Access-Control-Expose-Headers",
                "X-New-Access-Token,X-New-Refresh-Token")
        }

        c.Next()
    }
}
```

<br>

# Redis 实现商户缓存

---

![image-20250804172952690](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250804172952690.png)

> 实现流程图

![image-20250804173103282](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250804173103282.png)

> [!important]
>
> 我们来做一个最小实现，下面的数据部分

```go
	package main

import (
    "context"
    "database/sql"
    "fmt"
    "github.com/gin-gonic/gin"
    "github.com/redis/go-redis/v9"
    _ "github.com/go-sql-driver/mysql"
    "net/http"
    "strconv"
    "time"
)

var (
    ctx = context.Background()
    rdb = redis.NewClient(&redis.Options{
        Addr: "localhost:6379",
    })
    db *sql.DB
)

func main() {
    // 初始化数据库（请替换 DSN）
    var err error
    db, err = sql.Open("mysql", "root:password@tcp(127.0.0.1:3306)/testdb")
    if err != nil {
        panic(err)
    }

    r := gin.Default()

    r.GET("/products/:id", func(c *gin.Context) {
        idStr := c.Param("id")
        id, _ := strconv.Atoi(idStr)
        redisKey := fmt.Sprintf("product:%d", id)

        // ✅ 1. 先查 Redis
        if val, err := rdb.Get(ctx, redisKey).Result(); err == nil {
            c.String(http.StatusOK, "[From Redis] "+val)
            return
        }

        // ✅ 2. Redis 未命中 → 查数据库
        row := db.QueryRow("SELECT name, price FROM products WHERE id = ?", id)
        var name string
        var price int
        if err := row.Scan(&name, &price); err != nil {
            c.JSON(http.StatusNotFound, gin.H{"error": "Product not found"})
            return
        }

        value := fmt.Sprintf("%s (%d元)", name, price)

        // ✅ 3. 回写 Redis（缓存 5 分钟）
        rdb.Set(ctx, redisKey, value, 5*time.Minute)

        c.String(http.StatusOK, "[From DB] "+value)
    })

    r.Run(":8080")
}
```

<br>

## 1. 缓存更新策略

---

![image-20250805171016711](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250805171016711.png)

### 主动更新策略

![image-20250805171315741](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250805171315741.png)

> 比较好的策略应该是如下的：

![image-20250806060856190](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250806060856190.png)



##  缓存穿透

> `缓存穿透`是指客户端请求的数据在缓存中和数据库中都不存在，这样缓存永远不会生效，这些请求都会打到数据库。

![image-20250806105621471](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250806105621471.png)

- 短期不一致就是说，我们如果一开始有一个数据是没有的，我们缓存为 null 放到了 redis，但是如果 SQL 里面数据更新了，但是外部访问这个时候，还是 TTL 时间内，访问仍未 null 数据，也就是一致性不太好，这个可以我们在更新 SQL 数据之后，主动更新外部缓存的

<br>

##  缓存雪崩

> `缓存雪崩`是指在同一时段大量的缓存`key`同时失效或者`Redis`服务岩机，导致大量请求到达数据库，带来巨大压力。
>
> - 说白了就是，redis 这一层失效了，请求全部打到 SQL 了

> [!important]
>
> 解决方案：
>
> - 给不同的Key的TTL添加随机值--> 防止一起失效
> - 利用Redis集群提高服务的可用性-->针对 Redis 宕机，这种集群哪怕挂了一个也没关系
> - 给缓存业务添加降级限流策略--> 服务挂了之后，我们就提供弱一点的服务，限制大流量请求，防止 SQL 也挂了
> - 给业务添加多级缓存 --> 多缓存策略，失效一层，也不会马上出问题

<br>

## 缓存击穿

> `缓存击穿问题`也叫热点Key问题，就是一个被**高并发访问**并且**缓存重建业务较复杂**的key突然失效了，无数的请求访问会在瞬间给数据库带来巨大的冲击。
>
> - 我们做一个假定，虽然说热点 key 按理来说，会一直刷新这个 key 的 TTL 的，但是我们假定这个失效了
> - 然后就会带来一个问题，很多个线程都访问未命中，然后开始重新构建这个 key，但是重建缓存这个业务消耗的资源特别大，而且多个线程同时在调用，所以就会导致资源占据问题

![image-20250806111259526](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250806111259526.png)

> [!important]
>
> 解决方案：
>
> - 互斥锁 --> 这个比较好理解，我们就不费口舌了
> - 逻辑过期

### 1. 互斥锁

![image-20250806111721015](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250806111721015.png)

### 2. 逻辑过期

> [!note]
>
> 原本是真的给 key 设置一个 TTL，逻辑过期就是在 value 里面用一个表示过期的时间
>
> - 这个 key 后期自己等热点过期了，在删除就行了，热点的时候占缓存也没关系

![image-20250806111841233](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250806111841233.png)

<br>

> 逻辑过期的解决思路：
>
> - 注意这里是改变了模式，也就是我们的 redis 数据

![image-20250806112832327](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250806112832327.png)

<br>

### 3. 两种解决方案的比较

<img src="https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250806113048284.png" alt="image-20250806113048284" style="zoom:67%;" />

<br>

# Redis 优惠卷秒杀

---

## 1. 全局唯一 ID

> [!note]
>
> 秒杀成功后，你需要给用户生成一个订单。
>
> 高并发下不能用数据库自增 ID（瓶颈严重、分布式不唯一），所以我们用**全局唯一 ID**来做订单编号（如雪花 ID）

### **雪花算法（Snowflake）**

```go
import "github.com/bwmarrin/snowflake"

var node *snowflake.Node

func init() {
    var err error
    node, err = snowflake.NewNode(1) // 1 表示机器 ID ---> 用于生成唯一标记
    if err != nil {
        panic(err)
    }
}

func GenerateID() int64 {
    return node.Generate().Int64()
}

```

<br>

## 超卖问题

> **超卖**指的是：系统在高并发下，**卖出了超过库存数量的商品**。

<img src="https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250806122455410.png" alt="image-20250806122455410" style="zoom:77%;" />

```go
stock := redis.Get("stock") // 当前库存是 20
if stock > 0 {
    redis.Set("stock", stock - 1)
}
```

- 两个线程同时访问的时候，拿到的 `stock` 都是 20

<br>

### 锁

---

![image-20250806123359319](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250806123359319.png)

> 乐观锁
>
> - 我们在单表数据中再加一个 `VERSION` 字段，

```sql
CREATE TABLE products (
    id BIGINT PRIMARY KEY,
    stock INT NOT NULL,				 `表示库存`
    version INT NOT NULL DEFAULT 0,   `表示版本号，每更新一次加 1，用来实现乐观锁`
);
```

> 代码实现下面的逻辑

![image-20250806124101333](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250806124101333.png)

```go
var (
    ctx        = context.Background()
    redisClient *redis.Client
    db         *sql.DB
)

// 乐观锁更新库存函数
func reduceStock(productID int64) error {
    for i := 0; i < 3; i++ { // 最多重试3次
        var stock, version int
        err := db.QueryRow("SELECT stock, version FROM products WHERE id = ?", productID).Scan(&stock, &version)
        if err != nil {
            return fmt.Errorf("查询库存失败: %v", err)
        }
        if stock <= 0 {
            return fmt.Errorf("库存不足")
        }

        // 执行乐观锁更新：只有version没变时才更新 --> 别的线程可能已经访问过了
        res, err := db.Exec(`UPDATE products SET stock = stock - 1, version = version + 1 WHERE id = ? AND version = ? AND stock > 0`, productID, version)
        if err != nil {
            return fmt.Errorf("更新库存失败: %v", err)
        }
        rows, _ := res.RowsAffected()
        if rows == 0 {
            // 说明版本号变化了（有并发改动），重试
            time.Sleep(50 * time.Millisecond)
            continue
        }

        // 更新缓存，先写库后更新缓存
        err = redisClient.Set(ctx, fmt.Sprintf("product_stock_%d", productID), stock-1, 0).Err()
        if err != nil {
            log.Printf("更新缓存失败: %v", err)
        }
        return nil
    }
    return fmt.Errorf("库存更新冲突，重试次数用尽")
}
```

<br>

### 集群模式下锁失效问题

---

> 背景：为了负载均衡，我们使用了，nginx 创建了多个后端，然后用代理让请求均匀分配到两个后端上，突然发现同一个用户的两个请求都成功了，即便我们做过 ID 限制防止同一个 ID 多次请求

- 解决方案：
  1. 分布式锁
  2. lua 脚本原子操作

<br>

