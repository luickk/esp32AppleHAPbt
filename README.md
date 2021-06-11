# esp32 Bluetooth Apple HAP

The projects is a component for the ESP-IDF framework which contains the necessary build tools and libs to compile a ready to deploy binary to flash onto the esp32.
With the component installed and setup, the project is able to advertise following the [Apple HAP Documentation](https://developer.apple.com/de/support/homekit-accessory-protocol/) procedures.

## Todo

- Implement encryption libs
- Implement pairing procedures
- Implement PDU com
- Implement threading & sync
- Implement accessory framework

At this point the project is only able to advertise but cannot be paired or communicate with a central device

## Installation

To install the esp32AppleHomeHAP component follow the steps below.
If you only want to continue the development the component does not have to be moved to the `esp-idf/components`.

1. `cd esp-idf/`

2. `git clone https://github.com/luickk/esp32AppleHAPbt`

3. `mv esp32AppleHAPbt/components/apple_hap_ble_interface components/apple_hap_ble_interface`
