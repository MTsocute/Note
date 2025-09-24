# Nginx

---

```yaml
version: '3.8'

services:
  nginx:
    container_name: my-nginx-vue
    image: nginx:latest
    ports:
      - "7878:80"
    volumes:
      - ./html:/usr/share/nginx/html:ro
      # 如果你有自定义配置，可以加：
      - ./nginx.conf:/etc/nginx/nginx.conf:ro
    restart: always

```

```nginx
user  nginx;
worker_processes  auto;

error_log  /var/log/nginx/error.log warn;
pid        /var/run/nginx.pid;

events {
    worker_connections  1024;
}

http {
    include       /etc/nginx/mime.types;
    default_type  application/octet-stream;

    sendfile        on;
    keepalive_timeout  65;

    server {
        listen       80;
        server_name  localhost;

        # 静态资源路径（对应挂载的 /usr/share/nginx/html）
        location / {
            root   /usr/share/nginx/html;
            index  index.html index.htm;
            try_files $uri $uri/ /index.html;
        }

        # 如果你有后端，比如 Go 的服务跑在宿主机 8080 端口
        # 注意：在 Docker 容器中用 host.docker.internal 访问宿主机
        # location /api/ {
        #     proxy_pass http://host.docker.internal:8080;
        #     proxy_set_header Host $host;
        #     proxy_set_header X-Real-IP $remote_addr;
        #     proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        #     proxy_set_header X-Forwarded-Proto $scheme;
        # }

        error_page 404 /404.html;

        error_page 500 502 503 504 /50x.html;
        location = /50x.html {
            root /usr/share/nginx/html;
        }
    }
}

```

