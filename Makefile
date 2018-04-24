
mc = 18F2550

CC = "/opt/microchip/xc8/v1.45/bin/xc8"
CFLAGS = -Q --chip=$(mc) --double=24 --float=24 --emi=wordwrite \
		--opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore \
		--mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  \
		--summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 \
		--runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib \
		--output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto \
		"--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" \
		"--msgformat=%f:%l: advisory: (%n) %s"

# $LDFLAGS += --memorysummary memoryfile.xml

OBJECT = $(program).p1
SOURCE = $(program).c

all: checkname $(program).hex

checkname:
ifndef program
	$(error 'program' is not set. Use 'make program=<name of program>')
endif


$(OBJECT): $(SOURCE)
	$(CC) --pass1 $(CFLAGS) -G -o$@ $< 


$(program).hex: $(OBJECT)  
	$(CC) $(CFLAGS) -o$@ $<     


clean:
	rm -f *.cmf *.elf *.hex *.hxl *.lst *.map *.mum *.obj *.rlf *.sdb *.sym *.d *.p1 *.pre memoryfile.xml
