CC := gcc
//FLAGS := -g -Wall
FLAGS := -g
LDFLAGS = -L /usr/local/mysql/lib -L /usr/local/Cellar/openssl/1.0.2a-1/lib -lmysqlclient -lcrypto


*.o: *.c
	$(CC) $(FLAGS) -c $^ $(LDFLAGS)
server: Server.o auth.o  serialized.o 
	$(CC) -o server Server.o auth.o serialized.o $(LDFLAGS) 
client: client.o auth.o serialized.o Md5encrypt.o 
	$(CC) -o client $^ $(LDFLAGS)
clean:
	rm *.o client server


