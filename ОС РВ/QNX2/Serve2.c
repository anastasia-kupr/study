
#include "D:\QNX2\properties.h"

char* Rotate(char* inputFileName, int angle) {
	char* outputFileName = "rorated.bmp";
	img_lib_t ilib = NULL;
	img_lib_attach(&ilib);
	int status;
	img_t inputImage, outputImage;
	inputImage.flags = 0;
	inputImage.format = IMG_FMT_PKLE_ARGB1555;
	inputImage.flags |= IMG_FORMAT;
	outputImage.flags = 0;
	outputImage.format = IMG_FMT_PKLE_ARGB1555;
	outputImage.flags |= IMG_FORMAT;
	if ((status = img_load_file(ilib, inputFileName, NULL, &inputImage)) != IMG_ERR_OK) {
        printf("img_load_file(%s) has been failed: %s\n", inputFileName, strerror(status));
		if (errno) {
			printf("Error: %s\n", strerror( errno ));
		}
		return inputFileName;
	}
	img_fixed_t ang = (int)((angle * 3.141592 / 180) * 65536.0);
	if((status = img_rotate_ortho(&inputImage, &outputImage, ang))!= IMG_ERR_OK){
		printf("img_rotate_ortho has been failed: %s\n", strerror(status));
		if (errno) {
			printf("Error: %s\n", strerror( errno ));
		}
		return inputFileName;
	}
	if ((status = img_write_file(ilib,outputFileName,NULL,&outputImage)) != IMG_ERR_OK) {
        printf("img_write_file(%s) has been failed: %s\n", outputFileName, strerror(status));
		if (errno) {
			printf("Error: %s\n", strerror( errno ));
		}
		return inputFileName;
	}
	img_lib_detach(ilib);
	return outputFileName;
}

int SendSizeInfo(int serverID) {
	int reply;
	MsgSend(serverID, &size, sizeof(size), &reply, sizeof(int));
	while(reply != EOK)	MsgSend(serverID, &size, sizeof(size), &reply, sizeof(int));
	return reply;
}

void MixPackets(int* index)
{
	int temp;
	int i;
	for(i = 0; i < BUFFER_SIZE; i++) {
	   temp = index[i];
	   index[i] = index[BUFFER_SIZE - 1 - i];
	   index[BUFFER_SIZE - 1 - i] = temp;
	}
}

void SendPacket(int* index, int serverID, dataPacket* buffer ){
	int reply;
	int i;
	for(i=0; i<BUFFER_SIZE;i++)	{
		if(buffer[index[i]].status == 0) continue;
		MsgSend(serverID, &buffer[index[i]], sizeof(buffer[index[i]]), &reply, sizeof(int));
		while(reply != EOK) MsgSend(serverID, &buffer[index[i]], sizeof(buffer[index[i]]), &reply, sizeof(int));
		buffer[index[i]].status = 0;
	}
}

void SendLastPacket(int serverID){
	int reply;
	dataPacket lastPacket;
	lastPacket.status = 2;
	MsgSend(serverID, &lastPacket, sizeof(lastPacket), &reply, sizeof(int));
}

int FindFreePlace(dataPacket* buffer)
{
	int i;
	for(i = 0; i<BUFFER_SIZE; i++) {
		if(buffer[i].status!=0) continue;
		return i;
	}
	return -1;
}

int SetToBuffer(FILE* file, int freePlace, dataPacket* buffer, int* id){
	int status = 1,
	stop = 0;
	int i;		
	for(i=0; i<PACKET_SIZE; i++) {
		status = fread(byte, sizeof(char), 1, file);
		if(status != 1) {
			buffer[freePlace].status = 1;
			buffer[freePlace].id = *id;
			bufferIsFull[selectedBuffer] = true;
			stop = 1;
			break;
		}
		buffer[freePlace].size  = i+1;
		buffer[freePlace].data[i] = byte[0];
	}
	if(stop == 1) return status;
	buffer[freePlace].status = 1;
	buffer[freePlace].id = *id;
	*id = *id + 1;
	return status;
}

void*  FirstThread( void*  arg ) {
	int serverID = -1;
	while(serverID == -1) serverID = name_open("One", NAME_FLAG_ATTACH_GLOBAL);
	int bufferNumber = 0, index[BUFFER_SIZE];
	bool transferIsFinished = false;
	int i;
	for (i = 0; i < BUFFER_SIZE; i++) index[i] = i;
	SendSizeInfo(serverID);
	while(!transferIsFinished) {
		if(bufferIsFull[bufferNumber]) {
			MixPackets(index);
			SendPacket(index, serverID, buffers[bufferNumber]);
			bufferIsFull[bufferNumber] = false;
			bufferNumber = 1 - bufferNumber;
		}
		if(!fileIsRead || bufferIsFull[bufferNumber] || bufferIsFull[1-bufferNumber]) continue;
		SendLastPacket(serverID);
		transferIsFinished = true;
	}
	name_close(serverID);
	exitLabel = -1;
}

void*  ThirdTread( void*  arg ) {
	FILE* file = fopen( sentFileName, "rb" );
	if (file == NULL) {
		printf("No data");
		return 0;
	}
	int stop = 0, id = 0;
	bool result = true;
	while(result && (exitLabel == EOK)) {
		if(bufferIsFull[selectedBuffer]) continue;
		int freePlace = FindFreePlace(buffers[selectedBuffer]);
		if(freePlace == -1) {
			bufferIsFull[selectedBuffer] = true;
		} else result = (bool)SetToBuffer(file, freePlace, buffers[selectedBuffer], &id);
	}
	fclose(file);
	fileIsRead = true;
}

int main(int argc, char *argv[]){
	int angle;
	printf("enter angle\n");
	scanf("%d", &angle);
	strncat(sentFileName, Rotate(INPUT,angle), 20);
	FILE* tmp;
	tmp = fopen(sentFileName,"rb");
	fseek(tmp,0,SEEK_END);
	size = ftell(tmp);
	fclose(tmp);
	pthread_t firstThread, thirdTread;
	pthread_create(&thirdTread, NULL, &ThirdTread, NULL );
	pthread_create(&firstThread, NULL, &FirstThread, NULL );
	while(exitLabel == EOK) selectedBuffer = (bufferIsFull[selectedBuffer] && !bufferIsFull[1-selectedBuffer]) ? 1-selectedBuffer : selectedBuffer;
	sleep(1);
	return EXIT_SUCCESS;
}