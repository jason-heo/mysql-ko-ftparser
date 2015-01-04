CFLAGS = `mysql_config --cflags` `mecab-config --cflags` -DMYSQL_DYNAMIC_PLUGIN -O2
LDFLAGS = `mecab-config --libs`

CC = g++

FTPARSER_SRCS = mysql-ko-ftparser.cc mecab_lib.cc
MECAB_SRCS = mecab_test.cc mecab_lib.cc

.cc.o:
	@echo "+------------------------"
	@echo "| Comipiling $<          "
	@echo "+------------------------"

	$(CC) -c $< $(CFLAGS)

all: mecab_test mysql-ko-ftparser.so

mysql-ko-ftparser.so: $(FTPARSER_SRCS:.cc=.o)
	$(CC) -o $@ -shared -fPIC $^ $(LDFLAGS)

mecab_test: $(MECAB_SRCS:.cc=.o)
	$(CC) -o $@ $^ $(LDFLAGS)

install: mysql-ko-ftparser.so
	cp mysql-ko-ftparser.so `mysql_config --plugindir`

clean:
	rm -f core *.so *.o mecab_test
