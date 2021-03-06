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
LIBS:EasyPCB_10_RFM69hw-cache
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
L RFM69HW U3
U 1 1 591328E6
P 6025 1995
F 0 "U3" H 5675 2245 40  0000 C CNN
F 1 "RFM69HW" H 6275 1245 40  0000 C CNN
F 2 "mysensors_radios:RFM69HW_SMD_ThroughHole_Handsoldering" H 6025 1995 30  0001 C CIN
F 3 "" H 6025 1995 60  0000 C CNN
	1    6025 1995
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P1
U 1 1 59132D95
P 2750 900
F 0 "P1" H 2750 1100 50  0000 C CNN
F 1 "Power" V 2850 900 50  0000 C CNN
F 2 "ASKicadLib:3_block_Screw_Terminal" H 2750 900 50  0001 C CNN
F 3 "" H 2750 900 50  0000 C CNN
	1    2750 900 
	0    1    -1   0   
$EndComp
$Comp
L C C2
U 1 1 591335B1
P 2350 2500
F 0 "C2" H 2375 2600 50  0000 L CNN
F 1 "10uF" H 2375 2400 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D5.0mm_P2.50mm" H 2388 2350 50  0001 C CNN
F 3 "" H 2350 2500 50  0000 C CNN
	1    2350 2500
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 59133609
P 3200 2550
F 0 "C3" H 3225 2650 50  0000 L CNN
F 1 "0,1uF" H 3225 2450 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D5.0mm_P2.50mm" H 3238 2400 50  0001 C CNN
F 3 "" H 3200 2550 50  0000 C CNN
	1    3200 2550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 59133924
P 2800 3000
F 0 "#PWR01" H 2800 2750 50  0001 C CNN
F 1 "GND" H 2800 2850 50  0000 C CNN
F 2 "" H 2800 3000 50  0000 C CNN
F 3 "" H 2800 3000 50  0000 C CNN
	1    2800 3000
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR02
U 1 1 59133CBE
P 2050 2700
F 0 "#PWR02" H 2050 2550 50  0001 C CNN
F 1 "+3.3V" H 2050 2840 50  0000 C CNN
F 2 "" H 2050 2700 50  0000 C CNN
F 3 "" H 2050 2700 50  0000 C CNN
	1    2050 2700
	-1   0    0    1   
$EndComp
$Comp
L ANT ANT2
U 1 1 591349B5
P 4975 1845
F 0 "ANT2" H 4975 2295 60  0000 C CNN
F 1 "ANTENNA" H 4975 1795 60  0001 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x01_Pitch2.54mm" H 4975 1845 60  0001 C CNN
F 3 "" H 4975 1845 60  0000 C CNN
	1    4975 1845
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 59135147
P 1805 6375
F 0 "#PWR03" H 1805 6125 50  0001 C CNN
F 1 "GND" H 1805 6225 50  0000 C CNN
F 2 "" H 1805 6375 50  0000 C CNN
F 3 "" H 1805 6375 50  0000 C CNN
	1    1805 6375
	0    1    1    0   
$EndComp
$Comp
L GND #PWR04
U 1 1 59135208
P 1805 6475
F 0 "#PWR04" H 1805 6225 50  0001 C CNN
F 1 "GND" H 1805 6325 50  0000 C CNN
F 2 "" H 1805 6475 50  0000 C CNN
F 3 "" H 1805 6475 50  0000 C CNN
	1    1805 6475
	0    1    1    0   
$EndComp
$Comp
L GND #PWR05
U 1 1 5913523D
P 1805 6575
F 0 "#PWR05" H 1805 6325 50  0001 C CNN
F 1 "GND" H 1805 6425 50  0000 C CNN
F 2 "" H 1805 6575 50  0000 C CNN
F 3 "" H 1805 6575 50  0000 C CNN
	1    1805 6575
	0    1    1    0   
$EndComp
$Comp
L +3.3V #PWR06
U 1 1 591356A7
P 1805 4275
F 0 "#PWR06" H 1805 4125 50  0001 C CNN
F 1 "+3.3V" H 1805 4415 50  0000 C CNN
F 2 "" H 1805 4275 50  0000 C CNN
F 3 "" H 1805 4275 50  0000 C CNN
	1    1805 4275
	0    -1   -1   0   
$EndComp
$Comp
L MYSX_2.4 P2
U 1 1 5913589B
P 6540 5485
F 0 "P2" H 6790 5585 50  0000 C CNN
F 1 "MYSX_2.4" H 7090 5585 50  0000 C CNN
F 2 "mysensors_connectors:MYSX_2.4" H 6540 4285 60  0001 C CNN
F 3 "" H 6540 4285 60  0000 C CNN
	1    6540 5485
	0    -1   -1   0   
