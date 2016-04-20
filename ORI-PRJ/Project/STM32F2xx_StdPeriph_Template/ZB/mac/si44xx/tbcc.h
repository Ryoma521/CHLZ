#ifndef TBCC_H
#define TBCC_H

#include <math.h>
#include <stdlib.h>

#define info_len_max 24
#ifdef SOFT_MERGE

const unsigned char out[64] =  {0, 3, 7, 4, 6, 5, 1, 2, 1, 2, 6, 5, 7, 4, 0, 3, 4, 7, 3, 0, 2, 1, 5, 6, 5, 6, 2, 1, 3, 0, 4, 7,
						  7, 4, 0, 3, 1, 2, 6, 5, 6, 5, 1, 2, 0, 3, 7, 4, 3, 0, 4, 7, 5, 6, 2, 1, 2, 1, 5, 6, 4, 7, 3, 0};

//byte to byte,
// ���룺info[info_len];�����code[info_len*3];�м����������out[64]--��¼stateת�ƣ�
//ԭ���㷨������м����������infoSeq[192]-- ���㷨�Ѿ�������Ҫ,codeSeq[576]--���㷨�Ѿ�������Ҫ
void tbcc_enc(unsigned char info[], unsigned char info_len, unsigned char code[])
{
	//initialization on state
	unsigned char state = 0;
	unsigned int i;//, j;
	unsigned char tempOut = 0;

	unsigned int infoSeq_len = info_len * 8;


	unsigned int k = 0;//������int�ͣ�һֱ���ۼ�

	state = ((info[info_len - 1] >> 5)%2); //-6 = -1-j, j = 6  

	for(i = 2; i < 7; i++)
	{

		state = (state << 1) + ((info[info_len - 1] >> (6-i))%2); // 7-i = 1 +��λ


	}

	k = 0;
	for(i = 0; i < infoSeq_len; i++)//bit length, infoSeq_len = info_len * 8;
	{
		
		if(((info[i/8]>>(7-(i%8)))%2) == 0)
			tempOut = out[state];
		else
			tempOut = 7 - out[state];

		//first bit
		if ((k%8)==0)
		{
		code[k/8] = 0;
		}
		code[k/8] = (code[k/8]<<1) + (tempOut >> 2);
		k++;

		//second bit
		if ((k%8)==0)
		{
		code[k/8] = 0;
		}
		code[k/8] = (code[k/8]<<1) + ((tempOut >> 1)%2);
		k++;

		//third bit
		if ((k%8)==0)
		{
		code[k/8] = 0;
		}
		code[k/8] = (code[k/8]<<1) + (tempOut%2);
		k++;

		state = ((state << 1) + ((info[i/8]>>(7-(i%8)))%2))%64;
    
	}
}


unsigned char prevSelState[(info_len_max*8 + 96)/8][64] = {0};//�µ�dec��Ψһ��Ҫ������


//bit to byte -2015-6-12 17:00:07 
//���룺recv_bit[info_len*24]�������infoDec[info_len]���м����������prevSelState[(info_len_max*8 + 96)/8][64]--���㷨�ռ佫Ϊ1/8��
//ԭ���㷨������м����������recvSeq[info_len_max*24]--���㷨�Ѿ�������Ҫ��infoSeqDec[info_len_max*8]--���㷨�Ѿ�������Ҫ
void tbcc_dec_bit2byte(unsigned char recv_bit[], unsigned char info_len, unsigned char infoDec[], unsigned char numTx)
{
	unsigned int i, m, j,iii = 0;
	unsigned char header_len = 48;
	unsigned char tail_len = 48;
	unsigned int total_len = 96;

	unsigned int prevPathMetric[64] = {0};
	unsigned int currPathMetric[64] = {0};

	unsigned char initPos;
	unsigned char recv0, recv1, recv2;
	unsigned char state;
	unsigned char prevState_upper, prevState_bottom;
	unsigned char out_upper, out0_upper, out1_upper, out2_upper;

	unsigned int branchMetric_upper, branchMetric_bottom;
	unsigned char surState;

	unsigned int minValue;
	unsigned char minIdx;

	unsigned int infoSeq_len = info_len*8;


        
    //ÿ�ζ�Ҫ��ʼ������ΪҪ��prevSelState��infoDec[i]�ۼӡ���Ҫ��
	for(i = 0; i < ((info_len_max*8+96)/8); i++)//info_len_max = 24,Ϊ���͵�������Ϣ������ֽ����������С��С��ԭ����1/8
	{
		if (i<info_len)
		   infoDec[i] = 0;//byte,�����,��ʼ��Ϊ0

		for(j = 0; j < 64; j++)
		{
			prevSelState[i][j]=0;//��ʼ��Ϊ0
		}
	}


	if(infoSeq_len < 48)
	{
		header_len = 48;
		tail_len = 48;
	}

	total_len = header_len + infoSeq_len + tail_len;

	initPos = (infoSeq_len - header_len%infoSeq_len)%infoSeq_len;

	for(i = 0; i < total_len; i++)
	{

	    recv0 = recv_bit[((initPos + i)%infoSeq_len)*3];// * numTx;
		recv1 = recv_bit[((initPos + i)%infoSeq_len)*3 + 1];// * numTx;
		recv2 = recv_bit[((initPos + i)%infoSeq_len)*3 + 2];// * numTx;


		for(state = 0; state < 64; state++)
		{
			prevState_upper = state >> 1;
			prevState_bottom = prevState_upper + 32;
			if(state%2 == 0)
				out_upper = out[prevState_upper];
			else
				out_upper = 7 - out[prevState_upper];

			out0_upper = numTx*(out_upper >> 2);
			out1_upper = numTx*((out_upper >> 1)%2);
			out2_upper = numTx*(out_upper%2);

			branchMetric_upper = (out0_upper - recv0)*(out0_upper  - recv0) + (out1_upper - recv1)*(out1_upper - recv1) + (out2_upper - recv2)*(out2_upper - recv2);
			branchMetric_bottom = (numTx-out0_upper-recv0)*(numTx-out0_upper-recv0) + (numTx-out1_upper-recv1)*(numTx-out1_upper-recv1) + (numTx-out2_upper-recv2)*(numTx-out2_upper-recv2);	

			if((prevPathMetric[prevState_upper] + branchMetric_upper) < (prevPathMetric[prevState_bottom] + branchMetric_bottom))
			{

				prevSelState[i/8][state] = (1<<(i%8)) + prevSelState[i/8][state];//// ����ӣ�1,ÿ�α���Ҫ��ʼ��������ʼʱ��Ҫ��ʼ��,��λ���������(1<<(i%8))��Ҫ�У���


				currPathMetric[state] = prevPathMetric[prevState_upper] + branchMetric_upper;
			}
			else
			{

				currPathMetric[state] = prevPathMetric[prevState_bottom] + branchMetric_bottom;
			}
		}

		for(m = 0; m < 64; m++)
		{
			prevPathMetric[m] = currPathMetric[m];
		}
	}

	minValue = currPathMetric[0];
	minIdx = 0;
	for(i = 0; i < 64; i++)
	{
		if(currPathMetric[i] < minValue)
		{
			minValue = currPathMetric[i];
			minIdx = i;
		}
	}
	surState = minIdx;


	for(i = total_len-1; i > header_len; i--)
	{


		if ((prevSelState[i/8][surState]&(1<<(i%8)))!=0)// �����
			surState = surState>>1;
		else
			surState = (surState>>1)+32;

		if(i <= (header_len + infoSeq_len))
		{

            iii = i - header_len - 1;
			infoDec[iii/8] = ((surState%2) <<(7-(iii%8))) + infoDec[iii/8];//���ۼӣ�һ��ʼ�����ʼ��Ϊ0.��λ��ǰ����λ�ں�
		
		}
	}
	//�Ѿ���bitֱ��תΪ��byte��infoDec���Ϊbyte
	
}


