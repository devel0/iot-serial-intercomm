# iot-serial-intercomm

<!-- TOC -->
* [Description](#description)
* [API](#api)
* [Quickstart](#quickstart)
* [Upload/Debug](#uploaddebug)
<!-- TOCEND -->

<hr/>

## Description

Reliable two host serial communication.

Features:
- send custom data (max len: 65536)
- header and data with internet checksum
- tunable retransmit ( [IC_RESEND_TRIES_MAX, IC_RESEND_INTERVAL_MS](https://github.com/devel0/iot-serial-intercomm/blob/569325662d767cdc149087717fe11196ebdc3700/include/intercomm.h#L42) )

![](data/shot.png)

## API

- [classes](data/api/index_classes.md)

## Quickstart

Install using vscode command palette `PlatformIO: New Terminal` referring to [platformio registry](https://registry.platformio.org/libraries/devel0/iot-serial-intercomm/installation) cli mode doc.

The example01 shows two host implementation ( host A, host B ) corresponding to two nucleo F446RE configured as follow:

- host A
    - uses Serial1 (rx:RB7, tx:PA9) for communication
- host B
    - uses Serial3 (rx:PC11_ALT1, tx:PC10_ALT1) for communication

- each host transmit its [host-protocol.h](https://github.com/devel0/iot-serial-intercomm/blob/569325662d767cdc149087717fe11196ebdc3700/examples/example01/host-protocol.h#L8) info, containing host millis() to other
- each host print received other host protocol info using `rxDataCallback` callback
- after each send [while (!ic.ackReceived()) { ... }](https://github.com/devel0/iot-serial-intercomm/blob/569325662d767cdc149087717fe11196ebdc3700/examples/example01/host-A.cpp#L41) loop ensure packet is acknowledge by counterpart

## Examples

- [hostA example](https://github.com/devel0/iot-serial-intercomm/blob/569325662d767cdc149087717fe11196ebdc3700/examples/example01/host-A.cpp#L19)
- [hostB example](https://github.com/devel0/iot-serial-intercomm/blob/569325662d767cdc149087717fe11196ebdc3700/examples/example01/host-B.cpp#L19)

## Upload/Debug

- to upload two board simultaneosly connected must configure `custom_hla_serial` id in platformio.ini
- select which environment with Platformio: Switch Project Environment
- this allow start two debug session ( use CTRL+ALT+B to ensure rebuild and reload may required ) then hit F5
