# Honda Hack
Challenges related to the recent Honda keyfob replay attack.

The car had a big sheet of paper with a QR code on it. I scanned the QR code, and it brought me to a Github page with Flipper Zero firmware that can perform a replay attack to unlock the doors:

[FlipperZero Honda Hack Firmware](https://github.com/nonamecoder/FlipperZeroHondaFirmware)

## What is the Honda Keyfob’s FCC ID? - 150

Website for flipper zero firmware says the FCC ID is:

```
KR5V2X
```


## What frequency does the key fob operate on? - 200
HINT: Check the available  frequencies on Flipper with custom firmware

From the same firmware page:

```
433.65
```