$EndComp
$Comp
L Jumper_NO_Small JP3
U 1 1 59135AB1
P 3100 1650
F 0 "JP3" H 3100 1730 50  0000 C CNN
F 1 "RAW" H 3110 1590 50  0001 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 3100 1650 50  0001 C CNN
F 3 "" H 3100 1650 50  0000 C CNN
	1    3100 1650
	1    0    0    -1  
$EndComp
Text GLabel 6540 5635 3    60   Input ~ 0
RAW
Text GLabel 3350 1650 2    60   Input ~ 0
RAW
$Comp
L +3.3V #PWR07
U 1 1 59135DCD
P 6540 4285
F 0 "#PWR07" H 6540 4135 50  0001 C CNN
F 1 "+3.3V" H 6540 4425 50  0000 C CNN
F 2 "" H 6540 4285 50  0000 C CNN
F 3 "" H 6540 4285 50  0000 C CNN
	1    6540 4285
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 59135E66
P 6640 3985
F 0 "#PWR08" H 6640 3735 50  0001 C CNN
F 1 "GND" H 6640 3835 50  0000 C CNN
F 2 "" H 6640 3985 50  0000 C CNN
F 3 "" H 6640 3985 50  0000 C CNN
	1    6640 3985
	-1   0    0    1   
$EndComp
$Comp
L +3.3V #PWR09
U 1 1 59135EA1
P 6640 6035
F 0 "#PWR09" H 6640 5885 50  0001 C CNN
F 1 "+3.3V" H 6640 6175 50  0000 C CNN
F 2 "" H 6640 6035 50  0000 C CNN
F 3 "" H 6640 6035 50  0000 C CNN
	1    6640 6035
	-1   0    0    1   
$EndComp
Text GLabel 6740 5635 3    60   Input ~ 0
TX
Text GLabel 6740 4235 1    60   Input ~ 0
RX
Text GLabel 3855 4375 2    60   Input ~ 0
TX
Text GLabel 4055 4275 2    60   Input ~ 0
RX
$Comp
L GND #PWR010
U 1 1 591365EA
P 6840 3935
F 0 "#PWR010" H 6840 3685 50  0001 C CNN
F 1 "GND" H 6840 3785 50  0000 C CNN
F 2 "" H 6840 3935 50  0000 C CNN
F 3 "" H 6840 3935 50  0000 C CNN
	1    6840 3935
	-1   0    0    1   
$EndComp
Text GLabel 6940 5635 3    60   Input ~ 0
D3
Text GLabel 6940 4235 1    60   Input ~ 0
D2
Text GLabel 6840 5885 3    60   Input ~ 0
Reset
Text GLabel 7040 6035 3    60   Input ~ 0
D5
Text GLabel 7040 3935 1    60   Input ~ 0
D6
Text GLabel 7140 5635 3    60   Input ~ 0
A5
Text GLabel 7140 4235 1    60   Input ~ 0
A4
Text GLabel 7240 6035 3    60   Input ~ 0
A0
Text GLabel 7240 3935 1    60   Input ~ 0
A1
Text GLabel 3855 5875 2    60   Input ~ 0
A0
Text GLabel 4005 5975 2    60   Input ~ 0
A1
Text GLabel 4005 6375 2    60   Input ~ 0
A5
Text GLabel 3855 6275 2    60   Input ~ 0
A4
Text GLabel 4055 4475 2    60   Input ~ 0
D2
Text GLabel 4205 4575 2    60   Input ~ 0
D3
Text GLabel 3855 4775 2    60   Input ~ 0
D5
Text GLabel 4055 4875 2    60   Input ~ 0
D6
Text GLabel 3855 5575 2    60   Input ~ 0
MISO
Text GLabel 4205 5475 2    60   Input ~ 0
MOSI
Text GLabel 4205 5675 2    60   Input ~ 0
SCK
Text GLabel 3905 5275 2    60   Input ~ 0
ss
Text GLabel 5425 2045 0    60   Input ~ 0
ss
Text GLabel 5275 2145 0    60   Input ~ 0
MOSI
Text GLabel 5425 2245 0    60   Input ~ 0
MISO
Text GLabel 5275 2345 0    60   Input ~ 0
SCK
$Comp
L GND #PWR011
U 1 1 59138581
P 6025 2945
F 0 "#PWR011" H 6025 2695 50  0001 C CNN
F 1 "GND" H 6025 2795 50  0000 C CNN
F 2 "" H 6025 2945 50  0000 C CNN
F 3 "" H 6025 2945 50  0000 C CNN
	1    6025 2945
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR012
U 1 1 591385BC
P 6025 1545
F 0 "#PWR012" H 6025 1395 50  0001 C CNN
F 1 "+3.3V" H 6025 1685 50  0000 C CNN
F 2 "" H 6025 1545 50  0000 C CNN
F 3 "" H 6025 1545 50  0000 C CNN
	1    6025 1545
	1    0    0    -1  
