# matttorrent

[![Build
Status](https://travis-ci.org/westrik/matttorrent.svg?branch=master)](https://travis-ci.org/westrik/matttorrent)

## About

`matttorrent` is a simple BitTorrent client.
Not working yet.


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
- [x] parse bencode
- [x] parse torrent file
- [x] connect to tracker, parse bencoded response
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