void ResetBeaconBuffer(unsigned char bufferbeacontbcc[],unsigned char bufferbeaconEncode[],unsigned char bufferBeaconInterleave[])
{
  uint8 i ;
  
  for (i=0;i<BeaconTBCCOutputLength;i++)
       bufferbeacontbcc[i] = 0;
  
   for (i=0;i<BeaconLength;i++)
   {
       bufferbeaconEncode[i] = 0;
       bufferBeaconInterleave[i] = 0;
   }
}


void ResetInfodataBuffer(unsigned char DataEncodeOutput[],unsigned char dataCRC[])
{
  uint8 i ;
  
  for (i=0;i<72;i++)
       DataEncodeOutput[i] = 0;
  
   for (i=0;i<24;i++)
   {
       dataCRC[i] = 0;
    
   }
}

//return minValue;��tbcc_dec_bit2byteһ�£�ֻ�����з���ֵ
unsigned int tbcc_dec_bit2byte_infodata(unsigned char recv_bit[], unsigned char info_len, unsigned char infoDec[], unsigned char numTx )
{
	unsigned int i, m, j,iii = 0;
	unsigned char header_len = 48;
	unsigned char tail_len = 48;
	unsigned int total_len = 96;

	unsigned int prevPathMetric[64] = {0};
	unsigned int currPathMetric[64] = {0};

	unsigned char initPos;
	unsigned char recv0, recv1, recv2;
	unsigned char state;
	unsigned char prevState_upper, prevState_bottom;
	unsigned char out_upper, out0_upper, out1_upper, out2_upper;

	unsigned int branchMetric_upper, branchMetric_bottom;
	unsigned char surState;

	unsigned int minValue;
	unsigned char minIdx;

	unsigned int infoSeq_len = info_len*8;


        
    //ÿ�ζ�Ҫ��ʼ������ΪҪ��prevSelState��infoDec[i]�ۼӡ���Ҫ��
	for(i = 0; i < ((info_len_max*8+96)/8); i++)//info_len_max = 24,Ϊ���͵�������Ϣ������ֽ����������С��С��ԭ����1/8
	{
		if (i<info_len)
		   infoDec[i] = 0;//byte,�����,��ʼ��Ϊ0

		for(j = 0; j < 64; j++)
		{
			prevSelState[i][j]=0;//��ʼ��Ϊ0
		}
	}


	if(infoSeq_len < 48)
	{
		header_len = 48;
		tail_len = 48;
	}

	total_len = header_len + infoSeq_len + tail_len;

	initPos = (infoSeq_len - header_len%infoSeq_len)%infoSeq_len;

	for(i = 0; i < total_len; i++)
	{

	    recv0 = recv_bit[((initPos + i)%infoSeq_len)*3];// * numTx;
		recv1 = recv_bit[((initPos + i)%infoSeq_len)*3 + 1];// * numTx;
		recv2 = recv_bit[((initPos + i)%infoSeq_len)*3 + 2];// * numTx;


		for(state = 0; state < 64; state++)
		{
			prevState_upper = state >> 1;
			prevState_bottom = prevState_upper + 32;
			if(state%2 == 0)
				out_upper = out[prevState_upper];
			else
				out_upper = 7 - out[prevState_upper];

			out0_upper = numTx*(out_upper >> 2);
			out1_upper = numTx*((out_upper >> 1)%2);
			out2_upper = numTx*(out_upper%2);

			branchMetric_upper = (out0_upper - recv0)*(out0_upper  - recv0) + (out1_upper - recv1)*(out1_upper - recv1) + (out2_upper - recv2)*(out2_upper - recv2);
			branchMetric_bottom = (numTx-out0_upper-recv0)*(numTx-out0_upper-recv0) + (numTx-out1_upper-recv1)*(numTx-out1_upper-recv1) + (numTx-out2_upper-recv2)*(numTx-out2_upper-recv2);	

			if((prevPathMetric[prevState_upper] + branchMetric_upper) < (prevPathMetric[prevState_bottom] + branchMetric_bottom))
			{

				prevSelState[i/8][state] = (1<<(i%8)) + prevSelState[i/8][state];//// ����ӣ�1,ÿ�α���Ҫ��ʼ��������ʼʱ��Ҫ��ʼ��,��λ���������(1<<(i%8))��Ҫ�У���


				currPathMetric[state] = prevPathMetric[prevState_upper] + branchMetric_upper;
			}
			else
			{

				currPathMetric[state] = prevPathMetric[prevState_bottom] + branchMetric_bottom;
			}
		}

		for(m = 0; m < 64; m++)
		{
			prevPathMetric[m] = currPathMetric[m];
		}
	}

	minValue = currPathMetric[0];
	minIdx = 0;
	for(i = 0; i < 64; i++)
	{
		if(currPathMetric[i] < minValue)
		{
			minValue = currPathMetric[i];
			minIdx = i;
		}
	}
	surState = minIdx;


	for(i = total_len-1; i > header_len; i--)
	{


		if ((prevSelState[i/8][surState]&(1<<(i%8)))!=0)// �����
			surState = surState>>1;
		else
			surState = (surState>>1)+32;

		if(i <= (header_len + infoSeq_len))
		{

            iii = i - header_len - 1;
			infoDec[iii/8] = ((surState%2) <<(7-(iii%8))) + infoDec[iii/8];//���ۼӣ�һ��ʼ�����ʼ��Ϊ0.��λ��ǰ����λ�ں�
		
		}
	}
	
	return minValue;

}



