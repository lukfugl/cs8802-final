GCC=g++ -std=c++11

all: build/main.o build/Environment.o build/Guard.o build/Brain.o build/CoupledEMSensor.o build/CoupledForwardSensor.o build/EMSensor.o build/ForwardSensor.o build/NoisyMap.o build/Bond.o build/Simulator.o build/ParticleFilter.o build/GuardModel.o build/OnlineSLAM.o
	$(GCC) -o bond build/main.o build/Environment.o build/Guard.o vendor/jsoncpp/lib/json_writer.o vendor/jsoncpp/lib/json_reader.o vendor/jsoncpp/lib/json_value.o build/Brain.o build/CoupledEMSensor.o build/CoupledForwardSensor.o build/EMSensor.o build/ForwardSensor.o build/NoisyMap.o build/Bond.o build/Simulator.o build/ParticleFilter.o build/GuardModel.o build/OnlineSLAM.o

build/main.o: src/main.cc
	$(GCC) -o build/main.o -c src/main.cc

build/Brain.o: src/Brain.cc src/Brain.h
	$(GCC) -o build/Brain.o -c src/Brain.cc

build/Environment.o: src/Environment.cc src/Environment.h src/Coordinate.h src/Guard.h
	$(GCC) -o build/Environment.o -c src/Environment.cc

build/CoupledForwardSensor.o: src/CoupledForwardSensor.cc src/CoupledForwardSensor.h
	$(GCC) -o build/CoupledForwardSensor.o -c src/CoupledForwardSensor.cc

build/CoupledEMSensor.o: src/CoupledEMSensor.cc src/CoupledEMSensor.h
	$(GCC) -o build/CoupledEMSensor.o -c src/CoupledEMSensor.cc

build/ForwardSensor.o: src/ForwardSensor.cc src/ForwardSensor.h src/Environment.h src/Coordinate.h
	$(GCC) -o build/ForwardSensor.o -c src/ForwardSensor.cc

build/EMSensor.o: src/EMSensor.cc src/EMSensor.h src/Environment.h src/Coordinate.h src/Guard.h
	$(GCC) -o build/EMSensor.o -c src/EMSensor.cc

build/NoisyMap.o: src/NoisyMap.cc src/NoisyMap.h src/Environment.h src/Coordinate.h
	$(GCC) -o build/NoisyMap.o -c src/NoisyMap.cc

build/Guard.o: src/Guard.cc src/Guard.h src/Coordinate.h
	$(GCC) -o build/Guard.o -c src/Guard.cc

build/Bond.o: src/Bond.cc src/Bond.h
	$(GCC) -o build/Bond.o -c src/Bond.cc

build/Simulator.o: src/Simulator.cc src/Simulator.h
	$(GCC) -o build/Simulator.o -c src/Simulator.cc

build/ParticleFilter.o: src/ParticleFilter.cc src/ParticleFilter.h
	$(GCC) -o build/ParticleFilter.o -c src/ParticleFilter.cc

build/GuardModel.o: src/GuardModel.cc src/GuardModel.h
	$(GCC) -o build/GuardModel.o -c src/GuardModel.cc

build/OnlineSLAM.o: src/OnlineSLAM.cc src/OnlineSLAM.h
	$(GCC) -o build/OnlineSLAM.o -c src/OnlineSLAM.cc

test: test/build/Environment test/build/ForwardSensor test/build/EMSensor test/build/NoisyMap test/build/Guard
	test/build/Environment
	test/build/ForwardSensor
	test/build/EMSensor
	test/build/NoisyMap
	test/build/Guard

test/build/Environment: test/Environment.cc src/Environment.h build/Environment.o build/Guard.o build/EMSensor.o build/ForwardSensor.o build/NoisyMap.o build/Bond.o
	$(GCC) -o test/build/Environment test/Environment.cc build/Environment.o build/Guard.o vendor/jsoncpp/lib/json_writer.o vendor/jsoncpp/lib/json_reader.o vendor/jsoncpp/lib/json_value.o build/EMSensor.o build/ForwardSensor.o build/NoisyMap.o build/Bond.o

test/build/ForwardSensor: test/ForwardSensor.cc src/Environment.h build/Environment.o build/ForwardSensor.o build/Guard.o test/ConstantNoise.h build/EMSensor.o build/NoisyMap.o build/Bond.o
	$(GCC) -o test/build/ForwardSensor test/ForwardSensor.cc build/Environment.o build/ForwardSensor.o build/Guard.o vendor/jsoncpp/lib/json_writer.o vendor/jsoncpp/lib/json_reader.o vendor/jsoncpp/lib/json_value.o build/EMSensor.o build/NoisyMap.o build/Bond.o

test/build/EMSensor: test/EMSensor.cc src/Environment.h build/Environment.o build/EMSensor.o build/Guard.o test/ConstantNoise.h build/ForwardSensor.o build/NoisyMap.o build/Bond.o
	$(GCC) -o test/build/EMSensor test/EMSensor.cc build/Environment.o build/EMSensor.o build/Guard.o vendor/jsoncpp/lib/json_writer.o vendor/jsoncpp/lib/json_reader.o vendor/jsoncpp/lib/json_value.o build/ForwardSensor.o build/NoisyMap.o build/Bond.o

test/build/NoisyMap: test/NoisyMap.cc src/Environment.h build/Environment.o build/NoisyMap.o build/Guard.o test/DeterministicNoise.h build/EMSensor.o build/ForwardSensor.o build/Bond.o
	$(GCC) -o test/build/NoisyMap test/NoisyMap.cc build/Environment.o build/NoisyMap.o build/Guard.o vendor/jsoncpp/lib/json_writer.o vendor/jsoncpp/lib/json_reader.o vendor/jsoncpp/lib/json_value.o build/EMSensor.o build/ForwardSensor.o build/Bond.o

test/build/Guard: test/Guard.cc src/Guard.h build/Guard.o build/Bond.o
	$(GCC) -o test/build/Guard test/Guard.cc build/Guard.o build/Bond.o

clean:
	rm -f build/* test/build/* bond
