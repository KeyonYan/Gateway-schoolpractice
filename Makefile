CC 		= gcc			# ������
CFLAGS 	= -Wall -c -g
LDFLAGS	= -lpthread		# ���߳�����
SROUCES = $(wildcard ./*.c)
OBJS	= $(patsubst %.c, %.o, $(SROUCES))
INCLUDES= -I ./
TARGET  = gateway

$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) $(INCLUDES) -c $< -o $@ $(CFLAGS)

clean:
	rm $(OBJ) $(TARGET)
