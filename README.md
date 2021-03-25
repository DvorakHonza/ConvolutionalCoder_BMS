# Convolutional coder/decoder

Encodes/Decodes input string into convolutional code with coding rate 1/2. Degree of the coder can bet set to desired value along with upper and lower feedback. Feedback values are input as integers whose bit representation corresponds to feedback units that will be taken into account during the encoding and decoding process. For example when upper feedback value will be set to default 53d=110101b then the encoded bit will be determined based on first, third, fifth and sixth feedback unit.

This project was created for the Wireless and Mobile Networks (BMS) course at BUT FIT.

## Files

- bms.cpp
- ConvolutionalCoder.h
- ConovolutionalCoder.cpp
- Makefile

## Usage

```*
./bms {-e|-d} [-X {degree}, -Y {upper}, -Z {lower}]
    -e          Encode string from standard input
    -d          Decode string from standard input
    -X degree   Set number of delay units to degree, default 5
    -Y upper    Set upper feedback to upper, default 53
    -Z lower    Set lower feedback to lower, default 46
    Values degree, upper, lower are expected to be positive integer values.
```

## Examples

```*
# Encoding
$ ./bms -e <<< A
00100111011011100111011011

$ ./bms -e -X 3 -Y 5 -Z 6 <<< A
0010011100000010011100

# Decoding
$ ./bms -d <<< 00100111011011100111011011
A

$ ./bms -d -X 3 -Y 5 -Z 6 <<< 0010011100000010011100
A

```
