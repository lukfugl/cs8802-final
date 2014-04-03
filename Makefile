all: build/main.o build/Environment.o
	g++ -o bond build/main.o build/Environment.o

build/main.o: src/main.cc
	g++ -o build/main.o -c src/main.cc

build/Environment.o: src/Environment.cc src/Environment.h
	g++ -o build/Environment.o -c src/Environment.cc

build/ForwardSensor.o: src/ForwardSensor.cc src/ForwardSensor.h src/Environment.h
	g++ -o build/ForwardSensor.o -c src/ForwardSensor.cc

build/BioSensor.o: src/BioSensor.cc src/BioSensor.h src/Environment.h
	g++ -o build/BioSensor.o -c src/BioSensor.cc

test: test/build/Environment test/build/ForwardSensor test/build/BioSensor
	test/build/Environment
	test/build/ForwardSensor
	test/build/BioSensor

test/build/Environment: test/Environment.cc src/Environment.h build/Environment.o
	g++ -o test/build/Environment test/Environment.cc build/Environment.o vendor/jsoncpp/lib/json_writer.o vendor/jsoncpp/lib/json_reader.o vendor/jsoncpp/lib/json_value.o

test/build/ForwardSensor: test/ForwardSensor.cc src/Environment.h build/Environment.o build/ForwardSensor.o test/ConstantNoise.h
	g++ -o test/build/ForwardSensor test/ForwardSensor.cc build/Environment.o build/ForwardSensor.o vendor/jsoncpp/lib/json_writer.o vendor/jsoncpp/lib/json_reader.o vendor/jsoncpp/lib/json_value.o

test/build/BioSensor: test/BioSensor.cc src/Environment.h build/Environment.o build/BioSensor.o test/ConstantNoise.h
	g++ -o test/build/BioSensor test/BioSensor.cc build/Environment.o build/BioSensor.o vendor/jsoncpp/lib/json_writer.o vendor/jsoncpp/lib/json_reader.o vendor/jsoncpp/lib/json_value.o

clean:
	rm -f build/* test/build/* bond
