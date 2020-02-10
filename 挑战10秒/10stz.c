

#include<reg51.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int

sbit SK=P2^0; 
sbit SER=P2^1;
sbit RK=P2^2;
sbit but=P3^2;//按钮
bit flag = 1; //计时标志位
	uint i;
uchar code smgduan[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
void delays(uint x)
{
	uchar y,z;
	for(y=x;y>0;y--)
	for(z=110;z>0;z--);
}

/* 595发送数据*/
void Hc595SendByte(uchar dat,uchar dat1,uchar dat2,uchar dat3)
{
	uchar a;
	SK=0;
	RK=0;
	for(a=0;a<8;a++)
	{
		SER=dat>>7;
		dat<<=1;

		SK=1;
		_nop_();
		_nop_();
		SK=0;	
	}

	for(a=0;a<8;a++)
	{
		SER=dat1>>7;
		dat1<<=1;

		SK=1;
		_nop_();
		_nop_();
		SK=0;	
	}
	   for(a=0;a<8;a++)
	{
		SER=dat2>>7;
		dat2<<=1;

		SK=1;
		_nop_();
		_nop_();
		SK=0;	
	}
	for(a=0;a<8;a++)
	{
		SER=dat3>>7;
		dat3<<=1;

		SK=1;
		_nop_();
		_nop_();
		SK=0;	
	}


	RK=1;
	_nop_();
	_nop_();
	RK=0;
}

/*显示函数*/
void display()
{
			uchar ge,shi,bai,qian;
			 qian=i/1000;
			 bai=i%1000/100;
			 shi=i%100/10;
			 ge=i%10;
			 if(flag!=0)
			 {
		 		Hc595SendByte(~smgduan[ge],~smgduan[shi],~smgduan[bai]&~0x80,~smgduan[qian]);
			}
	
}

/*定时器初始化*/
void Timer0Init()
{
	TMOD|=0X01;

	TH0=0XDC;
	TL0=0X00;	
	ET0=1;
	EA=1;
	TR0=0;
	
	
		EX0 = 1;	//打开外部中断0
	IT0 = 1;	//外部中断0中断方式为跳变沿，从高到低触发
				
}

/*外部中断服务函数*/
void key() interrupt 0
{	 
			if(but==0)
			{
			   delays(10);
			   if(but==0)
			   {
				if(TR0==0&&i!=0)
				{
					i=0;
					display();
					flag=1;
				}
				else
				{
					   if(TR0==1)
					   {
					   		
					   	 TR0=0;
						 flag=0;
					   }
					   else if(TR0==0)
					   {
					   	TR0=1;
						flag=1;
					   }
				 }

				 while(!but);
        		delays(10);
        		while(!but);
			   }
			}
} 

void main()
{
		 Timer0Init(); 
		  i=0;
		 while(1)
		 {
			 display();
			// key();
			 
		}
	
}
/*定时器中断服务函数*/
void Timer0() interrupt 1
{

	TH0=0XDC;
	TL0=0X00;
	i++;
	if(i==100000)
	{
		i=0;
	}	
}