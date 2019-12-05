/*����¼ 
Ŀǰδ��:npc,Ǯ��,�̵����;�������,�Ի�����;����,������,������ʽ;(������) 
*/ 
//�������ƶ��ж�,��ɫ�ƶ���;
//#include<iostream>
#include"graphics.h"
#include<stdio.h>
//#include<string.h>
#define SIZE 32
#define BACKPACK 27
class object
{
	public:
	char name[64];//name of object
	PIMAGE imaobj;//image of object
	unsigned char objid;//ID of object
	bool weapon;//is it weapon?
	int effect;//what effect is it?
	int effectnum; 
};   
PIMAGE objectimage(int num);                                        //��ȡ��Ʒͼ�� 
void mapload();                                                //��ȡ��ͼ 
void loadmap(int xbegin, int xend,int ybegin, int yend);                                      //���ص�ͼ 
void the_death();                                        //�������� 
void mybackpack();                                       //�������� 
void read_savebp();                                      //��ȡ�浵 
void write_savebp();                                     //�浵 
void beginning();                                       //��ʼ����   
void trans_64(int x);                                  //intת��Ϊ�ַ���,�浵 
int transback_64();                                    //��ת��,��ȡ�浵�ַ���,����int 
void movejudge(int x,int y,int speed,int wx,int wy);
void shop();
void attack();
int hp=5,p1x=64,p1y=128,p1speed=16,money=99999,atk=5,mapx=0,mapy=0;             
char buffer64[3]={0,0,0}; //�浵������ 
unsigned char map[1024][1024]={};                                       //��ͼ
int ground[16]={0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned backpack[BACKPACK]={2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int numbp[BACKPACK]={2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
//int shop[16]={};
	int px=(p1x+32)/SIZE;
    int py=(p1y+32)/SIZE;
int main(int argc,char** argv){
	setinitmode (0);
	mapload();
	beginning();
	
	initgraph(528,528);
    int end=0;
    PIMAGE p1;
	p1=newimage();
	getimage(p1,"project\\person 1.jpg");

	loadmap(px-16,px+16,py-16,py+16);
    while (end==0)
    {
    	px=(p1x+32)/SIZE;
    	py=(p1y+32)/SIZE;
    	loadmap((px-2),(px+2),(py-2),(py+2));
    	putimage(p1x%512,p1y%512,p1);
    	
    	if(hp==0)end=1;
    	else
    	{
    		
    		char k;
    		k=getch();
    		switch(k)
    		{
    			case 'a':
    			case 'A':movejudge(p1x,p1y,p1speed,-1,0);break;//����
    			case 's':
    			case 'S':movejudge(p1x,p1y,p1speed,0,1);break;//���� 
    			case 'w':
    			case 'W':movejudge(p1x,p1y,p1speed,0,-1);break;//���� 
    			case 'd':
    			case 'D':movejudge(p1x,p1y,p1speed,1,0);break;//���� 
    			case 'e':
    			case 'E':mybackpack();break;//���� 
    			case 'r':
				case 'R':write_savebp();break;//���� 
   				case 'u':
    			case 'U':read_savebp();break;//��ȡ
    			case 'h':
    			case 'H':shop();break;
    			case 'j':
    			case 'J':attack();
			}
		}
	}
    the_death();
	delimage(p1);
	getch();
	closegraph();
	return 0;
} 
void mapload()
{
	FILE *fp=fopen("map.bin","r");
	int length,width;
	unsigned char buffer;
	fscanf(fp,"%d %d ",&length,&width);
	for(int j=0; j<length;j++)
	{
		for (int i=0; i<length; i++)
		{
			fscanf(fp,"%c",&buffer);
			map[i][j]=buffer;
		}
	}
} 
void the_death()
{
	PIMAGE endimage;
	endimage=newimage();
	getimage(endimage,"project\\endimage.jpg");
	putimage(0,0,endimage);
	delimage(endimage);
}
void mybackpack()
{
	PIMAGE bpi; //bpi==backpackimage
	PIMAGE object;
	bpi=newimage();
	getimage(bpi,"project\\backpack.bmp");
	putimage(0,0,bpi);
	delimage(bpi);
	
	for(int i=0;i<BACKPACK;i++)
	{
		object=newimage();
		object=objectimage(backpack[i]);
		putimage(16,18*(i+1),object);
		delimage(object);	
	}
	getch();
	loadmap(px-16,px+16,py-16,py+16);
}
void write_savebp()
{
	FILE *fp=fopen("backpackp1.txt","w");                       //���汳����Ʒ 
	for(int i=0;i<BACKPACK;i++)
	{
		fprintf(fp,"%c",backpack[i]);
	}
	for(int i=0;i<BACKPACK;i++)
	{
		fprintf(fp,"%c",numbp[i]);
	}
	fclose(fp);
	
	
	FILE *fp1=fopen("savep1.txt","w");	                          //��������״̬    
	trans_64(p1x);
	for(int i=0;i<3;i++)
	{
		fprintf(fp1,"%c",buffer64[i]);
	}
	trans_64(p1y);
	for(int i=0;i<3;i++)
	{
	    fprintf(fp1,"%c",buffer64[i]);
    }
	trans_64(hp);
	for(int i=0;i<3;i++)
	{
	    fprintf(fp1,"%c",buffer64[i]);
    }  
	fclose(fp1);
	
	
	outtextxy(0,512,"you saved successfully");
}
void read_savebp()
{
	char buffer[BACKPACK];                                         //��ȡ���� 
	FILE *fp; 
	fp=fopen("backpackp1.txt","r");
	fseek(fp,0,SEEK_SET);
	fread(buffer,1,BACKPACK,fp);
	for (int i=0;i<BACKPACK;i++)
	{
		backpack[i]=buffer[i];
	}
	fread(buffer,1,BACKPACK,fp);
	for (int i=0;i<BACKPACK;i++)       
	{
		numbp[i]=buffer[i];
	}
	fclose(fp);
	FILE *fp1;                                              //��ȡ����״̬(x,y,hp) 
	fp1=fopen("savep1.txt","r");
	fseek(fp1,0,SEEK_SET);
	fread(buffer64,3,1,fp1);
	p1x=transback_64();
	fread(buffer64,3,1,fp1);
	p1y=transback_64();
	fread(buffer64,3,1,fp1);
	hp=transback_64();
	outtextxy(0,512,"you read successfully");
	loadmap(px-16,px+16,py-16,py+16);
}
void trans_64(int x)
{
	buffer64[2]=(x/64)/64;
	buffer64[1]=(x/64)%64;
	buffer64[0]=x%64;
}
int transback_64()
{
	int n;
	n=buffer64[2]*4096+buffer64[1]*64+buffer64[0];
	return n;
}
void movejudge(int x,int y,int speed,int wx,int wy)
{
	int mapx0=(x+wx*speed)/SIZE;
	int mapy0=(y+wy*speed)/SIZE;
	int mapy1=(y+wy*speed+SIZE-1)/SIZE;
	int mapx1=(x+wx*speed+SIZE-1)/SIZE;
	int map0x=x/SIZE;
	int map0y=y/SIZE;
	int map1x=(x-1)/SIZE+1;
	int map1y=(y-1)/SIZE+1;
	int b1=ground[map[mapx0][mapy0]];
	int b2=ground[map[mapx1][mapy0]];
	int b3=ground[map[mapx1][mapy1]];
	int b4=ground[map[mapx0][mapy1]];
	if(!b1)
	{
		if(wx)
		{
			p1x=map0x*SIZE;
			p1y+=speed*wy;
		}
		else
		{
			p1x+=speed*wx;
			p1y=map0y*SIZE;
		}
	}
	else if(!b2)
	{
		if(wx)
		{
			p1y+=speed*wy;p1x=map1x*SIZE;
		}
		else
		{
			p1x+=speed*wx;p1y=map0y*SIZE;
		}
	}
	else if(!b3)
	{
		if(wx)
		{
			p1y+=speed*wy;p1x=map1x*SIZE;
		}
		else
		{
			p1x+=speed*wx;p1y=map1y*SIZE;
		}
	}
	else if(!b4)
	{
		if(wx)
		{
			p1y+=speed*wy;p1x=map0x*SIZE;
		}
		else
		{
			p1x+=speed*wx;p1y=map1y*SIZE;
		}
	}
	else
	{
		p1y+=speed*wy;p1x+=speed*wx;
	 } 
}
void beginning()
{
	setinitmode(0);
	initgraph(512,528);
	PIMAGE beginimage;
	beginimage=newimage();
	getimage(beginimage,"project\\beginimage.jpg");
	putimage(0,0,beginimage);
	delimage(beginimage);
	outtextxy(0,256,"�Ƿ���ڴ浵�������ȡ���밴u");
	outtextxy(0,312,"���������ʼ");
	char k=getch();
	switch(k)
	{
		case 'u':
		case 'U':read_savebp();break;
	}
}
void shop()
{
	int mapx=(p1x+32)/SIZE;
	int mapy=(p1y+32)/SIZE;
	if(map[mapx][mapy]==2)
	{
		PIMAGE shopima;
		shopima=newimage();
		getimage(shopima,"project//shopbackground.bmp");
		putimage(0,0,shopima);
		delimage(shopima);
		for(int i=0; i<BACKPACK; i++)
		{
			PIMAGE objectshop;
		}
	}
	char kk;
	kk=getch();
	/*switch(kk)
	{
		case '1':
		case '2':
	}*/
} 
void attack()
{
	int atk;
	switch(backpack[0])
	{
		case 2: atk=20;break;
		default: atk=5;break;
	}
	
}
void loadmap(int xbegin, int xend,int ybegin, int yend)
{
	if(xbegin<0+16*mapx)xbegin=0+16*mapx;
	if(ybegin<0+16*mapx)ybegin=0+16*mapx;
	if(xend>16+16*mapx)xend=16+16*mapx;
	if(yend>16+16*mapx)yend=16+16*mapx;
	for (int i=xbegin; i<xend;i++)
	{
		for (int j=ybegin; j<yend; j++)
		{
			PIMAGE img,img1;
    		img=newimage(); 
    		int m=map[i][j];
    		switch (m)
    	    {
    	    	case 0:getimage(img,"project\\0.bmp");putimage(SIZE*(i%16),SIZE*(j%16),img);break;        //��ͼID0: ˮ�� 
   		    	case 1:getimage(img,"project\\1.bmp");putimage(SIZE*(i%16),SIZE*(j%16),img);break;        //��ͼID1:�ݵ� 
   		    	case 2:getimage(img,"project\\2.jpeg");putimage(SIZE*(i%16),SIZE*(j%16),img);break;        //��ͼID2:�̵� 
			    default: outtextxy(SIZE*i,SIZE*j,"default");break;
			}  
			delimage(img);
		}
	}
} 
PIMAGE objectimage(int num)
{
	PIMAGE object=newimage();
	
	switch(num)
		{
			case 1:getimage(object,"project\\object 0.bmp");break;
			case 2:getimage(object,"project\\object 1.bmp");break;
			default:getimage(object,"project\\errorobject.bmp");break;
		}
		return object;
} 
