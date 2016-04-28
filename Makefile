CC := gcc
//FLAGS := -g -Wall
FLAGS := -g
*.o: *.c *.h
	$(CC) $(FLAGS) -c $^ -I/usr/include/mysql -lmysqlclient -lcrypto
server: Server.o auth.o deserialized.o serialized.o 
	$(CC) -o server Server.o auth.o deserialized.o serialized.o -lmysqlclient
client: client.o auth.o deserialized.o serialized.o Md5encrypt.o
	$(CC) -o client $^ -lcrypto -lmysqlclient
	

clean:
	rm *.o