//byte to byte -2015-6-12 17:00:07 
//���룺recv[info_len*3]�������infoDec[info_len]���м����������prevSelState[(info_len_max*8 + 96)/8][64]--���㷨�ռ佫Ϊ1/8��
//ԭ���㷨������м����������recvSeq[info_len_max*24]--���㷨�Ѿ�������Ҫ��infoSeqDec[info_len_max*8]--���㷨�Ѿ�������Ҫ
void tbcc_dec(unsigned char recv[], unsigned char info_len, unsigned char infoDec[], unsigned char numTx)
{
	unsigned int i, m, j,iii = 0;
	unsigned char header_len = 48;
	unsigned char tail_len = 48;
	unsigned int total_len = 96;

	unsigned int prevPathMetric[64] = {0};
	unsigned int currPathMetric[64] = {0};

	unsigned char initPos;
	unsigned char recv0, recv1, recv2;
	unsigned char state;
	unsigned char prevState_upper, prevState_bottom;
	unsigned char out_upper, out0_upper, out1_upper, out2_upper;

	unsigned int branchMetric_upper, branchMetric_bottom;
	unsigned char surState;

	unsigned int minValue;
	unsigned char minIdx;

	unsigned int infoSeq_len = info_len*8;


        
    //ÿ�ζ�Ҫ��ʼ������ΪҪ��prevSelState��infoDec[i]�ۼӡ���Ҫ��
	for(i = 0; i < ((info_len_max*8+96)/8); i++)//info_len_max = 24,Ϊ���͵�������Ϣ������ֽ����������С��С��ԭ����1/8
	{
		if (i<info_len)
		   infoDec[i] = 0;//byte,�����,��ʼ��Ϊ0

		for(j = 0; j < 64; j++)
		{
			prevSelState[i][j]=0;//��ʼ��Ϊ0
		}
	}


	if(infoSeq_len < 48)
	{
		header_len = 48;
		tail_len = 48;
	}

	total_len = header_len + infoSeq_len + tail_len;

	initPos = (infoSeq_len - header_len%infoSeq_len)%infoSeq_len;

	for(i = 0; i < total_len; i++)
	{

	  //recv0 = recv_bit[((initPos + i)%infoSeq_len)*3];// * numTx;
	  //recv1 = recv_bit[((initPos + i)%infoSeq_len)*3 + 1];// * numTx;
	  //recv2 = recv_bit[((initPos + i)%infoSeq_len)*3 + 2];// * numTx;
            recv0 = 0;recv1=0;recv2=0;
		recv0 = (recv[(((initPos + i)%infoSeq_len)*3)/8]>>(7-((((initPos + i)%infoSeq_len)*3)%8)))%2;// * numTx;
		recv1 = (recv[(((initPos + i)%infoSeq_len)*3+1)/8]>>(7-((((initPos + i)%infoSeq_len)*3+1)%8)))%2;// * numTx;
		recv2 = (recv[(((initPos + i)%infoSeq_len)*3+2)/8]>>(7-((((initPos + i)%infoSeq_len)*3+2)%8)))%2;// * numTx;

		for(state = 0; state < 64; state++)
		{
			prevState_upper = state >> 1;
			prevState_bottom = prevState_upper + 32;
			if(state%2 == 0)
				out_upper = out[prevState_upper];
			else
				out_upper = 7 - out[prevState_upper];

			out0_upper = numTx*(out_upper >> 2);
			out1_upper = numTx*((out_upper >> 1)%2);
			out2_upper = numTx*(out_upper%2);

			branchMetric_upper = (out0_upper - recv0)*(out0_upper  - recv0) + (out1_upper - recv1)*(out1_upper - recv1) + (out2_upper - recv2)*(out2_upper - recv2);
			branchMetric_bottom = (numTx-out0_upper-recv0)*(numTx-out0_upper-recv0) + (numTx-out1_upper-recv1)*(numTx-out1_upper-recv1) + (numTx-out2_upper-recv2)*(numTx-out2_upper-recv2);	

			if((prevPathMetric[prevState_upper] + branchMetric_upper) < (prevPathMetric[prevState_bottom] + branchMetric_bottom))
			{

				prevSelState[i/8][state] = (1<<(i%8)) + prevSelState[i/8][state];//// ����ӣ�1,ÿ�α���Ҫ��ʼ��������ʼʱ��Ҫ��ʼ��,��λ���������(1<<(i%8))��Ҫ�У���


				currPathMetric[state] = prevPathMetric[prevState_upper] + branchMetric_upper;
			}
			else
			{

				currPathMetric[state] = prevPathMetric[prevState_bottom] + branchMetric_bottom;
			}
		}

		for(m = 0; m < 64; m++)
		{
			prevPathMetric[m] = currPathMetric[m];
		}
	}

	minValue = currPathMetric[0];
	minIdx = 0;
	for(i = 0; i < 64; i++)
	{
		if(currPathMetric[i] < minValue)
		{
			minValue = currPathMetric[i];
			minIdx = i;
		}
	}
	surState = minIdx;


	for(i = total_len-1; i > header_len; i--)
	{


		if ((prevSelState[i/8][surState]&(1<<(i%8)))!=0)// �����
			surState = surState>>1;
		else
			surState = (surState>>1)+32;

		if(i <= (header_len + infoSeq_len))
		{

            iii = i - header_len - 1;
			infoDec[iii/8] = ((surState%2) <<(7-(iii%8))) + infoDec[iii/8];//���ۼӣ�һ��ʼ�����ʼ��Ϊ0.��λ��ǰ����λ�ں�
		
		}
	}
	//�Ѿ���bitֱ��תΪ��byte��infoDec���Ϊbyte
	
}


//byte to byte -2015-6-12 17:00:07 infodata,����������ϲ��㷨��
//���룺recv[info_len*3]�������infoDec[info_len]���м����������prevSelState[(info_len_max*8 + 96)/8][64]--���㷨�ռ佫Ϊ1/8��
//ԭ���㷨������м����������recvSeq[info_len_max*24]--���㷨�Ѿ�������Ҫ��infoSeqDec[info_len_max*8]--���㷨�Ѿ�������Ҫ
void tbcc_dec_1(unsigned char recv[], unsigned char recv_1[], unsigned char info_len, unsigned char infoDec[], unsigned char numTx)
{
	unsigned int i, m, j,iii = 0;
	unsigned char header_len = 48;
	unsigned char tail_len = 48;
	unsigned int total_len = 96;

	unsigned int prevPathMetric[64] = {0};
	unsigned int currPathMetric[64] = {0};

	unsigned char initPos;
	unsigned char recv0, recv1, recv2;
	unsigned char state;
	unsigned char prevState_upper, prevState_bottom;
	unsigned char out_upper, out0_upper, out1_upper, out2_upper;

	unsigned int branchMetric_upper, branchMetric_bottom;
	unsigned char surState;

	unsigned int minValue;
	unsigned char minIdx;

	unsigned int infoSeq_len = info_len*8;


        
    //ÿ�ζ�Ҫ��ʼ������ΪҪ��prevSelState��infoDec[i]�ۼӡ���Ҫ��
	for(i = 0; i < ((info_len_max*8+96)/8); i++)//info_len_max = 24,Ϊ���͵�������Ϣ������ֽ����������С��С��ԭ����1/8
	{
		if (i<info_len)
		   infoDec[i] = 0;//byte,�����,��ʼ��Ϊ0

		for(j = 0; j < 64; j++)
		{
			prevSelState[i][j]=0;//��ʼ��Ϊ0
		}
	}


	if(infoSeq_len < 48)
	{
		header_len = 48;
		tail_len = 48;
	}

	total_len = header_len + infoSeq_len + tail_len;

	initPos = (infoSeq_len - header_len%infoSeq_len)%infoSeq_len;

	for(i = 0; i < total_len; i++)
	{

	  //recv0 = recv_bit[((initPos + i)%infoSeq_len)*3];// * numTx;
	  //recv1 = recv_bit[((initPos + i)%infoSeq_len)*3 + 1];// * numTx;
	  //recv2 = recv_bit[((initPos + i)%infoSeq_len)*3 + 2];// * numTx;

		recv0 = 0;recv1=0;recv2=0;

		recv0 = (recv[(((initPos + i)%infoSeq_len)*3)/8]>>(7-((((initPos + i)%infoSeq_len)*3)%8)))%2;// * numTx;
		recv1 = (recv[(((initPos + i)%infoSeq_len)*3+1)/8]>>(7-((((initPos + i)%infoSeq_len)*3+1)%8)))%2;// * numTx;
		recv2 = (recv[(((initPos + i)%infoSeq_len)*3+2)/8]>>(7-((((initPos + i)%infoSeq_len)*3+2)%8)))%2;// * numTx;


		recv0 = recv0 + (recv_1[(((initPos + i)%infoSeq_len)*3)/8]>>(7-((((initPos + i)%infoSeq_len)*3)%8)))%2;// * numTx;
		recv1 = recv1 + (recv_1[(((initPos + i)%infoSeq_len)*3+1)/8]>>(7-((((initPos + i)%infoSeq_len)*3+1)%8)))%2;// * numTx;
		recv2 = recv2 + (recv_1[(((initPos + i)%infoSeq_len)*3+2)/8]>>(7-((((initPos + i)%infoSeq_len)*3+2)%8)))%2;// * numTx;


		for(state = 0; state < 64; state++)
		{
			prevState_upper = state >> 1;
			prevState_bottom = prevState_upper + 32;
			if(state%2 == 0)
				out_upper = out[prevState_upper];
			else
				out_upper = 7 - out[prevState_upper];

			out0_upper = numTx*(out_upper >> 2);
			out1_upper = numTx*((out_upper >> 1)%2);
			out2_upper = numTx*(out_upper%2);

			branchMetric_upper = (out0_upper - recv0)*(out0_upper  - recv0) + (out1_upper - recv1)*(out1_upper - recv1) + (out2_upper - recv2)*(out2_upper - recv2);
			branchMetric_bottom = (numTx-out0_upper-recv0)*(numTx-out0_upper-recv0) + (numTx-out1_upper-recv1)*(numTx-out1_upper-recv1) + (numTx-out2_upper-recv2)*(numTx-out2_upper-recv2);	

			if((prevPathMetric[prevState_upper] + branchMetric_upper) < (prevPathMetric[prevState_bottom] + branchMetric_bottom))
			{

				prevSelState[i/8][state] = (1<<(i%8)) + prevSelState[i/8][state];//// ����ӣ�1,ÿ�α���Ҫ��ʼ��������ʼʱ��Ҫ��ʼ��,��λ���������(1<<(i%8))��Ҫ�У���


				currPathMetric[state] = prevPathMetric[prevState_upper] + branchMetric_upper;
			}
			else
			{

				currPathMetric[state] = prevPathMetric[prevState_bottom] + branchMetric_bottom;
			}
		}

		for(m = 0; m < 64; m++)
		{
			prevPathMetric[m] = currPathMetric[m];
		}
	}

	minValue = currPathMetric[0];
	minIdx = 0;
	for(i = 0; i < 64; i++)
	{
		if(currPathMetric[i] < minValue)
		{
			minValue = currPathMetric[i];
			minIdx = i;
		}
	}
	surState = minIdx;


	for(i = total_len-1; i > header_len; i--)
	{


		if ((prevSelState[i/8][surState]&(1<<(i%8)))!=0)// �����
			surState = surState>>1;
		else
			surState = (surState>>1)+32;

		if(i <= (header_len + infoSeq_len))
		{

            iii = i - header_len - 1;
			infoDec[iii/8] = ((surState%2) <<(7-(iii%8))) + infoDec[iii/8];//���ۼӣ�һ��ʼ�����ʼ��Ϊ0.��λ��ǰ����λ�ں�
		
		}
	}
	//�Ѿ���bitֱ��תΪ��byte��infoDec���Ϊbyte
	
}


