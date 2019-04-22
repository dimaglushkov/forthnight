# ------------------------------------------------
# ForthrIM, a Forthress dialect improve
#
# Author: igorjirkov@gmail.com
# Improves by ddimaglushkov@ya.ru
#
# ------------------------------------------------

ASM         = nasm
ASMFLAGS    = -felf64 -g -Isrc/
LINKER      = ld

all: bin/forthress

bin/forthress: obj/forthress.o obj/util.o
	mkdir -p bin
	$(LINKER) -o bin/forthress  $(LINKERFLAGS) -o bin/forthress obj/forthress.o obj/util.o $(LIBS)

obj/forthress.o: src/forthress.asm src/macro.inc src/words.inc src/util.inc
	mkdir -p obj
	$(ASM) $(ASMFLAGS) src/forthress.asm -o obj/forthress.o

obj/util.o: src/util.inc src/util.asm
	mkdir -p obj
	$(ASM) $(ASMFLAGS) src/util.asm -o obj/util.o

clean:
	rm -rf bin obj

.PHONY: clean

