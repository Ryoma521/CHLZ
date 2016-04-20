
#include "hal_types.h"
#include "stm32f2xx.h"
#include "Debug.h"
#include <stdlib.h>

static uint8 TestByte;
uint8 TesttxBuffer[128];//Tx测试发送缓冲区
uint8 Test_TaskID;

void TestTask_Init(uint8 TaskID)
{
  Test_TaskID = TaskID;
}

uint16 TestTask_ProcessEvent( uint8 task_id, uint16 events )
{
  return 0;
}

void InitTxBuffer(void)
{
  uint16 i = 0;
  
  for(i=0; i<128; i++)
    TesttxBuffer[i] = 0;
 
}

void TestCondif(void)
{
 // TestByte = TI_CC_SPIReadReg(TI_CCxxx0_IOCFG2       );//00        // GDO2 output pin configuration

  TestByte++;
}

void TestTx(void)
{
        uint8 BeaconInit[BeaconInitLength] = {0};
	uint8 DataInit[DataInitLength] = {0};
	uint8 ACKInit[ACKInitLength] = {0};

	uint8 BeaconCRCOutput[BeaconCRCOutputLength] = {0};
	uint8 DataCRCOutput[DataCRCOutputLength] = {0};
	uint8 ACKCRCOutput[ACKCRCOutputLength] = {0};

	uint8 BeaconEncodeOutput[BeaconLength] = {0};
	uint8 DataEncodeOutput[DataLength] = {0};
	uint8 ACKEncodeOutput[ACKLength] = {0};

	uint8 BeaconInterleaveOutput[BeaconLength] = {0};
	uint8 DataInterleaveOutput[DataLength] = {0};
	uint8 ACKInterleaveOutput[ACKLength] = {0};
  
        InitTxBuffer();
        
   //begin
 if (txframetype == beacon)
        {
         txPayloadLength = BeaconLength;
         
          
        SourceGenerate(BeaconInit,BeaconInitLength,sourceSeed);
//	SourceGenerate(DataInit,DataInitLength,100);
//	SourceGenerate(ACKInit,ACKInitLength,100);
        
        myCRC(BeaconInit,BeaconCRCOutput,beacon);
//	CRC(DataInit,DataCRCOutput,infodata);
//	CRC(ACKInit,ACKCRCOutput,ACK);
        
        encode(BeaconCRCOutput,BeaconEncodeOutput,beacon);
//	encode(DataCRCOutput,DataEncodeOutput,infodata);
//	encode(ACKCRCOutput,ACKEncodeOutput,ACK);
        
        interleave(BeaconEncodeOutput,BeaconInterleaveOutput,beacon);
 //       interleave(DataEncodeOutput,DataInterleaveOutput,infodata);
//	interleave(ACKEncodeOutput,ACKInterleaveOutput,ACK);
  
        
      /*   for(uint8 i=1; i<BeaconLength+1; i++)
          txBuffer[i] = BeaconInterleaveOutput[i-1];
        txBuffer[0] = PKTLEN;//txPayloadLength = BeaconLength*/
               
                for(uint8 i=0; i<BeaconLength; i++)
          TesttxBuffer[i] = BeaconInterleaveOutput[i];
       // txBuffer[0] = PKTLEN;//txPayloadLength = BeaconLength
        }
        
 if (txframetype == infodata)
        {
           txPayloadLength = DataLength;
          
 //       SourceGenerate(BeaconInit,BeaconInitLength,100);
	SourceGenerate(DataInit,DataInitLength,sourceSeed);
//	SourceGenerate(ACKInit,ACKInitLength,100);
        
//        CRC(BeaconInit,BeaconCRCOutput,beacon);
	myCRC(DataInit,DataCRCOutput,infodata);
//	CRC(ACKInit,ACKCRCOutput,ACK);
        
//        encode(BeaconCRCOutput,BeaconEncodeOutput,beacon);
	encode(DataCRCOutput,DataEncodeOutput,infodata);
//	encode(ACKCRCOutput,ACKEncodeOutput,ACK);
        
//        interleave(BeaconEncodeOutput,BeaconInterleaveOutput,beacon);
        interleave(DataEncodeOutput,DataInterleaveOutput,infodata);
//	interleave(ACKEncodeOutput,ACKInterleaveOutput,ACK);
  
        
      /*   for(uint8 i=1; i<BeaconLength+1; i++)
          txBuffer[i] = BeaconInterleaveOutput[i-1];
        txBuffer[0] = PKTLEN;//txPayloadLength = BeaconLength*/
               
                for(uint8 i=0; i<DataLength; i++)
          TesttxBuffer[i] = DataInterleaveOutput[i];
       // txBuffer[0] = PKTLEN;//txPayloadLength = BeaconLength
        }
        
if (txframetype == ACK)
        {
          txPayloadLength = ACKLength;
          
 //       SourceGenerate(BeaconInit,BeaconInitLength,100);
//	SourceGenerate(DataInit,DataInitLength,100);
	SourceGenerate(ACKInit,ACKInitLength,sourceSeed);
        
//        CRC(BeaconInit,BeaconCRCOutput,beacon);
//	CRC(DataInit,DataCRCOutput,infodata);
	myCRC(ACKInit,ACKCRCOutput,ACK);
        
//        encode(BeaconCRCOutput,BeaconEncodeOutput,beacon);
//	encode(DataCRCOutput,DataEncodeOutput,infodata);
	encode(ACKCRCOutput,ACKEncodeOutput,ACK);
        
//        interleave(BeaconEncodeOutput,BeaconInterleaveOutput,beacon);
//        interleave(DataEncodeOutput,DataInterleaveOutput,infodata);
	interleave(ACKEncodeOutput,ACKInterleaveOutput,ACK);
  
        
      /*   for(uint8 i=1; i<BeaconLength+1; i++)
          txBuffer[i] = BeaconInterleaveOutput[i-1];
        txBuffer[0] = PKTLEN;//txPayloadLength = BeaconLength*/
               
                for(uint8 i=0; i<ACKLength; i++)
          TesttxBuffer[i] = ACKInterleaveOutput[i];
       // txBuffer[0] = PKTLEN;//txPayloadLength = BeaconLength
        }
        
  
  //InitTxBuffer();
  //TesttxBuffer[0] = 19;                        // Packet length ，长度域 = 待发数据总长度 - 1
  // CC1120_Transmit(TesttxBuffer, 20); //参数1：发送缓冲区；参数2：待发数据总长度
  
 /*       InitTxBuffer();
        for(uint8 i=1; i<BeaconLength+1; i++)
          TesttxBuffer[i] = BeaconInterleaveOutput[i-1];
        TesttxBuffer[0] = txPayloadLength;//txPayloadLength = BeaconLength*/
        CC1120_Transmit(TesttxBuffer, txPayloadLength);
        
       
}