$EndComp
Text GLabel 6675 1945 1    60   Input ~ 0
D2
$Comp
L GND #PWR013
U 1 1 59139799
P 8600 2700
F 0 "#PWR013" H 8600 2450 50  0001 C CNN
F 1 "GND" H 8600 2550 50  0000 C CNN
F 2 "" H 8600 2700 50  0000 C CNN
F 3 "" H 8600 2700 50  0000 C CNN
	1    8600 2700
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR014
U 1 1 591397D7
P 8600 1500
F 0 "#PWR014" H 8600 1350 50  0001 C CNN
F 1 "+3.3V" H 8600 1640 50  0000 C CNN
F 2 "" H 8600 1500 50  0000 C CNN
F 3 "" H 8600 1500 50  0000 C CNN
	1    8600 1500
	1    0    0    -1  
$EndComp
NoConn ~ 3805 4675
NoConn ~ 3805 4975
NoConn ~ 3805 6075
NoConn ~ 3805 6475
NoConn ~ 3805 6575
NoConn ~ 1905 6175
NoConn ~ 1905 4575
NoConn ~ 9100 1550
NoConn ~ 9100 1650
NoConn ~ 9100 1750
NoConn ~ 9100 1850
NoConn ~ 9100 2300
NoConn ~ 9100 2400
NoConn ~ 9100 2500
NoConn ~ 9100 2600
NoConn ~ 9600 2400
NoConn ~ 9600 2300
NoConn ~ 9600 1650
NoConn ~ 9600 1550
NoConn ~ 5475 2545
NoConn ~ 5475 2645
NoConn ~ 6575 2095
NoConn ~ 6575 2195
NoConn ~ 6575 2295
NoConn ~ 6575 2395
NoConn ~ 6575 2495
$Comp
L Jumper_NO_Small JP1
U 1 1 5913D478
P 1950 2050
F 0 "JP1" H 1950 2130 50  0000 C CNN
F 1 "REG" H 1960 1990 50  0001 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 1950 2050 50  0001 C CNN
F 3 "" H 1950 2050 50  0000 C CNN
	1    1950 2050
	0    1    1    0   
$EndComp
Text Notes 1900 2100 1    60   ~ 0
Reg
$Comp
L GND #PWR015
U 1 1 5913DD7F
P 1800 1950
F 0 "#PWR015" H 1800 1700 50  0001 C CNN
F 1 "GND" H 1800 1800 50  0000 C CNN
F 2 "" H 1800 1950 50  0000 C CNN
F 3 "" H 1800 1950 50  0000 C CNN
	1    1800 1950
	-1   0    0    1   
$EndComp
$Comp
L Jumper_NO_Small JP2
U 1 1 5913E327
P 2050 1500
F 0 "JP2" H 2050 1580 50  0000 C CNN
F 1 "BAT" H 2060 1440 50  0001 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 2050 1500 50  0001 C CNN
F 3 "" H 2050 1500 50  0000 C CNN
	1    2050 1500
	-1   0    0    1   
$EndComp
Text Notes 2150 1450 2    60   ~ 0
Bat
$Comp
L C C1
U 1 1 5913E5EC
P 1700 2500
F 0 "C1" H 1725 2600 50  0000 L CNN
F 1 "0,1uF" H 1725 2400 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D5.0mm_P2.50mm" H 1738 2350 50  0001 C CNN
F 3 "" H 1700 2500 50  0000 C CNN
	1    1700 2500
	0    1    1    0   
$EndComp
$Comp
L GND #PWR016
U 1 1 5913E6DD
P 1500 2500
F 0 "#PWR016" H 1500 2250 50  0001 C CNN
F 1 "GND" H 1500 2350 50  0000 C CNN
F 2 "" H 1500 2500 50  0000 C CNN
F 3 "" H 1500 2500 50  0000 C CNN
	1    1500 2500
	0    1    1    0   
