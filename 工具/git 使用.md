# git 的使用

---

<br>

## 1. 关于分支

| 操作             | 命令                                               |
| ---------------- | :------------------------------------------------- |
| 查看所有本地分支 | `git branch`                                       |
| 创建分支         | `git branch 分支名`                                |
| 切换分支         | `git checkout 分支名` 或 `git switch 分支名`       |
| 创建并切换分支   | `git checkout -b 分支名` 或 `git switch -c 分支名` |
| 推送分支到远程   | `git push origin 分支名`                           |
| 删除本地分支     | `git branch -d 分支名`                             |
| 合并分枝到 main  | `git merge 分支名`                                 |

<br>

## 2. git 回溯到某个版本

```bash
git log --oneline
```

> 下面为输出结果

```bash
b44e1ab (HEAD -> main, origin/main, origin/HEAD) Add initial logical layer implementation
cc93215 (origin/feature_logical_layer, feature_logical_layer) 实现逻辑层
dc55110 做了份文件，分类了 MsgNode 类型，实现了完全的 tlv （ID+LEN+MSG）
e1f17c2 实现了 Json 序列化通讯
55a1be4 实现了网络和本地大小端的问题
dcfc1a5 理解 HandleRead 的机制，并添加了对应的注释
5518a2b 删除 template 中的 gitignore 文件
88f9249 Remove gitignore file from tracking
2f23e98 添加了一些注释
1d87112 Initial commit
```

|  场景  | 说明                                           | 推荐命令                                      |
| :----: | ---------------------------------------------- | --------------------------------------------- |
| 场景 1 | 临时查看旧版本，**不改变历史**                 | `git checkout <commit>`                       |
| 场景 2 | 回退到旧版本，**修改当前分支指向**（保留历史） | `git reset --soft/hard <commit>`              |
| 场景 3 | 回退并提交（**让版本历史看起来像没发生过**）   | `git reset --hard <commit>`，再 `git push -f` |
| 场景 4 | 创建一个分支来保留旧代码                       | `git checkout -b backup <commit>`             |
