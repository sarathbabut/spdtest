#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int upload_file_kb( char *url,  int size_inkb)
{
	char buff[1024];
	int ret;
	int ret_val;
	FILE *fp;

	memset (buff,0x00,1024);

	sprintf(buff,"dd if=data of=%dKB bs=1024 count=%d",size_inkb,size_inkb);

	ret = system(buff);

	if (ret !=0)
		goto UP_KB_ERROR;

	memset(buff,0x00,1024);
	sprintf(buff,"curl -k  --connect-timeout  12  -m  %d  --upload-file %dKB https://transfer.sh/%dKB > /tmp/tmpurl",size_inkb, size_inkb,size_inkb);

	ret =  system(buff);

	if (ret !=0)
	{
		remove("/tmp/tmpurl");
		goto UP_KB_ERROR;
	}

	fp = fopen("/tmp/tmpurl","r");

	if (fp == NULL)
		goto UP_KB_ERROR;


	ret = fscanf (fp,"%s",url);

	fclose(fp);
	
	remove("/tmp/tmpurl");

	if (ret == 1 )
	{
		memset(buff,0x00,1024);
		fp = fopen("url_file","a+");
		fprintf(fp,"%dKB %s\n", size_inkb,url);	
		fclose(fp);		
	
		return 0;
	}

	url = NULL;	

UP_KB_ERROR:

	return -1;
}

int main (void)
{	
	int ret;
	FILE *fp=NULL;

	char url_buf[1024];;	

	remove("url_file");
	
	upload_file_kb(url_buf,32);
	upload_file_kb(url_buf,64);
	upload_file_kb(url_buf,128);
	upload_file_kb(url_buf,256);
	upload_file_kb(url_buf,512);

	upload_file_kb(url_buf,1024);
	upload_file_kb(url_buf,2*1024);
	upload_file_kb(url_buf,4*1024);
	upload_file_kb(url_buf,8*1024);
}