void DelayUUs(uint16 x)
{
  uint8 i;
  while(x--)
  {
    for(i=0;i<1;i++)
      asm("nop");
  }
}


void DelayUs(uint16 x)
{
  uint8 i;
  while(x--)
  {
    for(i=0;i<4;i++)
      asm("nop");
  }
}

void DelayMs(uint16 x)
{
  while(x--)
  {
    DelayUs(1000);
  }
}

void InitRandom(void)
{
#if defined MANUAL_ADDR
  uint8 seed = MANUAL_ADDR&0xFF;
#else
  uint8 seed = 0xFF;
#endif  
  srand(seed);
}

uint8 GetRandom(void)
{
  //srand(RFRND);
  return rand();
  //return 1;
}

static int16 RealRssi;
static uint8 TestRssi;

void ReadRSSI(void)
{
 // TI_CC_SPIStrobe(TI_CCxxx0_SIDLE); 
  //TI_CC_SPIStrobe(TI_CCxxx0_SRX);
  
  
   uint8 rssi2compl,rssiValid;
   
//  TI_CC_SPIStrobe(CC112X_SIDLE); 
//  TI_CC_SPIStrobe(CC112X_SRX);
  
  
  
//    TI_CC_SPIReadBurstReg(CC112X_RSSI0, &rssiValid,1);
  if(rssiValid & 0x01)
  {
    /* Read RSSI from MSB register */
//    TI_CC_SPIReadBurstReg(CC112X_RSSI1, &rssi2compl,1);
    TestRssi=rssi2compl;
  //  NowRssirecovery = (int16)((int8)rssi2compl) - cc112xRssiOffset;
  }
  
  //TestRssi = TI_CC_SPIReadReg(TI_CCxxx0_RSSI);
 // TI_CC_SPIStrobe(TI_CCxxx0_SIDLE); 
  //TI_CC_SPIStrobe(TI_CCxxx0_SRX);
  
//  TI_CC_SPIStrobe(CC112X_SIDLE); 
//  TI_CC_SPIStrobe(CC112X_SRX);
}