$EndComp
$Comp
L R R1
U 1 1 5913FCD1
P 4755 4975
F 0 "R1" V 4835 4975 50  0000 C CNN
F 1 "R" V 4755 4975 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4685 4975 50  0001 C CNN
F 3 "" H 4755 4975 50  0000 C CNN
	1    4755 4975
	0    1    1    0   
$EndComp
$Comp
L +3.3V #PWR017
U 1 1 5913FDF0
P 5005 4975
F 0 "#PWR017" H 5005 4825 50  0001 C CNN
F 1 "+3.3V" H 5005 5115 50  0000 C CNN
F 2 "" H 5005 4975 50  0000 C CNN
F 3 "" H 5005 4975 50  0000 C CNN
	1    5005 4975
	0    1    1    0   
$EndComp
$Comp
L LE33 U2
U 1 1 5914149A
P 2800 2650
F 0 "U2" H 2900 2400 50  0000 R BNN
F 1 "LE33" H 2700 2900 50  0000 C TNN
F 2 "ASKicadLib:TO-92_LE33_Andreas" H 2700 2700 60  0001 C CNN
F 3 "" H 2700 2700 60  0000 C CNN
	1    2800 2650
	0    1    1    0   
$EndComp
$Comp
L DC/DCBooster U1
U 1 1 59141F91
P 1550 2000
F 0 "U1" H 1650 1750 50  0000 R BNN
F 1 "DC/DCBooster" H 1450 2250 50  0000 C TNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm" H 1450 2050 60  0001 C CNN
F 3 "" H 1450 2050 60  0000 C CNN
	1    1550 2000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR018
U 1 1 59142EA5
P 2750 1200
F 0 "#PWR018" H 2750 950 50  0001 C CNN
F 1 "GND" H 2750 1050 50  0000 C CNN
F 2 "" H 2750 1200 50  0000 C CNN
F 3 "" H 2750 1200 50  0000 C CNN
	1    2750 1200
	1    0    0    -1  
$EndComp
Text GLabel 1905 6075 0    60   Input ~ 0
Reset
$Comp
L ArduinoProMini IC1
U 1 1 59132A1C
P 2805 5375
F 0 "IC1" H 2055 6625 40  0000 L BNN
F 1 "ArduinoProMini" H 3205 3975 40  0000 L BNN
F 2 "mysensors_arduino:pro_mini" H 2805 5375 30  0001 C CIN
F 3 "" H 2805 5375 60  0000 C CNN
	1    2805 5375
	1    0    0    -1  
$EndComp
$Comp
L ANTENNA ANT1
U 1 1 59144C3F
P 5225 1245
F 0 "ANT1" V 4975 1195 60  0000 C CNN
F 1 "ANTENNA" V 5075 1295 60  0000 C CNN
F 2 "kicad-footprints:SMA_EDGE" H 5225 1245 60  0001 C CNN
F 3 "" H 5225 1245 60  0001 C CNN
	1    5225 1245
	0    1    1    0   
$EndComp
$Comp
L GND #PWR019
U 1 1 59144F93
P 5425 1645
F 0 "#PWR019" H 5425 1395 50  0001 C CNN
F 1 "GND" H 5425 1495 50  0000 C CNN
F 2 "" H 5425 1645 50  0000 C CNN
F 3 "" H 5425 1645 50  0000 C CNN
	1    5425 1645
	0    -1   -1   0   
$EndComp
$Comp
L CONN_01X04 P4
U 1 1 591458E9
P 8850 1700
F 0 "P4" H 8850 1950 50  0000 C CNN
F 1 "X4" V 8950 1700 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 8850 1700 50  0001 C CNN
F 3 "" H 8850 1700 50  0000 C CNN
	1    8850 1700
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 P5
U 1 1 59145958
P 8850 2450
F 0 "P5" H 8850 2700 50  0000 C CNN
F 1 "X5" V 8950 2450 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 8850 2450 50  0001 C CNN
F 3 "" H 8850 2450 50  0000 C CNN
	1    8850 2450
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 P6
U 1 1 591459AB
P 9300 1700
F 0 "P6" H 9300 1950 50  0000 C CNN
F 1 "X6" V 9400 1700 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 9300 1700 50  0001 C CNN
F 3 "" H 9300 1700 50  0000 C CNN
	1    9300 1700
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 P7
U 1 1 591459FF
P 9300 2450
F 0 "P7" H 9300 2700 50  0000 C CNN
F 1 "X7" V 9400 2450 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 9300 2450 50  0001 C CNN
F 3 "" H 9300 2450 50  0000 C CNN
	1    9300 2450
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 591463EB
P 1600 1300
F 0 "R3" V 1680 1300 50  0000 C CNN
F 1 "1M" V 1600 1300 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 1530 1300 50  0001 C CNN
F 3 "" H 1600 1300 50  0000 C CNN
	1    1600 1300
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 59146692
P 1600 1150
F 0 "R2" V 1680 1150 50  0000 C CNN
F 1 "470k" V 1600 1150 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 1530 1150 50  0001 C CNN
F 3 "" H 1600 1150 50  0000 C CNN
	1    1600 1150
	0    1    1    0   
