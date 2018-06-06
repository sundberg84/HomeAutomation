EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:mysensors_arduino
LIBS:mysensors_connectors
LIBS:mysensors_logic
LIBS:mysensors_mcu
LIBS:mysensors_memories
LIBS:mysensors_network
LIBS:mysensors_radios
LIBS:mysensors_regulators
LIBS:mysensors_security
LIBS:mysensors_sensors
LIBS:3p5mmstereo
LIBS:3res
LIBS:5pinheaderSmd
LIBS:8res
LIBS:74hct595
LIBS:74vhc595
LIBS:abracon
LIBS:alps
LIBS:analogdevices
LIBS:atmega16u4
LIBS:avago
LIBS:avrisp
LIBS:az431l
LIBS:bq24230
LIBS:br-106
LIBS:burrbrown
LIBS:buspirate
LIBS:cd74hct165
LIBS:cts
LIBS:dip_switch_4
LIBS:dip_switch_8
LIBS:edac
LIBS:elecret
LIBS:enc28j60
LIBS:esp
LIBS:fairchild
LIBS:fan
LIBS:ft232rl
LIBS:ftdi
LIBS:gdm1602k
LIBS:head156
LIBS:heatsink
LIBS:HP
LIBS:ina126
LIBS:innoline
LIBS:linear_tech
LIBS:lite-on
LIBS:lm285
LIBS:lm1875
LIBS:lm3940
LIBS:lm4040
LIBS:ltp-587hr
LIBS:max6651
LIBS:max9812
LIBS:max9814
LIBS:maxim
LIBS:mbt3946dw1t1
LIBS:mc14016b
LIBS:mcp1703a
LIBS:mcp1825
LIBS:mcp23008
LIBS:Mic
LIBS:micrel
LIBS:microchip_dac
LIBS:microchip_eth
LIBS:microchip_id
LIBS:microchip_pot
LIBS:microsd
LIBS:miniusb
LIBS:mm232r
LIBS:mma8452q
LIBS:mpx4250
LIBS:nkk_switches
LIBS:nl4md
LIBS:nordic_semi
LIBS:nxp
LIBS:on
LIBS:optek
LIBS:PA6B
LIBS:PA6C
LIBS:qdsp-6064
LIBS:relay_spdt
LIBS:relay_spdt_g5la
LIBS:rfm22
LIBS:rgb_led
LIBS:silicon_labs
LIBS:sn75240
LIBS:st
LIBS:stellaris
LIBS:SundbergKiCadLib
LIBS:switchcraft
LIBS:switches
LIBS:tc4469
LIBS:texas_counters
LIBS:texas_opamps
LIBS:texas_other
LIBS:texas_shiftreg
LIBS:tps2044bd
LIBS:tps61202
LIBS:tps63001
LIBS:tusb2046b
LIBS:ua7805
LIBS:usb_a
LIBS:usr-tcp232
LIBS:vishay
LIBS:xrp7620
LIBS:antenna
LIBS:rf_antenna
LIBS:rf_lna
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "EASY/Newbie PCB RFM69 H/HW edition"
Date "2018-02-02"
Rev "Rev 10 (RFM69 ed)"
Comp "MySensors.org, Openhardware.io"
Comment1 "By Andreas Sundberg"
Comment2 "License: CERN Open Hardware License"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MYSX_2.4 P2
U 1 1 5913589B
P 6560 3920
F 0 "P2" H 6810 4020 50  0000 C CNN
F 1 "MYSX_2.4" H 7110 4020 50  0000 C CNN
F 2 "mysensors_connectors:MYSX_2.4" H 6560 2720 60  0001 C CNN
F 3 "" H 6560 2720 60  0000 C CNN
	1    6560 3920
	0    -1   -1   0   
$EndComp
Text GLabel 6560 4070 3    60   Input ~ 0
RAW
$Comp
L +3.3V #PWR07
U 1 1 59135DCD
P 6560 2720
F 0 "#PWR07" H 6560 2570 50  0001 C CNN
F 1 "+3.3V" H 6560 2860 50  0000 C CNN
F 2 "" H 6560 2720 50  0000 C CNN
F 3 "" H 6560 2720 50  0000 C CNN
	1    6560 2720
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 59135E66
P 6660 2420
F 0 "#PWR08" H 6660 2170 50  0001 C CNN
F 1 "GND" H 6660 2270 50  0000 C CNN
F 2 "" H 6660 2420 50  0000 C CNN
F 3 "" H 6660 2420 50  0000 C CNN
	1    6660 2420
	-1   0    0    1   
