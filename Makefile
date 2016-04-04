CC		= gcc -g
CXX		= g++ -std=c++11 -g

TARGET	= wave
SRCS	= wave.c
HDRS	= wave.h
OBJS	= $(subst .c,.o,$(SRCS))
RMFILES	= $(OBJS) $(TARGET)

$(TARGET): $(SRCS) $(HDRS) $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

%.o: %.c $(HDRS)
	$(CXX) -o $@ -c $<

.PHONY: clean
clean:
	rm -f $(RMFILES) test*.wav
