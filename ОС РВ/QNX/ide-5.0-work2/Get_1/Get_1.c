#include <stdlib.h>
#include <stdio.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <errno.h>
#include <time.h>
#include <img/img.h>

#define LENGTH_BUFFER 16
#define LENGTH_WORD 2048
#define SCALE 10

#define NAME_OUT_FILE "data.dat"

/* We specify the header as being at least a pulse */
typedef struct _pulse msg_header_t;

typedef struct
{
	int id;
	char data[LENGTH_WORD];
	int length;
	int status;
} packet;

typedef struct
{
	char      file_name[30];
	long int  size;
} info_file;

info_file info;

int threadBuf = 1;
int buffer1Full = 0;
int buffer2Full = 0;
int EXITCODE = 0;

packet buffer1[LENGTH_BUFFER];
packet buffer2[LENGTH_BUFFER];

void*  ThreadTwo( void*  arg ){
	msg_header_t header;

	name_attach_t* attach;

	int rcvid, ENDPACKET = 0;

	if (!( attach = name_attach(NULL,"One",NAME_FLAG_ATTACH_GLOBAL) ) )
	{
		EXITCODE = ECONNREFUSED;
		return EXIT_FAILURE;
	}

	rcvid = MsgReceive(attach->chid, &header, sizeof(header), NULL);

	if (rcvid == -1)
	{
		ConnectDetach(header.scoid);
		EXITCODE = ECONNREFUSED;
		return EXIT_FAILURE;
	}

	// name_open() sends a connect message, must EOK this.
	if (header.type == _IO_CONNECT ) {
	    MsgReply( rcvid, EOK, NULL, 0 );
	}

	GetInformationMsgFromServer(&rcvid, attach->chid);

	while(ENDPACKET == 0)
	{
		switch(threadBuf)
		{
			case 1:
				if(buffer1Full == 0)
				{
					GetDataFromServer(attach->chid, &ENDPACKET, buffer1 );

					buffer1Full = 1;
				}
				break;
			case 2:
				if(buffer2Full == 0)
				{
					GetDataFromServer(attach->chid, &ENDPACKET, buffer2 );

					buffer2Full = 1;
				}
				break;
		}
	}

	/* Remove the name from the space */
	name_detach(attach, 0);
	return(0);
}

void*  ThreadFour( void*  arg ){
	FILE *file;
	file = fopen(NAME_OUT_FILE,"wb");
	fclose(file);

	clock_t ca, ce;

	int numBuf = 1;

	ca = clock();

	while(EXITCODE == 0)
	{
		switch(numBuf)
		{
			case 1:
				if(buffer1Full == 1)
				{
					int i;

					file = fopen(NAME_OUT_FILE,"ab");

					int x = ReturnIndexMinIdInBuffer(buffer1);

					if(x == -2){
						EXITCODE = 1;
						fclose(file);
						buffer1Full = 0;
						numBuf = 2;
						break;
					}

					fwrite(buffer1[x].data, sizeof(char),buffer1[x].length, file );
					buffer1[x].status = 0;

					for(i=0; i<LENGTH_BUFFER - 1;i++)
					{
						x = FindNextIndexId(buffer1[x].id,buffer1);

						switch(x)
						{
							case -1:
								break;
							case -2:
								EXITCODE = 1;
								break;
							default:
								fwrite(buffer1[x].data, sizeof(char),buffer1[x].length, file );
								buffer1[x].status = 0;
								break;
						}
					}

					fseek(file,0,SEEK_END);
					printf("%ld / %ld byte \n", ftell(file), info.size);

					fclose(file);
					buffer1Full = 0;
					numBuf = 2;
				}
				break;
			case 2:
				if(buffer2Full == 1)
				{
					int i;

					file = fopen(NAME_OUT_FILE,"ab");

					int x = ReturnIndexMinIdInBuffer(buffer2);

					if(x==-2){
						EXITCODE = 1;
						fclose(file);
						buffer2Full = 0;
						numBuf = 1;
						break;
					}

					fwrite(buffer2[x].data, sizeof(char),buffer2[x].length, file );
					buffer2[x].status = 0;

					for(i=0; i<LENGTH_BUFFER - 1;i++)
					{
						x = FindNextIndexId(buffer2[x].id,buffer2);

						switch(x)
						{
							case -1:
								break;
							case -2:
								EXITCODE = 1;
								break;
							default:
								fwrite(buffer2[x].data, sizeof(char),buffer2[x].length, file );
								buffer2[x].status = 0;
								break;
						}
					}

     				fseek(file,0,SEEK_END);
     				printf("%ld / %ld byte \n", ftell(file), info.size);

					fclose(file);
					buffer2Full = 0;
					numBuf = 1;
				}
				break;
		}
	}

	ce = clock();

	printf("time spent on CPU: %lf sec\n", (ce - ca)/(double)CLOCKS_PER_SEC);

	GetImageAndScale(info.file_name, SCALE);

	EXITCODE = -1;
	return(0);
}