$EndComp
$Comp
L C C4
U 1 1 5914694A
P 1600 950
F 0 "C4" H 1625 1050 50  0000 L CNN
F 1 "0,1uF" H 1625 850 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D5.0mm_P2.50mm" H 1638 800 50  0001 C CNN
F 3 "" H 1600 950 50  0000 C CNN
	1    1600 950 
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR020
U 1 1 59146E8B
P 1850 1150
F 0 "#PWR020" H 1850 900 50  0001 C CNN
F 1 "GND" H 1850 1000 50  0000 C CNN
F 2 "" H 1850 1150 50  0000 C CNN
F 3 "" H 1850 1150 50  0000 C CNN
	1    1850 1150
	0    -1   -1   0   
$EndComp
Text GLabel 1300 1150 0    60   Input ~ 0
A0
Text Notes 700  650  0    60   ~ 0
Power
Text Notes 700  1050 0    60   ~ 0
Bat. measurer
Text Notes 1000 2250 0    60   ~ 0
Booster\n
Text Notes 2650 2200 0    60   ~ 0
Volt. reg,
Text Notes 6115 3490 0    60   ~ 0
MysX 2,4
Text Notes 7850 650  0    60   ~ 0
Extra (Prototyping)
Text Notes 4015 650  0    60   ~ 0
RFM69 Module
Text Notes 650  3500 0    60   ~ 0
Arduino Pro Mini 3.3v
Text Notes 7855 3540 0    60   ~ 0
Signing - OPTIONAL\nAdv function
$Comp
L ATSHA204A U4
U 1 1 5A74C779
P 8945 4335
F 0 "U4" H 8595 4585 40  0000 C CNN
F 1 "ATSHA204A" H 8795 4085 40  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 8695 4335 30  0001 C CIN
F 3 "" H 8945 4335 60  0000 C CNN
	1    8945 4335
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR021
U 1 1 5A74CDEE
P 8745 3870
F 0 "#PWR021" H 8745 3720 50  0001 C CNN
F 1 "+3.3V" H 8745 4010 50  0000 C CNN
F 2 "" H 8745 3870 50  0000 C CNN
F 3 "" H 8745 3870 50  0000 C CNN
	1    8745 3870
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR022
U 1 1 5A74CE99
P 8745 4855
F 0 "#PWR022" H 8745 4605 50  0001 C CNN
F 1 "GND" H 8745 4705 50  0000 C CNN
F 2 "" H 8745 4855 50  0000 C CNN
F 3 "" H 8745 4855 50  0000 C CNN
	1    8745 4855
	1    0    0    -1  
$EndComp
Text GLabel 8140 4335 0    60   Input ~ 0
A3
$Comp
L C C5
U 1 1 5A74F3A9
P 9225 4310
F 0 "C5" H 9250 4410 50  0000 L CNN
F 1 "0,1uF" H 9250 4210 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 9263 4160 50  0001 C CNN
F 3 "" H 9225 4310 50  0000 C CNN
	1    9225 4310
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 5A74FC35
P 8290 4130
F 0 "R4" V 8370 4130 50  0000 C CNN
F 1 "10k" V 8290 4130 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 8220 4130 50  0001 C CNN
F 3 "" H 8290 4130 50  0000 C CNN
	1    8290 4130
	-1   0    0    1   
$EndComp
Text GLabel 4060 6175 2    60   Input ~ 0
A3
NoConn ~ 3805 5175
Wire Wire Line
	2850 1950 3250 1950
Wire Wire Line
	3250 1950 3250 2250
Wire Wire Line
	3250 2250 2800 2250
Connection ~ 2050 2250
Wire Wire Line
	2650 1800 1950 1800
Wire Wire Line
	3200 2400 3200 2250
Connection ~ 3200 2250
Wire Wire Line
	3200 2900 3200 2700
Wire Wire Line
	5075 1845 5475 1845
Wire Wire Line
	5175 1845 5175 1545
Connection ~ 5175 1845
Wire Wire Line
	1805 6575 1905 6575
Wire Wire Line
	1905 6475 1805 6475
Wire Wire Line
	1805 6375 1905 6375
