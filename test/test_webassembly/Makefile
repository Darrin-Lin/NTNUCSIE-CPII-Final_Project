all: add.c
	emcc add.c -o add.js --embed-file=test.txt
clean:
	-rm add.js add.wasm

# emrun --no_browser --port 8080 .