EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr USLegal 14000 8500
encoding utf-8
Sheet 1 1
Title "Caveatron PCB Rev B.2"
Date "2020-12-09"
Rev "B.2"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Caveatron_Components:AltIMU U3
U 1 1 5D380883
P 6850 750
F 0 "U3" H 7128 346 50  0000 L CNN
F 1 "AltIMU" H 7128 255 50  0000 L CNN
F 2 "Caveatron_Components-Simplified:AltIMU-S" H 6850 750 50  0001 C CNN
F 3 "" H 6850 750 50  0001 C CNN
	1    6850 750 
	1    0    0    -1  
$EndComp
$Comp
L Caveatron_Components:LSM303D U4
U 1 1 5D381B0C
P 6850 1900
F 0 "U4" H 7128 1396 50  0000 L CNN
F 1 "LSM303D" H 7128 1305 50  0000 L CNN
F 2 "Caveatron_Components-Simplified:LSM303D-S" H 6850 1900 50  0001 C CNN
F 3 "" H 6850 1900 50  0001 C CNN
	1    6850 1900
	1    0    0    -1  
$EndComp
$Comp
L Caveatron_Components:Batt_Charger U6
U 1 1 5D382457
P 8150 3150
F 0 "U6" H 8150 3265 50  0000 C CNN
F 1 "Batt_Charger" H 8150 3174 50  0000 C CNN
F 2 "Caveatron_Components-Simplified:Adafruit_Li-Ion_Charger-S" H 8150 3150 50  0001 C CNN
F 3 "" H 8150 3150 50  0001 C CNN
	1    8150 3150
	1    0    0    -1  
$EndComp
$Comp
L Caveatron_Components:FONT_IC U1
U 1 1 5D382DB7
P 2250 2600
F 0 "U1" H 2528 2246 50  0000 L CNN
F 1 "FONT_IC" H 2528 2155 50  0000 L CNN
F 2 "Caveatron Components:FONT_IC" H 2250 2600 50  0001 C CNN
F 3 "" H 2250 2600 50  0001 C CNN
	1    2250 2600
	-1   0    0    1   
$EndComp
$Comp
L Caveatron_Components:Power_Switch U11
U 1 1 5D387C4C
P 9750 3150
F 0 "U11" H 9750 3265 50  0000 C CNN
F 1 "Power_Switch" H 9750 3174 50  0000 C CNN
F 2 "Caveatron_Components-Simplified:Pololu_Pwr_Switch-S" H 9750 3150 50  0001 C CNN
F 3 "" H 9750 3150 50  0001 C CNN
	1    9750 3150
	1    0    0    -1  
$EndComp
$Comp
L SparkFun-Connectors:CONN_02JST-PTH-2 J9
U 1 1 5D380350
P 12150 6450
F 0 "J9" H 11922 6405 45  0000 R CNN
F 1 "CONN_02JST-PTH-2" H 11922 6489 45  0000 R CNN
F 2 "Connector_JST:JST_XH_B2B-XH-A_1x02_P2.50mm_Vertical" H 12150 6750 20  0001 C CNN
F 3 "" H 12150 6450 50  0001 C CNN
F 4 "CONN-09863" H 11922 6584 60  0000 R CNN "Field4"
	1    12150 6450
	-1   0    0    1   
$EndComp
$Comp
L SparkFun-Connectors:CONN_05JST J4
U 1 1 5D37DABE
P 12150 2500
F 0 "J4" H 11922 2605 45  0000 R CNN
F 1 "CONN_05JST" H 11922 2689 45  0000 R CNN
F 2 "Connector_JST:JST_XH_B5B-XH-A_1x05_P2.50mm_Vertical" H 12150 3100 20  0001 C CNN
F 3 "" H 12150 2500 50  0001 C CNN
F 4 "PRT-09917" H 11922 2784 60  0000 R CNN "Field4"
	1    12150 2500
	-1   0    0    1   
$EndComp
$Comp
L SparkFun-Connectors:CONN_05JST J3
U 1 1 5D37D194
P 12150 1750
F 0 "J3" H 11922 1855 45  0000 R CNN
F 1 "CONN_05JST" H 11922 1939 45  0000 R CNN
F 2 "Connector_JST:JST_XH_B5B-XH-A_1x05_P2.50mm_Vertical" H 12150 2350 20  0001 C CNN
F 3 "" H 12150 1750 50  0001 C CNN
F 4 "PRT-09917" H 11922 2034 60  0000 R CNN "Field4"
	1    12150 1750
	-1   0    0    1   
$EndComp
$Comp
L SparkFun-Connectors:CONN_05JST J2
U 1 1 5D37C793
P 12150 1000
F 0 "J2" H 11922 1105 45  0000 R CNN
F 1 "CONN_05JST" H 11922 1189 45  0000 R CNN
F 2 "Connector_JST:JST_XH_B5B-XH-A_1x05_P2.50mm_Vertical" H 12150 1600 20  0001 C CNN
F 3 "" H 12150 1000 50  0001 C CNN
F 4 "PRT-09917" H 11922 1284 60  0000 R CNN "Field4"
	1    12150 1000
	-1   0    0    1   
$EndComp
$Comp
L Caveatron_Components:L3GD20H U5
U 1 1 5D38131B
P 6850 3000
F 0 "U5" H 7128 2496 50  0000 L CNN
F 1 "L3GD20H" H 7128 2405 50  0000 L CNN
F 2 "Caveatron_Components-Simplified:L3GD20H-S" H 6850 3000 50  0001 C CNN
F 3 "" H 6850 3000 50  0001 C CNN
	1    6850 3000
	1    0    0    -1  