//byte to byte -2015-6-12 17:00:07 infodata,����������ϲ��㷨��
//���룺recv[info_len*3]�������infoDec[info_len]���м����������prevSelState[(info_len_max*8 + 96)/8][64]--���㷨�ռ佫Ϊ1/8��
//ԭ���㷨������м����������recvSeq[info_len_max*24]--���㷨�Ѿ�������Ҫ��infoSeqDec[info_len_max*8]--���㷨�Ѿ�������Ҫ
void tbcc_dec_2(unsigned char recv[], unsigned char recv_1[], unsigned char recv_2[], unsigned char info_len, unsigned char infoDec[], unsigned char numTx)
{
	unsigned int i, m, j,iii = 0;
	unsigned char header_len = 48;
	unsigned char tail_len = 48;
	unsigned int total_len = 96;

	unsigned int prevPathMetric[64] = {0};
	unsigned int currPathMetric[64] = {0};

	unsigned char initPos;
	unsigned char recv0, recv1, recv2;
	unsigned char state;
	unsigned char prevState_upper, prevState_bottom;
	unsigned char out_upper, out0_upper, out1_upper, out2_upper;

	unsigned int branchMetric_upper, branchMetric_bottom;
	unsigned char surState;

	unsigned int minValue;
	unsigned char minIdx;

	unsigned int infoSeq_len = info_len*8;


        
    //ÿ�ζ�Ҫ��ʼ������ΪҪ��prevSelState��infoDec[i]�ۼӡ���Ҫ��
	for(i = 0; i < ((info_len_max*8+96)/8); i++)//info_len_max = 24,Ϊ���͵�������Ϣ������ֽ����������С��С��ԭ����1/8
	{
		if (i<info_len)
		   infoDec[i] = 0;//byte,�����,��ʼ��Ϊ0

		for(j = 0; j < 64; j++)
		{
			prevSelState[i][j]=0;//��ʼ��Ϊ0
		}
	}


	if(infoSeq_len < 48)
	{
		header_len = 48;
		tail_len = 48;
	}

	total_len = header_len + infoSeq_len + tail_len;

	initPos = (infoSeq_len - header_len%infoSeq_len)%infoSeq_len;

	for(i = 0; i < total_len; i++)
	{

	  //recv0 = recv_bit[((initPos + i)%infoSeq_len)*3];// * numTx;
	  //recv1 = recv_bit[((initPos + i)%infoSeq_len)*3 + 1];// * numTx;
	  //recv2 = recv_bit[((initPos + i)%infoSeq_len)*3 + 2];// * numTx;

		recv0 = 0;recv1=0;recv2=0;

		recv0 = (recv[(((initPos + i)%infoSeq_len)*3)/8]>>(7-((((initPos + i)%infoSeq_len)*3)%8)))%2;// * numTx;
		recv1 = (recv[(((initPos + i)%infoSeq_len)*3+1)/8]>>(7-((((initPos + i)%infoSeq_len)*3+1)%8)))%2;// * numTx;
		recv2 = (recv[(((initPos + i)%infoSeq_len)*3+2)/8]>>(7-((((initPos + i)%infoSeq_len)*3+2)%8)))%2;// * numTx;


		recv0 = recv0 + (recv_1[(((initPos + i)%infoSeq_len)*3)/8]>>(7-((((initPos + i)%infoSeq_len)*3)%8)))%2;// * numTx;
		recv1 = recv1 + (recv_1[(((initPos + i)%infoSeq_len)*3+1)/8]>>(7-((((initPos + i)%infoSeq_len)*3+1)%8)))%2;// * numTx;
		recv2 = recv2 + (recv_1[(((initPos + i)%infoSeq_len)*3+2)/8]>>(7-((((initPos + i)%infoSeq_len)*3+2)%8)))%2;// * numTx;

		recv0 = recv0 + (recv_2[(((initPos + i)%infoSeq_len)*3)/8]>>(7-((((initPos + i)%infoSeq_len)*3)%8)))%2;// * numTx;
		recv1 = recv1 + (recv_2[(((initPos + i)%infoSeq_len)*3+1)/8]>>(7-((((initPos + i)%infoSeq_len)*3+1)%8)))%2;// * numTx;
		recv2 = recv2 + (recv_2[(((initPos + i)%infoSeq_len)*3+2)/8]>>(7-((((initPos + i)%infoSeq_len)*3+2)%8)))%2;// * numTx;


		for(state = 0; state < 64; state++)
		{
			prevState_upper = state >> 1;
			prevState_bottom = prevState_upper + 32;
			if(state%2 == 0)
				out_upper = out[prevState_upper];
			else
				out_upper = 7 - out[prevState_upper];

			out0_upper = numTx*(out_upper >> 2);
			out1_upper = numTx*((out_upper >> 1)%2);
			out2_upper = numTx*(out_upper%2);

			branchMetric_upper = (out0_upper - recv0)*(out0_upper  - recv0) + (out1_upper - recv1)*(out1_upper - recv1) + (out2_upper - recv2)*(out2_upper - recv2);
			branchMetric_bottom = (numTx-out0_upper-recv0)*(numTx-out0_upper-recv0) + (numTx-out1_upper-recv1)*(numTx-out1_upper-recv1) + (numTx-out2_upper-recv2)*(numTx-out2_upper-recv2);	

			if((prevPathMetric[prevState_upper] + branchMetric_upper) < (prevPathMetric[prevState_bottom] + branchMetric_bottom))
			{

				prevSelState[i/8][state] = (1<<(i%8)) + prevSelState[i/8][state];//// ����ӣ�1,ÿ�α���Ҫ��ʼ��������ʼʱ��Ҫ��ʼ��,��λ���������(1<<(i%8))��Ҫ�У���


				currPathMetric[state] = prevPathMetric[prevState_upper] + branchMetric_upper;
			}
			else
			{

				currPathMetric[state] = prevPathMetric[prevState_bottom] + branchMetric_bottom;
			}
		}

		for(m = 0; m < 64; m++)
		{
			prevPathMetric[m] = currPathMetric[m];
		}
	}

	minValue = currPathMetric[0];
	minIdx = 0;
	for(i = 0; i < 64; i++)
	{
		if(currPathMetric[i] < minValue)
		{
			minValue = currPathMetric[i];
			minIdx = i;
		}
	}
	surState = minIdx;


	for(i = total_len-1; i > header_len; i--)
	{


		if ((prevSelState[i/8][surState]&(1<<(i%8)))!=0)// �����
			surState = surState>>1;
		else
			surState = (surState>>1)+32;

		if(i <= (header_len + infoSeq_len))
		{

            iii = i - header_len - 1;
			infoDec[iii/8] = ((surState%2) <<(7-(iii%8))) + infoDec[iii/8];//���ۼӣ�һ��ʼ�����ʼ��Ϊ0.��λ��ǰ����λ�ں�
		
		}
	}
	//�Ѿ���bitֱ��תΪ��byte��infoDec���Ϊbyte
	
}


