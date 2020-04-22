CC 		= gcc			# ������
AFLAGS 	= -Wall -c -g	# ����
LDFLAGS	= -lpthread		# ���߳�����
OBJS	= main.o
TARGET  = gateway

$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJS):%.o:%.c
	$(CC) $(AFLAGS) $< -o $@

.PHONY:clean
clean:
	rm *.o gateway