CC=gcc

SRCS =	main.c
 
OBJS = $(SRCS:.c=.o)

TARGET = Temperature

CFLAG_WANNING = -W -Wall -Wno-unused-parameter -Wconversion -Wshadow -Wcast-qual
CFLAG = -c -DNO_SSL -DUSE_WEBSOCKET $(CFLAG_WANNING)
LFLAG = -lpthread -lrt -lwiringPi -lmysqlclient 
LPATH  = -L/usr/lib -L/usr/local/lib

# Debug build settings
DBGDIR = debug
DBGEXE = $(DBGDIR)/$(TARGET)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
CFLAGD = $(CFLAG) -g -DDEBUG

# Release build settings
RELDIR = release
RELEXE = $(RELDIR)/$(TARGET)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
CFLAGS = $(CFLAG) -O

.PHONY: all version clean debug prep release new cp $(DBGEXE) $(RELEXE)

# Default build
all : release

version:
	echo '$(DATE)' > version.cpp

# Debug rules
debug: prep version $(DBGEXE) cp

$(DBGEXE): $(DBGOBJS)
	$(CC) -o $(TARGET) $^ $(LFLAG) $(LPATH) 

$(DBGDIR)/%.o: %.c
	$(CC) -o $@ $< -c $(INC) $(CFLAGD) 

# Release rules
release: prep version $(RELEXE) cp

$(RELEXE): $(RELOBJS)
	$(CC) -o $(TARGET) $^ $(LFLAG) $(LPATH) 

$(RELDIR)/%.o: %.c
	$(CC) -o $@ $< -c $(INC) $(CFLAGS)

# Other rules
prep:
	@mkdir -p $(DBGDIR) $(RELDIR)

#삭제
clean :
	rm -f $(TARGET) core* *.log
	rm -f $(RELDIR)/*.o $(RELDIR)/*.bak $(RELDIR)/*.log 
	rm -f $(DBGDIR)/*.o $(DBGDIR)/*.bak $(DBGDIR)/*.log 

#새로
new : clean all

newdebug : clean debug

#copy
cp :
	@mkdir -p ../bin
	cp $(TARGET) ../bin/$(TARGET)

########################################################################
#$< 는 .o 파일보다 더 최근에 갱신된 .c 파일은 자동적으로 컴파일이 된다.
#.cpp.o :
#	$(CC) $< $(INC) $(CFLAGS)

	