# Git

## 相关概念

### 系统凭据（Windows）

- 概念

  

- 如何访问

  方式一：`控制面板 → 用户账户 → 凭据管理器 → 普通凭据`

  方式二：`rundll32.exe keymgr.dll,KRShowKeyMgr`

### .ssh 文件

## 远程仓库认证

​	这里先做一个概念的澄清说明，拿 GitHub 来举例说明，GitHub 作为代码仓库，也就是作为本地代码的远程仓库。一般仓库有两种类型，一个是公共，默认所有用户都有拉取代码的权限（就也是说不要在这种仓库放置任何敏感信息），但是想要为代码仓库共享代码，就需要仓库的管理负责人审批。一个是私有，默认所有用户没有仓库的任何权限。但是作为仓库的创建者、拥有者，又或者是经过授权认证的账号是可以仓库的各种权限的

​	然后，权限认证的方式有两种，一种是通过账号密码的用户认证，也就是账号名和密码对应的用户，需要被仓库的创建人设置了相应权限，比如说，作为管理员（当然账号就是仓库的创建人就不说了），通过 HTTPS 协议访问远程仓库，默认就是采用这种方式；另一种是密钥认证，这个也是直接和账号相关，原理就是说，每个 GitHub 账户可以创建设置多个密钥对，然后将密钥对的所有密钥对的公钥配置到 GitHub 账号的 [SSH and GPG keys](https://github.com/settings/keys)，那么只要拥有这些公钥任意一个对应的私钥的PC端，就具有相应账户的权限了，如果账户正好有远程仓库的相应权限，那么就可以进行相关的操作了。通过 SSH 协议访问远程仓库，默认就是这种方式，具体的配置，实操见相关模块

​	如果你不想让别人看到 Git 库，有两个办法，一个是交点保护费，让 GitHub 把公开的仓库变成私有的，这样别人就看不见了，不可读更不可写（这是早期的说法，GitHub 为了推崇开源精神，才做成申请私有仓库是要收费的，到了现在，GitHub 已经没有相关限制了）；另一个办法是自己动手，搭一个Git服务器，因为是你自己的Git服务器，所以别人也是看不见的。这个方法我们后面会讲到的，相当简单，公司内部开发必备

### 客户端

#### SourceTree

**问题**

1. 每次重启电脑之后，SourceTree 访问 GitHub 的私有仓库，就显示找不到

​	解决：浏览器界面得放访问 GitHub，且是相关账号的登录状态；SourceTree 在远程仓库配置了正确的账号信息。满足这两个条件，才能正常拉取、推送代码

​	如何为 SourceTree 添加账号信息：点击新增项目的➕ → 点击 Remote图标 → 点击 添加一个账户 → Host 选 GitHub，Crentials 选 Basic，然后输入账户名和密码

终极解决方案1（不推荐）：

​	如果是 Windows 操作系统，可以把所以凭证信息全删了，重新绑定关联凭证信息

终极解决方案2（不推荐）：

​	git credential-manager uninstall
​	git credential-manager install

#### Original Git



### 认证方式

GitHub 任何仓库的访问地址，都有如下两种方式：

基于 HTTPS 协议 - https://github.com/minQie/xxx.git
基于 SSH 协议 - git://github.com/minQie/xxx.git（和 git@github.com:minQie/xxx.git 等效）

通过 ssh 支持的原生 git 协议速度最快的，使用 https 除了速度慢一些以外，还有个最大的麻烦是每次推送都必须输入口令，但是在某些只开放 http 端口的公司内部就无法使用 ssh 协议而只能用 https

#### HTTPS

​	通过账号密码的方式，Windows 平台下，会和凭证管理有较大的关系

#### SSH

- 密钥对概念

  在操作系统本地通过相关命令，将生成的秘钥对的公钥配置到远程仓库平台的账户，拥有配对的私钥的电脑就能访问账户下的所有仓库，又或者说 GitHub 需要识别出你推送的提交确实是你推送的，而不是别人冒充的，而 Git 支持 SSH 协议，所以，GitHub 只要知道了你的公钥，就可以确认只有是你自己推送的

  如果生成秘钥时指定了密码（passphrase），那么发起连接时，在私钥和公钥校验之前，还需要输入密码（passphrase）。实际的化，输入指令后，一直回车下去就行，没必要设置密码，如果设置了，那么在通过秘钥各种场景的连接，还需要输入该密码，是否指定密码的实际效果如下

  - 生成秘钥时的“Enter passphrase for key”直接回车

  ![image-20201102195702063](Git.assets/image-20201102195702063.png)

  - 生成秘钥时的“Enter passphrase for key”输入了

  ![image-20201102195714822](Git.assets/image-20201102195714822.png)

  

  - Windows 下可以参考如下命令

  ```bash
  ssh-keygen -t "rsa" -f "C:\Users\用户名\.ssh\xxx_rsa" -C "1459049487@qq.com"
  # 查看命令帮助
  ssh-keygen --help
  ```

  - 在linux使用 `ssh-keygen` 命令失败的解决办法

  ```shell
  # 查看服务是否开启
  systemctl status sshd
  # 启动服务
  systemctl start sshd
  ```

  Linux 操作系统下，密钥对文件默认生成到 `~/.ssh（即/root/.ssh）`，Windows 操作系统下，密钥对文件默认生成到 `C:\Users\用户名\.ssh`。生成的密钥对文件，`id_rsa` 是私钥文件，`id_rsa.pub` 是公钥文件（公钥加密的数据，你可以用私钥解密）。就是这个路径和文件名是有讲究的，如果你修改了，Git 就找不到了，如果修改了也是可用通过配置让 Git 找到的

  - windows

  ```
  文件（不存在就创建）：
  C:\Users\用户名\.ssh\config
  
  添加如下内容：
  Host github.com
  HostName github.com
  PreferredAuthentications publickey
  IdentityFile C:\Users\用户名\.ssh\my_laptop_rsa
  ```

  - linux

  ```
  文件（不存在就创建）：
  /etc/ssh/ssh_config
  
  添加如下内容：
  Host github.com
  HostName github.com
  PreferredAuthentications publickey
  IdentityFile ~/.ssh/my_laptop_rsa
  ```

  - 配置解释

  ```markdown
  - Host
  Host 配置项的值会影响 git 相关命令，例如：
  Host mygithub 这样定义的话，命令如下，即 git@ 后面紧跟的名字改为 mygithub
  git clone git@mygithub:minQie/xxx.git
  ```
  
  假定你有若干电脑，你一会儿在公司提交，一会儿在家里提交，只要把每台电脑的 Key 都添加到 GitHub，就可以在每台电脑上往 GitHub 推送了。类比到实际多人合作开发也是这样的，但是注意，不是说主账户创建多个密钥对，然后把私钥发别人，而是说谁想要加入，就应该自己创建密钥对，然后把公钥发给仓库创建主账户，主账户把公钥配置上就可以了
  
  ```
  # GitHub 配置步骤大概如下
  Settings（Personal setting） -> SSH and GPG Keys -> New SSH key -> Title随意、 Key填入公钥
  ```

## 多远程仓库

​	本篇提到的远程仓库，都是指 GitHub，这里引出 `多` 的概念，就再提一个 Gitee，GitHub 和 Gitee 都是功能比较完善的代码仓库平台，但是，在国内码云的代码拉取和推送速度远大于 GitHub

​	比如说将本地仓库和一个远程仓库关联起来：`git remote add origin https://gitee.com/minQie/tangjia.git`，然后你又想将该本地仓库与 Gitee 远程仓库关联起来，仅是简单的改一些命令的仓库地址是不行的。Git 的理念是分布式版本控制系统，可以同步到一个远程库，当然也可以同步到另外两个远程库。使用多个远程库，Git 给远程库起的默认名称是 origin，如果有多个远程库，我们需要用不同的名称来标识不同的远程库，具体：

```
git remote remove origin
git remote add origin_gitee https://github.com/minQie/tangjia.git
git remote add origin_github https://gitee.com/minQie/tangjia.git
```

**问题**

​	上面的例子是说，基于两个不同的远程仓库平台，域名和协议开头都是不同的，使用中是能够清晰的区分，不会出现使用上的问题，但是如果是某一个平台的两个账号，就会遇到其中一个账号下的仓库对应的本地仓库拉取或推送代码发生错误，这里就不描述具体的问题场景了，从原理说清楚这里边的概念，当然，主要是围绕 SSH 协议展开来说

**原理**

```
git@github.com:minQie/awesome.git
```

那上面的例子来说，这是GitHub 上的一个远程仓库，基于 git 协议访问的连接地址，那么当本地在通过 git 相关命令尝试操作该仓库，GitHub 会有一个鉴权机制，核心就是 [SSH](#SSH) 中的密钥对，就是说这样的密钥对的私钥会有一个默认的存放路径和默认的文件名，操作系统会读取该文件，用于鉴权。就也是像里边描述的，如果你修改了，就需要对 `.ssh\config` 配置文件进行额外的修改和配置，核心就是这个 `config`  文件，也不难看出，**不同域名所需要的密钥文件的对应关系，就是在里边配置的**。所以如果我们有多个远程仓库平台或者一个平台的多个账号，就都需要在 `config` 文件里边进行配置区分，来看具体的例子

```markdown
- 在 GitHub 平台上的两个账户 A 和 B
https://github.com/A
https://github.com/B

- 本地创建了两对密钥（并且已经将公钥配置到了对应的 GitHub 账户下的 SSH keys）
a_rsa a_rsa.pub
b_rsa b_rsa.pub

- config 文件应该这样配置
Host A.github.com
HostName github.com
PreferredAuthentications publickey
IdentityFile C:\Users\用户名\.ssh\a_rsa

Host B.github.com
HostName github.coma
PreferredAuthentications publickey
IdentityFile C:\Users\用户名\.ssh\b_rsa

- 测试是否配置成功，除了实际本地仓库拉取推送代码，还可以通过下面的命令来检测
ssh -T git@A.github.com（上面配置的 Host）
ssh -T git@B.github.com

成功的话，会出现
Hi A! You've successfully authenticated, but GitHub does not provide shell access.
Hi B! You've successfully authenticated, but GitHub does not provide shell access.

- 现在账户A 下有名为 a 的仓库，账户B 下有名为 b 的仓库
git@github.com:A/a.git
git@github.com:B/b.git

就是这样的仓库地址，是 GitHub 按照默认规则 提供的，应该按照如下配置远程仓库地址才能将其和 config 文件的配置对应起来
git@A.github.com:A/a.git
git@A.github.com:B/b.git
```

还有一点需要注意的，就是上例中，创建了两对密钥，你可能想，本地一个密钥对，本地留着密钥就行，然后将这个密钥对应的公钥配置到各个平台，平台的各个账户，不是最简单。实际上，GitHub 就有做唯一校验，所以单一平台的多个账户使用一个密钥对是不现实的

## 待办

不同的协议、不同的客户端，可以借助广第的电脑来把这个问题搞清楚