$EndComp
Entry Wire Line
	3150 1150 3250 1050
Entry Wire Line
	3150 1250 3250 1150
Entry Wire Line
	3150 1350 3250 1250
Entry Wire Line
	3150 1450 3250 1350
Entry Wire Line
	3150 1550 3250 1450
Entry Wire Line
	3150 1650 3250 1550
Entry Wire Line
	3150 1750 3250 1650
Entry Wire Line
	3150 1850 3250 1750
Entry Wire Line
	3150 1950 3250 1850
Entry Wire Line
	3150 5050 3250 4950
Entry Wire Line
	3150 5250 3250 5150
Entry Wire Line
	3150 2550 3250 2450
Entry Wire Line
	3150 2650 3250 2550
Entry Wire Line
	3150 2750 3250 2650
Entry Wire Line
	3150 2850 3250 2750
Entry Wire Line
	3150 2950 3250 2850
Entry Wire Line
	3150 3050 3250 2950
Entry Wire Line
	3150 3150 3250 3050
Entry Wire Line
	5650 4550 5750 4650
Entry Wire Line
	5650 4650 5750 4750
Entry Wire Line
	5650 4750 5750 4850
Entry Wire Line
	5650 4850 5750 4950
Entry Wire Line
	5650 4950 5750 5050
Entry Wire Line
	5650 5050 5750 5150
Entry Wire Line
	5650 5150 5750 5250
Text Label 3450 1050 2    39   ~ 0
DB4
Text Label 3450 1150 2    39   ~ 0
DB5
Text Label 3450 1250 2    39   ~ 0
DB8
Text Label 3450 1350 2    39   ~ 0
L_CS
Text Label 3450 1450 2    39   ~ 0
RSET
Text Label 3450 1550 2    39   ~ 0
DB15
Text Label 3450 1650 2    39   ~ 0
DB12
Text Label 3450 1750 2    39   ~ 0
DB10
Text Label 3450 1850 2    39   ~ 0
DB11
Text Label 3450 2450 2    39   ~ 0
TCLK
Text Label 3450 2550 2    39   ~ 0
T_CS
Text Label 3450 2650 2    39   ~ 0
TDIN
Text Label 3450 2750 2    39   ~ 0
TDOUT
Text Label 3450 2850 2    39   ~ 0
TIRQ
Text Label 3450 2950 2    39   ~ 0
DB6
Text Label 3450 3050 2    39   ~ 0
DB7
Text Label 3450 4950 2    39   ~ 0
DB9
Text Label 3450 5150 2    39   ~ 0
DB0
Text Label 5450 4550 0    39   ~ 0
RS
Text Label 5450 4650 0    39   ~ 0
WR
Text Label 5450 4750 0    39   ~ 0
DB14
Text Label 5450 4850 0    39   ~ 0
DB13
Text Label 5450 4950 0    39   ~ 0
DB2
Text Label 5450 5050 0    39   ~ 0
DB3
Text Label 5450 5150 0    39   ~ 0
DB1
NoConn ~ 5450 950 
NoConn ~ 5450 1050
NoConn ~ 5450 1150
NoConn ~ 5450 1250
NoConn ~ 5450 1350
NoConn ~ 5450 1450
NoConn ~ 5450 1550
NoConn ~ 5450 2250
NoConn ~ 5450 2350
NoConn ~ 5450 2450
NoConn ~ 5450 2550
NoConn ~ 5450 2650
NoConn ~ 5450 2750
NoConn ~ 5450 2850
NoConn ~ 5450 2950
NoConn ~ 5450 3050
NoConn ~ 5450 3150
NoConn ~ 5450 3550
NoConn ~ 5450 3650
NoConn ~ 5450 3850
NoConn ~ 5450 3950
NoConn ~ 5450 4050
NoConn ~ 5450 4350
NoConn ~ 3450 3650
NoConn ~ 3450 3750
NoConn ~ 5450 3250
NoConn ~ 6400 3650
NoConn ~ 6400 3750
NoConn ~ 6400 3850
NoConn ~ 6400 3950
NoConn ~ 6400 3150
NoConn ~ 6400 1450
NoConn ~ 6400 1350
Text Notes 12050 900  0    59   ~ 0
LRF Connector
Text Notes 12050 1650 0    59   ~ 0
LIDAR Connector
Text Notes 12050 2400 0    59   ~ 0
PWR_SWITCH Connector
$Comp
L Caveatron_Components:Pololu_Adj_Reg U8
U 1 1 5D386E7A
P 9700 1350
F 0 "U8" H 9978 1096 50  0000 L CNN
F 1 "Pololu_Adj_Reg" H 9978 1005 50  0000 L CNN
F 2 "Caveatron Components:Pololu_Adj_Reg" H 9700 1350 50  0001 C CNN
F 3 "" H 9700 1350 50  0001 C CNN
	1    9700 1350
	-1   0    0    1   
$EndComp
$Comp
L Caveatron_Components:Pololu_5V_Reg U10
U 1 1 5D386AB5
P 9700 2550
F 0 "U10" H 9978 2596 50  0000 L CNN
F 1 "Pololu_5V_Reg" H 9978 2505 50  0000 L CNN
F 2 "Caveatron Components:Pololu_5V_Reg" H 9700 2550 50  0001 C CNN
F 3 "" H 9700 2550 50  0001 C CNN
	1    9700 2550
	-1   0    0    1   
