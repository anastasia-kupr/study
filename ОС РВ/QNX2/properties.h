#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <img/img.h>
#include <stdbool.h>
#define BUFFER_SIZE 16
#define PACKET_SIZE 2048
#define INPUT "pic1.bmp"
#define GOTDATA "tmp.dat"

typedef struct {
	int  id;
	char data[PACKET_SIZE];
	int  size;
	int  status;
} dataPacket;

dataPacket buffers[4][BUFFER_SIZE];
bool bufferIsFull[4] = {false, false, false , false};
int selectedBuffer = 0;
int clientSelectedBuffer = 0;
int exitLabel = 0;
int clientExitLabel = 0;
bool fileIsRead = false;
char sentFileName[20];
float scale;
float progress;
float done;
long int size;
char* byte[1];