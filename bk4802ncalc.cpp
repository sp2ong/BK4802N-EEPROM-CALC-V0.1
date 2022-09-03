//
// SP2ONG 2022. Modify code for complie under linux
// compile: g++ -o bk4802ncalc bk4802ncalc.cpp
// run: bk4802ncalc

#include<iostream>
#include<fstream>

using namespace std;
char txfreq[6];
char rxfreq[6];
char slove[256];
float fr;
char regset[]={'\x00','\x7C','\x04','\x02','\x40','\xF1', '\x00' ,'\xED' ,'\xE0' ,'\x17' ,'\xE0' ,'\xE0' ,'\x43' ,'\x85' ,'\x00' ,'\x07' ,
'\x66' ,'\xA0' ,'\xFF' ,'\xFF','\x00' ,'\x03' ,'\x04' ,'\x0E' ,'\x40' ,'\xF1' ,'\x00' ,'\xED' ,'\xE0' ,'\x3F' ,'\xE0' ,'\xE0' ,
'\x43' ,'\x85' ,'\x00' ,'\x07' ,'\x66' ,'\xA0' ,'\xFF' ,'\xFF' ,'\xE0' ,'\xFF' ,'\xE0' ,'\xFF' ,'\x00' ,'\x03' ,'\x3C' ,'\x9E' ,
'\x00' ,'\x1F' ,'\xc1' ,'\xD1' ,'\x0F' ,'\x20' ,'\xFF' ,'\x01' ,'\x00' ,'\xE0' ,'\x35' ,'\x02' ,'\xE0' ,'\x88' ,'\xFF' ,'\xFF'};
	
 char HIGHBYTE(short int bi)//Get higer 8 bit
	 {
		 unsigned char highER ;
		 highER=bi>>8;
		 return highER;
	 }
 char 	LOWBYTE(short int bi)//Get lower 8 bit
	 {
		 unsigned char lowER;
		 lowER=bi&0xff;
		 return lowER;
	 }
 short int HIGHBYTE16( int bi)//Get higer 16 bit
	 {
		 short int highER;
		 highER=bi>>16;
		 return highER;
	 }
 short int 	LOWBYTE16( int bi)//Get lower 16 bit
	 {
		 short int lowER;
		 lowER=bi&0xffff;
		 return lowER;
	 }
	 
void calc(float freq)
{
	int rx,tx;
	short int rxh,rxl,txh,txl;
	char ndiv;
	if(freq<=512&&freq>=384)
	{
	 txfreq[5]=0x00;
	 rxfreq[5]=0x00;
	 txfreq[4]=0x00;
	 rxfreq[4]=0x00;
	 ndiv=4;
    }
	if(freq<=170&&freq>=128)
	{
	 txfreq[5]=0x20;
	 rxfreq[5]=0x20;
	 txfreq[4]=0x02;
	 rxfreq[4]=0x02;
	 ndiv=12;
    }
	if(freq<=57&&freq>=43)
	{
	 txfreq[5]=0x80;
	 rxfreq[5]=0x80;
	 txfreq[4]=0x08;
	 rxfreq[4]=0x08;
	 ndiv=36;
	}
	if(freq<=46&&freq>=35)
	{
	 txfreq[5]=0xa0;
	 rxfreq[5]=0xa0;
	 txfreq[4]=0x0a;
	 rxfreq[4]=0x0a;
	 ndiv=44;
    }
	if(freq<=32&&freq>=24)
	{
	 txfreq[5]=0xc0;
	 rxfreq[5]=0xc0;
	 txfreq[4]=0x0f;
	 rxfreq[4]=0x0f;
	 ndiv=64;
    }
	else if(freq<24||freq>512)
	cout<<"error: Frequency is out of range! "<<endl;

	rx=((freq-0.137)*ndiv*16777216)/21.25;
	tx=(freq*ndiv*16777216)/21.25;
	cout<<hex<<tx<<endl;
	cout<<hex<<rx<<endl;
	txh=HIGHBYTE16(tx);
	txl=LOWBYTE16(tx);
	
	rxh=HIGHBYTE16(rx);
	rxl=LOWBYTE16(rx);
	
	txfreq[1]=HIGHBYTE(txh);
	txfreq[0]=LOWBYTE(txh);
	txfreq[3]=HIGHBYTE(txl);
	txfreq[2]=LOWBYTE(txl);
	
	rxfreq[1]=HIGHBYTE(rxh);
	rxfreq[0]=LOWBYTE(rxh);
	rxfreq[3]=HIGHBYTE(rxl);
	rxfreq[2]=LOWBYTE(rxl);

}
int main()
{
    int s=0;

	ofstream ofs;
	cout << "BK4802N FREQ CALC BIN GEN (Linux version)" << endl;
	ofs.open("BK4802NE2PROM.BIN",ios::out|ios::binary);
	cout<<"Strat. Please key in 16 channels frequency (MHz)"<<endl;
	if(ofs)
	{
		for(int i=0;i<16;i++)
		{
		    cout<<dec<<"Key in channel "<<i+1<<":"<<endl;
		  	cin>>fr;
		  	calc(fr);
		  	cout<<"write this channel freq is: "<<fr<<endl;

		  for(int k=1;k<=6;k++)
		  {
		  	slove[s]=txfreq[k-1];
		  	s++;
		  }
		slove[s]=0x00;
		s++;
		slove[s]=0x00;
		s++;
		for(int j=1;j<=6;j++)
		  {
		  	slove[s]=rxfreq[j-1];
		  	s++;
		  }
		  slove[s]=0x00;
		s++;
		slove[s]=0x00;
        s++;
	    }
	    
	    
          ofs.write(slove,256);
          ofs.write(regset,64);
          cout<<dec<<s<<endl;
   } 
   ofs.close();
   cout<<"The file has been created!. You can use fr exmaple the CH341 programmer 24c04 EEPROM to program and write the frequency."<<endl;
  // system("pause");
   return 0;
}

// Default set register
//
//00 7C 04 02 40 F1 00 ED E0 17 E0 E0 43 85 00 07 
//66 A0 FF FF 00 03 04 0E 40 F1 00 ED E0 3F E0 E0 
//43 85 00 07 66 A0 FF FF E0 FF E0 FF 00 03 3C 9E 
//00 1F c1 D1 0F 20 FF 01 00 E0 35 02 E0 88 FF FF 

