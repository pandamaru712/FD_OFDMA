UL_OFDMA: main.o setting.o
	gcc -o $@ $^

main.o: src/main.c
	gcc -c $^

setting.o: src/setting.c
	gcc -c $^

clean:
	rm main.o setting.o
