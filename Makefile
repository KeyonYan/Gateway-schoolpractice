CC 		= gcc			# 编译器
AFLAGS 	= -Wall -c -g	# 参数
LDFLAGS	= -lpthread		# 多线程依赖
OBJS	= main.o
TARGET  = gateway

$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJS):%.o:%.c
	$(CC) $(AFLAGS) $< -o $@

.PHONY:clean
clean:
	rm *.o gateway