$EndComp
NoConn ~ 9300 3700
NoConn ~ 9300 3900
$Comp
L SparkFun-Connectors:CONN_02JST-PTH-2 J5
U 1 1 5D51881E
P 12150 3250
F 0 "J5" H 11922 3205 45  0000 R CNN
F 1 "CONN_02JST-PTH-2" H 11922 3289 45  0000 R CNN
F 2 "Connector_JST:JST_XH_B2B-XH-A_1x02_P2.50mm_Vertical" H 12150 3550 20  0001 C CNN
F 3 "" H 12150 3250 50  0001 C CNN
F 4 "CONN-09863" H 11922 3384 60  0000 R CNN "Field4"
	1    12150 3250
	-1   0    0    1   
$EndComp
Text Notes 12050 3150 0    59   ~ 0
BUZZER Connector
NoConn ~ 6400 2850
NoConn ~ 6400 2750
NoConn ~ 6400 2650
NoConn ~ 6400 2550
NoConn ~ 6400 2050
Text Label 3450 4650 2    39   ~ 0
SYS_OFF
Text Label 3450 4550 2    39   ~ 0
LRF_RST
Text Label 3450 4350 2    39   ~ 0
SDA
Text Label 3450 4250 2    39   ~ 0
SCL
Text Label 3450 4150 2    39   ~ 0
BUZZER
Text Label 3450 4050 2    39   ~ 0
LIDAR_PWM
Text Label 3450 3950 2    39   ~ 0
LIDAR_RX
Text Label 3450 3850 2    39   ~ 0
LIDAR_TX
$Comp
L BS-7:BS-7 BH1
U 1 1 5D3B5507
P 1050 3950
F 0 "BH1" H 1304 3946 50  0000 L CNN
F 1 "BS-7" H 1304 3855 50  0000 L CNN
F 2 "BS-7:BS-7" H 1050 3950 50  0001 L BNN
F 3 "" H 1050 3950 50  0001 L BNN
F 4 "MPD" H 1050 3950 50  0001 L BNN "Field4"
F 5 "Unavailable" H 1050 3950 50  0001 L BNN "Field5"
F 6 "None" H 1050 3950 50  0001 L BNN "Field6"
F 7 "BS-7" H 1050 3950 50  0001 L BNN "Field7"
F 8 "None" H 1050 3950 50  0001 L BNN "Field8"
	1    1050 3950
	1    0    0    -1  
$EndComp
Text Label 3450 3150 2    39   ~ 0
LRF_RX
Text Label 3450 3250 2    39   ~ 0
LRF_TX
Text Label 3450 3350 2    39   ~ 0
RTC_BATT
NoConn ~ 3450 3450
NoConn ~ 5450 3750
$Comp
L Caveatron_Components:Fuel_Gauge U7
U 1 1 5D385FE6
P 8200 4200
F 0 "U7" H 8200 4315 50  0000 C CNN
F 1 "Fuel_Gauge" H 8200 4224 50  0000 C CNN
F 2 "Caveatron_Components-Simplified:Fuel_Gauge-S" H 8200 4200 50  0001 C CNN
F 3 "" H 8200 4200 50  0001 C CNN
	1    8200 4200
	1    0    0    -1  
$EndComp
NoConn ~ 7750 4650
Text Label 3450 5050 2    39   ~ 0
LRF_PWR
Text Label 3450 4450 2    39   ~ 0
LIDAR_PWR
Entry Wire Line
	2150 5200 2250 5100
Entry Wire Line
	2150 4650 2250 4550
Entry Wire Line
	1950 3950 2050 3850
Entry Wire Line
	1950 4050 2050 3950
Entry Wire Line
	1950 4150 2050 4050
Entry Wire Line
	2150 3250 2250 3150
Entry Wire Line
	2150 3350 2250 3250
Entry Wire Line
	1950 4550 2050 4450
Entry Wire Line
	11100 1300 11200 1200
Entry Wire Line
	11100 1400 11200 1300
Entry Wire Line
	11100 1500 11200 1400
Entry Wire Line
	11000 1200 11100 1300
Entry Wire Line
	11300 2050 11400 1950
Entry Wire Line
	11300 2150 11400 2050
Entry Wire Line
	11300 2250 11400 2150
Text Label 12050 1200 2    39   ~ 0
LRF_RX
Text Label 12050 1300 2    39   ~ 0
LRF_TX
Text Label 12050 1400 2    39   ~ 0
LRF_RST
Text Label 12050 1950 2    39   ~ 0
LIDAR_RX
Text Label 12050 2050 2    39   ~ 0
LIDAR_TX
Text Label 12050 2150 2    39   ~ 0
LIDAR_PWM
Text Label 10150 900  0    39   ~ 0
LRF_PWR
Text Label 10150 1750 0    39   ~ 0
LIDAR_PWR
Entry Wire Line
	11600 5100 11700 5000
Entry Wire Line
	11600 5200 11700 5100
$Comp
L SparkFun-Connectors:CONN_02JST-PTH-2 J8
U 1 1 5D38017C
P 12150 6000
F 0 "J8" H 11922 5955 45  0000 R CNN
F 1 "CONN_02JST-PTH-2" H 11922 6039 45  0000 R CNN
F 2 "Connector_JST:JST_XH_B2B-XH-A_1x02_P2.50mm_Vertical" H 12150 6300 20  0001 C CNN
F 3 "" H 12150 6000 50  0001 C CNN
F 4 "CONN-09863" H 11922 6134 60  0000 R CNN "Field4"
	1    12150 6000
	-1   0    0    1   
