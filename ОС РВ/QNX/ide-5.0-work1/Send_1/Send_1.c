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

#define LENGTH_BUFFER 16
#define LENGTH_WORD 2048
#define NAME_IN_FILE "input_2.bmp"
#define DEGREE 90


typedef struct
{
	int  id;                // Идентификатор части.
	char data[LENGTH_WORD]; // Содержимое.
	int  length;			// Длина содержимого (т.к. последний пакет может содержать меньше данных чем массив data).
	int  status;			// Статус части (занят или свободен).
} packet;

typedef struct
{
	char      file_name[30];
	long int  size;
} info_file;

info_file info;
packet    buffer[2][LENGTH_BUFFER];

int buffer_full[2] = {0 , 0};
int select_buf     = 1;
int exit_code      = 0;
int end_read_file  = 0;
char file_to_send[30];

// Периодически (100 мс) выбирает из буфера часть и отправляет её пакетами (один пакет =
// одно сообщение) по сети потоку №2.
void*  ThreadOne( void*  arg )
{
	int server_coid = -1;

	while(server_coid == -1)
	{
		server_coid = name_open("One", NAME_FLAG_ATTACH_GLOBAL);
	}

	// Буфер с каким работаем.
	int num_buf = 1;

	// Указывает на конец передачи.
	int end_transmitted = 0;

	// Массив индексов необходим для смешивания пакетов.
	int mas_index[LENGTH_BUFFER];

	int i;

	for (i=0; i < LENGTH_BUFFER; i++)
		mas_index[i] = i;

	SendInformationMsgToClient(server_coid);

	while(end_transmitted != 1)
	{
		switch(num_buf)
		{
			case 1:
				if(buffer_full[0] == 1)
				{
					RandomArrayPacket(mas_index); // Перемешиваем пакеты.

					SendPacketToClient(mas_index, server_coid, buffer[0]);

					buffer_full[0] = 0; num_buf = 2;
				}
				if(end_read_file == 1 && buffer_full[0] == 0 && buffer_full[1] == 0)
				{
					end_transmitted = SendLastPacketToClient(server_coid);
				}
				break;
			case 2:
				if(buffer_full[1] == 1)
				{
					RandomArrayPacket(mas_index);

					SendPacketToClient(mas_index, server_coid, buffer[1]);

					buffer_full[1] = 0; num_buf = 1;
				}
				if(end_read_file == 1 && buffer_full[1] == 0 && buffer_full[0] == 0)
				{
					end_transmitted = SendLastPacketToClient(server_coid);
				}
				break;
		} // EndSwitch.
	} // EndWhile.

	name_close(server_coid);
	exit_code = -1;

	printf("End send file, thread 1 - CLOSE\n");

	return( 0 );
}

// Считывает входной файл, разбивает его на части и помещает их в буфер.
void*  ThreadThree( void*  arg )
{
	FILE* file = fopen( file_to_send, "rb" );

	if( file != NULL )
	{
		int i, stop = 0, id = 0;

		char* byte[1];

		int result_read = 1;

		while(result_read == 1 && exit_code == EOK)
		{
			switch(select_buf)
			{
				case 1:
					if(buffer_full[0] == 0)
					{
						int free_idbuf = GetFreeIdPlaceBuffer(buffer[0]);

						if(free_idbuf != -1)
						{
							result_read = SetDataToBuffer(file, free_idbuf, buffer[0], &id, byte, 0);
						}
						else buffer_full[0] = 1;
					}
					break;
				case 2:
					if(buffer_full[1] == 0)
					{
						int free_idbuf = GetFreeIdPlaceBuffer(buffer[1]);

						if(free_idbuf != -1)
						{
							result_read = SetDataToBuffer(file, free_idbuf, buffer[1], &id, byte, 1);
						}
						else buffer_full[1] = 1;
					}
					break;
			}
		}

		fclose(file); end_read_file = 1;
	}

	printf("End read file, thread 3 - CLOSE\n");

	return( 0 );
}

char* GetImageAndRotate(char* fileName, int degree);

int main(int argc, char *argv[]){

	strncat(file_to_send, GetImageAndRotate(NAME_IN_FILE,DEGREE), 30);

	SetInfoFile(file_to_send);

	pthread_t thread1_tid, thread3_tid;

	pthread_create(&thread3_tid, NULL, &ThreadThree, NULL );

	pthread_create(&thread1_tid, NULL, &ThreadOne, NULL );

	while(exit_code == EOK)
	{
		switch(select_buf)
		{
		case 1:
			select_buf = (buffer_full[0] == 1 && buffer_full[1] == 0) ? 2 : 1;
			break;
		case 2:
			select_buf = (buffer_full[1] == 1 && buffer_full[0] == 0) ? 1 : 2;
			break;
		}
	}

	sleep(1);
	printf("File has been sent!");
	sleep(1);

	return EXIT_SUCCESS;
}

