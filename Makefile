all: build/main.o build/Environment.o build/Guard.o
	g++ -o bond build/main.o build/Environment.o build/Guard.o vendor/jsoncpp/lib/json_writer.o vendor/jsoncpp/lib/json_reader.o vendor/jsoncpp/lib/json_value.o

build/main.o: src/main.cc
	g++ -o build/main.o -c src/main.cc

build/Environment.o: src/Environment.cc src/Environment.h src/Coordinate.h src/Guard.h
	g++ -o build/Environment.o -c src/Environment.cc

build/ForwardSensor.o: src/ForwardSensor.cc src/ForwardSensor.h src/Environment.h src/Coordinate.h
	g++ -o build/ForwardSensor.o -c src/ForwardSensor.cc

build/BioSensor.o: src/BioSensor.cc src/BioSensor.h src/Environment.h src/Coordinate.h src/Guard.h
	g++ -o build/BioSensor.o -c src/BioSensor.cc

build/NoisyMap.o: src/NoisyMap.cc src/NoisyMap.h src/Environment.h src/Coordinate.h
	g++ -o build/NoisyMap.o -c src/NoisyMap.cc

build/Guard.o: src/Guard.cc src/Guard.h src/Coordinate.h
	g++ -o build/Guard.o -c src/Guard.cc

test: test/build/Environment test/build/ForwardSensor test/build/BioSensor test/build/NoisyMap test/build/Guard
	test/build/Environment
	test/build/ForwardSensor
	test/build/BioSensor
	test/build/NoisyMap
	test/build/Guard

test/build/Environment: test/Environment.cc src/Environment.h build/Environment.o build/Guard.o
	g++ -o test/build/Environment test/Environment.cc build/Environment.o build/Guard.o vendor/jsoncpp/lib/json_writer.o vendor/jsoncpp/lib/json_reader.o vendor/jsoncpp/lib/json_value.o

test/build/ForwardSensor: test/ForwardSensor.cc src/Environment.h build/Environment.o build/ForwardSensor.o build/Guard.o test/ConstantNoise.h
	g++ -o test/build/ForwardSensor test/ForwardSensor.cc build/Environment.o build/ForwardSensor.o build/Guard.o vendor/jsoncpp/lib/json_writer.o vendor/jsoncpp/lib/json_reader.o vendor/jsoncpp/lib/json_value.o

test/build/BioSensor: test/BioSensor.cc src/Environment.h build/Environment.o build/BioSensor.o build/Guard.o test/ConstantNoise.h
	g++ -o test/build/BioSensor test/BioSensor.cc build/Environment.o build/BioSensor.o build/Guard.o vendor/jsoncpp/lib/json_writer.o vendor/jsoncpp/lib/json_reader.o vendor/jsoncpp/lib/json_value.o

test/build/NoisyMap: test/NoisyMap.cc src/Environment.h build/Environment.o build/NoisyMap.o build/Guard.o test/DeterministicNoise.h
	g++ -o test/build/NoisyMap test/NoisyMap.cc build/Environment.o build/NoisyMap.o build/Guard.o vendor/jsoncpp/lib/json_writer.o vendor/jsoncpp/lib/json_reader.o vendor/jsoncpp/lib/json_value.o

test/build/Guard: test/Guard.cc src/Guard.h build/Guard.o
	g++ -o test/build/Guard test/Guard.cc build/Guard.o

clean:
	rm -f build/* test/build/* bond
