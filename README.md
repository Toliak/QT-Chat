# QT Basic chat
QT Chat client and server. Using websocket protocol

# Compilation

## Dependencies

* QT5 dev packages (base and websocket)
* cmake version greater 3.4

### Windows

```ShellSession
$ cmake -H. -B cmake-build-debug
$ cmake --build cmake-build-debug
```

The Visual Studio solution will be created

### Linux

```ShellSession
$ cmake -H. -B cmake-build-debug
$ cmake --build cmake-build-debug
```

The binaries will be created

### Docker (server only)

```ShellSession
$ docker build . --tag toliak_chat
$ docker run --rm -it -p 8880:8880 toliak_chat
```