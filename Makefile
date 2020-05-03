all: unit bin data unit.dta test

UNIT_HOME = $(HOME)/.unit

unit: unit.c
	gcc -lm $< -o $@

bin: bin.c
	gcc $< -o $@

data: data.c
	gcc $< -o $@

unit.dta: bin unit.asc
	./bin

test: unit unit.dta
	sh ./test.sh

install: unit.dta
	mkdir -p $(UNIT_HOME)
	install fr/* $(UNIT_HOME)

install-en: unit.dta
	mkdir -p $(UNIT_HOME)
	install en/* $(UNIT_HOME)
