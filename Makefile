CC 		= gcc								# 编译器
CFLAGS 	= -Wall -c -g
LDFLAGS	= -lpthread							# 多线程依赖
SROUCES = $(wildcard ./src/*.c) 			# 源文件目录
OBJS	= $(patsubst %.c, %.o, $(SROUCES))
INCLUDES= -I ./include/ 					# 头文件目录
TARGET  = gateway							# 生成项目名

$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	rm ./src/*.o

%.o:%.c
	$(CC) $(INCLUDES) -c $< -o $@ $(CFLAGS)

clean:
	rm $(OBJ) $(TARGET)

tc:
	$(CC) ./test/client.c -o client
	$(CC) ./test/select_server3.c -o server3