$EndComp
$Comp
L SparkFun-Connectors:CONN_05JST J6
U 1 1 5D38A34C
P 12150 4800
F 0 "J6" H 11922 4905 45  0000 R CNN
F 1 "CONN_05JST" H 11922 4989 45  0000 R CNN
F 2 "Connector_JST:JST_XH_B5B-XH-A_1x05_P2.50mm_Vertical" H 12150 5400 20  0001 C CNN
F 3 "" H 12150 4800 50  0001 C CNN
F 4 "PRT-09917" H 11922 5084 60  0000 R CNN "Field4"
	1    12150 4800
	-1   0    0    1   
$EndComp
Entry Wire Line
	5550 1750 5650 1850
Entry Wire Line
	5550 1650 5650 1750
Entry Wire Line
	5550 1850 5650 1950
Entry Wire Line
	5550 1950 5650 2050
NoConn ~ 5450 2050
NoConn ~ 5450 2150
Text Label 5450 1650 0    39   ~ 0
D50
Text Label 5450 1750 0    39   ~ 0
D49
Text Label 5450 1850 0    39   ~ 0
TX6
Text Label 5450 1950 0    39   ~ 0
RX6
Text Label 12050 5000 2    39   ~ 0
RX6
Text Label 12050 5100 2    39   ~ 0
TX6
Text Notes 12050 6350 0    59   ~ 0
PWR_5V Connector
NoConn ~ 8650 4650
Entry Wire Line
	4200 7500 4300 7400
Entry Wire Line
	4300 7500 4400 7400
Entry Wire Line
	4700 7500 4800 7400
Entry Wire Line
	4800 7500 4900 7400
Entry Wire Line
	5000 7500 5100 7400
NoConn ~ 3700 7250
Entry Wire Line
	3800 6500 3900 6600
Entry Wire Line
	3900 6500 4000 6600
Entry Wire Line
	4500 6500 4600 6600
Entry Wire Line
	4700 6500 4800 6600
Entry Wire Line
	4600 7550 4700 7650
Wire Wire Line
	3250 1050 3450 1050
Wire Wire Line
	3250 1150 3450 1150
Wire Wire Line
	3250 1250 3450 1250
Wire Wire Line
	3250 1350 3450 1350
Wire Wire Line
	3250 1450 3450 1450
Wire Wire Line
	3250 1550 3450 1550
Wire Wire Line
	3250 1650 3450 1650
Wire Wire Line
	3250 1750 3450 1750
Wire Wire Line
	3250 1850 3450 1850
Wire Wire Line
	3450 5150 3250 5150
Wire Wire Line
	3450 4950 3250 4950
Wire Wire Line
	3250 2450 3450 2450
Wire Wire Line
	3450 2550 3250 2550
Wire Wire Line
	3250 2650 3450 2650
Wire Wire Line
	3450 2750 3250 2750
Wire Wire Line
	3250 2850 3450 2850
Wire Wire Line
	3450 2950 3250 2950
Wire Wire Line
	3250 3050 3450 3050
Wire Wire Line
	5650 5150 5450 5150
Wire Wire Line
	5450 5050 5650 5050
Wire Wire Line
	5650 4950 5450 4950
Wire Wire Line
	5450 4850 5650 4850
Wire Wire Line
	5650 4750 5450 4750
Wire Wire Line
	5450 4650 5650 4650
Wire Wire Line
	5650 4550 5450 4550
Wire Wire Line
	3050 2350 3450 2350
Wire Wire Line
	3450 2150 3050 2150
Wire Wire Line
	2950 2100 2950 2350
Wire Wire Line
	3050 2150 3050 1950
Wire Wire Line
	3050 2350 3050 2450
Wire Wire Line
	2850 4850 3450 4850
Wire Wire Line
	2700 1950 3050 1950
Wire Wire Line
	2700 2250 3450 2250
Wire Wire Line
	2700 2350 2950 2350
Wire Wire Line
	2700 2150 2750 2150
Wire Wire Line
	2750 2150 2750 600 
Wire Wire Line
	2700 2050 2850 2050
Wire Wire Line
	2850 2050 2850 4850
Wire Wire Line
	2750 600  3350 600 
Connection ~ 6300 1150
Wire Wire Line
	3450 950  3350 950 
Wire Wire Line
	3350 950  3350 600 
Connection ~ 3350 600 
Wire Wire Line
	3450 4750 2750 4750
Wire Wire Line
	2750 4750 2750 3550
Connection ~ 2750 2150
Wire Wire Line
	3450 4250 2450 4250
Wire Wire Line
	3450 4350 2550 4350
Wire Wire Line
	2550 4350 2550 5350
Wire Wire Line
	2450 4250 2450 5450
Connection ~ 6200 4450
Wire Wire Line
	6300 600  6300 1150
Connection ~ 6000 5350
Connection ~ 6100 5450
Connection ~ 6300 600 
Wire Wire Line
	10350 3300 10200 3300
Wire Wire Line
	10350 3300 10350 3400
Wire Wire Line
	10200 3400 10350 3400
Connection ~ 10350 3400
Connection ~ 10350 3300
Connection ~ 10350 1850
Wire Wire Line
	10450 3500 10200 3500
Wire Wire Line
	10200 3600 10200 3500
Connection ~ 10200 3500
Wire Wire Line
	9300 3600 9300 3500
Connection ~ 9300 3500
Connection ~ 10450 1950
Wire Wire Line
	10450 600  10450 1100
Wire Wire Line
	10350 1000 10350 1850
Wire Wire Line
	2550 5350 6000 5350
Wire Wire Line
	2450 5450 6100 5450
Wire Wire Line
	3350 600  6300 600 
Wire Wire Line
	6000 3450 6400 3450
Connection ~ 6000 3450
Wire Wire Line
	6000 3450 6000 5350
