# Badge Hack
Hacking the CHV Badge (Raspberry Pi Pico)

## Resources
[Getting Started With Pico](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)
[Pico C/C++ SDK](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)
[Pico MicroPython SDK](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-python-sdk.pdf)
[RP2040 Microcontroller](https://www.raspberrypi.com/documentation/microcontrollers/rp2040.html)
[How To: Buzzer Music with Pico](https://www.tomshardware.com/how-to/buzzer-music-raspberry-pi-pico)
[Raspberry Pi Buzzer Player (many songs)](https://github.com/gumslone/raspi_buzzer_player)

## 7 note Mario - 200

Make the buzzer play the first 7 notes of the Super Mario theme

## LED sync - 150

Make some LEDs sync with the Super Mario. Make it flashy!

## Solution
This code solves both challenges (plays Mario theme, with LEDs in eyes flashing to the music).

```python
#!/usr/bin/env python
from machine import Pin, PWM
from utime import sleep
from random import choice


BUZZER_PIN = 20
buzzer = PWM(Pin(BUZZER_PIN))
volume = 50  # % of max
level = round(5000 * volume / 100)  # volume level for duty cycle

LEYEC_PIN = 26
LEYEC = Pin(LEYEC_PIN, Pin.OUT)
LEYE1_PIN = 22
LEYE1 = Pin(LEYE1_PIN, Pin.OUT)
LEYE2_PIN = 21
LEYE2 = Pin(LEYE2_PIN, Pin.OUT)
LEYE3_PIN = 27
LEYE3 = Pin(LEYE3_PIN, Pin.OUT)

REYEC_PIN = 10
REYEC = Pin(REYEC_PIN, Pin.OUT)
REYE1_PIN = 12
REYE1 = Pin(REYE1_PIN, Pin.OUT)
REYE2_PIN = 13
REYE2 = Pin(REYE2_PIN, Pin.OUT)
REYE3_PIN = 11
REYE3 = Pin(REYE3_PIN, Pin.OUT)

LEYE = [LEYEC, LEYE1, LEYE2, LEYE3]
LEYE_IRIS = [LEYE1, LEYE2, LEYE3]
REYE = [REYE1, REYE1, REYE2, REYE3]
REYE_IRIS = [REYE1, REYE2, REYE3]
PUPILS = [LEYEC, REYEC]
IRISES = LEYE_IRIS + REYE_IRIS
EYES = LEYE + REYE
EYES_GROUPS = [LEYE, REYE, LEYE_IRIS, REYE_IRIS, PUPILS, IRISES, EYES]

notes = {
    "B0": 31,
    "C1": 33,
    "CS1": 35,
    "D1": 37,
    "DS1": 39,
    "EB1": 39,
    "E1": 41,
    "F1": 44,
    "FS1": 46,
    "G1": 49,
    "GS1": 52,
    "A1": 55,
    "AS1": 58,
    "BB1": 58,
    "B1": 62,
    "C2": 65,
    "CS2": 69,
    "D2": 73,
    "DS2": 78,
    "EB2": 78,
    "E2": 82,
    "F2": 87,
    "FS2": 93,
    "G2": 98,
    "GS2": 104,
    "A2": 110,
    "AS2": 117,
    "BB2": 123,
    "B2": 123,
    "C3": 131,
    "CS3": 139,
    "D3": 147,
    "DS3": 156,
    "EB3": 156,
    "E3": 165,
    "F3": 175,
    "FS3": 185,
    "G3": 196,
    "GS3": 208,
    "A3": 220,
    "AS3": 233,
    "BB3": 233,
    "B3": 247,
    "C4": 262,
    "CS4": 277,
    "D4": 294,
    "DS4": 311,
    "EB4": 311,
    "E4": 330,
    "F4": 349,
    "FS4": 370,
    "G4": 392,
    "GS4": 415,
    "A4": 440,
    "AS4": 466,
    "BB4": 466,
    "B4": 494,
    "C5": 523,
    "CS5": 554,
    "D5": 587,
    "DS5": 622,
    "EB5": 622,
    "E5": 659,
    "F5": 698,
    "FS5": 740,
    "G5": 784,
    "GS5": 831,
    "A5": 880,
    "AS5": 932,
    "BB5": 932,
    "B5": 988,
    "C6": 1047,
    "CS6": 1109,
    "D6": 1175,
    "DS6": 1245,
    "EB6": 1245,
    "E6": 1319,
    "F6": 1397,
    "FS6": 1480,
    "G6": 1568,
    "GS6": 1661,
    "A6": 1760,
    "AS6": 1865,
    "BB6": 1865,
    "B6": 1976,
    "C7": 2093,
    "CS7": 2217,
    "D7": 2349,
    "DS7": 2489,
    "EB7": 2489,
    "E7": 2637,
    "F7": 2794,
    "FS7": 2960,
    "G7": 3136,
    "GS7": 3322,
    "A7": 3520,
    "AS7": 3729,
    "BB7": 3729,
    "B7": 3951,
    "C8": 4186,
    "CS8": 4435,
    "D8": 4699,
    "DS8": 4978,
}

melody = [
    notes["E7"],
    notes["E7"],
    0,
    notes["E7"],
    0,
    notes["C7"],
    notes["E7"],
    0,
    notes["G7"],
    0,
    0,
    0,
    notes["G6"],
    0,
    0,
    0,
    notes["C7"],
    0,
    0,
    notes["G6"],
    0,
    0,
    notes["E6"],
    0,
    0,
    notes["A6"],
    0,
    notes["B6"],
    0,
    notes["AS6"],
    notes["A6"],
    0,
    notes["G6"],
    notes["E7"],
    notes["G7"],
    notes["A7"],
    0,
    notes["F7"],
    notes["G7"],
    0,
    notes["E7"],
    0,
    notes["C7"],
    notes["D7"],
    notes["B6"],
    0,
    0,
    notes["C7"],
    0,
    0,
    notes["G6"],
    0,
    0,
    notes["E6"],
    0,
    0,
    notes["A6"],
    0,
    notes["B6"],
    0,
    notes["AS6"],
    notes["A6"],
    0,
    notes["G6"],
    notes["E7"],
    notes["G7"],
    notes["A7"],
    0,
    notes["F7"],
    notes["G7"],
    0,
    notes["E7"],
    0,
    notes["C7"],
    notes["D7"],
    notes["B6"],
    0,
    0,
]

tempo = [
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    9,
    9,
    9,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    9,
    9,
    9,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
]


def toggle_leds(leds):
    for led in leds:
        led.toggle()


def buzz(frequency, length):
    if frequency == 0:
        sleep(length)
        return

    buzzer.freq(frequency)
    buzzer.duty_u16(level)
    leds = choice(EYES_GROUPS)
    toggle_leds(leds)
    sleep(length)
    toggle_leds(leds)
    buzzer.duty_u16(0)


def play(melody, tempo, pause, pace=0.800):
    for i in range(0, len(melody)):  # Play song
        noteDuration = pace / tempo[i]
        buzz(melody[i], noteDuration)  # Change the frequency along the song note

        pauseBetweenNotes = noteDuration * pause
        sleep(pauseBetweenNotes)


def silence():
    buzzer.duty_u16(0)


if __name__ == "__main__":  # Program start from here
    print("Playing Super Mario Brothers")
    try:
        while True:
            play(melody, tempo, 0.30, 1.2000)
            sleep(1)
    except:
        silence()
```