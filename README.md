# SimpleDCTMusic
Compression of Music with the DCT, intended for use on embedded architectures.
Music should be 16bit integer little endian, stereo

Compile with g++.
Usage:
 cat orig.raw | ./enc > compressed
 cat compressed | ./dec > decompressed.raw