all: build/main.o build/Environment.o build/Guard.o build/EMSensor.o build/ForwardSensor.o build/NoisyMap.o build/Bond.o
	g++ -o bond build/main.o build/Environment.o build/Guard.o vendor/jsoncpp/lib/json_writer.o vendor/jsoncpp/lib/json_reader.o vendor/jsoncpp/lib/json_value.o build/EMSensor.o build/ForwardSensor.o build/NoisyMap.o build/Bond.o

build/main.o: src/main.cc
	g++ -o build/main.o -c src/main.cc

build/Environment.o: src/Environment.cc src/Environment.h src/Coordinate.h src/Guard.h
	g++ -o build/Environment.o -c src/Environment.cc

build/ForwardSensor.o: src/ForwardSensor.cc src/ForwardSensor.h src/Environment.h src/Coordinate.h
	g++ -o build/ForwardSensor.o -c src/ForwardSensor.cc

build/EMSensor.o: src/EMSensor.cc src/EMSensor.h src/Environment.h src/Coordinate.h src/Guard.h
	g++ -o build/EMSensor.o -c src/EMSensor.cc

build/NoisyMap.o: src/NoisyMap.cc src/NoisyMap.h src/Environment.h src/Coordinate.h
	g++ -o build/NoisyMap.o -c src/NoisyMap.cc

build/Guard.o: src/Guard.cc src/Guard.h src/Coordinate.h
	g++ -o build/Guard.o -c src/Guard.cc

build/Bond.o: src/Bond.cc src/Bond.h
	g++ -o build/Bond.o -c src/Bond.cc

test: test/build/Environment test/build/ForwardSensor test/build/EMSensor test/build/NoisyMap test/build/Guard
	test/build/Environment
	test/build/ForwardSensor
	test/build/EMSensor
	test/build/NoisyMap
	test/build/Guard

test/build/Environment: test/Environment.cc src/Environment.h build/Environment.o build/Guard.o build/EMSensor.o build/ForwardSensor.o build/NoisyMap.o build/Bond.o
	g++ -o test/build/Environment test/Environment.cc build/Environment.o build/Guard.o vendor/jsoncpp/lib/json_writer.o vendor/jsoncpp/lib/json_reader.o vendor/jsoncpp/lib/json_value.o build/EMSensor.o build/ForwardSensor.o build/NoisyMap.o build/Bond.o

test/build/ForwardSensor: test/ForwardSensor.cc src/Environment.h build/Environment.o build/ForwardSensor.o build/Guard.o test/ConstantNoise.h build/EMSensor.o build/NoisyMap.o build/Bond.o
	g++ -o test/build/ForwardSensor test/ForwardSensor.cc build/Environment.o build/ForwardSensor.o build/Guard.o vendor/jsoncpp/lib/json_writer.o vendor/jsoncpp/lib/json_reader.o vendor/jsoncpp/lib/json_value.o build/EMSensor.o build/NoisyMap.o build/Bond.o

test/build/EMSensor: test/EMSensor.cc src/Environment.h build/Environment.o build/EMSensor.o build/Guard.o test/ConstantNoise.h build/ForwardSensor.o build/NoisyMap.o build/Bond.o
	g++ -o test/build/EMSensor test/EMSensor.cc build/Environment.o build/EMSensor.o build/Guard.o vendor/jsoncpp/lib/json_writer.o vendor/jsoncpp/lib/json_reader.o vendor/jsoncpp/lib/json_value.o build/ForwardSensor.o build/NoisyMap.o build/Bond.o

test/build/NoisyMap: test/NoisyMap.cc src/Environment.h build/Environment.o build/NoisyMap.o build/Guard.o test/DeterministicNoise.h build/EMSensor.o build/ForwardSensor.o build/Bond.o
	g++ -o test/build/NoisyMap test/NoisyMap.cc build/Environment.o build/NoisyMap.o build/Guard.o vendor/jsoncpp/lib/json_writer.o vendor/jsoncpp/lib/json_reader.o vendor/jsoncpp/lib/json_value.o build/EMSensor.o build/ForwardSensor.o build/Bond.o

test/build/Guard: test/Guard.cc src/Guard.h build/Guard.o
	g++ -o test/build/Guard test/Guard.cc build/Guard.o

clean:
	rm -f build/* test/build/* bond