//byte to byte -2015-6-12 17:00:07 infodata,�ĸ�������ϲ��㷨��
//���룺recv[info_len*3]�������infoDec[info_len]���м����������prevSelState[(info_len_max*8 + 96)/8][64]--���㷨�ռ佫Ϊ1/8��
//ԭ���㷨������м����������recvSeq[info_len_max*24]--���㷨�Ѿ�������Ҫ��infoSeqDec[info_len_max*8]--���㷨�Ѿ�������Ҫ
void tbcc_dec_3(unsigned char recv[], unsigned char recv_1[], unsigned char recv_2[], unsigned char recv_3[], unsigned char info_len, unsigned char infoDec[], unsigned char numTx)
{
	unsigned int i, m, j,iii = 0;
	unsigned char header_len = 48;
	unsigned char tail_len = 48;
	unsigned int total_len = 96;

	unsigned int prevPathMetric[64] = {0};
	unsigned int currPathMetric[64] = {0};

	unsigned char initPos;
	unsigned char recv0, recv1, recv2;
	unsigned char state;
	unsigned char prevState_upper, prevState_bottom;
	unsigned char out_upper, out0_upper, out1_upper, out2_upper;

	unsigned int branchMetric_upper, branchMetric_bottom;
	unsigned char surState;

	unsigned int minValue;
	unsigned char minIdx;

	unsigned int infoSeq_len = info_len*8;


        
    //ÿ�ζ�Ҫ��ʼ������ΪҪ��prevSelState��infoDec[i]�ۼӡ���Ҫ��
	for(i = 0; i < ((info_len_max*8+96)/8); i++)//info_len_max = 24,Ϊ���͵�������Ϣ������ֽ����������С��С��ԭ����1/8
	{
		if (i<info_len)
		   infoDec[i] = 0;//byte,�����,��ʼ��Ϊ0

		for(j = 0; j < 64; j++)
		{
			prevSelState[i][j]=0;//��ʼ��Ϊ0
		}
	}


	if(infoSeq_len < 48)
	{
		header_len = 48;
		tail_len = 48;
	}

	total_len = header_len + infoSeq_len + tail_len;

	initPos = (infoSeq_len - header_len%infoSeq_len)%infoSeq_len;

	for(i = 0; i < total_len; i++)
	{

	  //recv0 = recv_bit[((initPos + i)%infoSeq_len)*3];// * numTx;
	  //recv1 = recv_bit[((initPos + i)%infoSeq_len)*3 + 1];// * numTx;
	  //recv2 = recv_bit[((initPos + i)%infoSeq_len)*3 + 2];// * numTx;

		recv0 = 0;recv1=0;recv2=0;

		recv0 = (recv[(((initPos + i)%infoSeq_len)*3)/8]>>(7-((((initPos + i)%infoSeq_len)*3)%8)))%2;// * numTx;
		recv1 = (recv[(((initPos + i)%infoSeq_len)*3+1)/8]>>(7-((((initPos + i)%infoSeq_len)*3+1)%8)))%2;// * numTx;
		recv2 = (recv[(((initPos + i)%infoSeq_len)*3+2)/8]>>(7-((((initPos + i)%infoSeq_len)*3+2)%8)))%2;// * numTx;


		recv0 = recv0 + (recv_1[(((initPos + i)%infoSeq_len)*3)/8]>>(7-((((initPos + i)%infoSeq_len)*3)%8)))%2;// * numTx;
		recv1 = recv1 + (recv_1[(((initPos + i)%infoSeq_len)*3+1)/8]>>(7-((((initPos + i)%infoSeq_len)*3+1)%8)))%2;// * numTx;
		recv2 = recv2 + (recv_1[(((initPos + i)%infoSeq_len)*3+2)/8]>>(7-((((initPos + i)%infoSeq_len)*3+2)%8)))%2;// * numTx;

		recv0 = recv0 + (recv_2[(((initPos + i)%infoSeq_len)*3)/8]>>(7-((((initPos + i)%infoSeq_len)*3)%8)))%2;// * numTx;
		recv1 = recv1 + (recv_2[(((initPos + i)%infoSeq_len)*3+1)/8]>>(7-((((initPos + i)%infoSeq_len)*3+1)%8)))%2;// * numTx;
		recv2 = recv2 + (recv_2[(((initPos + i)%infoSeq_len)*3+2)/8]>>(7-((((initPos + i)%infoSeq_len)*3+2)%8)))%2;// * numTx;

		recv0 = recv0 + (recv_3[(((initPos + i)%infoSeq_len)*3)/8]>>(7-((((initPos + i)%infoSeq_len)*3)%8)))%2;// * numTx;
		recv1 = recv1 + (recv_3[(((initPos + i)%infoSeq_len)*3+1)/8]>>(7-((((initPos + i)%infoSeq_len)*3+1)%8)))%2;// * numTx;
		recv2 = recv2 + (recv_3[(((initPos + i)%infoSeq_len)*3+2)/8]>>(7-((((initPos + i)%infoSeq_len)*3+2)%8)))%2;// * numTx;



		for(state = 0; state < 64; state++)
		{
			prevState_upper = state >> 1;
			prevState_bottom = prevState_upper + 32;
			if(state%2 == 0)
				out_upper = out[prevState_upper];
			else
				out_upper = 7 - out[prevState_upper];

			out0_upper = numTx*(out_upper >> 2);
			out1_upper = numTx*((out_upper >> 1)%2);
			out2_upper = numTx*(out_upper%2);

			branchMetric_upper = (out0_upper - recv0)*(out0_upper  - recv0) + (out1_upper - recv1)*(out1_upper - recv1) + (out2_upper - recv2)*(out2_upper - recv2);
			branchMetric_bottom = (numTx-out0_upper-recv0)*(numTx-out0_upper-recv0) + (numTx-out1_upper-recv1)*(numTx-out1_upper-recv1) + (numTx-out2_upper-recv2)*(numTx-out2_upper-recv2);	

			if((prevPathMetric[prevState_upper] + branchMetric_upper) < (prevPathMetric[prevState_bottom] + branchMetric_bottom))
			{

				prevSelState[i/8][state] = (1<<(i%8)) + prevSelState[i/8][state];//// ����ӣ�1,ÿ�α���Ҫ��ʼ��������ʼʱ��Ҫ��ʼ��,��λ���������(1<<(i%8))��Ҫ�У���


				currPathMetric[state] = prevPathMetric[prevState_upper] + branchMetric_upper;
			}
			else
			{

				currPathMetric[state] = prevPathMetric[prevState_bottom] + branchMetric_bottom;
			}
		}

		for(m = 0; m < 64; m++)
		{
			prevPathMetric[m] = currPathMetric[m];
		}
	}

	minValue = currPathMetric[0];
	minIdx = 0;
	for(i = 0; i < 64; i++)
	{
		if(currPathMetric[i] < minValue)
		{
			minValue = currPathMetric[i];
			minIdx = i;
		}
	}
	surState = minIdx;


	for(i = total_len-1; i > header_len; i--)
	{


		if ((prevSelState[i/8][surState]&(1<<(i%8)))!=0)// �����
			surState = surState>>1;
		else
			surState = (surState>>1)+32;

		if(i <= (header_len + infoSeq_len))
		{

            iii = i - header_len - 1;
			infoDec[iii/8] = ((surState%2) <<(7-(iii%8))) + infoDec[iii/8];//���ۼӣ�һ��ʼ�����ʼ��Ϊ0.��λ��ǰ����λ�ں�
		
		}
	}
	//�Ѿ���bitֱ��תΪ��byte��infoDec���Ϊbyte
	
}


