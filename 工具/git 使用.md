# git 的使用

---

<br>

## 关于分支

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

---

## git 回溯到某个版本

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

<br>

---

##  git 使用 ssh 管理 remote 仓库

> 检查本地的 ssh 是否和 github 端验证

```shell
ssh -T git@github.com
```

> [!warning]
>
> 出现了如下内容说明你的 ssh token 是绑定完成了的，不然的话[看看这篇文章](https://blog.csdn.net/weixin_42310154/article/details/118340458)

```txt
Hi MTsocute! You've successfully authenticated, but GitHub does not provide shell access.
```

> 切换我们的登录验证方式为 `ssh` 不然的话默认是 `http` 但是这个方法早就被 ban 了，没法 http 控制仓库
>
> 我们先看看当前是哪种方式

```shell
git remote -v
```

```shell
origin	https://github.com/MTsocute/Go_Blog.git (fetch)
origin	https://github.com/MTsocute/Go_Blog.git (push)
```

> 果然是 http, 那么我们用下面的命令切换到 ssh

```shell
git remote set-url origin git@github.com:MTsocute/Go_Blog.git
```

<br>

## 上传打包的文件并标定 Tag

---

| 功能                 | 命令                                               | 说明                                  |
| -------------------- | -------------------------------------------------- | ------------------------------------- |
| 查看所有标签         | `git tag`                                          | 列出所有标签                          |
| 模糊搜索标签         | `git tag -l "v1.*"`                                | 按模式筛选标签                        |
| 创建轻量标签         | `git tag v1.0.0`                                   | 简单打标签，不含说明                  |
| 创建附注标签         | `git tag -a v1.0.0 -m "Release v1.0.0"`            | 推荐方式，包含作者/日期/说明          |
| 给历史 commit 打标签 | `git tag -a v0.9.0 <commit-hash> -m "Old release"` | 给指定提交加标签                      |
| 查看标签详情         | `git show v1.0.0`                                  | 查看标签对应的提交和说明              |
| 删除本地标签         | `git tag -d v1.0.0`                                | 删除本地标签                          |
| 删除远程标签         | `git push origin --delete v1.0.0`                  | 删除远程仓库标签                      |
| 推送单个标签         | `git push origin v1.0.0`                           | 把某个标签推送到远程                  |
| 推送所有标签         | `git push origin --tags`                           | 把所有本地标签推送到远程              |
| 检出标签             | `git checkout v1.0.0`                              | 切换到标签对应的提交（detached HEAD） |
| 基于标签建分支       | `git checkout -b release-1.0 v1.0.0`               | 从标签创建新分支                      |
| 创建签名标签         | `git tag -s v1.0.0 -m "Signed release"`            | 使用 GPG 签名标签                     |