void TestRSSI(void)
{
  uint8 RawRssi = CC1120_Read_RSSI();
  
  RealRssi = CC1120_Convert_RSSI(RawRssi);
}

/************************************************************************
Generate CRC for subframe. In this function, CRC is calculated by bit for
beacon subframe and data subframe. For ack subframe, no CRC is applied.
CRC of each bit is calculated by adding the remainder of dividing double 
of the CRC of the former bit by polynomial and the remainder of dividing
current bit by polynomial. CRC is appended at the end of information bits 
after generated.
************************************************************************/
void myCRC(uint8 data_input[],uint8 data_output[], uint8 flag)
{
	uint16 crc = 0;
	uint8 j=0;
        uint8 zgw;
	if(flag == beacon)
	{
		while(j<BeaconInitLength)
		{
			for(uint8 i = 0x80; i != 0; i /= 2)
			{
				crc *= 2;//Multiply CRC of former bit by 2.
				if((crc&0x100) !=0)
					crc ^= poly1;//Calculate the first remainder.
				if((data_input[j]&i) != 0)    
				{
					crc ^= poly1&0xFF;//crc = (crc+0x100)^0x19A;
				}
        }
        j++;
		}
		for(uint8 k=0;k<BeaconInitLength;k++)
		{
			data_output[k] = data_input[k];
		}
		data_output[BeaconInitLength] = crc;//add the CRC to the end of information bits.
	}

	if(flag == infodata)
	{
        /*  
          int i;
       //   unsigned int crc = 0;
          unsigned int current;
       
          for (i = 0; i < DataCRCOutputLength; i++)
          {
          current = data_input[i] << 8;
          for (j = 0; j < 8; j++)
          {
             if ((int)(crc ^ current) < 0)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
            current <<= 1;           
           }
           }      
      */    
	/*	while(j<DataInitLength)
		{
			for(uint8 i = 0x80; i != 0; i /= 2)
			{
				crc *= 2;//Multiply CRC of former bit by 2.
				if((crc&0x10000) !=0) 
					crc ^= poly2;//Calculate the first remainder.
				if((data_input[j]&i) != 0)    
				{
					crc ^= poly2&0xFFFF;//crc = (crc+0x10000)^0x11021;
				}
        }
        j++;
		}
    */
                
                
		while(j<DataInitLength)
		{
			for(uint8 i = 0x80; i != 0; i /= 2)
			{
                              if (crc & 0x8000 )
				{
					zgw=1;
				}
				else
				{
					zgw=0;
				}
                          crc = crc<<1;
				//crc *= 2;//Multiply CRC of former bit by 2.
				if(zgw!=0) 
				   crc ^= poly2;//Calculate the first remainder.
                               
				if((data_input[j]&i) != 0)    
				{
					crc ^= poly2&0xFFFF;//crc = (crc+0x10000)^0x11021;
				}
                         }
                        j++;
		}
                
                
		for(uint8 k=0;k<DataInitLength;k++)
		{
			data_output[k] = data_input[k];
		}
		data_output[DataInitLength] = crc/256;//add the CRC to the end of information bits.
		data_output[DataInitLength+1] = crc%256;
           
            //    return crc;
          
	}
	if(flag == ACK)
	{
		for(uint8 k=0;k<ACKInitLength;k++)
		{
			data_output[k] = data_input[k];
		}
	}
}

