#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define BUF_SIZE 500

void listenOnNewThread(int socketfd);
void readConsole(int socketfd);
void listenAndPrint(int socketfd);

int main(int argc, char *argv[])
{
	struct sockaddr_in server_address;
	char msg[BUF_SIZE];
	char buf[BUF_SIZE];
	int s = socket(AF_INET, SOCK_STREAM, 0);
	int status = 0;
	int portNum = 3500;
	
	if(argv[1] != NULL)
	{
		//printf(argv[1]);
		portNum = atoi(argv[1]);
	}

	if(s < 0)
	{
		perror("Socket creation failure");
		exit(EXIT_FAILURE);
	}

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(portNum); // Instead of PORT_NUM use argv[?] to allow any vacant port
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_zero[8] = '\0';

	status = connect(s, (struct sockaddr*)&server_address, sizeof(server_address));

	if(status < 0)
	{
		perror("Couldn't connect to the server");
		exit(EXIT_FAILURE);
	}

	/*
	   while(1)//!(strcmp(msg, "exit")))
	   {
	   printf("Message: \n");
	   fgets(msg, BUF_SIZE, stdin);
	//msg[BUF_SIZE - 1] = '\0';
	send(s, msg, strlen(msg), 0);
	//read(s, buf, 100);
	printf("Message from client: %s\n", msg);
	}
	*/

	listenOnNewThread(s);
	readConsole(s);


	close(s);

	return 0;
}

void listenAndPrint(int socketfd)
{
	char buf[BUF_SIZE];
	ssize_t amount_rec;

	while(1)
	{
		amount_rec = recv(socketfd, buf, BUF_SIZE, 0);

		if(amount_rec > 0)
		{
			buf[amount_rec] = 0;
			printf("%s\n", buf);
		}
		else if(amount_rec = 0)
		{
			break;
		}
	}
	close(socketfd);
}

void readConsole(int socketfd)
{
	char *line, *welcome = NULL;
	size_t lineSize = 0;
	char buf[BUF_SIZE];
	ssize_t charCount;
	char *name = NULL;
	size_t nameSize = 0;
	ssize_t nameCount, welCount;
	ssize_t amountSent;

	printf("Enter your username: ");
	nameCount = getline(&name, &nameSize, stdin);

	name[nameCount - 1] = 0;
	//welcome = ("Welcome to the server %s!", name);
	//amountSent = send(socketfd, welcome, sizeof(welcome), 0);

	while(1)
	{
		charCount = getline(&line, &lineSize, stdin);
		line[charCount - 1] = 0;

		sprintf(buf, "%s: %s", name, line);

		if(charCount > 0)
		{
			if(strcasecmp(line, "exit") == 0)
			{
				break;
			}
			amountSent = send(socketfd, buf, strlen(buf), 0);
		}
	}
}

void listenOnNewThread(int socketfd)
{
	pthread_t id;
	pthread_create(&id, NULL, listenAndPrint, socketfd);
}
