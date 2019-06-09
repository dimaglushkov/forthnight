ASMFLAGS = -felf64 -g -Isrc/

all: bin/forthnight

bin/forthnight: obj/forthnight.o obj/utilities.o
	mkdir -p bin
	ld -o bin/forthnight -o bin/forthnight obj/forthnight.o obj/utilities.o

obj/forthnight.o: src/forthnight.asm src/macro.inc src/words.inc src/utilities.inc
	mkdir -p obj
	nasm $(ASMFLAGS) src/forthnight.asm -o obj/forthnight.o

obj/utilities.o: src/utilities.inc src/utilities.asm
	mkdir -p obj
	nasm $(ASMFLAGS) src/utilities.asm -o obj/utilities.o

clean:
	rm -rf bin obj

.PHONY: clean

