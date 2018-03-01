
#include "D:\QNX2\properties.h"

typedef struct _pulse msg_header_t;

void Scale() {
	if (scale<=0) scale=1;
	img_lib_t ilib = NULL;
	img_lib_attach(&ilib);
	int status;
	img_t inputImage, outputImage;
	inputImage.flags = 0;
	inputImage.format = IMG_FMT_PKLE_ARGB1555;
	inputImage.flags |= IMG_FORMAT;
	outputImage.flags = 0;
	outputImage.flags = IMG_W;
	outputImage.format = IMG_FMT_PKLE_ARGB1555;
	if ((status = img_load_file(ilib, GOTDATA, NULL, &inputImage)) != IMG_ERR_OK) {
		printf("img_load_file(%s) failed: %s\n", GOTDATA, strerror(status));
		if (errno) {
			printf("Error: %s\n", strerror( errno ));
		}
	   return;
	}
	outputImage.h = (unsigned)(inputImage.h * scale);
	outputImage.w = (unsigned)(inputImage.h * scale);
	if ((status = img_resize_fs(&inputImage, &outputImage)) != IMG_ERR_OK) {
        printf("img_resize_fs failed: %s\n", strerror(status));
		if (errno) {
			printf("Error: %s\n", strerror( errno ));
		}
        return;
	}
	char* outputFileName="result.bmp";
	if ((status = img_write_file(ilib,outputFileName,NULL,&outputImage)) != IMG_ERR_OK) {
        printf("img_write_file(%s) failed: %s\n", outputFileName, strerror(status));
		if (errno) {
			printf("Error: %s\n", strerror( errno ));
		}
        return;
	}
	img_lib_detach(ilib);
}

void GetSizeInfo(int* rcvid, int chid){
	*rcvid = MsgReceive(chid, &size, sizeof(size), NULL);
	MsgReply( *rcvid, EOK, NULL, 0 );
}

void GetPacket(int chid, int* lastPacket, dataPacket* buffer ){
	int i, rcvid;
	dataPacket packet;
	for(i=0; i<BUFFER_SIZE; i++) {
		rcvid = MsgReceive(chid, &packet, sizeof(packet), NULL);
		if(packet.status == 2) {
			*lastPacket = 1;
			buffer[i].status = packet.status;
			MsgReply( rcvid, EOK, NULL, 0 );
			break;
		}
		if(packet.status == 1) {
			buffer[i].id = packet.id;
			buffer[i].size = packet.size;
			buffer[i].status = packet.status;
			int j;
			for(j=0;j<PACKET_SIZE; j++)	buffer[i].data[j] = packet.data[j];
		}
		MsgReply( rcvid, EOK, NULL, 0 );
	}
}

int FindIndex(dataPacket* buffer) {
	int i, j = -1;
	dataPacket tmp;
	for(i=0; i<BUFFER_SIZE; i++) {
		if (buffer[i].status != 1) continue;
		tmp = buffer[i];
		j = i;
		break;
	}
	if(j == -1) {
		for(i=0; i<BUFFER_SIZE; i++)
			if(buffer[i].status == 2) return -2;
		return j;
	}
	for (i = 0; i < BUFFER_SIZE; i++)
		if (buffer[i].status == 1 && tmp.id > buffer[i].id ){
			tmp=buffer[i];
			j=i;
		}
	return j;
}

int FindNextIndex(int id, dataPacket* buffer) {
	int i = -1;
	for(i=0; i<BUFFER_SIZE; i++)
		if(buffer[i].status == 1 && buffer[i].id == id + 1) return i;
	for(i=0; i<BUFFER_SIZE; i++)
		if(buffer[i].status == 2) return -2;
	return i;
}

void* SecondTread(void* arg){
	msg_header_t header;
	name_attach_t* attach;
	int rcvid, lastPacket = 0;
	if (!(attach = name_attach(NULL,"One",NAME_FLAG_ATTACH_GLOBAL))) {
		clientExitLabel = ECONNREFUSED;
		return (void*)EXIT_FAILURE;
	}
	if ((rcvid = MsgReceive(attach->chid, &header, sizeof(header), NULL)) == -1) {
		ConnectDetach(header.scoid);
		clientExitLabel = ECONNREFUSED;
		return (void*)EXIT_FAILURE;
	}
	if (header.type == _IO_CONNECT ) {
	    MsgReply( rcvid, EOK, NULL, 0 );
	}
	GetSizeInfo(&rcvid, attach->chid);
	while(lastPacket == 0) {
		if (bufferIsFull[clientSelectedBuffer+2]) continue;
		GetPacket(attach->chid, &lastPacket, buffers[clientSelectedBuffer+2] );
		bufferIsFull[clientSelectedBuffer+2] = true;
	}
	name_detach(attach, 0);
	return(0);
}

void* FourthTread(void* arg) {
	FILE *file;
	int choisedBuffer = 0;
	while(clientExitLabel == 0)	{
		if(!bufferIsFull[choisedBuffer+2]) continue;
		int i;
		file = fopen(GOTDATA,"ab");
		int index = FindIndex(buffers[choisedBuffer+2]);
		if(index == -2){
			clientExitLabel = 1;
			fclose(file);
			bufferIsFull[choisedBuffer+2] = false;
			choisedBuffer = 1-choisedBuffer;
			break;
		}
		fwrite(buffers[choisedBuffer+2][index].data, sizeof(char),buffers[choisedBuffer+2][index].size, file );
		buffers[choisedBuffer+2][index].status = 0;
		for(i=0; i<BUFFER_SIZE - 1;i++) {
			index = FindNextIndex(buffers[choisedBuffer+2][index].id, buffers[choisedBuffer+2]);
			if (index==-1) continue;
			if (index==-2) {
				clientExitLabel = 1;
				continue;
			}
			fwrite(buffers[choisedBuffer+2][index].data, sizeof(char),buffers[choisedBuffer+2][index].size, file );
			buffers[choisedBuffer+2][index].status = 0;
		}
		fseek(file, 0, SEEK_END);
		done = ftell(file)*1.0;
		progress = done/size*100;
		printf("progress: %.2f% \n", progress);
		fclose(file);
		bufferIsFull[choisedBuffer+2] = false;
		choisedBuffer = 1-choisedBuffer;
	}
	Scale();
	clientExitLabel = -1;
	return(0);
}

int main(int argc, char *argv[]) {
	printf("enter scale\n");
	scanf("%f", &scale);
	pthread_t secondTread, fourthTread;
	pthread_create(&secondTread, NULL, &SecondTread, NULL );
	pthread_create(&fourthTread, NULL, &FourthTread, NULL );
	while(clientExitLabel != -1) clientSelectedBuffer = (bufferIsFull[clientSelectedBuffer+2] && !bufferIsFull[3-clientSelectedBuffer]) ? 1-clientSelectedBuffer : clientSelectedBuffer;
	remove(GOTDATA);
	sleep(1);
	printf("Image is scaled successfully\n");
	return EXIT_SUCCESS;
}