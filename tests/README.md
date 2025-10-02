# Testing Webserv


### Build and run the unit tests

```bash
# Make test executable `unit_tests`
make test

# List available tests
./unit_tests --list-test-cases

# Run by substring match
./unit_tests -tc=" <Substring of test name> "

# Run single test
./unit_tests --test-case=" <Full name of test> "
```

---

<br/>


# 2xx

## Uploads

```bash
# POST → 201
curl -v -X POST http://127.0.0.1:8080/uploads \
  -F "file=@tests/test_files/document.pdf;type=application/pdf" \
  -F "title=Quarterly Report"
```

```bash
# POST → 201 
curl -v -X POST http://127.0.0.1:8080/uploads \
  -H "Transfer-Encoding: chunked" \
  -F "file=@tests/test_files/cat.png;type=image/png" \
  -F "title=Image_1" \
  --no-buffer

# GET → 200
curl -v http://127.0.0.1:8080/uploads/Image_1.png --output "Image_1.png"

# DELETE → 204
curl -v -X DELETE http://127.0.0.1:8080/uploads/Image_1.png
```

## CGI

```bash
# GET → 200
curl "http://127.0.0.1:8080/scripts/form.py?name=John"

# POST → 201
curl -X POST \
  -H "Content-Type: application/x-www-form-urlencoded" \
  -d "name=John&age=30" \
  http://127.0.0.1:8080/scripts/form.py
```

## Chunking

```bash
# POST → 201
{ printf 'POST /uploads HTTP/1.1\r\n'
  printf 'Host: 127.0.0.1:8080\r\n'
  printf 'Transfer-Encoding: chunked\r\n'
  printf '\r\n'
  printf '9\r\nWikipedia\r\n'
  printf '9\r\nMore text\r\n'
  printf '0\r\n\r\n'
} | nc 127.0.0.1 8080

```

<br/>

---

<br/>
<br/>


# 4xx

### 400 Bad Request


### 401 Unauthorized


### 403 Forbidden


### 404 Not Found


### 405 Method Not Allowed


## 408 Request Time-out

```bash
# Commit to sending large request but only send 2 bytes
curl -v http://127.0.0.1:8080/uploads \
  -H "Content-Length: 1000000" \
  --data-binary @<(printf 'hi')

# Sleep between chunks
{ 
  printf 'POST /upload HTTP/1.1\r\n'
  printf 'Host: 127.0.0.1:8080\r\n'
  printf 'Transfer-Encoding: chunked\r\n'
  printf 'Content-Type: text/plain\r\n'
  printf '\r\n'

  printf '4\r\nWiki\r\n'      # 4-byte chunk
  sleep 12
  printf '5\r\npedia\r\n'     # 5-byte chunk
  sleep 12
  printf '0\r\n\r\n'          # end of chunks
} | nc 127.0.0.1 8080

```


<br/>

---

<br/>
<br/>


# 5xx

### 503 Service Unavailable

```bash

```


<br/>

---