Wire Wire Line
	6100 3550 6400 3550
Connection ~ 6100 3550
Wire Wire Line
	6100 3550 6100 5450
Wire Wire Line
	6300 3350 6400 3350
Connection ~ 6300 3350
Wire Wire Line
	6400 3250 6200 3250
Connection ~ 6200 3250
Wire Wire Line
	6200 3250 6200 4450
Wire Wire Line
	6100 950  6400 950 
Wire Wire Line
	6000 1050 6400 1050
Wire Wire Line
	6300 1150 6400 1150
Wire Wire Line
	6200 1250 6400 1250
Wire Wire Line
	6300 1150 6300 2250
Wire Wire Line
	6200 1250 6200 2150
Wire Wire Line
	6000 1050 6000 2350
Wire Wire Line
	6100 950  6100 2450
Wire Wire Line
	6400 2150 6200 2150
Connection ~ 6200 2150
Wire Wire Line
	6200 2150 6200 3250
Wire Wire Line
	6400 2250 6300 2250
Connection ~ 6300 2250
Wire Wire Line
	6300 2250 6300 3350
Wire Wire Line
	6400 2350 6000 2350
Connection ~ 6000 2350
Wire Wire Line
	6000 2350 6000 3450
Wire Wire Line
	6400 2450 6100 2450
Connection ~ 6100 2450
Wire Wire Line
	6100 2450 6100 3550
Wire Wire Line
	10450 1100 12050 1100
Connection ~ 10450 1100
Wire Wire Line
	10450 1100 10450 1950
Wire Wire Line
	10450 1950 10650 1950
Wire Wire Line
	10750 1200 10750 1000
Wire Wire Line
	10750 1000 12050 1000
Wire Wire Line
	3450 4650 2650 4650
Wire Wire Line
	5450 4250 5850 4250
Wire Wire Line
	5850 700  5850 4250
Wire Wire Line
	6300 3350 6300 4550
Wire Wire Line
	9200 3800 9300 3800
Wire Wire Line
	9300 3300 9300 3400
Wire Wire Line
	9200 3400 9200 3500
Wire Wire Line
	9200 3500 9300 3500
Wire Wire Line
	9100 4150 9100 3600
Wire Wire Line
	9100 4150 11600 4150
Wire Wire Line
	9000 4250 11700 4250
Wire Wire Line
	9100 5350 9100 5050
Wire Wire Line
	6000 5350 9100 5350
Wire Wire Line
	9000 4550 9000 4950
Wire Wire Line
	6100 5450 9000 5450
Connection ~ 9300 3300
Wire Wire Line
	6300 600  10450 600 
Wire Wire Line
	7650 3400 7550 3400
Wire Wire Line
	7550 4150 7550 3400
Wire Wire Line
	10450 1950 10450 2550
Wire Wire Line
	7750 4350 7750 4050
Wire Wire Line
	7650 3500 7650 4550
Connection ~ 7650 4550
Wire Wire Line
	7650 4550 7750 4550
Wire Wire Line
	10150 1000 10350 1000
Wire Wire Line
	10150 1100 10450 1100
Wire Wire Line
	10150 1200 10750 1200
Wire Wire Line
	10150 1850 10350 1850
Wire Wire Line
	10150 1950 10450 1950
Wire Wire Line
	10150 2450 10250 2450
Wire Wire Line
	6300 4550 6300 6650
Connection ~ 6300 4550
Wire Wire Line
	5850 700  8750 700 
Connection ~ 6300 6650
Connection ~ 6200 6550
Wire Wire Line
	1600 3550 1600 4550
Wire Wire Line
	3450 3350 2400 3350
Wire Wire Line
	2400 3350 2400 3450
Wire Wire Line
	2350 4150 2350 5700
Wire Wire Line
	2050 3850 3450 3850
Wire Wire Line
	2050 3950 3450 3950
Wire Wire Line
	2050 4050 3450 4050
Wire Wire Line
	2350 4150 3450 4150
Wire Wire Line
	2250 3250 3450 3250
Wire Wire Line
	2050 4450 3450 4450
Wire Wire Line
	2250 4550 3450 4550
Wire Wire Line
	12050 1200 11200 1200
Wire Wire Line
	11200 1300 12050 1300
Wire Wire Line
	10150 900  11000 900 
Wire Wire Line
	11000 900  11000 1200
Wire Wire Line
	11400 2150 12050 2150
Wire Wire Line
	11400 2050 12050 2050
Wire Wire Line
	12050 1950 11400 1950
Wire Wire Line
	11200 1400 12050 1400
Wire Bus Line
	5650 4350 7100 4350
Wire Bus Line
	7100 4350 7100 6150
Wire Wire Line
	11700 5000 12050 5000
Wire Wire Line
	11700 5100 12050 5100
Wire Wire Line
	5550 1650 5450 1650
Wire Wire Line
	5450 1750 5550 1750
Wire Wire Line
	5450 1850 5550 1850
Wire Wire Line
	5450 1950 5550 1950
Wire Wire Line
	11800 6550 11800 6000
Wire Wire Line
	11800 6550 9500 6550
Wire Wire Line
	11800 4800 12050 4800
Wire Wire Line
	11800 6000 12050 6000
Connection ~ 11800 6000
Wire Wire Line
	11800 6000 11800 4800
Wire Wire Line
	11900 6650 11900 6550
Wire Wire Line
	11900 4900 12050 4900
Wire Wire Line
	11900 6550 12050 6550
Connection ~ 11900 6550
Wire Wire Line
	11900 6550 11900 6100
Wire Wire Line
	11900 6100 12050 6100
