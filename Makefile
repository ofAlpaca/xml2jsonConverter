LIB_DIR = ./libs
IFLAGS = -I ./ezxml -I ./cJSON-master -I ./xml2jsonConverter -g
LFLAGS =  -L $(LIB_DIR)
MAKE = make
FLAGS = ${IFLAGS} ${LFLAGS} -std=c99 -lm -w

all: EXAMPLES_EXE

EZXML_LIB: $(LIB_DIR)/libezxml.a
CJSON_LIB: $(LIB_DIR)/libcjson.a

%ezxml.a:
	@echo make static lib of ezxml.
	cd ezxml && $(MAKE) 
	mv ./ezxml/libezxml.a $(LIB_DIR)

%cjson.a:
	@echo make static lib of cjson.
	cd cJSON-master && $(MAKE)
	mv ./cJSON-master/libcjson.a $(LIB_DIR)

CONVERTER_LIB:
	gcc -c xml2jsonConverter/ezxml2cJson.c $(FLAGS)
	ar crsv libconverter.a ezxml2cJson.o
	mv libconverter.a $(LIB_DIR)
	rm ezxml2cJson.o

EXAMPLES_EXE: EZXML_LIB CJSON_LIB CONVERTER_LIB
	gcc ipdr2cJson.c $(FLAGS) -lconverter -lcjson -lezxml -o ipdr2cJson

clean:
	rm -f $(EXE_FILES) $(LIB_DIR)/*.a  
	cd ezxml && $(MAKE) clean
	cd cJSON-master && $(MAKE) clean
