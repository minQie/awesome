## Jenkins PipeLine

## 官方文档地址

https://www.jenkins.io/zh/doc/book/pipeline/example/#%E5%A3%B0%E6%98%8E%E5%BC%8F%E6%B5%81%E6%B0%B4%E7%BA%BF

## 简介

> 声明：来自该博客：https://blog.csdn.net/u014761412/article/details/106136120

### 概念

1. Jenkins Pipeline是一组插件，让Jenkins可以实现持续交付管道的落地和实施。
2. 持续交付管道(CD Pipeline)是将软件从版本控制阶段到交付给用户或客户的完
   整过程的自动化表现。
3. 软件的每一次更改（提交到源代码管理系统）都要经过一个复杂的过程才能被发布
4. Pipeline提供了一组可扩展的工具，通过Pipeline Domain Specific Language
   (DSL) syntax可以达到Pipeline as Code的目的
5. Pipeline as Code：Jenkinsfile 存储在项目的源代码库

### 为什么使用

1. 本质上，Jenkins 是一个自动化引擎，它支持许多自动模式。 Pipeline向Jenkins中添加了一组强大的工具, 支持用例 简单的CI到全面的CD pipeline。通过对一系列的相关任务进行建模, 用户可以利用pipeline的很多特性

2. 代码

   Pipeline以代码的形式实现，通常被检入源代码控制，使团队能够编辑，审查和迭代其CD流程

3. 可持续性

   Jenkins重启或者中断后都不会影响Pipeline Job

4. 停顿

   Pipeline可以选择停止并等待人工输入或批准，然后再继续Pipeline运行

5. 多功能

   Pipeline支持现实世界的复杂CD要求，包括fork/join子进程，循环和
    并行执行工作的能力

6. 可扩展

   Pipeline插件支持其DSL的自定义扩展以及与其他插件集成的多个选项

## 语法

### 声明式

#### 举例

```Jenkins Pipeline
pipeline {
    agent any
    stages {
        stage("build"){steps(){}}
        stage("test") {steps(){}}
    }
}
```

#### 语法

