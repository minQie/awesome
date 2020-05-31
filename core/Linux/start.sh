#!/bin/sh
# 开启启动执行的命令

# 开机启动nginx
/usr/local/webserver/nginx/sbin/nginx

# 开机启动ngrok
setsid /usr/ngrok/ngrok/bin/ngrokd \
 -tlsKey=/opt/cert/ngrok_wangmincong_xyz/ssl.key \
 -tlsCrt=/opt/cert/ngrok_wangmincong_xyz/ssl.pem \
 -domain=ngrok.wangmincong.xyz \
 -httpAddr=127.0.0.1:80 \
 -httpsAddr=127.0.0.1:443 \
 -tunnelAddr=:4443 \
 -log="/usr/ngrok/log/ngrok.log"