Connection ~ 11900 6100
Wire Wire Line
	11900 6100 11900 4900
Wire Wire Line
	6200 4450 7750 4450
Wire Wire Line
	6300 4550 7650 4550
Wire Wire Line
	8650 4550 9000 4550
Wire Wire Line
	5450 4450 6200 4450
Wire Wire Line
	8650 4450 9100 4450
Wire Wire Line
	8650 3300 9300 3300
Wire Wire Line
	6300 6650 6300 7950
Wire Wire Line
	6200 6550 6200 7850
Wire Wire Line
	2650 4650 2650 5600
Wire Wire Line
	2650 5600 9200 5600
Wire Wire Line
	9200 3800 9200 5600
Wire Bus Line
	2150 5850 11100 5850
Wire Bus Line
	1950 5950 11300 5950
Wire Wire Line
	5100 7400 5100 7250
Wire Wire Line
	4300 7250 4300 7400
Wire Wire Line
	4400 7250 4400 7400
Wire Wire Line
	4800 7250 4800 7400
Wire Wire Line
	4900 7400 4900 7250
Wire Wire Line
	3900 6600 3900 6750
Wire Wire Line
	4000 6600 4000 6750
Wire Wire Line
	4600 6600 4600 6750
Wire Wire Line
	4800 6600 4800 6750
Wire Bus Line
	3150 6500 3150 7500
Wire Bus Line
	5750 6350 5750 7650
Wire Wire Line
	4600 7250 4600 7450
Connection ~ 5750 6350
Connection ~ 3150 6500
Wire Wire Line
	2250 3150 3100 3150
Wire Wire Line
	3100 3150 3100 3200
Wire Wire Line
	3100 3200 3200 3200
Wire Wire Line
	3200 3200 3200 3150
Wire Wire Line
	3200 3150 3450 3150
Wire Wire Line
	3200 5050 3200 5100
Wire Wire Line
	3200 5100 2250 5100
Wire Wire Line
	3200 5050 3450 5050
Wire Wire Line
	3200 2100 3200 2050
Wire Wire Line
	3200 2100 2950 2100
Wire Wire Line
	3200 2050 3450 2050
Text Label 5100 7250 3    39   ~ 0
DB4
Text Label 5100 6750 1    39   ~ 0
DB5
Text Label 5000 7250 3    39   ~ 0
DB6
Text Label 5000 6750 1    39   ~ 0
DB7
Text Label 3800 7250 3    39   ~ 0
TCLK
Text Label 4000 6750 1    39   ~ 0
T_CS
Text Label 3800 6750 1    39   ~ 0
TDIN
Text Label 3900 6750 1    39   ~ 0
TDOUT
Text Label 3900 7250 3    39   ~ 0
TIRQ
Text Label 5200 6750 1    39   ~ 0
DB3
Text Label 5200 7250 3    39   ~ 0
DB2
Text Label 5300 6750 1    39   ~ 0
DB1
Text Label 5300 7250 3    39   ~ 0
DB0
Text Label 4200 7250 3    39   ~ 0
RS
Text Label 4300 6750 1    39   ~ 0
WR
Text Label 4900 7250 3    39   ~ 0
DB8
Text Label 4900 6750 1    39   ~ 0
DB9
Text Label 4800 7250 3    39   ~ 0
DB10
Text Label 4800 6750 1    39   ~ 0
DB11
Text Label 4700 7250 3    39   ~ 0
DB12
Text Label 4700 6750 1    39   ~ 0
DB13
Text Label 4600 7250 3    39   ~ 0
DB14
Text Label 4600 6750 1    39   ~ 0
DB15
Text Label 4300 7250 3    39   ~ 0
L_CS
Text Label 4400 7250 3    39   ~ 0
RSET
Wire Wire Line
	2400 3450 1050 3450
Wire Wire Line
	7550 2950 7550 3400
Connection ~ 7550 3400
Wire Wire Line
	8650 4350 10350 4350
Wire Wire Line
	12050 5200 11700 5200
Text Notes 12050 5900 0    59   ~ 0
PWR_3V3 Connector
Wire Wire Line
	10350 3400 10350 4350
Wire Wire Line
	6300 6650 9600 6650
Wire Bus Line
	7100 6150 11600 6150
Wire Wire Line
	2350 5700 10900 5700
Wire Wire Line
	11900 3800 11900 2900
Wire Wire Line
	11900 2900 12050 2900
Wire Wire Line
	12050 2800 11800 2800
Wire Wire Line
	11800 2800 11800 3700
Wire Wire Line
	10200 3800 11900 3800
Wire Wire Line
	10200 3700 11800 3700
Wire Wire Line
	11700 4250 11700 2700
Wire Wire Line
	11700 2700 12050 2700
Wire Wire Line
	12050 2600 11600 2600
Wire Wire Line
	11600 2600 11600 4150
Wire Wire Line
	10600 2950 10600 2500
Wire Wire Line
	10600 2500 12050 2500
Wire Wire Line
	10750 6450 12050 6450
Wire Wire Line
	10900 3250 10900 5700
Wire Wire Line
	10900 3250 12050 3250
Wire Wire Line
	12050 3350 10450 3350
Connection ~ 10450 3350
Wire Wire Line
	10450 3350 10450 3500
Entry Wire Line
	11200 1950 11300 2050
Wire Wire Line
	7550 2950 10600 2950
Wire Wire Line
	8750 700  8750 2250
Entry Wire Line
	11600 5650 11700 5550
Entry Wire Line
	11600 5750 11700 5650
Wire Wire Line
	11700 5200 11700 5550