1. 最外层必须由pipline{ //do something }来进行包裹
2. 不需要分号作为分隔符，每个语句必须在一行内
3. 不能直接使用groovy语句（例如循环判断等），需要被script {}包裹

#### 语法项

##### pipeline（必须）

##### agent（必须）

定义在全局下边（下边都会用全局的来描述定义在pipeline下）

- any：在任何可用的agent 上执行Pipeline或stage
- none：当在pipeline块的顶层使用none时，将不会为整个Pipeline运行分配全局agent ，每个stage部分将需要包含其自己的agent部分
- label：使用提供的label标签，在Jenkins环境中可用的代理上执行Pipeline或stage。例如：agent { label ‘my-defined-label’ }
- node：agent { node { label ‘labelName’ } }，等同于 agent { label ‘labelName’ }，但node允许其他选项（如customWorkspace）
- docker：定义此参数时，执行Pipeline或stage时会动态供应一个docker节点去接受Docker-based的Pipelines。 docker还可以接受一个args，直接传递给docker run调用。例如：agent { docker ‘maven:3-alpine’ }
- dockerfile：使用从Dockerfile源存储库中包含的容器来构建执行Pipeline或stage 。为了使用此选项，Jenkinsfile必须从Multibranch Pipeline或“Pipeline from SCM"加载

##### stages（必须）

​	定义在全局下边

##### stage（必须）

​	定义在stages内部，定义不同的阶段

##### steps（必须）

​	定义在stage内部，每个阶段的最小执行单元

##### post

​	定义在全局下边，？

##### environment

​	定义在全局下边，用于定义局部或者全局变量

##### options

​	定义在全局下边

- buildDiscarder

  指定build history与console的保存数量
  用法：options { buildDiscarder(logRotator(numToKeepStr: ‘1’)) }

- disableConcurrentBuilds

  设置job不能够同时运行
  用法：options { disableConcurrentBuilds() }

- skipDefaultCheckout

  跳过默认设置的代码check out
  用法：options { skipDefaultCheckout() }

- skipStagesAfterUnstable

  一旦构建状态变得UNSTABLE，跳过该阶段
  用法：options { skipStagesAfterUnstable() }

- checkoutToSubdirectory

  在工作空间的子目录进行check out
  用法：options { checkoutToSubdirectory(‘children_path’) }

- timeout

  设置jenkins运行的超时时间，超过超时时间，job会自动被终止
  用法：options { timeout(time: 1, unit: ‘MINUTES’) }

- retry

  设置retry作用域范围的重试次数
  用法：options { retry(3) }

- timestamps

  为控制台输出增加时间戳
  options { timestamps() }

##### parameters

定义在全局下边，只能定一个，用于定义全局的参数

```Jenkins Pipeline
pipeline{
    agent any
    parameters {
        string(name: 'P1', defaultValue: 'it is p1', description: 'it is p1')
        booleanParam(name: 'P2', defaultValue: true, description: 'it is p2')
    }
    stages{
        stage("stage1"){
            steps{
                echo "$P1"
                echo "$P2"
            }
        }
    }
}
```

##### trigger

​	定义在全局下边

- cron

  作用：以指定的时间来运行pipeline

  用法：triggers { cron(’*/1 * * * *’) }

- pollSCM

  作用：以固定的时间检查代码仓库更新（或者当代码仓库有更新时）自动触发pipeline构建

  用法：triggers { pollSCM(‘H */4 * * 1-5’) }或者triggers { pollSCM() }（后者需要配置post-commit/post-receive钩子）

- upstream

  作用：可以利用上游Job的运行状态来进行触发

  用法：triggers { upstream(upstreamProjects: ‘job1,job2’, threshold: hudson.model.Result.SUCCESS) }

##### tools

​	定义在全局下边，用于引用配置好的工具（管理页面的全局工具配置）

	pipeline {
	agent any
	tools {
	    maven 'apache-maven-3.0.1' 
	}
	stages {
	    stage('Example') {
	        steps {sh 'mvn --version'}
	    }
	}
	}
##### input

​	定义下stage下边，input指令允许暂时中断pipeline执行，等待用户输入，根据用户输入进行下一步动作

​	

```
pipeline {
    agent any
    stages {
        stage('Example') {
            input {
                message "Should we continue?"
                ok "Yes, we should."
                submitter "alice,bob"
                parameters {
                    string(name: 'PERSON', defaultValue: 'Mr Jenkins', description: 'Who should I say hello to?')
                }
            }
            steps {
                echo "Hello, ${PERSON}, nice to meet you."
            }
        }
    }
}
```

##### when

​	定义在stage下边，根据when指令的判断结果来决定是否执行后面的阶段

 - branch 

   判断分支名称是否符合预期

   用法：when { branch ‘master’ }

 - environment 

   判断环境变量是否符合预期

   用法：when { environment name: ‘DEPLOY_TO’, value: ‘production’ }

- expression

  判断表达式是否符合预期

  用法：when { expression { return params.DEBUG_BUILD } }

- not

  判断条件是否为假

  用法：when { not { branch ‘master’ } }

- allOf

  判断所有条件是不是都为真

  用法：when { allOf { branch ‘master’; environment name: ‘DEPLOY_TO’, value: ‘production’ } }

- anyOf

  判断是否有一个条件为真

  用法：when { anyOf { branch ‘master’; branch ‘staging’ } }

```
pipeline {
    agent none
    stages {
        stage('Example Build') {
            steps {
                echo 'Hello World'
            }
        }
        stage('Example Deploy') {
            agent {
                label "some-label"
            }
            when {
                // 设置先对条件进行判断，符合预期才进入steps
                // 下面这样的就是beforeAgent的值要为true，branch的值要为production才走下面的steps
                beforeAgent true
                branch 'production'
            }
            steps {
                echo 'Deploying'
            }
        }
    }
}
```

##### script

​	定义在step下边

```
pipeline {
    agent any
    stages {
        stage('stage 1') {
            steps {
                script{
                    if ( "1" =="1" ) {
                        echo "true"
                    } else {
                        echo "false"
                    }
                }
            }
        }
    }
}
```

##### try catch

​	定义在script下边

```
pipeline {
    agent any
    stages {
        stage('stage 1') {
            steps {
                script{
                    try {
                        sh 'exit 1'
                    } catch (e) {
                        echo 'Something wrong when exec script'
                    }
                }
            }
        }
    }
}
```



### 脚本式

#### 举例

```Jenkins Pipeline
node {
    stage("build") {}
    stage("test"){}
}
```

#### 语法

1. 最外层有node{}包裹
2. 可直接使用groovy语句

#### 不举例子了

## 创建项目

- 概念：你可能刚创建项目还在想，在哪里配置项目的git仓库地址，而事实上你需要自己编写

1. 创建类型为：中文-流水线、英文-Pipeline
2. 选择Definition，开始编写脚本

## stage

- 拉取代码

```
stage('下载代码') {
    steps {
        echo 'This is a pull step'
        git credentialsId: '33c35478-9e64-4d2c-bfde-fa77a67237d1', url:
        'http://www.xxx.com:3000/xx/k8s-deploy.git'
    }
}
```

- maven构建

```
stage('编译打包') {
    steps {
        echo 'This is a build step'
        sh"""
        cd $WORKSPACE
        mvn clean package
        """
    }
}
```

- 执行自定义脚本部署更新

```
stage('部署更新') {
    steps {
        // (可能要用script将下面的命令括起来)
        // 删除老jar
        // 复制新jar
        // 执行脚本部署更新
        sh /opt/project/$PROJECT_NAME/project.sh restart $JAR_NAME
    }
}
```

- 发送邮件

```

```

