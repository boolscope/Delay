all:
test:
	@avr-g++ -mmcu=atmega328p -F_CPU=16000000UL \
		-I~/Arduino/libraries \
		-I/usr/lib/avr/include \
		-I/usr/share/arduino/hardware/arduino/avr/cores/arduino \
		-I/usr/share/arduino/hardware/arduino/avr/variants/standard \
		./src/Delay.cpp -o Delay.out
doxygen:
	@doxygen doc/doxygen.conf