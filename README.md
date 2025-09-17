# webserv

## Notes

- What does `poll()` do? What are the alturnatives?

- CGI (Common Gateway Interface) -> run external programs e.g. Python.

- Introduction to [RFCs](https://www.ietf.org/process/rfcs/#introduction)

- For MacOS use `fcntl()` instead of `write()`

---

<br/>



## Running Tests

This project uses [doctest](https://github.com/doctest/doctest) for unit testing. The tests are located under the `tests/` directory and can be built/run using the Makefile.

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


## Testing with `curl`

```bash
curl -v -X POST "http://127.0.0.1:<PORT>/submit" \
  -H "Content-Length: 26" \
  --data "name=Alice&age=30&city=AMS"
```

---

<br/>

