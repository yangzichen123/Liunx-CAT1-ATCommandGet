#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include "main.h"

#define MAX_LINESIZE 256
char readbuffer[MAX_LINESIZE] = {0};
char recvBuffer[MAX_LINESIZE] = {0};

//需要输入参数
int main(int argc,char *argv[]) 
{
	//校验输入参数
	if(argc != 3)
	{
		printf("Lose Parameter,Plz Check!!!!\r\n");
		return -1;
	}
	printf("Device Name is %s\r\n",argv[1]);
	printf("Command File Name is %s\r\n",argv[2]);
	    // Open the serial port
	//打开串口
    int fd = open(argv[1], O_RDWR | O_NOCTTY);
    if (fd == -1) {
        perror("Failed to open serial port");
        return 1;
    }
    // 配置串口
    struct termios options;
    memset(&options,0,sizeof(options));
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 100;

	cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);

	//清除未处理的输入和输出数据，然后配置
	tcflush(fd,TCIFLUSH);
    if(tcsetattr(fd, TCSANOW, &options)!=0)
	{
		printf("set com err!!!!\r\n");
		return -1;
	}

	//获取当前时间，以当前时间为文件名称打开log保存文件
	time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char filename_timestamp[80];
    strftime(filename_timestamp, 80, "%Y-%m-%d_%H-%M-%S\r\n", timeinfo);
	FILE *log_fp = fopen(filename_timestamp, "w");
    if (!log_fp) {
        printf("Error opening log file %s\r\n", filename_timestamp);
        return 1;
    }
	printf("%s",filename_timestamp);

	//打开需要读取的AT指令文件
	FILE *fp = fopen(argv[2], "r");
    if (!fp) {
        printf("Error opening file %s\r\n", argv[2]);
        return 1;
    }
	//按行读取AT指令
	while(fgets(readbuffer,MAX_LINESIZE,fp)!=NULL)
	{
		//判断空行
		if(strcmp(readbuffer,"\n")==0)
		{
			continue;
		}
		//发送AT指令，等待返回，写入LOG文件
		printf("read line is %s",readbuffer);
		time(&rawtime);
    	timeinfo = localtime(&rawtime);
		strftime(filename_timestamp, 80, "[%Y-%m-%d_%H-%M-%S]---->", timeinfo);
		write(fd,readbuffer,strlen(readbuffer));
		fprintf(log_fp,"%s",filename_timestamp);
		fprintf(log_fp,"%s",readbuffer);
		int bytes_read = read(fd,recvBuffer,256);
		printf("recv data is %s",recvBuffer);
		time(&rawtime);
    	timeinfo = localtime(&rawtime);
    	strftime(filename_timestamp, 80, "[%Y-%m-%d_%H-%M-%S]<----", timeinfo);
		fprintf(log_fp,"%s",filename_timestamp);
		fprintf(log_fp,"%s",recvBuffer);
		memset(recvBuffer,0,MAX_LINESIZE);
		memset(readbuffer,0,MAX_LINESIZE);	
	}
	printf("End!!!!!!\r\n");
	//关闭打开的每个IO
	close(fd);
	fclose(fp);
	fclose(log_fp);
}
