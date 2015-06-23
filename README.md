# SimpleDCTMusic
Compression of Music with the DCT, intended for use on embedded architectures.
Music should be 16bit integer little endian, signed stereo audio

### Compiling
```
g++ -o encode enc.c
g++ -o decode dec.c
```

### Usage
Where `input.whatever` is your audio file:

 ```bash
 ffmpeg -i input.whatever -f s16le -acodec pcm_s16le input.raw
 cat input.raw | ./encode > compressed.raw
 # Here you can use the compressed audio, "compressed.raw"
 
 # To decompress it into "processed.wav":
 cat compressed.raw | ./decode > decompressed.raw
 ffmpeg -f u16le -ar 22050 -ac 2 -i decompressed.raw processed.wav

 ```
