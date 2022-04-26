---
title: include/internet-checksum.h


---

# include/internet-checksum.h












## Functions

|                | Name           |
| -------------- | -------------- |
| uint16_t | **[InternetChecksum](https://github.com/devel0/iot-serial-intercomm/tree/main/data/api/Files/internet-checksum_8h.md#function-internetchecksum)**(const uint8_t * data, uint16_t len) <br>Compute Internet checksum of the given buffer for "len bytes. [http://www.faqs.org/rfcs/rfc1071.html](http://www.faqs.org/rfcs/rfc1071.html) (4.1)  |








## Functions Documentation

### function InternetChecksum

```cpp
uint16_t InternetChecksum(
    const uint8_t * data,
    uint16_t len
)
```

Compute Internet checksum of the given buffer for "len bytes. [http://www.faqs.org/rfcs/rfc1071.html](http://www.faqs.org/rfcs/rfc1071.html) (4.1) 
































## Source code

```cpp
#ifndef _INTENET_CHECKSUM_H
#define _INTENET_CHECKSUM_H

#include <Arduino.h>

uint16_t InternetChecksum(const uint8_t *data, uint16_t len);

#endif
```


-------------------------------