Wire Wire Line
	1805 4275 1905 4275
Wire Wire Line
	3000 1650 2850 1650
Connection ~ 2850 1650
Wire Wire Line
	3200 1650 3350 1650
Wire Wire Line
	6540 5485 6540 5635
Wire Wire Line
	6540 4285 6540 4435
Wire Wire Line
	6640 4435 6640 3985
Wire Wire Line
	6640 6035 6640 5485
Wire Wire Line
	6740 5485 6740 5635
Wire Wire Line
	6740 4435 6740 4235
Wire Wire Line
	3805 4375 3855 4375
Wire Wire Line
	3805 4275 4055 4275
Wire Wire Line
	6840 4435 6840 3935
Wire Wire Line
	6940 5485 6940 5635
Wire Wire Line
	7040 5485 7040 6035
Wire Wire Line
	7140 5485 7140 5635
Wire Wire Line
	7240 5485 7240 6035
Wire Wire Line
	7040 4435 7040 3935
Wire Wire Line
	7140 4435 7140 4235
Wire Wire Line
	7240 4435 7240 3935
Wire Wire Line
	4005 5975 3805 5975
Wire Wire Line
	3805 5875 3855 5875
Wire Wire Line
	4005 6375 3805 6375
Wire Wire Line
	3805 6275 3855 6275
Wire Wire Line
	4055 4475 3805 4475
Wire Wire Line
	3805 4575 4205 4575
Wire Wire Line
	4055 4875 3805 4875
Wire Wire Line
	3805 4775 3855 4775
Wire Wire Line
	3805 5575 3855 5575
Wire Wire Line
	3805 5675 4205 5675
Wire Wire Line
	4205 5475 3805 5475
Wire Wire Line
	3805 5275 3905 5275
Wire Wire Line
	5475 2045 5425 2045
Wire Wire Line
	5475 2145 5275 2145
Wire Wire Line
	5475 2245 5425 2245
Wire Wire Line
	5475 2345 5275 2345
Wire Wire Line
	5925 2845 5925 2895
Wire Wire Line
	5925 2895 6125 2895
Wire Wire Line
	6125 2895 6125 2845
Wire Wire Line
	6025 2895 6025 2945
Connection ~ 6025 2895
Wire Wire Line
	6025 1545 6025 1645
Wire Wire Line
	6575 1995 6675 1995
Wire Wire Line
	6675 1995 6675 1945
Wire Wire Line
	6940 4435 6940 4235
Wire Wire Line
	8600 2300 8600 2700
Wire Wire Line
	8650 2300 8600 2300
Connection ~ 8600 2300
Wire Wire Line
	8650 2400 8600 2400
Connection ~ 8600 2400
Wire Wire Line
	8650 2500 8600 2500
Connection ~ 8600 2500
Wire Wire Line
	8650 2600 8600 2600
Connection ~ 8600 2600
Wire Wire Line
	8600 1500 8600 1850
Wire Wire Line
	8650 1550 8600 1550
Connection ~ 8600 1550
Wire Wire Line
	8650 1650 8600 1650
Connection ~ 8600 1650
Wire Wire Line
	8650 1750 8600 1750
Connection ~ 8600 1750
Wire Wire Line
	8600 1850 8650 1850
Connection ~ 8600 1850
Wire Wire Line
	1950 1800 1950 1950
Wire Wire Line
	1950 2350 1950 2150
Wire Wire Line
	2050 2250 2050 2700
Connection ~ 2650 1500
Connection ~ 1950 2250
Wire Wire Line
	1950 2250 2550 2250
Wire Wire Line
	1100 1500 1950 1500
Wire Wire Line
	2150 1500 2650 1500
Wire Wire Line
	4105 4725 4355 4725
Wire Wire Line
	4355 4725 4355 4975
Wire Wire Line
	4355 4975 4605 4975
Wire Wire Line
	4905 4975 5005 4975
Wire Wire Line
	2650 1100 2650 1800
Wire Wire Line
	2850 1100 2850 1950
Wire Wire Line
	2550 2250 2550 2550
Wire Wire Line
	2800 2850 2800 3000
Wire Wire Line
	2350 2250 2350 2350
Connection ~ 2350 2250
Wire Wire Line
	2350 2650 2350 2900
Wire Wire Line
	2350 2900 3200 2900
Connection ~ 2800 2900
Wire Wire Line
	1450 2250 1450 2350
Wire Wire Line
	1450 2350 1950 2350
Wire Wire Line
	1750 2000 1800 2000
Wire Wire Line
	1800 2000 1800 1950
Wire Wire Line
	1850 2350 1850 2500
