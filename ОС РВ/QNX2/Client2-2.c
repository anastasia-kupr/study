
#include "D:\QNX2\properties.h"

typedef struct _pulse msg_header_t;
infoPacket info;

void GetDataFromServer(int chid, int* lastPacket, dataPacket* buffer_r ){
	int i, rcvid;
	dataPacket buffer;
	for(i=0; i<BUFFER_SIZE; i++) {
		rcvid = MsgReceive(chid, &buffer, sizeof(buffer), NULL);
		if(buffer.status == 2) {
			*lastPacket = 1;
			buffer_r[i].status = buffer.status;
			MsgReply( rcvid, EOK, NULL, 0 );
			break;
		}
		if(buffer.status == 1) {
			buffer_r[i].id = buffer.id;
			buffer_r[i].size = buffer.size;
			buffer_r[i].status = buffer.status;
			int j;
			for(j=0;j<PACKET_SIZE; j++)	buffer_r[i].data[j] = buffer.data[j];
		}
		MsgReply( rcvid, EOK, NULL, 0 );
	}
}

void GetInfo(int* rcvid, int chid){
	*rcvid = MsgReceive(chid, &info, sizeof(info), NULL);
	MsgReply( *rcvid, EOK, NULL, 0 );
}

void*  SecondTread( void*  arg ){
	msg_header_t header;
	name_attach_t* attach;
	int rcvid, ENDPACKET = 0;
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
	GetInfo(&rcvid, attach->chid);
	while(ENDPACKET == 0) {
		if (bufferIsFull[clientSelectedBuffer+1]) continue;
		GetDataFromServer(attach->chid, &ENDPACKET, buffers[clientSelectedBuffer+1] );
		bufferIsFull[clientSelectedBuffer+1] = true;
	}
	name_detach(attach, 0);
	return(0);
}

int FindNextIndexId(int id, dataPacket* src) {
	int i = -1;
	for(i=0; i<BUFFER_SIZE; i++)
		if(src[i].status == 1 && src[i].id == id + 1) return i;
	for(i=0; i<BUFFER_SIZE; i++)
		if(src[i].status == 2) return -2;
	return i;
}

void Scale() {
	if (scale==0) scale=1;
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
	if ((status = img_load_file(ilib, OUTPUT, NULL, &inputImage)) != IMG_ERR_OK) {
		printf("img_load_file(%s) failed: %s\n", OUTPUT, strerror(status));
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

int ReturnIndexMinIdInBuffer(dataPacket* src)
{
	int i, j = -1;

	dataPacket tmp;

	for(i=0; i<BUFFER_SIZE; i++){
		if(src[i].status == 1){
			tmp = src[i];
			j = i;
			break;
		}
	}

	if(j == -1){
		for(i=0; i<BUFFER_SIZE; i++){
			if(src[i].status == 2) return -2;
		}

		return j;
	}


	for (i = 0; i < BUFFER_SIZE; i++)
	{
		if (src[i].status == 1 && tmp.id > src[i].id ){
			tmp=src[i];
			j=i;
		}
	}

	return j;
}

void*  FourthTread( void*  arg ) {
	FILE *file;
//	file = fopen(OUTPUT,"wb");
//	fclose(file);
	int numBuf = 1;
	while(clientExitLabel == 0)
	{
		switch(numBuf)
		{
			case 1:
				if(bufferIsFull[2]) {
					int i;
					file = fopen(OUTPUT,"ab");
					int x = ReturnIndexMinIdInBuffer(buffers[2]);
					if(x == -2){
						clientExitLabel = 1;
						fclose(file);
						bufferIsFull[2] = false;
						numBuf = 2;
						break;
					}
					fwrite(buffers[2][x].data, sizeof(char),buffers[2][x].size, file );
					buffers[2][x].status = 0;
					for(i=0; i<BUFFER_SIZE - 1;i++) {
						x = FindNextIndexId(buffers[2][x].id,buffers[2]);
						switch(x) {
							case -1:
								break;
							case -2:
								clientExitLabel = 1;
								break;
							default:
								fwrite(buffers[2][x].data, sizeof(char),buffers[2][x].size, file );
								buffers[2][x].status = 0;
								break;
						}
					}
					fseek(file,0,SEEK_END);
					printf("%ld / %ld byte \n", ftell(file), info.size);
					fclose(file);
					bufferIsFull[2] = false;
					numBuf = 2;
				}
				break;
			case 2:
				if(bufferIsFull[3]) {
					int i;
					file = fopen(OUTPUT,"ab");
					int x = ReturnIndexMinIdInBuffer(buffers[3]);
					if(x==-2){
						clientExitLabel = 1;
						fclose(file);
						bufferIsFull[3] = false;
						numBuf = 1;
						break;
					}
					fwrite(buffers[3][x].data, sizeof(char),buffers[3][x].size, file );
					buffers[3][x].status = 0;
					for(i=0; i<BUFFER_SIZE - 1;i++) {
						x = FindNextIndexId(buffers[3][x].id,buffers[3]);
						switch(x) {
							case -1:
								break;
							case -2:
								clientExitLabel = 1;
								break;
							default:
								fwrite(buffers[3][x].data, sizeof(char),buffers[3][x].size, file );
								buffers[3][x].status = 0;
								break;
						}
					}
     				fseek(file,0,SEEK_END);
     				printf("%ld / %ld byte \n", ftell(file), info.size);
					fclose(file);
					bufferIsFull[3] = false;
					numBuf = 1;
				}
				break;
		}
	}
	Scale();
	clientExitLabel = -1;
	return(0);
}


void RenameInputFile(char* fileName)
{
	rename(OUTPUT, fileName);
}

int main(int argc, char *argv[]) {
	printf("enter scale\n");
	scanf("%f", &scale);
	pthread_t secondTread, fourthTread;

	pthread_create(&secondTread, NULL, &SecondTread, NULL );

	pthread_create(&fourthTread, NULL, &FourthTread, NULL );

	while(clientExitLabel != -1)
	{
		switch(clientSelectedBuffer)
		{
		case 1:
			clientSelectedBuffer = (bufferIsFull[2] && !bufferIsFull[3]) ? 2 : 1;
			break;
		case 2:
			clientSelectedBuffer = (bufferIsFull[3] && !bufferIsFull[2]) ? 1 : 2;
			break;
		}
	}

	RenameInputFile(info.fileName);

	sleep(1);
	printf("Image is scaled successfully\n");
	return EXIT_SUCCESS;
}