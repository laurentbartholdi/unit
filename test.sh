#!/bin/sh
UNIT_BIN="${HOME}/bin"

test_unit()
{
    echo "\nTesting :\n" "$1"
    echo "$1 quit" | ${UNIT_BIN}/unit
}

test_unit "12 mm en yard  last en pt"
test_unit "12.4 v 5.2 a * en w  last 15 s * en kcal last en j  last en gev"
test_unit "0 oC en oK  0 oF en oC  0 oC en oF  32 oF en oC  100 oF en oC  100 oC en oF"
test_unit ": vsphere  3 ^  4 pi *  3 / ;  1 m vsphere en m3"
test_unit "1 mu vs en mm3"
test_unit "infix 3.2 v * 5.9 a en kw"
test_unit "infix 3.2 v * 5.9 a * 2 hr en kwh  last en kcal  last en gev"
test_unit "infix 1000 j def kjoule  3.2 v * 5.9 a * 2 hr en kjoule"
test_unit "infix 800 kilo j def p.muesli  82 kgf * 2000 m en p.muesli  last ?"
test_unit "1 quit"
