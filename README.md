# QT Basic chat
[![Build Status](https://travis-ci.com/Toliak/QT-Chat.svg?branch=master)](https://travis-ci.com/Toliak/QT-Chat)
[![Build status](https://ci.appveyor.com/api/projects/status/ebju131fpw2ykhf3/branch/master?svg=true)](https://ci.appveyor.com/project/Toliak/bmstu-al-hw5/branch/master)

QT Chat client and server. Using websocket protocol

# Compilation

## Dependencies

* QT5 dev packages (base and websocket)
* cmake version greater 3.4

## Windows

```ShellSession
cmake -H. -B cmake-build-debug
cmake --build cmake-build-debug
```

The Visual Studio solution will be created

## Linux

```ShellSession
cmake -H. -B cmake-build-debug
cmake --build cmake-build-debug
```

The binaries will be created

## Docker (server only)

```ShellSession
docker build . --tag qt_chat
docker run --rm -itd -p 8880:8880 \
                     -v $(pwd)/config:/home/qt_chat/config \
                     -u $(id -u):$(id -g) \
                     qt_chat
```