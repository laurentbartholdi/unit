all: unit bin data unit.dta test

UNIT_HOME     = $(HOME)/.unit
UNIT_HOME_BIN = $(HOME)/bin

unit: unit.c
	gcc -lm $< -o $@

bin: bin.c
	gcc $< -o $@

data: data.c
	gcc $< -o $@

unit.dta: bin unit.asc
	./bin

test: unit unit.dta
	chmod 755 test.sh
	sh ./test.sh > unit-test.log
	diff unit-test.log.orig unit-test.log

install: unit.dta
	mkdir -p $(UNIT_HOME)
	install unit.mac unit.asc unit.dta fr/* test.sh  $(UNIT_HOME)
	install unit $(UNIT_HOME_BIN)

install-en: unit.dta
	mkdir -p $(UNIT_HOME)
	install en/* $(UNIT_HOME)
	install unit.mac unit.asc unit.dta fr/* test.sh  $(UNIT_HOME)
	install unit $(UNIT_HOME_BIN)