/************************************************************************
Encoding for subframe. 4-repetition code and 16-repetition code are
applied to beacon subframe and data subframe. For data subframe, no
encoding is applied.
************************************************************************/
void encode(uint8 data_input[],uint8 data_output[], uint8 flag)
{
	if(flag == beacon)
	{
		for(uint8 i=0;i<BeaconLength;i++)
		{
			data_output[i] = data_input[i%BeaconCRCOutputLength];//4-repetition code
		}
	}
	if(flag == infodata)
	{
		for(uint8 i=0;i<DataLength;i++)
		{
			data_output[i] = data_input[i];
		}
	}
	if(flag == ACK)
	{
		if(data_input[0]&0x80)
		{
			data_output[0] = 0xFF;//16-repetition code
			data_output[1] = 0xFF;
		}
	}
}

/************************************************************************
Matrix-based interleaving for subframe. Matrix is filled row by row while
the each bit will be read out column by column.
************************************************************************/
void interleave(uint8 data_input[],uint8 data_output[], uint8 flag)
{
	uint8 matrix[16][12]={0};//initialize the interleaving matrix.
	uint8 i,j;
	if(flag == beacon || flag == infodata)
	{
		for(i=0;i<16;i++)//Fill the data in row by row. Data are filled in by bit.
		{
			for(j=0;j<12;j++)
			{
				matrix[i][j] = (data_input[(12*i+j)/8] & 0x80>>((12*i+j)%8)) >> (7-(12*i+j)%8);
			}
		}
		for(j=0;j<12;j++)
		{
			for(i=0;i<16;i++)//Read the data out column by column. Also, data are read out by bit.
			{
				if(matrix[i][j] == 1)
				data_output[(16*j+i)/8] += (0x80 >> (16*j+i)%8);
			}
		}
		//for(i=0;i<16;i++)//Print the interleaving matrix.
		//{
		//	for(j=0;j<12;j++)
		//	{
		//		printf("%d ",matrix[i][j]);
		//	}
		//	printf("\n");
		//}
		//printf("\n");
	}
	if(flag == ACK)
	{
		if(data_input[0]&0x80)
		{
			data_output[0] = data_input[0];
			data_output[1] = data_input[1];
		}
	}
}

/************************************************************************
Matrix-based deinterleaving for subframe. Deinterleaving is the inverse
process of interleaving.
************************************************************************/
void deinterleave(uint8 data_input[],uint8 data_output[], uint8 flag)
{
	uint8 matrix[16][12]={0};//initialize the  deinterleaving matrix.
	uint8 i,j;
	if(flag == beacon || flag == infodata)
	{
		for(j=0;j<12;j++)//Fill the data in column by column.
		{
			for(i=0;i<16;i++)
			{
				matrix[i][j] = (data_input[(j*16+i)/8] & 0x80>>((j*16+i)%8)) >> (7-(j*16+i)%8);

			}
		}
		for(i=0;i<16;i++)//Read the data out row by row.
		{
			for(j=0;j<12;j++)
			{	
				if(matrix[i][j] == 1)
				data_output[(12*i+j)/8] += (0x80 >> (12*i+j)%8);
			}
		}	
		/*for(i=0;i<16;i++)//Print the deinterleaving matrix.
		{
			for(j=0;j<12;j++)
			{
				printf("%d ",matrix[i][j]);
			}
			printf("\n");
		}
		printf("\n");*/
	}
	if(flag == ACK)
	{
		if(data_input[0]&0x80)
		{
			data_output[0] = data_input[0];
			data_output[1] = data_input[1];
		}
	}
}

/************************************************************************
Decoding for subframe. Decoding is the inverse process of encoding.
************************************************************************/
void decode(uint8 data_input[],uint8 data_output[], uint8 flag)
{
	if(flag == beacon)
        {
	

          
		for(uint8 i=0;i<BeaconCRCOutputLength;i++)
		{
			data_output[i] = data_input[i];
		}
	}
	if(flag == infodata)
	{
		for(uint8 i=0;i<DataCRCOutputLength;i++)
		{
			data_output[i] = data_input[i];
		}
	}
	if(flag == ACK)
	{
		data_output[0] = data_input[0] & 0x80;
	}
}


