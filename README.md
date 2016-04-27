# matttorrent

[![Build
Status](https://travis-ci.org/westrik/matttorrent.svg?branch=master)](https://travis-ci.org/westrik/matttorrent)

## About

`matttorrent` is a rudimentary BitTorrent client I'm working on to get better at 
writing systems code (multi-threading, event-driven programming, networking,
etc.).

Not currently functional.
Testing on OS X 10.11 and Ubuntu 12.04.


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

- [x] dictionary ADT for bencoded data
- [ ] parse bencode
- [ ] parse torrent file
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

