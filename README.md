# matttorrent

[![Build
Status](https://travis-ci.org/westrik/matttorrent.svg?branch=master)](https://travis-ci.org/westrik/matttorrent)

## About

`matttorrent` is a simple event-driven BitTorrent client, implemented in C using
libuv. 


## Build

Install libuv:
````
brew install --HEAD libuv
````

Compile:
````
make
````

Run tests:
````
make test
````

Run:
````
./matttorrent data/ubuntu.torrent
````
