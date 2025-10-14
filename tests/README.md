# Testing Webserv


# Unit Tests

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
<br/>


# Siege

Test webserv under load using `siege`

- **Mac** `brew install siege`
- **Linux** `apt install siege`

```bash
# Uptime
siege -c 50 -t 30s http://127.0.0.1:8080/

# CGI uptime
siege -c 50 -t 30s "http://127.0.0.1:8080/scripts/form.py POST name=homer&msg=doh"
```

---

<br/>
<br/>


# Curl

# 2xx

## Hostname

```bash
curl -i --resolve example.com:8080:127.0.0.1 http://example.com:8080/
curl -i --resolve test.com:8081:127.0.0.1 http://test.com:8081/
```

## Uploads

```bash
# POST → 201
curl -v -X POST http://127.0.0.1:8080/uploads \
  -F "file=@var/www/test_files/document.pdf;type=application/pdf" \
  -F "title=Quarterly Report"
```

```bash
# POST → 201 
curl -v -X POST http://127.0.0.1:8080/uploads \
  -H "Transfer-Encoding: chunked" \
  -F "file=@var/www/test_files/cat.png;type=image/png" \
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

---

<br/>
<br/>


# 3xx

```bash
curl -v http://127.0.0.1:8080/redirect
```

---

<br/>
<br/>


# 4xx

### 400 Bad Request

```bash
# Invalid start line
{ 
  printf 'POST/upload HTTP/1.1\r\n'
  printf 'Host: 127.0.0.1:8080\r\n'
  printf 'Transfer-Encoding: chunked\r\n'
  printf 'Content-Type: text/plain\r\n'
  printf '\r\n'
} | nc 127.0.0.1 8080

# invalid Content-Length
{ printf 'POST / HTTP/1.1\r\nHost: 127.0.0.1:8080\r\nContent-Length: 10\r\n\r\nhi'; } | nc 127.0.0.1 8080

# invalid Content-Length (chunked)
{ printf 'POST /uploads HTTP/1.1\r\nHost: 127.0.0.1:8080\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nABCD\r\n0\r\n\r\n'; } | nc 127.0.0.1 8080

# Duplicate headers
{ printf 'GET / HTTP/1.1\r\nHost: a\r\nHost: b\r\n\r\n'; } | nc 127.0.0.1 8080
```

### 401 Unauthorized

```bash
curl -v http://127.0.0.1:8080/test_files/secret.txt
```

### 403 Forbidden

```bash
curl -v http://127.0.0.1:8080/test_files/secret.txt

# Auto index
# Make 000 folder
curl -v http://127.0.0.1:8080/not_allowd/
```

### 404 Not Found

```bash
curl -v http://127.0.0.1:8080/this_file_not_found.html
```

### 405 Method Not Allowed

```bash
# Illegal PUT
curl -v -X PUT http://127.0.0.1:8080/

# Delete from /scripts
curl -v -X DELETE http://127.0.0.1:8080/scripts/form.py
```

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

# CGI Timeout
curl -X POST \
  -H "Content-Type: application/x-www-form-urlencoded" \
  -d "name=John&age=30" \
  http://127.0.0.1:8080/scripts/loop.py
```

## 411 Length is Required

```bash
{ printf 'POST /uploads HTTP/1.1\r\nHost: 127.0.0.1:8080\r\n\r\nHi'; } | nc 127.0.0.1 8080
```

## 413 Payload Too Large

```bash
# send ~12MiB
head -c $((12*1024*1024)) </dev/zero | tr '\0' 'A' > /tmp/big.bin
curl -v -X POST http://127.0.0.1:8080/test_files --data-binary @/tmp/big.bin \
  -H "Content-Type: application/octet-stream"
```

## 414 URI Too Long

```bash
long=$(printf 'a%.0s' {1..9000})
curl -v "http://127.0.0.1:8080/${long}"
```

---

<br/>
<br/>



# 5xx

## 500

```bash
# Failing CGI process
curl -v http://127.0.0.1:8080/scripts/boom.py
```


<br/>
<br/>
