all: build/main.o build/Environment.o
	g++ -o bond build/main.o build/Environment.o

build/main.o: src/main.cc
	g++ -o build/main.o -c src/main.cc

build/Environment.o: src/Environment.cc src/Environment.h
	g++ -o build/Environment.o -c src/Environment.cc

build/ForwardSensor.o: src/ForwardSensor.cc src/ForwardSensor.h src/Environment.h
	g++ -o build/ForwardSensor.o -c src/ForwardSensor.cc

test: test/build/Environment test/build/ForwardSensor
	test/build/Environment
	test/build/ForwardSensor

test/build/Environment: test/Environment.cc src/Environment.h build/Environment.o
	g++ -o test/build/Environment test/Environment.cc build/Environment.o

test/build/ForwardSensor: test/ForwardSensor.cc src/Environment.h build/Environment.o build/ForwardSensor.o
	g++ -o test/build/ForwardSensor test/ForwardSensor.cc build/Environment.o build/ForwardSensor.o

clean:
	rm -f build/* test/build/* test/runner bond