//byte to byte -2015-6-12 17:00:07 infodata,���������ϲ��㷨��
//���룺recv[info_len*3]�������infoDec[info_len]���м����������prevSelState[(info_len_max*8 + 96)/8][64]--���㷨�ռ佫Ϊ1/8��
//ԭ���㷨������м����������recvSeq[info_len_max*24]--���㷨�Ѿ�������Ҫ��infoSeqDec[info_len_max*8]--���㷨�Ѿ�������Ҫ
void tbcc_dec_4(unsigned char recv[], unsigned char recv_1[], unsigned char recv_2[], unsigned char recv_3[],unsigned char recv_4[], unsigned char info_len, unsigned char infoDec[], unsigned char numTx)
{
	unsigned int i, m, j,iii = 0;
	unsigned char header_len = 48;
	unsigned char tail_len = 48;
	unsigned int total_len = 96;

	unsigned int prevPathMetric[64] = {0};
	unsigned int currPathMetric[64] = {0};

	unsigned char initPos;
	unsigned char recv0, recv1, recv2;
	unsigned char state;
	unsigned char prevState_upper, prevState_bottom;
	unsigned char out_upper, out0_upper, out1_upper, out2_upper;

	unsigned int branchMetric_upper, branchMetric_bottom;
	unsigned char surState;

	unsigned int minValue;
	unsigned char minIdx;

	unsigned int infoSeq_len = info_len*8;


        
    //ÿ�ζ�Ҫ��ʼ������ΪҪ��prevSelState��infoDec[i]�ۼӡ���Ҫ��
	for(i = 0; i < ((info_len_max*8+96)/8); i++)//info_len_max = 24,Ϊ���͵�������Ϣ������ֽ����������С��С��ԭ����1/8
	{
		if (i<info_len)
		   infoDec[i] = 0;//byte,�����,��ʼ��Ϊ0

		for(j = 0; j < 64; j++)
		{
			prevSelState[i][j]=0;//��ʼ��Ϊ0
		}
	}


	if(infoSeq_len < 48)
	{
		header_len = 48;
		tail_len = 48;
	}

	total_len = header_len + infoSeq_len + tail_len;

	initPos = (infoSeq_len - header_len%infoSeq_len)%infoSeq_len;

	for(i = 0; i < total_len; i++)
	{

	  //recv0 = recv_bit[((initPos + i)%infoSeq_len)*3];// * numTx;
	  //recv1 = recv_bit[((initPos + i)%infoSeq_len)*3 + 1];// * numTx;
	  //recv2 = recv_bit[((initPos + i)%infoSeq_len)*3 + 2];// * numTx;

		recv0 = 0;recv1=0;recv2=0;

		recv0 = (recv[(((initPos + i)%infoSeq_len)*3)/8]>>(7-((((initPos + i)%infoSeq_len)*3)%8)))%2;// * numTx;
		recv1 = (recv[(((initPos + i)%infoSeq_len)*3+1)/8]>>(7-((((initPos + i)%infoSeq_len)*3+1)%8)))%2;// * numTx;
		recv2 = (recv[(((initPos + i)%infoSeq_len)*3+2)/8]>>(7-((((initPos + i)%infoSeq_len)*3+2)%8)))%2;// * numTx;


		recv0 = recv0 + (recv_1[(((initPos + i)%infoSeq_len)*3)/8]>>(7-((((initPos + i)%infoSeq_len)*3)%8)))%2;// * numTx;
		recv1 = recv1 + (recv_1[(((initPos + i)%infoSeq_len)*3+1)/8]>>(7-((((initPos + i)%infoSeq_len)*3+1)%8)))%2;// * numTx;
		recv2 = recv2 + (recv_1[(((initPos + i)%infoSeq_len)*3+2)/8]>>(7-((((initPos + i)%infoSeq_len)*3+2)%8)))%2;// * numTx;

		recv0 = recv0 + (recv_2[(((initPos + i)%infoSeq_len)*3)/8]>>(7-((((initPos + i)%infoSeq_len)*3)%8)))%2;// * numTx;
		recv1 = recv1 + (recv_2[(((initPos + i)%infoSeq_len)*3+1)/8]>>(7-((((initPos + i)%infoSeq_len)*3+1)%8)))%2;// * numTx;
		recv2 = recv2 + (recv_2[(((initPos + i)%infoSeq_len)*3+2)/8]>>(7-((((initPos + i)%infoSeq_len)*3+2)%8)))%2;// * numTx;

		recv0 = recv0 + (recv_3[(((initPos + i)%infoSeq_len)*3)/8]>>(7-((((initPos + i)%infoSeq_len)*3)%8)))%2;// * numTx;
		recv1 = recv1 + (recv_3[(((initPos + i)%infoSeq_len)*3+1)/8]>>(7-((((initPos + i)%infoSeq_len)*3+1)%8)))%2;// * numTx;
		recv2 = recv2 + (recv_3[(((initPos + i)%infoSeq_len)*3+2)/8]>>(7-((((initPos + i)%infoSeq_len)*3+2)%8)))%2;// * numTx;

		recv0 = recv0 + (recv_4[(((initPos + i)%infoSeq_len)*3)/8]>>(7-((((initPos + i)%infoSeq_len)*3)%8)))%2;// * numTx;
		recv1 = recv1 + (recv_4[(((initPos + i)%infoSeq_len)*3+1)/8]>>(7-((((initPos + i)%infoSeq_len)*3+1)%8)))%2;// * numTx;
		recv2 = recv2 + (recv_4[(((initPos + i)%infoSeq_len)*3+2)/8]>>(7-((((initPos + i)%infoSeq_len)*3+2)%8)))%2;// * numTx;


		for(state = 0; state < 64; state++)
		{
			prevState_upper = state >> 1;
			prevState_bottom = prevState_upper + 32;
			if(state%2 == 0)
				out_upper = out[prevState_upper];
			else
				out_upper = 7 - out[prevState_upper];

			out0_upper = numTx*(out_upper >> 2);
			out1_upper = numTx*((out_upper >> 1)%2);
			out2_upper = numTx*(out_upper%2);

			branchMetric_upper = (out0_upper - recv0)*(out0_upper  - recv0) + (out1_upper - recv1)*(out1_upper - recv1) + (out2_upper - recv2)*(out2_upper - recv2);
			branchMetric_bottom = (numTx-out0_upper-recv0)*(numTx-out0_upper-recv0) + (numTx-out1_upper-recv1)*(numTx-out1_upper-recv1) + (numTx-out2_upper-recv2)*(numTx-out2_upper-recv2);	

			if((prevPathMetric[prevState_upper] + branchMetric_upper) < (prevPathMetric[prevState_bottom] + branchMetric_bottom))
			{

				prevSelState[i/8][state] = (1<<(i%8)) + prevSelState[i/8][state];//// ����ӣ�1,ÿ�α���Ҫ��ʼ��������ʼʱ��Ҫ��ʼ��,��λ���������(1<<(i%8))��Ҫ�У���


				currPathMetric[state] = prevPathMetric[prevState_upper] + branchMetric_upper;
			}
			else
			{

				currPathMetric[state] = prevPathMetric[prevState_bottom] + branchMetric_bottom;
			}
		}

		for(m = 0; m < 64; m++)
		{
			prevPathMetric[m] = currPathMetric[m];
		}
	}

	minValue = currPathMetric[0];
	minIdx = 0;
	for(i = 0; i < 64; i++)
	{
		if(currPathMetric[i] < minValue)
		{
			minValue = currPathMetric[i];
			minIdx = i;
		}
	}
	surState = minIdx;


	for(i = total_len-1; i > header_len; i--)
	{


		if ((prevSelState[i/8][surState]&(1<<(i%8)))!=0)// �����
			surState = surState>>1;
		else
			surState = (surState>>1)+32;

		if(i <= (header_len + infoSeq_len))
		{

            iii = i - header_len - 1;
			infoDec[iii/8] = ((surState%2) <<(7-(iii%8))) + infoDec[iii/8];//���ۼӣ�һ��ʼ�����ʼ��Ϊ0.��λ��ǰ����λ�ں�
		
		}
	}
	//�Ѿ���bitֱ��תΪ��byte��infoDec���Ϊbyte
	
}