Connection ~ 1850 2350
Wire Wire Line
	1550 2500 1500 2500
Wire Wire Line
	1150 2000 1100 2000
Wire Wire Line
	1100 2000 1100 1500
Wire Wire Line
	2750 1100 2750 1200
Wire Wire Line
	6840 5485 6840 5885
Wire Wire Line
	5275 1545 5275 1645
Wire Wire Line
	5275 1645 5425 1645
Connection ~ 1750 1500
Wire Wire Line
	1400 1300 1450 1300
Wire Wire Line
	1400 950  1400 1300
Wire Wire Line
	1300 1150 1450 1150
Connection ~ 1400 1150
Wire Wire Line
	1400 950  1450 950 
Wire Wire Line
	1750 950  1800 950 
Wire Wire Line
	1750 1150 1850 1150
Wire Wire Line
	1800 950  1800 1150
Connection ~ 1800 1150
Wire Notes Line
	10700 3350 600  3350
Wire Notes Line
	3950 3350 3950 500 
Wire Notes Line
	600  700  10700 700 
Wire Notes Line
	600  500  600  7300
Wire Notes Line
	600  500  10700 500 
Wire Notes Line
	10700 500  10700 6450
Wire Notes Line
	600  7300 6050 7300
Wire Notes Line
	10700 3550 550  3550
Wire Notes Line
	10700 6450 4820 6450
Wire Notes Line
	7800 500  7800 6450
Wire Wire Line
	1750 1300 1890 1300
Wire Wire Line
	1800 1300 1800 1450
Wire Wire Line
	1800 1450 1750 1450
Wire Wire Line
	1750 1450 1750 1500
Wire Notes Line
	6050 7300 6050 6450
Wire Notes Line
	6050 6445 6050 3350
Wire Wire Line
	8140 4335 8395 4335
Wire Wire Line
	8745 3985 8745 3870
Wire Wire Line
	8745 4685 8745 4855
Wire Wire Line
	8290 3925 9225 3925
Wire Wire Line
	9225 3925 9225 4160
Connection ~ 8745 3925
Wire Wire Line
	8745 4770 9225 4770
Wire Wire Line
	9225 4770 9225 4460
Connection ~ 8745 4770
Wire Wire Line
	8290 4280 8290 4335
Connection ~ 8290 4335
Wire Wire Line
	8290 3980 8290 3925
Wire Notes Line
	7795 5120 10695 5120
Wire Notes Line
	10695 5120 10695 5110
Wire Notes Line
	10695 5335 7795 5335
Wire Notes Line
	7795 5335 7795 5330
Wire Wire Line
	4060 6175 3805 6175
Wire Notes Line
	9485 5115 9485 3350
Text GLabel 1890 1300 2    60   Input ~ 0
AdvBat
Connection ~ 1800 1300
Text Notes 7900 5275 0    60   ~ 0
Ext Flash - OPTIONAL/Adv function
Wire Bus Line
	7775 3325 7775 6480
Wire Bus Line
	7775 6480 10720 6480
Wire Bus Line
	10720 6480 10720 3330
Wire Bus Line
	10720 3330 7775 3330
Text Notes 9570 3305 2    60   ~ 0
Optional functions for advanced users
Wire Wire Line
	3830 4775 3830 4695
Wire Wire Line
	3830 4695 4105 4695
Wire Wire Line
	4105 4695 4105 4725
Connection ~ 3830 4775
$Comp
L AT25DF512C U5
U 1 1 5A775000
P 9280 5925
F 0 "U5" H 8930 6175 40  0000 C CNN
F 1 "AT25DF512C" H 9480 5675 40  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 9280 5925 30  0001 C CIN
F 3 "" H 9280 5925 60  0000 C CNN
	1    9280 5925
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR023
U 1 1 5A7750C2
P 9380 6335
F 0 "#PWR023" H 9380 6085 50  0001 C CNN
F 1 "GND" H 9380 6185 50  0000 C CNN
F 2 "" H 9380 6335 50  0000 C CNN
F 3 "" H 9380 6335 50  0000 C CNN
	1    9380 6335
	0    -1   -1   0   
$EndComp
$Comp
L +3.3V #PWR024
U 1 1 5A7752FB
P 9150 5540
F 0 "#PWR024" H 9150 5390 50  0001 C CNN
F 1 "+3.3V" H 9150 5680 50  0000 C CNN
F 2 "" H 9150 5540 50  0000 C CNN
F 3 "" H 9150 5540 50  0000 C CNN
	1    9150 5540
	0    -1   -1   0   
