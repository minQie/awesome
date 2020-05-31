# Curl

## Windows

- 规则
  1. 不能有单引号
  2. json格式数据中的双引号要加上转义字符

- 示例

  `curl localhost:8080/api/v1/user/auth/login -X POST -d "{\"account\": \"admin\", \"password\": \"you_never_guess_right\", \"loginPlatform\": 1}" --header "Content-Type: application/json"`

## Linux

- 示例

  `curl 'http://localhost:8080/api/v1/user/auth/login' -Method 'POST' -Body '{"account": "admin", "password": "you_never_guess_right", "loginPlatform": 1}' -Headers @{"Content-Type"="application/json"}`