/************************************************************************
Decoding for subframe. Decoding is the inverse process of encoding.
***********************************************************************/
void decode1(uint8 data_input[],uint8 data_output[], uint8 flag)
{
  

     if(flag == beacon)
	{
		for(uint8 i=0;i<BeaconCRCOutputLength;i++)
		{
			data_output[i] = data_input[i];
		}
	}
	if(flag == infodata)
	{
		for(uint8 i=0;i<DataCRCOutputLength;i++)
		{
			data_output[i] = data_input[i];
		}
	}
	if(flag == ACK)
	{
                uint8 temp0=data_input[0];
                uint8 temp1=data_input[1];
                
                uint8 BYTEOF1BITcount=0;
                uint8 k;
     
                for (uint8 j=0;j<8;j++)
                {
                k=temp0&0x01;
                BYTEOF1BITcount=BYTEOF1BITcount+k;
                temp0=temp0>>1;
                }
                for (uint8 j=0;j<8;j++)
                {
                k=temp1&0x01;
                BYTEOF1BITcount=BYTEOF1BITcount+k;
                temp1=temp1>>1;
                }
    
                if (BYTEOF1BITcount>=10)
                  data_output[0] = 0x80;
                else 
                  data_output[0] = 0;
		//data_output[0] = data_input[0] & 0x80;
	}
}

