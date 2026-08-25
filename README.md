# webserv

A lightweight HTTP/1.1 server written in C++98 — 42 school project.

---

## Requirements

- `c++` (g++ / clang++)
- `make`
- A configuration file (`.conf`)

---

## Build

```bash
make        # compile
make re     # clean rebuild
make fclean # remove binaries and objects
```

---

## Run

```bash
./webserv [config_file]
```

**Example:**
```bash
./webserv config/default.conf
```

> Without argument, the server looks for a default config.

---

## Debug

```bash
make r      # fclean → rebuild → run with valgrind
```

---

## Config

The config file controls:

- `listen` — port and host
- `server_name` — virtual host name
- `root` — root directory
- `index` — default index file
- `location` — route-specific rules (methods, redirects, uploads…)
- `error_page` — custom error pages
- `client_max_body_size` — max request body size

**Minimal example:**
```nginx
server {
    listen 8080;
    server_name localhost;
    root ./www;
    index index.html;
    client_max_body_size 1m;

    location / {
        allow_methods GET POST;
    }
}
```

---

## Test

```bash
curl http://localhost:8080/
curl -X POST http://localhost:8080/upload -d "data"
```