$EndComp
$Comp
L C C7
U 1 1 5A775363
P 10340 5775
F 0 "C7" H 10365 5875 50  0000 L CNN
F 1 "0,1uF" H 10365 5675 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 10378 5625 50  0001 C CNN
F 3 "" H 10340 5775 50  0000 C CNN
	1    10340 5775
	1    0    0    -1  
$EndComp
Wire Wire Line
	9830 5925 10340 5925
Wire Wire Line
	9830 5775 10070 5775
Wire Wire Line
	10070 5775 10070 5925
Connection ~ 10070 5925
Wire Wire Line
	9150 5540 10340 5540
Wire Wire Line
	10340 5540 10340 5625
Wire Wire Line
	9280 5410 9280 5575
Connection ~ 9280 5540
Wire Wire Line
	8475 5775 8730 5775
Wire Wire Line
	8730 5875 8375 5875
Wire Wire Line
	8730 5975 8610 5975
Wire Wire Line
	8725 6075 8375 6075
Text GLabel 8610 5975 0    60   Input ~ 0
MOSI
Text GLabel 8375 5875 0    60   Input ~ 0
MISO
Text GLabel 8375 6075 0    60   Input ~ 0
SCK
$Comp
L R R7
U 1 1 5A77886B
P 8625 5545
F 0 "R7" V 8705 5545 50  0000 C CNN
F 1 "10k" V 8625 5545 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 8555 5545 50  0001 C CNN
F 3 "" H 8625 5545 50  0000 C CNN
	1    8625 5545
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9280 5410 8875 5410
Wire Wire Line
	8875 5410 8875 5545
Wire Wire Line
	8875 5545 8775 5545
Wire Wire Line
	8475 5775 8475 5545
Wire Wire Line
	8475 5665 8380 5665
Connection ~ 8475 5665
Text GLabel 8380 5665 0    60   Input ~ 0
Flash_CS
Text GLabel 3940 5075 2    60   Input ~ 0
Flash_CS
Wire Wire Line
	3805 5075 3940 5075
Wire Wire Line
	9280 6275 9280 6335
Wire Wire Line
	9280 6335 9380 6335
$Comp
L CONN_01X01 P11
U 1 1 5A88865D
P 5625 7065
F 0 "P11" H 5625 7165 50  0000 C CNN
F 1 "Mount_Hole" V 5725 7065 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm" H 5625 7065 50  0001 C CNN
F 3 "" H 5625 7065 50  0000 C CNN
	1    5625 7065
	0    1    1    0   
$EndComp
$Comp
L CONN_01X01 P10
U 1 1 5A88896A
P 5330 6655
F 0 "P10" H 5330 6755 50  0000 C CNN
F 1 "Mount_Hole" V 5430 6655 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm" H 5330 6655 50  0001 C CNN
F 3 "" H 5330 6655 50  0000 C CNN
	1    5330 6655
	0    -1   -1   0   
$EndComp
$Comp
L CONN_01X01 P3
U 1 1 5A888A06
P 5100 7015
F 0 "P3" H 5100 7115 50  0000 C CNN
F 1 "Mount_Hole" V 5200 7015 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm" H 5100 7015 50  0001 C CNN
F 3 "" H 5100 7015 50  0000 C CNN
	1    5100 7015
	0    1    1    0   
$EndComp
Wire Notes Line
	4820 6450 4820 7295
Wire Notes Line
	4815 6430 4815 6320
Wire Notes Line
	4815 6320 6050 6320
Text Notes 4835 6425 0    60   ~ 0
Mounting Holes\n
$Comp
L CONN_01X02 P9
U 1 1 5A889D48
P 9800 2350
F 0 "P9" H 9800 2500 50  0000 C CNN
F 1 "X9" V 9900 2350 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 9800 2350 50  0001 C CNN
F 3 "" H 9800 2350 50  0000 C CNN
	1    9800 2350
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 P8
U 1 1 5A95B1DD
P 9800 1600
F 0 "P8" H 9800 1750 50  0000 C CNN
F 1 "X" V 9900 1600 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 9800 1600 50  0001 C CNN
F 3 "" H 9800 1600 50  0000 C CNN
	1    9800 1600
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X01 P12
U 1 1 5A95BD72
P 5785 6670
F 0 "P12" H 5785 6770 50  0000 C CNN
F 1 "Mount_Hole" V 5885 6670 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.5mm" H 5785 6670 50  0001 C CNN
F 3 "" H 5785 6670 50  0000 C CNN
	1    5785 6670
	0    -1   -1   0   
$EndComp
$EndSCHEMATC
