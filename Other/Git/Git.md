# Git

## 远程仓库认证、鉴权

​	这里先做一个概念的澄清说明，拿 GitHub 来举例说明，GitHub 作为代码仓库，也就是作为本地代码的远程仓库。一般仓库有两种类型，一个是公共，默认所有用户都有拉取代码的权限（就也是说不要在这种仓库放置任何敏感信息），但是想要为代码仓库共享代码，就需要仓库的管理负责人审批。一个是私有，默认所有用户没有仓库的任何权限。但是作为仓库的创建者、拥有者，又或者是经过授权认证的账号是可以仓库的各种权限的。远程仓库的鉴权，核心就两点：

1. **账户具有仓库的权限**：由仓库负责人在 GitHub 管理平台上操作，通过 HTTPS 协议访问远程仓库，默认就是采用这种方式
2. **当前访问仓库的是这个账户，而不是别的账户**，有两种认证方式，第一种通过用户名和密码，第二种是你拥有指定公钥对应的私钥，账号密码在 Windows 操作上和上面提到的系统凭据相关；密钥对的概念就是，每个 GitHub 账户可以创建设置多个密钥对，然后将密钥对的所有密钥对的公钥配置到 GitHub 账号的 [SSH and GPG keys](https://github.com/settings/keys)，那么只要PC端拥有与任意一个公钥对应的私钥，就被认为是相应的账户在操作。通过 SSH 协议访问远程仓库，默认就是这种方式

​	如果你不想让别人看到 Git 库，有两个办法，一个是交点保护费，让 GitHub 把公开的仓库变成私有的，这样别人就看不见了，不可读更不可写（这是早期的说法，GitHub 为了推崇开源精神，才做成申请私有仓库是要收费的，到了现在，GitHub 已经没有相关限制了）；另一个办法是自己动手，搭一个Git服务器，因为是你自己的Git服务器，所以别人也是看不见的，公司内部开发必备

## 认证方式

GitHub 任何仓库的访问地址，都有如下两种方式：

```markdown
# 基于 HTTPS 协议
https://github.com/minQie/xxx.git
# 基于 SSH 协议
git@github.com/minQie/xxx.git
```

通过 SSH 支持的原生 Git 协议速度最快的，使用 HTTPS 除了速度慢一些以外，还有个最大的麻烦是每次推送都必须输入口令，但是在某些只开放 HTTP 端口的公司内部就无法使用 SSH 协议而只能用 HTTPS

### HTTPS

​	通过账号密码的方式，Windows 平台下，和凭据管理有直接关系

- **每次都要输入账号和密码的解决方案**

  If you're [cloning GitHub repositories using HTTPS](https://docs.github.com/github/using-git/which-remote-url-should-i-use), you can use a credential helper to tell Git to remember your credentials.

  ```markdown
  git config --global credential.helper manager
  # 官网说是下面这个命令，上面的也试一下
  git config --global credential.helper wincred
  ```

  核心就是上面的命令，效果就是，会在 `C:\Users\用户名\.gitconfig` 文件中添加如下一段配置

  ```
  [credential]
  	helper = manager
  ```

  下次，输入用户名和密码后，会在生成一个 `C:\Users\用户名\.git-credentials`，里面记录的就是用户名密码

- **系统凭据（Windows）**

  凭据管理器是 Windows 系统的一个系统组件，能够帮助用户完成本地访问时的认证工作。当用户第一次输入用户名和密码的时候，凭证管理器可以将这些访问凭据(用户、密码、证书等)保存在本地，再次访问该服务器站点时，Windows 系统会自动完成凭据的认证过程。有 Web 凭据、Windows 凭据（Windows 远程连接）、普通凭据（这是本篇的重点，微软账号、GitHub 账号等）

- 如何访问

  方式一：`控制面板 → 用户账户 → 凭据管理器 → 普通凭据`

  方式二：`rundll32.exe keymgr.dll,KRShowKeyMgr`

### SSH

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

  - Windows

  ```
  文件（不存在就创建）：
  C:\Users\用户名\.ssh\config
  
  添加如下内容：
  Host github.com
  HostName github.com
  PreferredAuthentications publickey
  IdentityFile C:\Users\用户名\.ssh\my_laptop_rsa
  ```

  - Linux

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

## 远程仓库冲突

​	本篇提到的远程仓库，都是指 GitHub，这里引出 `多` 的概念，就再提一个 Gitee，GitHub 和 Gitee 都是功能比较完善的代码仓库平台，但是，在国内码云的代码拉取和推送速度远大于 GitHub

​	比如说将本地仓库和一个远程仓库关联起来：`git remote add origin https://gitee.com/minQie/tangjia.git`，然后你又想将该本地仓库与 Gitee 远程仓库关联起来，仅是简单的改一些命令的仓库地址是不行的。Git 的理念是分布式版本控制系统，可以同步到一个远程库，当然也可以同步到另外两个远程库。使用多个远程库，Git 给远程库起的默认名称是 origin，如果有多个远程库，我们需要用不同的名称来标识不同的远程库，具体：

```
git remote remove origin
git remote add origin_gitee https://github.com/minQie/tangjia.git
git remote add origin_github https://gitee.com/minQie/tangjia.git
```

### SSH

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

仓库是别人的，但你的账号具有相应的权限就没问题（假设 A 和 B 账户都具有 c 仓库的权限）
git@A.github.com:C/c.git
git@B.github.com:C/c.git
```

还有一点需要注意的，就是上例中，创建了两对密钥，你可能想，本地一个密钥对，本地留着密钥就行，然后将这个密钥对应的公钥配置到各个平台，平台的各个账户，不是最简单。实际上，GitHub 就有做唯一校验，所以单一平台的多个账户使用一个密钥对是不现实的

### HTTPS

​	每次重启电脑之后，SourceTree 访问 GitHub 的私有仓库，就显示找不到

- 现象1：把 Windows 系统凭据管理的所有普通凭据删了，操作仓库，重新认证 → 可以访问

- 现象2：把认证管理器存储的信息删除，操作仓库，重新认证 → 可以访问

  git credential-manager uninstall

  git credential-manager install

- 现象总结：浏览器界面得放访问 GitHub，且是相关账号的登录状态；SourceTree 在远程仓库配置了正确的账号信息。满足这两个条件，才能正常拉取、推送代码

- 现象3：在 SourceTree 账户管理界面中将 rankingcong 的账号信息设置为默认 → 可以访问

**测试**

​	为了下面描述简明，定义一个场景 `Clear`：一个本地仓库，仅设置好了对应的远程仓库地址（原来是一个能够正常推送、拉取等操作的本地仓库，系统操作删除了所有的凭据）

- 测试1

  场景：Clear

  操作：控制台执行 `git pull` → 弹出 GitHub 的用户名和密码输入框 → 输入后，正常推送和拉取代码

  现象：凭据信息多出一条 `git@https://github.com`，用户名为 `Personal Access Token`

- 测试2

  场景：Clear

  操作：SourceTree 执行 `git pull` → 同上

- 测试3

  场景：基于上面测试的结果（在 SourceTree 的账户管理中可以看到新增的凭据）

  操作：添加一个账户

  现象：上面新增的凭据信息的用户名才被更新成对应的名称，并且新增了两个凭据 `git@用户名https://github.com` 和 `sourcetree-rest:git@用户名https://github.com` 

- 测试4

  猜测：因为第一步操作新增的凭据，因为名称的优先级问题，导致最上面的测试中，一直是在用另外一个账号进行登录，这当然无法访问

  场景：Clear

  操作：新增一个账户

  现象：上面提到的3个凭据，都出现了

  再测：将名为 `git@https://github.com` 的凭据删除，拉取代码

  现象：弹出 GitHub 的用户名和密码输入框...

- 总结

  结合上面的测试以及 **问题** 的 现象3，暂且得出在 SourceTree 中、以 HTTPS 协议访问 GitHub 的不同仓库，访问那个仓库，就应该将相应的修改默认的凭据

- 参考资料

  原文：https://medium.com/@XHama666/about-git-and-github-permission-ssh-and-https-single-account-and-multiple-accounts-13747ca8764e

  翻译：https://blog.csdn.net/weixin_26722031/article/details/108136838

  ---

- 火花

  可以在远程仓库的 url 上做文章，就是原理和 SSH 中的区分方式一模一样，例

  ```markdown
  - 在 GitHub 平台上的两个账户 A 和 B
  https://github.com/A
  https://github.com/B
  
  - 两个账户都可以访问账户 C 的仓库 c，那么可以通过下边设置远程仓库地址的方式来区分以哪个账户来访问
  https://A@github.com/C/c
  https://B@github.com/C/c
  ```

  本以为 https 协议的仓库因本地多账号的冲突已经解决了，实际并没有，实际还是要访问相应账号时，切换访问 github.com 的默认凭据（在 SourceTree 上时切换默认账号）

## SourceTree

​	SourceTree 本身就是一个 Git GUI，根本还是通过命令来操作的，单独分出一块是因为，SourceTree 是一款比较好用，日常开发用的比较多的工具，有必要好好熟悉一下工具的配置、用法等

**账户管理**

​	方式一：新建一个 Tab 的 ➕ → Remote → 添加一个账户

​	方式二：工具 → 选项 → 验证

**SSH 和 HTTPS**

​	从 SSH 协议说起，就是说通过 SSH 协议访问 GitHub 仓库，走的都是 `.ssh/config` 那一套，所以无论是使用命令还是 SourceTree 效果是一模一样的

​	然后使用的是 HTTPS 协议，参考该[官方文档](https://docs.github.com/en/free-pro-team@latest/github/using-git/caching-your-github-credentials-in-git)，就可以知道 Git 是有提供 `credential helper` 认证信息缓存的相关概念的，SourceTree 中这块内容叫账户。但是，最终它们在 Windows 操作系统上，都是通过凭据管理来体现的

## 其他

1. GitHub accesstoken 的认证方式访问仓库

   其实概念很简单，这里边就是 OAuth 的概念，token 唯一标识账户，且包含着加密信息

   并且同标识本地以哪个账号仓库的方式一样，只要将 token 放到仓库地址的前边就可以了

6. git 有个特殊的配置项

   ```
   就是说，配置了 url.xxx1.insteadof=xxx2，当你访问 xxx2 的仓库，实际上回去访问 xxx1
   
   # 例1
   url.https://rankingcong:2fb2cb12e6945b548a234e2a35cbc88d7e0b3d8f@github.com/shanghairanking.insteadof=https://github.com/Xxx
   
   # 例2
   url.https://minQie@github.com/minQie.insteadof=https://github.com/minQie
   
   配置了，仓库地址将直接被替换，都无法修改的那种（修改了，就又被替换了）
   ```

7. 补个现象提示，就是说你的 github 账户针对某个仓库没有 write 权限时，你推送分支，会得到仓库没找到的错误...