Wire Wire Line
	8650 3600 9100 3600
Wire Wire Line
	9000 4250 9000 3500
Wire Wire Line
	9000 3500 8650 3500
Text Notes 12050 4700 0    59   ~ 0
AUX_SERIAL Connector
Text Label 12050 5650 2    39   ~ 0
D50
Text Label 12050 5550 2    39   ~ 0
D49
Wire Wire Line
	12050 5650 11700 5650
Wire Wire Line
	12050 5550 11700 5550
$Comp
L Connector:Conn_01x02_Female J7
U 1 1 5D431CBC
P 12250 5550
F 0 "J7" H 12278 5526 50  0000 L CNN
F 1 "Conn_01x02_Female" H 12278 5435 50  0000 L CNN
F 2 "Connectors:1X02" H 12250 5550 50  0001 C CNN
F 3 "~" H 12250 5550 50  0001 C CNN
	1    12250 5550
	1    0    0    -1  
$EndComp
Text Notes 12050 5450 0    59   ~ 0
AUX_DIO Connector
Wire Wire Line
	10150 2550 10450 2550
Connection ~ 10450 2550
Wire Wire Line
	10450 2550 10450 3350
$Comp
L teensy:Teensy3.6 U2
U 1 1 5D372A7D
P 4450 3050
F 0 "U2" H 4450 5437 60  0000 C CNN
F 1 "Teensy3.6" H 4450 5331 60  0000 C CNN
F 2 "Caveatron_Components-Simplified:Teensy36-S" H 4450 3050 60  0001 C CNN
F 3 "" H 4450 3050 60  0000 C CNN
	1    4450 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	10450 4050 10450 3500
Connection ~ 10450 3500
Wire Wire Line
	8650 3400 9200 3400
Wire Wire Line
	7750 4050 10450 4050
Wire Wire Line
	1050 4550 1600 4550
Wire Wire Line
	1600 3550 2750 3550
Connection ~ 2750 3550
Wire Wire Line
	2750 3550 2750 2150
Wire Wire Line
	10250 2450 10250 2250
Wire Wire Line
	10250 2250 8750 2250
Wire Wire Line
	10350 1850 10350 2650
Wire Wire Line
	10250 2250 10750 2250
Connection ~ 10250 2250
Wire Wire Line
	10750 2250 10750 6450
Wire Wire Line
	10150 2650 10350 2650
Connection ~ 10350 2650
Wire Wire Line
	10350 2650 10350 3300
Wire Wire Line
	7550 4150 5450 4150
NoConn ~ 5450 3350
NoConn ~ 5450 3450
NoConn ~ 3450 3550
$Comp
L Device:CP1_Small C1
U 1 1 5D4D9397
P 11550 1750
F 0 "C1" H 11641 1796 50  0000 L CNN
F 1 "CP1" H 11641 1705 50  0000 L CNN
F 2 "Capacitors:1206" H 11550 1750 50  0001 C CNN
F 3 "~" H 11550 1750 50  0001 C CNN
	1    11550 1750
	1    0    0    -1  
$EndComp
Connection ~ 11550 1850
Wire Wire Line
	11550 1850 12050 1850
Wire Wire Line
	12050 1750 11900 1750
Wire Wire Line
	11900 1750 11900 1650
Wire Wire Line
	11900 1650 11550 1650
Connection ~ 11550 1650
Wire Wire Line
	11200 1750 11200 1950
$Comp
L Caveatron_Components:Pololu_Adj_Reg U9
U 1 1 5D387482
P 9700 2200
F 0 "U9" H 9978 1946 50  0000 L CNN
F 1 "Pololu_Adj_Reg" H 9978 1855 50  0000 L CNN
F 2 "Caveatron Components:Pololu_Adj_Reg" H 9700 2200 50  0001 C CNN
F 3 "" H 9700 2200 50  0001 C CNN
	1    9700 2200
	-1   0    0    1   
$EndComp
Wire Wire Line
	10150 1750 11200 1750
Wire Wire Line
	10150 2050 10800 2050
Wire Wire Line
	10800 1650 11550 1650
Wire Wire Line
	10650 1850 10650 1950
Wire Wire Line
	10800 2050 10800 1650
Wire Wire Line
	10650 1850 11550 1850
$Comp
L Connector_Generic:Conn_02x20_Odd_Even J1
U 1 1 5FD27DB6
P 4500 7050
F 0 "J1" V 4596 5963 50  0000 R CNN
F 1 "Conn_02x20_Odd_Even" V 4550 7450 50  0000 R CNN
F 2 "" H 4500 7050 50  0001 C CNN
F 3 "~" H 4500 7050 50  0001 C CNN
	1    4500 7050
	0    1    -1   0   
$EndComp
Wire Wire Line
	5400 7250 5400 7950
Wire Wire Line
	5400 7950 6300 7950
Wire Wire Line
	5400 6750 5400 6550
Wire Wire Line
	5400 6550 6200 6550
Wire Wire Line
	6200 4450 6200 6250
Entry Wire Line
	5300 6450 5400 6350
Entry Wire Line
	5200 6450 5300 6350
Entry Wire Line
	5200 7550 5300 7650
Entry Wire Line
	4700 6450 4800 6350
Entry Wire Line
	4300 6450 4400 6350
Entry Wire Line
	4200 7550 4300 7650
Wire Wire Line
	5300 6450 5300 6750
Wire Wire Line
	5200 6450 5200 6750
Wire Wire Line
	5200 7450 5150 7450
Wire Wire Line
	5150 7450 5150 7550
Wire Wire Line
	5150 7550 5200 7550
