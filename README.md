# Z5730M-Nixie-clock

## list of contents
1. [Links](#links)
2. [Nixie lamps](#nixie-lamps)
3. [Driver](#driver)
4. [MCU](#mcu)
5. [Power supply](#power-supply)
5. [BOM links](#bom-links)

---

## Links
* [Z5730M (RFT) Nixie Röhre / Nixie Tube](https://www.tube-tester.com/sites/nixie/data/z5730m/z5730m.htm)
* [74141 BCD decoder](https://kevinrye.net/index_files/driving_a_nixie_with_a_74141_bcd_decoder.php)'
* [74141 datasheets](https://www.tube-tester.com/sites/nixie/74141-NDT/74141-NDT.htm)
## Nixie lamps Z5730M
![alt text](image.png)
![alt text](image-1.png)
![alt text](image-3.png)
![alt text](image-8.png)

### Pinout
![alt text](image-4.png)
### Datasheet 
![alt text](image-5.png)
## Driver
![alt text](image-2.png)

### truth table
![alt text](image-6.png)
### Pinout
![alt text](image-7.png)
## MCU

## Power supply
Woking voltage for Z5730M is 140V, power supply will be 170V, cathode current for this type is 2mA.
R = Vcc-Vworking / I = (170-140) / 0.002 = 15kOhm

## BOM links

dip16 socket - https://www.digikey.pl/pl/products/detail/te-connectivity-amp-connectors/1-2199298-4/5022041?gclsrc=aw.ds&gad_source=1&gad_campaignid=20228863357&gbraid=0AAAAADrbLlhwpbC1IhmJ-bvT1xEubTayN&gclid=Cj0KCQiAtfXMBhDzARIsAJ0jp3AqTFtrzFdLOggFWOhUBCsz4lLAxhBbSbIDusreV6ZxRz36MtxcVf8aAlY8EALw_wcB

Pins for nixie - https://www.digikey.pl/en/products/detail/mill-max-manufacturing-corp/9407-0-15-15-11-27-10-0/437935

15kOhm resistor - https://www.digikey.pl/pl/products/detail/vishay-dale/RCS060315K0FKEA/5866942

SparkFun 16347 LED - https://www.digikey.pl/pl/products/detail/sparkfun-electronics/16347/11630204

ESP32-S3-WROOM-1-N16R2 - https://www.digikey.pl/pl/models/16162644