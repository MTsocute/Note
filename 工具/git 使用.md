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

<br>

---

## 远程仓库操作

| 功能                       | 命令                                             | 说明                                     |
| -------------------------- | ------------------------------------------------ | ---------------------------------------- |
| 查看远程仓库               | `git remote -v`                                  | 列出所有远程简写名及对应的 URL           |
| 添加远程仓库               | `git remote add <name> <url>`                    | 常用 `origin` 作为 `<name>`              |
| 删除远程仓库               | `git remote remove <name>` 或 `git remote rm <name>` | 删除指定的远程仓库                       |
| 重命名远程仓库             | `git remote rename <old> <new>`                  | 将远程简写名 `<old>` 改为 `<new>`        |
| 修改远程仓库 URL           | `git remote set-url <name> <newurl>`             | 更新 fetch 和 push 的 URL               |
| 查看远程仓库详细信息       | `git remote show <name>`                         | 包括分支追踪、URL、推拉状态              |
| 获取远程更新               | `git fetch <name>`                               | 同步远程分支最新提交，不自动合并         |
| 删除远程已删除的分支引用   | `git remote prune <name>`                        | 清理本地对已经删除的远程分支的无效引用   |
| 克隆远程仓库               | `git clone <url> [目录]`                         | 下载远程仓库到本地，新建目录可选         |
| 推送分支并设置上游         | `git push -u <name> <branch>`                    | 后续 `git push` 可省略远程名和分支名     |
| 推送所有本地分支           | `git push --all <name>`                          | 一次性推送所有本地分支到指定远程         |
| 拉取并合并远程分支         | `git pull <name> <branch>`                       | 等同 `git fetch` + `git merge`           |
| 拉取并重基                 | `git pull --rebase <name> <branch>`              | 等同 `git fetch` + `git rebase`          |

### 如何将本地仓库推送至 GitHub 远程仓库

1. 在本地项目根目录初始化 Git（若已初始化，可跳过）：
  ```bash
  git init
  ```

2. 在 GitHub 上创建新仓库，不勾选初始化选项，复制仓库地址（HTTPS 或 SSH）。

3. 添加远程仓库地址：

  > [!warning]
  >
  > 出现了问题的话，就看看 4.2 部分

  ```bash
  git remote add origin <仓库地址>
  ```

4. 将主分支重命名为 main（若已是 main 可跳过）：
  ```bash
  git branch -M main
  ```

5. 首次推送并设置上游：
  ```bash
  git add .
  git commit -m "首次提交"
  git push -u origin main
  ```

6. 后续更新时，执行：
  ```bash
  git add .
  git commit -m "更新说明"
  git push
  ```

<br>

###  git 使用 ssh 管理 remote 仓库

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

> 切换我们的登录验证方式为 `ssh` 不然的话默认是 `http` 但是这个方法早就官方被 ban 了，没法 http 控制仓库
>
> 我们先看看当前是哪种方式

```shell
git remote -v
```

```shell
origin	https://github.com/MTsocute/Go_Blog.git (fetch)
origin	https://github.com/MTsocute/Go_Blog.git (push)
```

> 果然是 http, 那么我们用下面的命令切换到 ssh，之后，我们就可以通过 git 命令操作仓库了

```shell
git remote set-url origin git@github.com:MTsocute/Go_Blog.git
```

### 使用 Github 登录工具登录

> [!note]
>
> 这个比起 4.2 方法要更方便，如果采用 http 方法的话，你得确保你有桌面程序不然没法用
>
> http 不能用就走 SSL

```bash
sudo apt update
sudo apt install gh
```

<br>

## 如何拉取指定的版本
---

以 [lvgl/lvgl](https://github.com/lvgl/lvgl) 仓库为例，常见拉取指定版本的方法如下：

### 拉取指定 Tag 版本

### 直接拉取指定分支

1. 克隆仓库并指定分支（如 release/v9.0）：
  ```bash
  git clone -b release/v9.0 https://github.com/lvgl/lvgl.git
  ```
  > 这样会直接拉取并切换到目标分支，无需再 checkout。

2. 如果已克隆仓库，切换分支：
  ```bash
  git fetch origin
  git checkout release/v9.0
  ```
1. 克隆仓库（可选，已有本地仓库可跳过）：
  ```bash
  git clone https://github.com/lvgl/lvgl.git
  cd lvgl
  ```
2. 查看所有 tag：
  ```bash
  git tag
  ```
3. 切换到指定 tag（如 v8.3.0）：
  ```bash
  git checkout v8.3.0
  ```
  > 此时处于 detached HEAD 状态，仅用于查看或使用该版本代码。

### 拉取指定 commit 版本

1. 查看 commit 历史，找到目标 commit 哈希：
  ```bash
  git log --oneline
  ```
2. 切换到指定 commit：
  ```bash
  git checkout <commit-hash>
  ```
  > 例如：`git checkout 1a2b3c4d`

### 拉取指定分支

1. 查看所有分支：
  ```bash
  git branch -a
  ```
2. 切换到远程分支（如 release/v9.0）：
  ```bash
  git checkout origin/release/v9.0
  ```
  或新建本地分支跟踪远程分支：
  ```bash
  git checkout -b release/v9.0 origin/release/v9.0
  ```

---

> [!tip]
> 切换到指定 tag 或 commit 后，属于 detached HEAD 状态，若需开发建议新建分支：
>
> ```bash
> git checkout -b my-feature-branch
> ```