Wire Wire Line
	5200 7450 5200 7250
Entry Wire Line
	5200 7500 5300 7400
Wire Wire Line
	5300 7400 5300 7250
Wire Wire Line
	4200 6250 6200 6250
Connection ~ 6200 6250
Wire Wire Line
	6200 6250 6200 6550
Wire Wire Line
	4200 6250 4200 6750
Entry Wire Line
	4900 7500 5000 7400
Wire Wire Line
	5000 7250 5000 7400
Wire Wire Line
	4700 6550 4650 6550
Wire Wire Line
	4650 6550 4650 6450
Wire Wire Line
	4650 6450 4700 6450
Wire Wire Line
	4700 6550 4700 6750
Entry Wire Line
	5000 6500 5100 6600
Wire Wire Line
	5100 6600 5100 6750
Entry Wire Line
	4900 6500 5000 6600
Wire Wire Line
	5000 6600 5000 6750
Entry Wire Line
	4800 6500 4900 6600
Wire Wire Line
	4900 6600 4900 6750
Wire Wire Line
	4600 7450 4550 7450
Wire Wire Line
	4550 7450 4550 7550
Wire Wire Line
	4550 7550 4600 7550
Entry Wire Line
	4600 7500 4700 7400
Wire Wire Line
	4700 7400 4700 7250
NoConn ~ 4500 7250
NoConn ~ 4500 6750
Wire Wire Line
	4300 6450 4300 6750
NoConn ~ 4400 6750
NoConn ~ 4100 7250
NoConn ~ 4100 6750
Entry Wire Line
	3800 7500 3900 7400
Wire Wire Line
	3900 7400 3900 7250
Wire Wire Line
	4000 7250 4000 7850
Wire Wire Line
	4000 7850 6200 7850
Entry Wire Line
	3700 7500 3800 7400
Wire Wire Line
	3800 7400 3800 7250
Entry Wire Line
	3700 6500 3800 6600
Wire Wire Line
	3800 6600 3800 6750
NoConn ~ 3700 6750
NoConn ~ 3600 6750
NoConn ~ 3600 7250
NoConn ~ 3500 7250
NoConn ~ 3500 6750
NoConn ~ 3450 1950
NoConn ~ 8500 7050
Text Label 3350 600  0    50   ~ 0
GND
Wire Wire Line
	2950 6250 4200 6250
Connection ~ 4200 6250
Text Label 4200 6250 0    50   ~ 0
3V3
Text Label 8600 4350 0    50   ~ 0
V_BAT
Text Label 5450 4250 0    50   ~ 0
5V
Wire Wire Line
	2700 2450 3050 2450
Text Label 2650 2450 0    50   ~ 0
3V3_1
Text Label 2650 1950 0    50   ~ 0
F_DI
Text Label 2650 2050 0    50   ~ 0
F_CLK
Text Label 2650 2250 0    50   ~ 0
F_DO
Text Label 2650 2350 0    50   ~ 0
F_CS
Text Label 12100 2600 2    39   ~ 0
RED_LED
Text Label 12100 2700 2    39   ~ 0
GRN_LED
Text Label 12100 2800 2    39   ~ 0
SWITCH_A
Text Label 12100 2900 2    39   ~ 0
SWITCH_B
Text Label 5450 4150 0    39   ~ 0
V_USB
Text Label 8650 3300 0    39   ~ 0
BATT+
Text Label 8650 3400 0    39   ~ 0
BATT-
$Comp
L SparkFun-Connectors:QWIIC_CONNECTORSR J10
U 1 1 5FD2BA21
P 9750 5250
F 0 "J10" H 9522 5495 45  0000 R CNN
F 1 "QWIIC_CONNECTOR (VERTICAL)" H 9522 5411 45  0000 R CNN
F 2 "Connector_JST:JST_SH_BM04B-SRSS-TB_1x04-1MP_P1.00mm_Vertical" H 9750 5750 20  0001 C CNN
F 3 "" H 9750 5250 50  0001 C CNN
F 4 "PRT-16766" H 9522 5316 60  0000 R CNN "Field4"
	1    9750 5250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9650 4950 9000 4950
Connection ~ 9000 4950
Wire Wire Line
	9000 4950 9000 5450
Wire Wire Line
	9650 5050 9100 5050
Connection ~ 9100 5050
Wire Wire Line
	9100 5050 9100 4450
Wire Wire Line
	9650 5150 9500 5150
Wire Wire Line
	9500 5150 9500 6550
Connection ~ 9500 6550
Wire Wire Line
	9500 6550 6200 6550
Wire Wire Line
	9650 5250 9600 5250
Wire Wire Line
	9600 5250 9600 6650
Connection ~ 9600 6650
Wire Wire Line
	9600 6650 11900 6650
Wire Wire Line
	10050 5150 9900 5150
Wire Wire Line
	9900 5150 9900 5050
Wire Wire Line
	4200 7250 4200 7550
Wire Bus Line
	11100 1300 11100 5850
Wire Bus Line
	11300 2050 11300 5950
Wire Bus Line
	4300 7650 5750 7650
Wire Bus Line
	5650 1750 5650 4350
Wire Bus Line
	11600 5100 11600 6150
Wire Bus Line
	1950 3950 1950 5950
Wire Bus Line
	2150 3250 2150 5850
Wire Bus Line
	4400 6350 5750 6350
Wire Bus Line
	5750 4650 5750 6350
Wire Bus Line
	3150 6500 5000 6500
Wire Bus Line
	3150 7500 5200 7500
Wire Bus Line
	3150 1150 3150 6500
$EndSCHEMATC
