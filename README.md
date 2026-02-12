## meta:
name: stormwater-drone  
date: 02-10-2026  
auth: wrental  
desc: vague pseudocode for drone/ctrlr process  

## general file structure:
```
.  
├── main (application)  
│   ├── stormwater_drone.c (app src)  
│   └── stormwater_drone.h (app hdr)  
├── components (app dependencies - per dir: 'CMakeLists.txt' 'user_component.c' 'user_component.h')  
│   └── lr1121_config  
├── managed_components (idf-component-registry dependencies, maintained by idf.py)  
│   └── waveshare__esp_lora_1121  
└── README.md  
```

## setup:
hardware:
- waveshare esp32-s3-tiny
- waveshare core1121XF

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
    - setTx()
    - status per <control>
    - data packet with avgData
    - fallback (Rx)


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
    - setTx()
    - send packet
    - setRx()
    - waitForFetch()
    - updateData()
    - setTx()

### current iteration (expected):
hardware features:
- Li-ion >> 5v BMS
- replacable battery, no charging
- bnc connectors for sensors, antenna
- other(?) connector for pump, spool
    - *spool requires 4x: +, -, D, FB

software features:
- read sensors/spool position
- control spool, pump
- send/receive controls, data packets
- data/status readout via connected terminal

### current iteration (hopeful):
hardware features:
- local, usb or sd card save (external access)
- LCD or other isolated readout (no PC connection)
- 2-pos switches for pump (Off/on), spool(Up/down)
- button for save

software features:
- save data snapshot

### dream implementation:
hardware:
- custom pcb:
    - ESP32-S3-WROOM-N16R8
    - SEMTECH LR1121
- waterproof 4/5pin connectors for all perifs:
    - +/-, D+/D- - software selectable 3v3 or 5v per I/O
- waterproof bnc for antenna
- waterproof usb-c for charging, but maintain replacable battery
- waterproof enclosure via molding/gaskets
