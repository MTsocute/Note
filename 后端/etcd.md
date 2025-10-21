# ETCD

---

- [ETCD 注册于发现的逻辑](https://www.bilibili.com/video/BV1h14y1r7mT/?vd_source=b47817c1aa0db593f452034d53d4273a)

## 环境搭建

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/163b753110e253ec5b9f17e6f5c0697f.png)

### 服务端的搭建

> 创建一个 ETCD 的服务端

```yaml
services:
  etcd:
    image: quay.io/coreos/etcd:v3.5.0
    container_name: etcd-server
    ports:
      - "2379:2379"
      - "2380:2380"
    environment:
      ETCD_NAME: etcd
      ETCD_LISTEN_CLIENT_URLS: http://0.0.0.0:2379
      ETCD_ADVERTISE_CLIENT_URLS: http://0.0.0.0:2379
      ETCD_LISTEN_PEER_URLS: http://0.0.0.0:2380
      ETCD_INITIAL_ADVERTISE_PEER_URLS: http://0.0.0.0:2380
      ETCD_INITIAL_CLUSTER: etcd=http://0.0.0.0:2380
      ETCD_INITIAL_CLUSTER_TOKEN: etcd-cluster
      ETCD_INITIAL_CLUSTER_STATE: new
    volumes:
      - etcd-data:/etcd-data
    command: etcd --data-dir /etcd-data

volumes:
  etcd-data:

```

### 客户端的搭建

[去这个地址下载客户端程序](https://github.com/tzfun/etcd-workbench/releases/tag/App-1.2.0)

<br>

## 服务的注册和发现

> etcd 的读写速度和 redis 是没法比的，但是他能够作为微服务这种分布式系统的键值操作的管理数据库，依靠的就是`强一致性`的优点

![image-20250927125929954](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250927125929954.png)

## go-zero 的 etcd 服务注册

- [go-zero 注册 etcd](https://go-zero.dev/docs/tutorials/grpc/server/example#1-%E5%88%9B%E5%BB%BA%E6%9C%8D%E5%8A%A1%E7%9B%AE%E5%BD%95%E5%88%9D%E5%A7%8B%E5%8C%96-go-module-%E5%B7%A5%E7%A8%8B)
- [视频解说](https://www.bilibili.com/video/BV12x4uecE5Y?spm_id_from=333.788.videopod.sections&vd_source=b47817c1aa0db593f452034d53d4273a)

> 去看下我的 github 仓库，那里比较详细
