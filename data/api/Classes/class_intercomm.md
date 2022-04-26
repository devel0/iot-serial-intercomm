---
title: Intercomm


---

# Intercomm






`#include <intercomm.h>`















## Public Functions

|                | Name           |
| -------------- | -------------- |
|  | **[Intercomm](https://github.com/devel0/iot-serial-intercomm/tree/main/data/api/Classes/class_intercomm.md#function-intercomm)**(HardwareSerial & _serial, unsigned long _speed, [InterCommRxDataCallback](https://github.com/devel0/iot-serial-intercomm/tree/main/data/api/Files/intercomm_8h.md#typedef-intercommrxdatacallback) _rxCallback, int _max_data_len =256)  |
|  | **[~Intercomm](https://github.com/devel0/iot-serial-intercomm/tree/main/data/api/Classes/class_intercomm.md#function-~intercomm)**()  |
| void | **[setup](https://github.com/devel0/iot-serial-intercomm/tree/main/data/api/Classes/class_intercomm.md#function-setup)**()  |
| void | **[send](https://github.com/devel0/iot-serial-intercomm/tree/main/data/api/Classes/class_intercomm.md#function-send)**(const uint8_t * tx_data_buf, int len)  |
| bool | **[ackReceived](https://github.com/devel0/iot-serial-intercomm/tree/main/data/api/Classes/class_intercomm.md#function-ackreceived)**() <br>after a send, place a "while (!ic.ackReceived()) { ... }"  |
| void | **[loop](https://github.com/devel0/iot-serial-intercomm/tree/main/data/api/Classes/class_intercomm.md#function-loop)**()  |
| uint32_t | **[getInvalidCrcTotal](https://github.com/devel0/iot-serial-intercomm/tree/main/data/api/Classes/class_intercomm.md#function-getinvalidcrctotal)**() const  |
| uint32_t | **[getRetransmissionTotal](https://github.com/devel0/iot-serial-intercomm/tree/main/data/api/Classes/class_intercomm.md#function-getretransmissiontotal)**() const  |





















## Public Functions Documentation

### function Intercomm

```cpp
Intercomm(
    HardwareSerial & _serial,
    unsigned long _speed,
    InterCommRxDataCallback _rxCallback,
    int _max_data_len =256
)
```





























### function ~Intercomm

```cpp
~Intercomm()
```





























### function setup

```cpp
void setup()
```





























### function send

```cpp
void send(
    const uint8_t * tx_data_buf,
    int len
)
```





























### function ackReceived

```cpp
bool ackReceived()
```

after a send, place a "while (!ic.ackReceived()) { ... }" 




























### function loop

```cpp
void loop()
```





























### function getInvalidCrcTotal

```cpp
inline uint32_t getInvalidCrcTotal() const
```





























### function getRetransmissionTotal

```cpp
inline uint32_t getRetransmissionTotal() const
```





































-------------------------------

