NAME = Truckr
DIR = .
PORT = /dev/ttyUSB0

ACLI = /home/cm/.arduino/arduino-cli

srcFiles = $(shell find $(dir)/src/ -name '*.cpp') $(shell find src/ -name '*.cpp')
headerFiles = $(shell find $(dir)/src/ -name '*.h')

.PHONY: upload clean

build: $(dir)/build/$(NAME).bin

$(dir)/build/$(NAME).bin: $(srcFiles) $(headerFiles) $(dir)/$(NAME).ino
	@mkdir -p $(dir)/build
	cd $(dir); \
	$(ACLI) compile --fqbn cm:esp32:ringo --optimize-for-debug -o build/$(NAME).bin $(NAME).ino

upload: $(dir)/build/$(NAME).bin
	@cd $(dir); \
	$(ACLI) upload --fqbn cm:esp32:ringo -p $(PORT) -i build/$(NAME).bin

clean:
	cd $(dir); \
	rm -rf build