/************************************************************************
Decoding for subframe. Decoding is the inverse process of encoding.
************************************************************************/
uint8 decode2(uint8 data_input[],uint8 data_output[], uint8 flag)
{
  uint8 crc = 0;

	if(flag == beacon)
	{		
		//initialization
		uint8 a[BeaconLength/4]={0};
		uint8 b[BeaconLength/4]={0};
		uint8 c[BeaconLength/4]={0};
		uint8 d[BeaconLength/4]={0};
		uint8 e[BeaconCRCOutputLength * 8]={0};
		uint8 f[BeaconLength/4]={0};
		uint8 f_temp[BeaconLength/4]={0};
		uint8 g[48]={0};
		uint8 i;
		uint8 j;
		uint8 K;

		j = 0;
		K = 0;

		for(i=0;i<BeaconLength/4;i++)//step 1
		{
			a[i] = data_input[i];
			b[i] = data_input[BeaconLength/4 + i];
			c[i] = data_input[BeaconLength/2 + i];
			d[i] = data_input[BeaconLength*3/4 + i];
		}

		for(i=0;i<BeaconLength/4;i++)//step 2
		{			
			uint8 temp = 0x01;
			for(j=0;j<8;j++)
			{
				if(a[i]&temp)
				{
					e[i*8 + j]++;
				}
				if(b[i]&temp)
				{
					e[i*8 + j]++;
				}
				if(c[i]&temp)
				{
					e[i*8 + j]++;
				}
				if(d[i]&temp)
				{
					e[i*8 + j]++;
				}

				if(e[i*8+j] >= 3)
				{
					f[i] += temp;
				}
				if(e[i*8+j] == 2)
				{
					g[K] = i*8 + j;
					K++;
				}
				temp = temp<<1;
			}
		}

		if(K == 0)//step 3
		{	
			for(j=0;j<BeaconLength/4;j++)
			{
				f_temp[j] = f[j];
			}
			//crc = 0;
                        crc = CRCCheck(f_temp, beacon);
		}

		if(K > 4)
		{
			for(j=0;j<BeaconLength/4;j++)
			{
				f_temp[j] = f[j];
			}
			crc = 1;
		}

		if(K>=0 && K<=4)//step 4
		{
			uint8 count = 0x01<<K;
			for(i=1;i<=count;i++)
			{
				if(i == 1)
				{
					for(j=0;j<BeaconLength/4;j++)
					{
						f_temp[j] = f[j];
					}	
					crc = CRCCheck(f_temp, beacon);
					if(crc==0)
					{
						break;
					}
				}

				if(i == 2)
				{
					for(j=0;j<BeaconLength/4;j++)
					{
						f_temp[j] = f[j];
					}						
					f_temp[g[0]/8] = f_temp[g[0]/8] + (0x01<<(g[0]%8));	
					crc = CRCCheck(f_temp, beacon);
					if(crc==0)
					{
						break;
					}
				}

				if(i == 3)
				{
					for(j=0;j<BeaconLength/4;j++)
					{
						f_temp[j] = f[j];
					}						
					f_temp[g[1]/8] = f_temp[g[1]/8] + (0x01<<(g[1]%8));	
					crc = CRCCheck(f_temp, beacon);
					if(crc==0)
					{
						break;
					}
				}

				if(i == 4)
				{
					for(j=0;j<BeaconLength/4;j++)
					{
						f_temp[j] = f[j];
					}	
					f_temp[g[0]/8] = f_temp[g[0]/8] + (0x01<<(g[0]%8));	
					f_temp[g[1]/8] = f_temp[g[1]/8] + (0x01<<(g[1]%8));	
					crc = CRCCheck(f_temp, beacon);
					if(crc==0)
					{
						break;
					}
				}

				if(i == 5)
				{
					for(j=0;j<BeaconLength/4;j++)
					{
						f_temp[j] = f[j];
					}	
					f_temp[g[2]/8] = f_temp[g[2]/8] + (0x01<<(g[2]%8));	
					crc = CRCCheck(f_temp, beacon);
					if(crc==0)
					{
						break;
					}
				}

				if(i == 6)
				{
					for(j=0;j<BeaconLength/4;j++)
					{
						f_temp[j] = f[j];
					}	
					f_temp[g[0]/8] = f_temp[g[0]/8] + (0x01<<(g[0]%8));	
					f_temp[g[2]/8] = f_temp[g[2]/8] + (0x01<<(g[2]%8));	
					crc = CRCCheck(f_temp, beacon);
					if(crc==0)
					{
						break;
					}
				}

				if(i == 7)
				{
					for(j=0;j<BeaconLength/4;j++)
					{
						f_temp[j] = f[j];
					}	
					f_temp[g[1]/8] = f_temp[g[1]/8] + (0x01<<(g[1]%8));	
					f_temp[g[2]/8] = f_temp[g[2]/8] + (0x01<<(g[2]%8));	
					crc = CRCCheck(f_temp, beacon);
					if(crc==0)
					{
						break;
					}
				}

				if(i == 8)
				{
					for(j=0;j<BeaconLength/4;j++)
					{
						f_temp[j] = f[j];
					}	
					f_temp[g[0]/8] = f_temp[g[0]/8] + (0x01<<(g[0]%8));	
					f_temp[g[1]/8] = f_temp[g[1]/8] + (0x01<<(g[1]%8));	
					f_temp[g[2]/8] = f_temp[g[2]/8] + (0x01<<(g[2]%8));	
					crc = CRCCheck(f_temp, beacon);
					if(crc==0)
					{
						break;
					}
				}

				if(i == 9)
				{
					for(j=0;j<BeaconLength/4;j++)
					{
						f_temp[j] = f[j];
					}	
					f_temp[g[3]/8] = f_temp[g[3]/8] + (0x01<<(g[3]%8));	
					crc = CRCCheck(f_temp, beacon);
					if(crc==0)
					{
						break;
					}
				}

				if(i == 10)
				{
					for(j=0;j<BeaconLength/4;j++)
					{
						f_temp[j] = f[j];
					}	
					f_temp[g[0]/8] = f_temp[g[0]/8] + (0x01<<(g[0]%8));	
					f_temp[g[3]/8] = f_temp[g[3]/8] + (0x01<<(g[3]%8));	
					crc = CRCCheck(f_temp, beacon);
					if(crc==0)
					{
						break;
					}
				}

				if(i == 11)
				{
					for(j=0;j<BeaconLength/4;j++)
					{
						f_temp[j] = f[j];
					}	
					f_temp[g[1]/8] = f_temp[g[1]/8] + (0x01<<(g[1]%8));	
					f_temp[g[3]/8] = f_temp[g[3]/8] + (0x01<<(g[3]%8));	
					crc = CRCCheck(f_temp, beacon);
					if(crc==0)
					{
						break;
					}
				}

				if(i == 12)
				{
					for(j=0;j<BeaconLength/4;j++)
					{
						f_temp[j] = f[j];
					}		
					f_temp[g[0]/8] = f_temp[g[0]/8] + (0x01<<(g[0]%8));	
					f_temp[g[1]/8] = f_temp[g[2]/8] + (0x01<<(g[2]%8));	
					f_temp[g[3]/8] = f_temp[g[3]/8] + (0x01<<(g[3]%8));	
					crc = CRCCheck(f_temp, beacon);
					if(crc==0)
					{
						break;
					}
				}

				if(i == 13)
				{
					for(j=0;j<BeaconLength/4;j++)
					{
						f_temp[j] = f[j];
					}	
					f_temp[g[2]/8] = f_temp[g[2]/8] + (0x01<<(g[2]%8));	
					f_temp[g[3]/8] = f_temp[g[3]/8] + (0x01<<(g[3]%8));	
					crc = CRCCheck(f_temp, beacon);
					if(crc==0)
					{
						break;
					}
				}

				if(i == 14)
				{
					for(j=0;j<BeaconLength/4;j++)
					{
						f_temp[j] = f[j];
					}	
					f_temp[g[0]/8] = f_temp[g[0]/8] + (0x01<<(g[0]%8));	
					f_temp[g[2]/8] = f_temp[g[2]/8] + (0x01<<(g[2]%8));	
					f_temp[g[3]/8] = f_temp[g[3]/8] + (0x01<<(g[3]%8));	
					crc = CRCCheck(f_temp, beacon);
					if(crc==0)
					{
						break;
					}
				}

				if(i == 15)
				{
					for(j=0;j<BeaconLength/4;j++)
					{
						f_temp[j] = f[j];
					}	
					f_temp[g[1]/8] = f_temp[g[1]/8] + (0x01<<(g[1]%8));	
					f_temp[g[2]/8] = f_temp[g[2]/8] + (0x01<<(g[2]%8));	
					f_temp[g[3]/8] = f_temp[g[3]/8] + (0x01<<(g[3]%8));	
					crc = CRCCheck(f_temp, beacon);
					if(crc==0)
					{
						break;
					}
				}

				if(i == 16)
				{
					for(j=0;j<BeaconLength/4;j++)
					{
						f_temp[j] = f[j];
					}	
					f_temp[g[0]/8] = f_temp[g[0]/8] + (0x01<<(g[0]%8));	
					f_temp[g[1]/8] = f_temp[g[1]/8] + (0x01<<(g[1]%8));	
					f_temp[g[2]/8] = f_temp[g[2]/8] + (0x01<<(g[2]%8));	
					f_temp[g[3]/8] = f_temp[g[3]/8] + (0x01<<(g[3]%8));	
					crc = CRCCheck(f_temp, beacon);
					if(crc==0)
					{
						break;
					}
				}
			}
		}
		for(i=0;i<BeaconLength/4;i++)
		{
			data_output[i] = f_temp[i];
		}
		return crc;
	}


	if(flag == infodata)
	{
		for(uint8 i=0;i<DataCRCOutputLength;i++)
		{
			data_output[i] = data_input[i];
		}
		crc = CRCCheck(data_output, infodata);
		return crc;
	}


	if(flag == ACK)
	{
          /*
                 uint8 temp0=data_input[0];
                uint8 temp1=data_input[1];
                
                uint8 BYTEOF1BITcount=0;
                uint8 k;
     
                for (uint8 j=0;j<8;j++)
                {
                k=temp0&0x01;
                BYTEOF1BITcount=BYTEOF1BITcount+k;
                temp0=temp0>>1;
                }
                for (uint8 j=0;j<8;j++)
                {
                k=temp1&0x01;
                BYTEOF1BITcount=BYTEOF1BITcount+k;
                temp1=temp1>>1;
                }
    
                if (BYTEOF1BITcount>=10)
                  data_output[0] = 0x80;
                else 
                  data_output[0] = 0;
          */
          
		uint8 temp = 0x01;
		uint8 sum = 0;
                
		for(uint8 i=0;i<ACKLength*8;i++)
		{
			if(data_input[i/8] & (temp<<(i%8)) )
			{
				sum++;
			}
		}
          
          
		if(sum >= ACKLength*5)
		{
			data_output[0] = 0x80;
		//	data_output[1] = 0;
		}
		else
		{
			data_output[0] = 0;
		//	data_output[1] = 0;
		}
		crc = CRCCheck(data_output, ACK);
		return crc;
	}
}
/************************************************************************
CRC checking for subframe. CRC checking is done by calculating the CRC
of input array. If the CRC of input array is 0, the result is correct. 
Otherwise，theremust be error in that array.
************************************************************************/

