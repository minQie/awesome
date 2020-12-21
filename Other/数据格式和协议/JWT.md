# JWT

## 简介

> 官网：https://jwt.io/introduction/

JWT是一个JSON信息传输的开放标准，它可以使用密钥对信息进行数字签名，以确保信息是可验证和可信任的

## 结构

分为三部分：head（头部）、payload（载荷）、signature（签名），以`.`分隔

### head

​	json串，通常由两部分组成：令牌的类型（即JWT）和所使用的签名算法，例如HMAC SHA256或RSA等等，会被**Base64**编码

```
{  "alg": "HS256", "typ": "JWT" }
```

### payload

​	json串，存放有效信息的地方，它可以存放JWT提供的现成字段，会被**Base64**编码

```
{
  "iss": "www.baidu.com",
  "sub": "you",
  "aud": "me",
  "name": "456",
  "admin": true,
  "iat": 1584091337,
  "exp": 1784091337,
}
```

- 说明

  | 字段名         | 说明                               |
  | -------------- | ---------------------------------- |
  | iss            | 该JWT的签发者                      |
  | sub            | 该JWT所面向的用户                  |
  | aud            | 接收该JWT的一方                    |
  | exp(expires)   | 什么时候过期，这里是一个Unix时间戳 |
  | iat(issued at) | 在什么时候签发的                   |

### signature

​	用于防止JWT内容被篡改。将上面的两个编码后的字符串都用英文句号，连接在一起（头部在前）就形成了。

连接在一起后还需要使用header中声明签名算法进行签名，使用HMAC SHA256签名如下

```
HMACSHA256( base64UrlEncode(header) + "." + base64UrlEncode(payload), secret)
```

上面自定义的`secret`就是JWT的第三部分`signature`

## 认证流程

用户先通过关键的 **用户名** 和 **密码** 请求到**JWT**，之后调用其他接口只要带上**JWT**，就可以正常访问了

### 应用场景

- 授权
- 信息交换

（其实叶飞的mes就是按照这个思想，这个要对比传统的服务端Session原理（Cookie：Jessionid）才能体现其优异性）