#else

const unsigned char out[64] =  {0, 3, 7, 4, 6, 5, 1, 2, 1, 2, 6, 5, 7, 4, 0, 3, 4, 7, 3, 0, 2, 1, 5, 6, 5, 6, 2, 1, 3, 0, 4, 7,
						  7, 4, 0, 3, 1, 2, 6, 5, 6, 5, 1, 2, 0, 3, 7, 4, 3, 0, 4, 7, 5, 6, 2, 1, 2, 1, 5, 6, 4, 7, 3, 0};

unsigned char infoSeq[info_len_max * 8] = {0};
unsigned char codeSeq[info_len_max * 24] = {0};

//byte to byte
void tbcc_enc(unsigned char info[], unsigned char info_len, unsigned char code[])
{
	//initialization on state
	unsigned char state = 0;
	unsigned int i, j;
	unsigned char tempOut = 0;

	unsigned int infoSeq_len = info_len * 8;
	
       for(i = 0; i < info_len*3; i++)
	{
        code[i] = 0;//2015.02.05��Ҫ
	}
        
        for(i = 0; i < info_len_max*24; i++)
	{
          codeSeq[i]=0;
	}
        for(i = 0; i < info_len_max*8; i++)
	{
          infoSeq[i]=0;
	}

	//DEC2BIN
	for(i = 0; i < info_len; i++)
	{
		for(j = 0; j < 8; j++)
		{
			infoSeq[7 - j + i*8] = (info[i] >> j)%2;
		}
	}

	state = infoSeq[infoSeq_len - 6];
	for(i = 2; i < 7; i++)
	{
		state = (state << 1) + infoSeq[infoSeq_len - (7 - i)];
	}

	for(i = 0; i < infoSeq_len; i++)
	{
		if(infoSeq[i] == 0)
			tempOut = out[state];
		else
			tempOut = 7 - out[state];

		codeSeq[3*i] = tempOut >> 2;
		codeSeq[3*i + 1] = (tempOut >> 1)%2;
		codeSeq[3*i + 2] = tempOut%2;
		state = ((state << 1) + infoSeq[i])%64;
	}

	//BIN2DEC
	for(i = 0; i < info_len*3; i++)
	{
		for(j = 0; j < 8; j++)
		{
			code[i] += (codeSeq[7 - j + 8*i] << j);
		}
	}
}


unsigned char prevSelState[info_len_max*8 + 96][64] = {0};
unsigned char recvSeq[info_len_max*24] = {0};
unsigned char infoSeqDec[info_len_max*8] = {0};