//0:correct ,1:wrong;
uint8 CRCCheck(uint8 data_input[], uint8 flag)
{
	unsigned int crc = 0;
	uint8 j=0;
        uint8 zgw;
        
	if(flag == beacon)
	{
		while(j<BeaconCRCOutputLength)
		{
			for(uint8 i = 0x80; i != 0; i /= 2)
			{
				crc *= 2;//Multiply CRC of former bit by 2.
				if((crc&0x100) !=0) 
					crc ^= poly1;//Calculate the first remainder.
				if((data_input[j]&i) != 0)  
				{
					crc ^= poly1&0xFF;//crc = (crc+0x100)^0x19A;
				}
        }
        j++;
		}
                
                return crc;
		/*if(crc == 0)//Print the result of CRC checking
		{
			printf("beacon: Correct!\n");
		}
		else
		{
			printf("beacon: Error!\n");
		}*/
	}

	if(flag == infodata)
	{
		while(j<DataCRCOutputLength)
		{
			for(uint8 i = 0x80; i != 0; i /= 2)
			{
				/*crc *= 2;//Multiply CRC of former bit by 2.
				if((crc&0x10000) !=0) 
					crc ^= poly2;//Calculate the first remainder.
				if((data_input[j]&i) != 0)    
				{
					crc ^= poly2&0xFFFF;//crc = (crc+0x10000)^0x11021;
				}*/
                           if (crc & 0x8000 )
				{
					zgw=1;
				}
				else
				{
					zgw=0;
				}
                          crc = crc<<1;
				//crc *= 2;//Multiply CRC of former bit by 2.
				if(zgw!=0) 
				   crc ^= poly2;//Calculate the first remainder.
                               
				if((data_input[j]&i) != 0)    
				{
					crc ^= poly2&0xFFFF;//crc = (crc+0x10000)^0x11021;
				}
                            }
                     j++;
		}
                
                return crc;
		/*if(crc == 0)//Print the result of CRC checking
		{
			printf("data:   Correct!\n");
		}
		else
		{
			printf("data:   Error!\n");
		}*/
	}
	if(flag == ACK)
	{
                 return 0;
		//printf("ACK:    no CRC is applied.\n");
	}
}

/************************************************************************
 Generate initial arrays for the program. Random arrays are generated using
 the function "rand()" with different seed. 
************************************************************************/
void SourceGenerate(uint8 source[],uint8 length,uint8 seed)
{
	uint8 i;
	if(seed!=0)
	{
		srand(seed*length);//Set "seed*length" as seed to ensure random arrays are different.
	}
        if (seed==100)//fixed initial byte.
        {
           if(length==1)//Generate initial byte for ACK subframe, which only has 1 data bit.
	  {
		source[0] = 0x80;
	  }
          else
          {
          for(i=0;i<length;i++)
			{
				source[i] = i;
			}
            }
        }
        else 
        {
	if(length==1)//Generate initial byte for ACK subframe, which only has 1 data bit.
	{
		source[0] = (uint8)(rand()&0x80);
	}
	else
	{
			for(i=0;i<length;i++)
			{
				source[i] = (uint8)rand();
			}
	}
        }

}




         