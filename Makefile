LIB_DIR = ./libs
IFLAGS = -I ./ezxml -I ./cJSON-master -I ./xml2jsonConverter -g
LFLAGS =  -L $(LIB_DIR)
MAKE = make
FLAGS = ${IFLAGS} ${LFLAGS} -std=c99 -lm -w

all: EZXML_LIB CJSON_LIB CONVERTER_LIB
test: test_xml2json test_json2xml

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

CONVERTER_LIB: xml2jsonConverter/xml2jsonConv.c
	gcc -c xml2jsonConverter/xml2jsonConv.c $(FLAGS)
	ar crsv libconverter.a xml2jsonConv.o
	mv libconverter.a $(LIB_DIR)
	rm xml2jsonConv.o

test_%: test_%.c xml2jsonConverter/xml2jsonConv.c
	gcc $< $(FLAGS) -lconverter -lcjson -lezxml -o $@

clean:
	rm -f $(EXE_FILES) $(LIB_DIR)/*.a  
	cd ezxml && $(MAKE) clean
	cd cJSON-master && $(MAKE) clean