char* GetImageAndRotate(char* fileName, int degree)
{
	img_lib_t ilib = NULL;

	img_lib_attach(&ilib);

	int rc;

	img_t img, imgOut;

	/* initialize an img_t by setting its flags to 0 */
	img.flags = 0;
	img.format = IMG_FMT_PKLE_ARGB1555;
	img.flags |= IMG_FORMAT;

	imgOut.flags = 0;
	imgOut.format = IMG_FMT_PKLE_ARGB1555;
	imgOut.flags |= IMG_FORMAT;

/*	char* type;

	switch(fileType){
	case 1:
		type = ".bmp";
		break;
	case 2:
		type = ".tga";
		break;
	default:
		return fileName;
		break;
	}
*/

	char filename[30];

	strcpy(filename, fileName);

	if ((rc = img_load_file(ilib, filename, NULL, &img)) != IMG_ERR_OK) {
        fprintf(stderr, "img_load_file(%s) failed: %d\n", filename, rc);
        return fileName;
	}

	fprintf(stdout, "Uploaded Image is %dx%dx%d (%s)\n", img.w, img.h, IMG_FMT_BPP(img.format), filename);

	img_fixed_t angel = (int)((degree * 3.141592 / 180) * 65536.0);

	if((rc = img_rotate_ortho(&img, &imgOut, angel))!= IMG_ERR_OK){
		fprintf(stderr, "img_rotate_ortho failed: %d\n", rc);
		return fileName;
	}

	printf("Rotated Image\n");

	char outfile[60];

	strcpy(outfile, "out_");

	strncat(outfile,filename,30);

	if ((rc = img_write_file(ilib,outfile,NULL,&imgOut)) != IMG_ERR_OK) {
        fprintf(stderr, "img_write_file(%s) failed: %d\n", outfile, rc);
        return fileName;
	}

	printf("Writed Image (%s)\n", outfile);

	/* for our purposes we''re done with the img lib */
	img_lib_detach(ilib);

	return outfile;
}

int SetDataToBuffer(FILE* file, int free_idbuf, packet* buffer, int* id_packet, char* byte, int num_buf){

	int i, rc = 1, stop = 0;

	for(i=0; i<LENGTH_WORD; i++)
	{
		rc = fread(byte, sizeof(char), 1, file);

		if(rc == 1)
		{
			buffer[free_idbuf].length  = i+1;
			buffer[free_idbuf].data[i] = byte[0];
		}
		else{
			buffer[free_idbuf].status  = 1;
			buffer[free_idbuf].id 	   = *id_packet;
			buffer_full[num_buf] 	   = 1;
			stop 					   = 1;
			break;
		}

	}

	if(stop != 1)
	{
		buffer[free_idbuf].status = 1;
		buffer[free_idbuf].id 	  = *id_packet;
		*id_packet                = *id_packet + 1;
	}

	return rc;
}

int SendInformationMsgToClient(int server_coid)
{
	int reply;

	// Отправляем пакет.
	MsgSend(server_coid, &info, sizeof(info), &reply, sizeof(int));

	while(reply != EOK)
	{
		MsgSend(server_coid, &info, sizeof(info), &reply, sizeof(int));
	}

	return reply;
}

void SendPacketToClient(int* mas_index, int server_coid, packet* buffer ){

	int i, reply_client;

	for(i=0; i<LENGTH_BUFFER;i++)
	{
		if(buffer[mas_index[i]].status != 0){
			// Отправляем пакет.
			MsgSend(server_coid, &buffer[mas_index[i]], sizeof(buffer[mas_index[i]]), &reply_client, sizeof(int));

			while(reply_client != EOK)
			{
				MsgSend(server_coid, &buffer[mas_index[i]], sizeof(buffer[mas_index[i]]), &reply_client, sizeof(int));
			}

			buffer[mas_index[i]].status = 0;
		}
	}
}

int SendLastPacketToClient(int server_coid){
	int reply_client;

	packet end_packet;

	end_packet.status = 250494;

	MsgSend(server_coid, &end_packet, sizeof(end_packet), &reply_client, sizeof(int));

/*	while(reply_client != EOK)
	{
		MsgSend(server_coid, &end_packet, sizeof(end_packet), &reply_client, sizeof(int));
	}
*/
	return 1;
}

void SetInfoFile(char* filename){
	FILE* file;

	file = fopen(filename,"rb");

	fseek(file,0,SEEK_END);

	info.size = ftell(file);

	strcpy(info.file_name, NAME_IN_FILE);

	fclose(file);
}

int GetFreeIdPlaceBuffer(packet* buffer)
{
	int i;

	for(i = 0; i<LENGTH_BUFFER; i++)
	{
		if(buffer[i].status==0)
		{
			return i;
		}
	}
	return -1;
}

void RandomArrayPacket(int* arrayIndex)
{
	int i; int j, temp;

	for(i = 0; i < LENGTH_BUFFER; i++)
	{
	   j = LENGTH_BUFFER - 1;

	   temp = arrayIndex[i];

	   arrayIndex[i] = arrayIndex[j];

	   arrayIndex[j] = temp;
	}
}
