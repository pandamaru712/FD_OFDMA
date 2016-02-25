FD_OFDMA: main.o bufferManager.o collision.o frameGen.o idle.o initialization.o result.o success.o debug.o setting.o
	gcc -Wall -O2 $^ -o $@

main.o: src/main.c
	gcc -c $^

bufferManager.o: src/bufferManager.c
	gcc -c $^

collision.o: src/collision.c
	gcc -c $^

frameGen.o: src/frameGen.c
	gcc -c $^

idle.o: src/idle.c
	gcc -c $^

initialization.o: src/initialization.c
	gcc -c $^

result.o: src/result.c
	gcc -c $^

success.o: src/success.c
	gcc -c $^

debug.o: src/debug.c
	gcc -c $^

setting.o: src/setting.c
	gcc -c $^

clean:
	rm main.o bufferManager.o collision.o frameGen.o idle.o initialization.o result.o success.o debug.o
