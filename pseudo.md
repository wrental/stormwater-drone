## meta:

name: pseudo.txt

date: 02-10-2026

auth: wrental

desc: vague pseudocode for drone process

## files:

- v2-stormwater-drone.c
- v2-stormwater-ctrlr.c


## setup:

hardware:
- waveshare esp32-s3-tiny
- waveshare core1121HF

software:
- main control from ctrlr
- send to/fetch from drone - not send-wait/send-wait
- allows for better instruction timeout control
- allows for consistent control sending 
- if fail on fetch, send another control packet and ask for fetch again

### drone:

core0:

setup()
- spi bus/lora config
- gpio
- start core1 loop

loop():
- readTemp()
- readDO2()
- readPH()
- calcDO2()
- calcPH()
- addData()
- if loop % 5 == 0:
    - avgData()
    - addDataAvg()
    - clearCurrentData()
    - resetLoop()


core1 (loop):
- readIncoming()
- if <control> != 0 {actuate(control, value)};
    - pump: 0,1
    - spool: 0,1
    - fetch: 0,1 (should always be 1 >>> send response)
- sendData():
    - status per <control>
    - data packet with avgData


### ctrlr:

core0:

setup()
- spi bus/lora config
- gpio
- start core1 loop

loop():
- readInputs()
    - switches: pump, spool, fetch
    - button: saveData - handling for single press
        - saveData()
    - save states to array
- refreshDisplay()

core1 (loop):
- sendPacket()
- waitForFetch()
    - updateData()
