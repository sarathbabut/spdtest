#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

float up_speed=0;
float dn_speed=0;

float up_time=0.0;
float dn_time=0.0;

float get_uptime (void)
{
	FILE *fp=NULL;
	float uptime=0.0;
	
	fp =fopen("/proc/uptime","r");
	
	fscanf (fp,"%f",&uptime);
	
	fclose(fp);
	
	return uptime; 
}

int download_file_kb (int size, char *url)
{
	int ret=0;

	char buff[1024];	
	float start_tm=0.0;     
        float end_tm=0.0; 
	
	sprintf(buff,"curl -k -s --connect-timeout 20 -m %d  %s > /dev/null", 2*size, url);
	
	start_tm = get_uptime();
	
	ret =system(buff);
	
	end_tm = get_uptime();

	if (ret !=0)
        {
		return -1;
        }
	
	dn_time = end_tm -start_tm;

        //fprintf(stdout, "End time %f start time %f\n",end_tm, start_tm );

        dn_speed = (size * 8.0) /dn_time;

        fprintf(stdout, "\nDownload_speed %f kbps Downloadload_time %f sec\n",dn_speed,dn_time);

                return 0;

	


}
int upload_file_kb( int size_inkb)
{
	char buff[1024];
	char file_name[80];
	int ret;
	int ret_val;
	FILE *fp;

	float start_tm=0.0;	
	float end_tm=0.0;	


	memset (buff,0x00,1024);
	memset (file_name,0x00,80);
	
	sprintf(file_name,"/tmp/%dKB",size_inkb);

	sprintf(buff,"dd if=/usr/bin/Xfbdev of=%s bs=1024 count=%d &> /dev/null",file_name, size_inkb);

	ret = system(buff);

	if (ret !=0)
		goto UP_KB_ERROR;

	memset(buff,0x00,1024);

	sprintf(buff,"curl -s -k --connect-timeout 20 -m %d --upload-file %s https://transfer.sh/%dKB",4*size_inkb,file_name, size_inkb);

	start_tm= get_uptime();

	ret =  system(buff);
	
	end_tm = get_uptime();
	
	remove("file_name");

	if (ret !=0)
	{
		
		goto UP_KB_ERROR;
	}
	
	up_time = end_tm -start_tm;

	//fprintf(stdout, "End time %f start time %f\n",end_tm, start_tm );
	
	up_speed = (size_inkb * 8.0) /up_time;
	
	fprintf(stdout, "\nUpload_speed %04f kbps Upload_time %f sec\n",up_speed,up_time);
	
		return 0;
UP_KB_ERROR:

	fprintf(stderr,"Upload Failed");
	return -1;
}



int download_url_file(void)
{
	int ret;
	
	float t_start,t_stop;
	
	char cmd[1024]="";
	
	memset(cmd,0x00,1024);
        
sprintf(cmd,"curl -s -k --connect-timeout 20 --max-time 40 https://raw.githubusercontent.com/sarathbabut/spdtest/master/url_file > /tmp/url_buff");
	
	t_start = get_uptime();
	
	ret =  system (cmd);

	
		
	if (ret == 0 )	
	{
	t_stop=get_uptime();
	fprintf(stdout, "%lf url\n",t_stop-t_start);
	return 0;
	}

	return -1;
}



int get_url(int option)
{

	int i;
	
	int ret=0;

	char buff[1024];

	char opt[24];
	char cpt[24];
	char url[512];
	
	int size=0;

	FILE *fp=NULL;
	int flag=0;
	

	memset(buff,0x00,1024);
	memset(opt,0x00,24);
	memset(url,0x00,512);

	switch (option)
	{

		case 1:
			strcpy(opt,"32KB");
			size=32;
			break;

		case 2:
			strcpy(opt,"64KB");
			size=64;
			break;


		case 3:
			strcpy(opt,"128KB");
			size=128;
			break;

		case 4:
			strcpy(opt,"256KB");
			size=256;
			break;

		case 5:
			strcpy(opt,"512KB");
			size=512;
			break;

		case 6:
			strcpy(opt,"1024KB");
			size=1024;
			break;

		case 7:
			strcpy(opt,"2048KB");
			size=2048;	
			break;

		case 8:
			strcpy(opt,"4096KB");
			size=4096;
			break;

		case 9:
			strcpy(opt,"8192KB");
			size=8192;
			break;
	}

	memset(buff,0x00,1024);

	fp = fopen("/tmp/url_buff","r");

	if (fp == NULL)
		return -1;

	for (i=0;i<9;i++)
	{
		ret=fscanf(fp,"%s %s",cpt,url);
		
		if (ret!=2)
		break;
			

		if (strcmp (opt,cpt)==0)
		{
			flag=1;
			break; 
		}

	}

	fclose(fp);

	if (flag == 0 )
		return -1;

//	puts(url);

//	puts(opt);
	
	
	ret= upload_file_kb(size);
	
	if (ret == 0 )
	fprintf(stdout,"Upload Test Success\n");
	else 
	fprintf(stdout,"Upload Test Failed\n");
	
	ret = download_file_kb(size,url);

	
	if (ret == 0 )
	fprintf(stdout,"Download Test Success\n");
	else 
	fprintf(stdout,"Download Test Failed\n");
	
	//ret = up_dn_test(size,opt,url);
	
	

	return 0;
}

int main ()
{

	int ret=0;

	int ret_val=0;
	
	int i=0;

	long int file_n;

	int seed=time(NULL);;

	char buffer[1024];
	
	int ch=0;
	
	for (i=0; i < 4 ;i ++ )
	{
		ret = download_url_file ();
		
		if (ret == 0 )
		break;
		else sleep(2);
	}
	
	if (ret!=0)
	{
		fprintf(stdout,"Device Unable to Download Configuration File/server\n");
		return -1;
	}

	printf("1. 32KB\n");
	printf("2. 64KB\n");
	printf("3. 128KB\n");
	printf("4. 256KB\n");
	printf("5. 512KB\n");
	printf("6. 1MB\n");
	printf("7. 2MB\n");
	printf("8. 4MB\n");
	printf("9. 8MB\n");
	printf("0. Exit\n");
	
	fprintf(stdout,"Selcet the file size for speed test:\n");
	scanf("%d",&ch);
	
	get_url(ch);
	
	return 0;
		
}
