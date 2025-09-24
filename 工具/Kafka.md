# Kafka

---

## Kafka 对比其他 MQ 的优势

![image-20250825155237174](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250825155237174.png)

## 使用场景

> Kafka 其实可以看成是一个**分布式的消息队列系统**，但它不仅仅是“队列”，更像是一个**分布式的日志存储系统**

<img src="https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250825153904064.png" alt="image-20250825153904064" style="zoom:70%;" />

## 基本概念

> [!important]
>
> **Broker（代理节点）**
>  Kafka 的一个服务器进程，多个 Broker 组成一个 Kafka 集群。多个 broker 之间的协调用的是 `ZooKeeper`
>
> **Topic（主题）**
>  类似“频道”或“消息分类”，消息按主题来组织
>
> **Partition（分区）**
>  每个 Topic 可以分成多个分区
>  每个分区就是一个**有序的、只能追加写入的日志文件**
>  分区内消息有严格顺序，分区之间没有全局顺序

![image-20250825154410265](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250825154410265.png)

> [!note]
>
> Kafka 不会“推”消息，而是消费者主动去“拉”消息。
>
> 也就是被订阅的主题内容，不会主动推送说自己更新了，而是要订阅者自己看是否更新了新的消息
>
> 消息不会因为被消费就删除，而是按配置的**保留策略**来清理（比如保存 7 天，或者日志文件超过多少 GB）

![image-20250825154759709](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250825154759709.png)

### Consumer Group（消费者组）

- 消费者是以`组`的形式来订阅的。
- 一个分区同一时间只能被同一个组里的一个消费者消费。
- 好处：可以横向扩展消费能力，分区数决定了最大并行度。

> [!important]
>
> **Offset（偏移量）**
>  分区里的每条消息都有一个唯一的编号，消费者靠 offset 知道自己读到哪里了。

![image-20250825154934367](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250825154934367.png)

> [!important]
>
> Producer 把消息写到某个 Topic 的某个分区里。分区的选择可以：
>
> - 轮询（负载均衡）
> - 根据 key 做哈希（相同 key 的消息保证进同一个分区，维持局部顺序）
>
> 消息写入分区后，会先进**磁盘日志文件**（顺序写，非常快，操作系统会用页缓存做优化）。

![image-20250825155109627](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250825155109627.png)

## kakfa 的主从模式

![image-20250825155754269](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250825155754269.png)



### 高可用与容错

- Kafka 每个分区会有多个副本：
  - **Leader**：负责收发消息。
  - **Follower**：被动同步 Leader 的数据。
- 如果 Leader 节点挂了，某个 Follower 会被提升为新的 Leader，保证系统可用。