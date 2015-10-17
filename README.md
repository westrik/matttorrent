# matttorrent

[![Build
Status](https://travis-ci.org/mattwestrik/matttorrent.svg?branch=master)](https://travis-ci.org/mattwestrik/matttorrent)

## About

`matttorrent` is a rudimentary BitTorrent client I'm writing to get better at
systems code (multi-threading, event-driven programming, networking, etc.).

Tested on OS X 10.11 and Ubuntu 12.04.


## Build

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
./matttorrent data/test.torrent
````


## Task list

- [ ] dictionary ADT for bencoded data
- [ ] parse bencode
- [ ] parse bencode file
- [ ] connect to tracker, parse bencoded response
- [ ] connect to peers
  - [ ] handshake 
- [ ] message passing
  - [ ] bitfield, have
  - [ ] choke/unchoke
  - [ ] interested/uninterested
  - [ ] request
  - [ ] piece
  - [ ] optional: cancel, port
- [ ] write to file
  - [ ] when whole file is downloaded
  - [ ] after each piece is verified