int main(int argc, char *argv[]) {

	pthread_t thread2_tid, thread4_tid;

	pthread_create(&thread2_tid, NULL, &ThreadTwo, NULL );

	pthread_create(&thread4_tid, NULL, &ThreadFour, NULL );

	while(EXITCODE != -1)
	{
		switch(threadBuf)
		{
		case 1:
			threadBuf = (buffer1Full == 1 && buffer2Full == 0) ? 2 : 1;
			break;
		case 2:
			threadBuf = (buffer2Full == 1 && buffer1Full == 0) ? 1 : 2;
			break;
		}
	}

	RenameInputFile(info.file_name);

	sleep(1);
	printf("File is received successfully!\n");

	return EXIT_SUCCESS;
}

int GetImageAndScale(char* file_name, int value){
	img_lib_t ilib = NULL;

	img_lib_attach(&ilib);

	int rc;

	img_t img, imgOut;

	/* initialize an img_t by setting its flags to 0 */
	img.flags = 0;
	img.format = IMG_FMT_PKLE_ARGB1555;
	img.flags |= IMG_FORMAT;

	imgOut.flags = IMG_W;
	imgOut.format = IMG_FMT_PKLE_ARGB1555;

	char filename[30];

	strcpy(filename, NAME_OUT_FILE);

	if ((rc = img_load_file(ilib, filename, NULL, &img)) != IMG_ERR_OK) {
	   fprintf(stderr, "img_load_file(%s) failed: %d\n", filename, rc);
	   return -2;
	}

	if(value != 0){
		if(value < 0){
			imgOut.h = img.h / (value * -1);
			imgOut.w = img.w / (value * -1);
		}
		else{
			imgOut.h = img.h * value;
			imgOut.w = img.h * value;
		}
	} else {
		imgOut.h = img.h;
		imgOut.w = img.w;
	}

	if ((rc = img_resize_fs(&img, &imgOut)) != IMG_ERR_OK) {
        fprintf(stderr, "img_resize_fs failed: %d\n", rc);
        return -3;
	}

	char outfile[60];

	strcpy(outfile, "scale_");

	strncat(outfile,file_name,30);

	if ((rc = img_write_file(ilib,outfile,NULL,&imgOut)) != IMG_ERR_OK) {
        fprintf(stderr, "img_write_file(%s) failed: %d\n", outfile, rc);
        return -4;
	}

	printf("Writed Image (%s)\n", outfile);

	/* for our purposes we''re done with the img lib */
	img_lib_detach(ilib);

	return 0;
}

void GetDataFromServer(int chid, int* end_packet, packet* buffer_r ){
	int i, rcvid;

	packet buffer;

	for(i=0; i<LENGTH_BUFFER; i++)
	{
		rcvid = MsgReceive(chid, &buffer, sizeof(buffer), NULL);

		if(buffer.status == 250494)
		{
			*end_packet 	   = 1;
			buffer_r[i].status = buffer.status;
			MsgReply( rcvid, EOK, NULL, 0 );
			break;
		}

		if(buffer.status == 1)
		{
			buffer_r[i].id 	   = buffer.id;
			buffer_r[i].length = buffer.length;
			buffer_r[i].status = buffer.status;
			int j;
			for(j=0;j<LENGTH_WORD; j++){
				buffer_r[i].data[j] = buffer.data[j];
			}
		}

		MsgReply( rcvid, EOK, NULL, 0 );
	}
}

void GetInformationMsgFromServer(int* rcvid, int chid){
	*rcvid = MsgReceive(chid, &info, sizeof(info), NULL);
	MsgReply( *rcvid, EOK, NULL, 0 );
}

void RenameInputFile(char* file_name)
{
	rename(NAME_OUT_FILE, file_name);
}

int ReturnIndexMinIdInBuffer(packet* src)
{
	int i, j = -1;

	packet tmp;

	for(i=0; i<LENGTH_BUFFER; i++){
		if(src[i].status == 1){
			tmp = src[i];
			j = i;
			break;
		}
	}

	if(j == -1){
		for(i=0; i<LENGTH_BUFFER; i++){
			if(src[i].status == 250494) return -2;
		}

		return j;
	}


	for (i = 0; i < LENGTH_BUFFER; i++)
	{
		if (src[i].status == 1 && tmp.id > src[i].id ){
			tmp=src[i];
			j=i;
		}
	}

	return j;
}

int FindNextIndexId(int id, packet* src)
{
	int i = -1;

	for(i=0; i<LENGTH_BUFFER; i++){
		if(src[i].status == 1 && src[i].id == id + 1) return i;
	}

	for(i=0; i<LENGTH_BUFFER; i++){
		if(src[i].status == 250494) return -2;
	}
	return i;
}