$EndComp
$Comp
L +3.3V #PWR09
U 1 1 59135EA1
P 6660 4470
F 0 "#PWR09" H 6660 4320 50  0001 C CNN
F 1 "+3.3V" H 6660 4610 50  0000 C CNN
F 2 "" H 6660 4470 50  0000 C CNN
F 3 "" H 6660 4470 50  0000 C CNN
	1    6660 4470
	-1   0    0    1   
$EndComp
Text GLabel 6760 4070 3    60   Input ~ 0
TX
Text GLabel 6760 2670 1    60   Input ~ 0
RX
$Comp
L GND #PWR010
U 1 1 591365EA
P 6860 2370
F 0 "#PWR010" H 6860 2120 50  0001 C CNN
F 1 "GND" H 6860 2220 50  0000 C CNN
F 2 "" H 6860 2370 50  0000 C CNN
F 3 "" H 6860 2370 50  0000 C CNN
	1    6860 2370
	-1   0    0    1   
$EndComp
Text GLabel 6960 4070 3    60   Input ~ 0
D3
Text GLabel 6960 2670 1    60   Input ~ 0
D2
Text GLabel 6860 4320 3    60   Input ~ 0
Reset
Text GLabel 7060 4470 3    60   Input ~ 0
D5
Text GLabel 7060 2370 1    60   Input ~ 0
D6
Text GLabel 7160 4070 3    60   Input ~ 0
A5
Text GLabel 7160 2670 1    60   Input ~ 0
A4
Text GLabel 7260 4470 3    60   Input ~ 0
A0
Text GLabel 7260 2370 1    60   Input ~ 0
A1
Text Notes 6135 1925 0    60   ~ 0
MysX 2,4
Wire Wire Line
	6560 3920 6560 4070
Wire Wire Line
	6560 2720 6560 2870
Wire Wire Line
	6660 2870 6660 2420
Wire Wire Line
	6660 4470 6660 3920
Wire Wire Line
	6760 3920 6760 4070
Wire Wire Line
	6760 2870 6760 2670
Wire Wire Line
	6860 2870 6860 2370
Wire Wire Line
	6960 3920 6960 4070
Wire Wire Line
	7060 3920 7060 4470
Wire Wire Line
	7160 3920 7160 4070
Wire Wire Line
	7260 3920 7260 4470
Wire Wire Line
	7060 2870 7060 2370
Wire Wire Line
	7160 2870 7160 2670
Wire Wire Line
	7260 2870 7260 2370
Wire Wire Line
	6960 2870 6960 2670
Wire Wire Line
	6860 3920 6860 4320
$Comp
L CONN_01X01 P12
U 1 1 5A95BD72
P 4060 3325
F 0 "P12" H 4060 3425 50  0000 C CNN
F 1 "Mount_Hole" V 4160 3325 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm" H 4060 3325 50  0001 C CNN
F 3 "" H 4060 3325 50  0000 C CNN
	1    4060 3325
	0    -1   -1   0   
$EndComp
Wire Notes Line
	3090 3085 3090 2975
$Comp
L CONN_01X01 P3
U 1 1 5A888A06
P 3375 3670
F 0 "P3" H 3375 3770 50  0000 C CNN
F 1 "Mount_Hole" V 3475 3670 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm" H 3375 3670 50  0001 C CNN
F 3 "" H 3375 3670 50  0000 C CNN
	1    3375 3670
	0    1    1    0   
$EndComp
$Comp
L CONN_01X01 P10
U 1 1 5A88896A
P 3605 3310
F 0 "P10" H 3605 3410 50  0000 C CNN
F 1 "Mount_Hole" V 3705 3310 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm" H 3605 3310 50  0001 C CNN
F 3 "" H 3605 3310 50  0000 C CNN
	1    3605 3310
	0    -1   -1   0   
$EndComp
$Comp
L CONN_01X01 P11
U 1 1 5A88865D
P 3900 3720
F 0 "P11" H 3900 3820 50  0000 C CNN
F 1 "Mount_Hole" V 4000 3720 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm" H 3900 3720 50  0001 C CNN
F 3 "" H 3900 3720 50  0000 C CNN
	1    3900 3720
	0    1    1    0   
$EndComp
Text Notes 3110 3080 0    60   ~ 0
Mounting Holes\n
$EndSCHEMATC