//byte to byte 
void tbcc_dec(unsigned char recv[], unsigned char info_len, unsigned char infoDec[], unsigned char numTx)
{
	unsigned int i, m, j;
	unsigned char header_len = 48;
	unsigned char tail_len = 48;
	unsigned int total_len = 96;

//        unsigned char prevSelState[info_len*8 + 24][64] = {0};
	unsigned int prevPathMetric[64] = {0};
	unsigned int currPathMetric[64] = {0};

	unsigned char initPos;
	unsigned char recv0, recv1, recv2;
	unsigned char state;
	unsigned char prevState_upper, prevState_bottom;
	unsigned char out_upper, out0_upper, out1_upper, out2_upper;

	unsigned int branchMetric_upper, branchMetric_bottom;
	unsigned char surState;
	unsigned int minValue;
	unsigned char minIdx;

	unsigned int infoSeq_len = info_len*8;


          for(i = 0; i < (info_len_max); i++)
	{
           infoDec[i]=0;//2015.02.05
	}
        //ÿ�ζ�Ҫ��ʼ��
        for(i = 0; i < (info_len_max*3+96); i++)
	{
		for(j = 0; j < 64; j++)
		{
			prevSelState[i][j]=0;
		}
	}
         for(i = 0; i < (info_len_max*24); i++)
	{
           recvSeq[i]=0;
	}
        
        for(i = 0; i < (info_len_max*8); i++)
	{
           infoSeqDec[i]=0;
	}
        
	//DEC2BIN
	for(i = 0; i < info_len*3; i++)
	{
		for(j = 0; j < 8; j++)
		{
			recvSeq[7 - j + i*8] = (recv[i] >> j)%2;
		}
	}

	if(infoSeq_len < 48)
	{
		header_len = 48;
		tail_len = 48;
	}

	total_len = header_len + infoSeq_len + tail_len;

	initPos = (infoSeq_len - header_len%infoSeq_len)%infoSeq_len;

	for(i = 0; i < total_len; i++)
	{
		recv0 = recvSeq[((initPos + i)%infoSeq_len)*3];// * numTx;
		recv1 = recvSeq[((initPos + i)%infoSeq_len)*3 + 1];// * numTx;
		recv2 = recvSeq[((initPos + i)%infoSeq_len)*3 + 2];// * numTx;

		for(state = 0; state < 64; state++)
		{
			prevState_upper = state >> 1;
			prevState_bottom = prevState_upper + 32;
			if(state%2 == 0)
				out_upper = out[prevState_upper];
			else
				out_upper = 7 - out[prevState_upper];

			out0_upper = numTx*(out_upper >> 2);
			out1_upper = numTx*((out_upper >> 1)%2);
			out2_upper = numTx*(out_upper%2);

			branchMetric_upper = (out0_upper - recv0)*(out0_upper  - recv0) + (out1_upper - recv1)*(out1_upper - recv1) + (out2_upper - recv2)*(out2_upper - recv2);
			branchMetric_bottom = (numTx-out0_upper-recv0)*(numTx-out0_upper-recv0) + (numTx-out1_upper-recv1)*(numTx-out1_upper-recv1) + (numTx-out2_upper-recv2)*(numTx-out2_upper-recv2);	

			if((prevPathMetric[prevState_upper] + branchMetric_upper) < (prevPathMetric[prevState_bottom] + branchMetric_bottom))
			{
				prevSelState[i][state] = prevState_upper;
				currPathMetric[state] = prevPathMetric[prevState_upper] + branchMetric_upper;
			}
			else
			{
				prevSelState[i][state] = prevState_bottom;
				currPathMetric[state] = prevPathMetric[prevState_bottom] + branchMetric_bottom;
			}
		}

		for(m = 0; m < 64; m++)
		{
			prevPathMetric[m] = currPathMetric[m];
		}
	}

	minValue = currPathMetric[0];
	minIdx = 0;
	for(i = 0; i < 64; i++)
	{
		if(currPathMetric[i] < minValue)
		{
			minValue = currPathMetric[i];
			minIdx = i;
		}
	}
	surState = minIdx;

	for(i = total_len-1; i > header_len; i--)
	{
		surState = prevSelState[i][surState];
		if(i <= header_len + infoSeq_len)
			infoSeqDec[i - header_len - 1] = surState%2;
	}

	//BIN2DEC
	for(i = 0; i < info_len; i++)
	{
		infoDec[i] = 0;
		for(j = 0; j < 8; j++)
		{
			infoDec[i] += (infoSeqDec[7 - j + 8*i] << j);
		}
	}

}


//bit to byte 
void tbcc_dec_bit2byte(unsigned char recv_bit[], unsigned char info_len, unsigned char infoDec[], unsigned char numTx)
{
	unsigned int i, m, j;
	unsigned char header_len = 48;
	unsigned char tail_len = 48;
	unsigned int total_len = 96;

//        unsigned char prevSelState[info_len*8 + 24][64] = {0};
	unsigned int prevPathMetric[64] = {0};
	unsigned int currPathMetric[64] = {0};

	unsigned char initPos;
	unsigned char recv0, recv1, recv2;
	unsigned char state;
	unsigned char prevState_upper, prevState_bottom;
	unsigned char out_upper, out0_upper, out1_upper, out2_upper;

	unsigned int branchMetric_upper, branchMetric_bottom;
	unsigned char surState;
	unsigned int minValue;
	unsigned char minIdx;

	unsigned int infoSeq_len = info_len*8;

  for(i = 0; i < (info_len_max); i++)
	{
           infoDec[i]=0;//2015.02.05
	}
        
        //ÿ�ζ�Ҫ��ʼ��
        for(i = 0; i < (info_len_max*3+96); i++)
	{
		for(j = 0; j < 64; j++)
		{
			prevSelState[i][j]=0;
		}
	}
         for(i = 0; i < (info_len_max*24); i++)
	{
           recvSeq[i]=0;
	}
        
        for(i = 0; i < (info_len_max*8); i++)
	{
           infoSeqDec[i]=0;
	}
        
	
	for(i = 0; i < info_len*24; i++)
	{
          recvSeq[i] = recv_bit[i];
	}

	if(infoSeq_len < 48)
	{
		header_len = 48;
		tail_len = 48;
	}

	total_len = header_len + infoSeq_len + tail_len;

	initPos = (infoSeq_len - header_len%infoSeq_len)%infoSeq_len;

	for(i = 0; i < total_len; i++)
	{
		recv0 = recvSeq[((initPos + i)%infoSeq_len)*3];// * numTx;
		recv1 = recvSeq[((initPos + i)%infoSeq_len)*3 + 1];// * numTx;
		recv2 = recvSeq[((initPos + i)%infoSeq_len)*3 + 2];// * numTx;

		for(state = 0; state < 64; state++)
		{
			prevState_upper = state >> 1;
			prevState_bottom = prevState_upper + 32;
			if(state%2 == 0)
				out_upper = out[prevState_upper];
			else
				out_upper = 7 - out[prevState_upper];

			out0_upper = numTx*(out_upper >> 2);
			out1_upper = numTx*((out_upper >> 1)%2);
			out2_upper = numTx*(out_upper%2);

			branchMetric_upper = (out0_upper - recv0)*(out0_upper  - recv0) + (out1_upper - recv1)*(out1_upper - recv1) + (out2_upper - recv2)*(out2_upper - recv2);
			branchMetric_bottom = (numTx-out0_upper-recv0)*(numTx-out0_upper-recv0) + (numTx-out1_upper-recv1)*(numTx-out1_upper-recv1) + (numTx-out2_upper-recv2)*(numTx-out2_upper-recv2);	

			if((prevPathMetric[prevState_upper] + branchMetric_upper) < (prevPathMetric[prevState_bottom] + branchMetric_bottom))
			{
				prevSelState[i][state] = prevState_upper;
				currPathMetric[state] = prevPathMetric[prevState_upper] + branchMetric_upper;
			}
			else
			{
				prevSelState[i][state] = prevState_bottom;
				currPathMetric[state] = prevPathMetric[prevState_bottom] + branchMetric_bottom;
			}
		}

		for(m = 0; m < 64; m++)
		{
			prevPathMetric[m] = currPathMetric[m];
		}
	}

	minValue = currPathMetric[0];
	minIdx = 0;
	for(i = 0; i < 64; i++)
	{
		if(currPathMetric[i] < minValue)
		{
			minValue = currPathMetric[i];
			minIdx = i;
		}
	}
	surState = minIdx;

	for(i = total_len-1; i > header_len; i--)
	{
		surState = prevSelState[i][surState];
		if(i <= header_len + infoSeq_len)
			infoSeqDec[i - header_len - 1] = surState%2;
	}

	//BIN2DEC
	for(i = 0; i < info_len; i++)
	{
		infoDec[i] = 0;
		for(j = 0; j < 8; j++)
		{
			infoDec[i] += (infoSeqDec[7 - j + 8*i] << j);
		}
	}

}
void ResetBeaconBuffer(unsigned char bufferbeacontbcc[],unsigned char bufferbeaconEncode[],unsigned char bufferBeaconInterleave[])
{
  uint8 i ;
  
  for (i=0;i<BeaconTBCCOutputLength;i++)
       bufferbeacontbcc[i] = 0;
  
   for (i=0;i<BeaconLength;i++)
   {
       bufferbeaconEncode[i] = 0;
       bufferBeaconInterleave[i] = 0;
   }
}

#endif

#endif
