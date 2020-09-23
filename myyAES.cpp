#include"myyAES.h"
#include<bitset>
#include<iostream>
#include<stdio.h>
using namespace std;
using std::bitset;

const int Max=8;

myyAES::myyAES(int *meg, int length){ //初始化各个变量
	len = length ;

	for(int i=0;i<len;i++)  MEG[i]=meg[i];

	blockCnt = 0 ;

	memset(roundKey,0,176);
	memset(ivInv,0,16) ;		
	round = 0;

	group(len);  // 分组  
	
	creatRoundKey() ;
	
	cout<<"\n\n轮密钥"<<endl; 
	for(int i=0;i<44;i++){
		for(int j=0;j<4;j++){
			printf("%x ", roundKey[i][j]);
		} 
		cout<<endl;
		if((i+1)%4==0)  cout<<endl;
	}
	
	
	lenInv = 0 ;
}


myyAES::~myyAES(){
		delete [] MEG;
		delete []megBlocks;
		blockCnt = 0 ;
		delete []roundKey ;
		round = 0 ;
		delete []iv ;
		delete []MixMatrix ;
		delete []paiS ;
		delete []rcon ;
		
		lenInv = 0;
		delete []cipherBlocks ;
		blockCntInv = 0 ;
		delete []ivInv;
		delete []paiSInv ;
		delete []MixMatrixInv ;
}

void myyAES::group(int len){
	// 填充
	int remain=16;
	if(len%16==0){  //正好够16个，就补充16个16 
		for(int i=0;i<16;i++)
			MEG[i+len] = 0x10 ;
	}
	else{  // 最后一块不够16个，补充16个remain 
		int had = len%16 ;
		if(had<0)
			had +=16 ;
		
		remain = 16-had ;
		for(int i=0;i<remain;i++)
			MEG[i+len] = remain ;
	}
	
	len+=remain;
	blockCnt = len/16 ;
	
	int  cnt=0;
	for(int i=0;i<blockCnt;i++){
		for(int j=0;j<4;j++){
			for(int k=0;k<4;k++){
					megBlocks[i][k][j] = MEG[cnt++] ;
				}
			}
		}
		
		
	} 

	

void myyAES::creatRoundKey(){
	roundKey[0][0]=0x6d;
	roundKey[0][1]=0x65;
	roundKey[0][2]=0x6e;
	roundKey[0][3]=0x67;
	roundKey[1][0]=0x79;
	roundKey[1][1]=0x61;
	roundKey[1][2]=0x79;
	roundKey[1][3]=0x75;
	roundKey[2][0]=0x61;
	roundKey[2][1]=0x6E;
	roundKey[2][2]=roundKey[2][3]=roundKey[3][0]=roundKey[3][1]=roundKey[3][2]=roundKey[3][3]=0x0;
	
	
	
	for(int i=4;i<44;i++){

		if(i%4!=0){
			for(int j=0;j<4;j++){  //异或  
				roundKey[i][j] = roundKey[i-4][j] ^ roundKey[i-1][j] ;
			}
		}

		else{
			//k[i] = k[i-4] xor tk[i-1]
			//字循环  字节代换  轮常量异或
			int a[4]={0};
			f(roundKey[i-1], a, i) ;
			for(int j=0;j<4;j++){	
				roundKey[i][j] = roundKey[i-4][j]^a[j] ;
			}
			
			
		}
		
	}
	
	
}


void myyAES::f(const int rk[4], int *a, int number){
	//字循环  字节代换  轮常量异或
	//字循环  一个字中的四个字节循环左移一个字节 
	for(int j=0;j<4;j++){
		a[j] = rk[(j+1)%4];
	}

	//字节代换
	for(int j=0;j<4;j++){
		int shi = a[j]/16 ;
		int ge = a[j]%16 ;
		a[j] = paiS[shi][ge] ;
	}
	

	//轮常量异或  用于第j轮的密钥就和con[j]异或  j=number/4 
	for(int j=0;j<4;j++){  //有四个字节  一个字节8位 
		int temp = a[j] ;
		a[j] = temp ^ rcon[number/4-1][j] ;
	}
	
}


// block和密钥的block异或 
void myyAES::addRoundKey(int number, int round){  //state,roundkey
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){ 
			int t = megBlocks[number][j][i] ;
		    megBlocks[number][j][i] = t ^ roundKey[4*round+i][j] ;
		}
	}
} 

// 字节代换 
void myyAES::subBytes(int number) {
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			int a = megBlocks[number][i][j]/16 ;
			int b = megBlocks[number][i][j]%16 ;
			int t =  paiS[a][b] ;
			megBlocks[number][i][j] = t ; 
		}
	}

} 

// 行移位 
void myyAES::shiftRows(int number){  //state
	for(int i=1;i<4;i++){
		int temp[4] ;
		for(int j=0;j<4;j++)  temp[j] = megBlocks[number][i][j] ; 
		for(int k=0;k<4;k++){
			megBlocks[number][i][k] = temp[(k+i)%4] ;
		}
	}

}  


int degree(const bitset<2*Max> a){
	for(int i=2*Max-1;i>=0;i--){
		if(a[i]==1)  return i ;
	}
	return 0 ;
}


bitset<2*Max> mod(bitset<2*Max> a){
	bitset<Max+1> px;
	bitset<Max> rx;
	px[8] = px[4] = px[3] = px[1] = px[0] = 1;
	
	rx[4] = rx[3] = rx[1] = rx[0] = 1;

	bitset<2*Max> _r = bitset<2*Max>(rx.to_string()) ;
	bitset<2*Max> ans = bitset<2*Max>(a.to_string().substr(Max)); // 原来的前面个不要 
	
	while(degree(a)>Max-1){
		ans.reset();
		ans = bitset<2*Max>(a.to_string().substr(Max));
		for(int i=Max;i<2*Max;i++){
			if(a[i])  ans^=_r<<(i-Max);
		}
		a=ans;
	}

	return ans;
}

bitset<2*Max> multiple(bitset<2*Max> a, bitset<2*Max> b){
	bitset<2*Max> ans ;
	bitset<2*Max> temp(a.to_string()) ;

	for(int i=0;i<2*Max;i++){
		if(b[i])  ans^=temp<<i ;

	}
	
	return mod(ans) ;
}

 
 //我可以的！ 
//列混合						megBlocks[number]
void myyAES::mixColumns(int number) { //state, mixmatrix
	//一行一行的算
	int bloc[4][4];
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++)
			bloc[i][j] = megBlocks[number][i][j] ;
	}
	
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			megBlocks[number][i][j] = 0 ;
			for(int k=0;k<4;k++){
				bitset<2*Max> a(MixMatrix[i][k]), b(bloc[k][j]) ;
				bitset<2*Max> t = multiple(a,b) ;
				megBlocks[number][i][j] ^= t.to_ulong()  ;
			}
		}
	}
	

} 





int myyAES::paiS[16][16]={
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 
} ;

int myyAES::MixMatrix[4][4]={0x02,0x03,0x01,0x01,
							 0x01,0x02,0x03,0x01,
			   				 0x01,0x01,0x02,0x03,
							 0x03,0x01,0x01,0x02
} ;

// 只有4的倍数才用到它  44里面只有10个4的倍数 
const int myyAES::rcon[10][4]={	 0x01, 0x00, 0x00, 0x00,
								 0x02, 0x00, 0x00, 0x00,
								 0x04, 0x00, 0x00, 0x00,
								 0x08, 0x00, 0x00, 0x00,
								 0x10, 0x00, 0x00, 0x00,
								 0x20, 0x00, 0x00, 0x00,
								 0x40, 0x00, 0x00, 0x00,
								 0x80, 0x00, 0x00, 0x00,
								 0x1b, 0x00, 0x00, 0x00,
								 0x36, 0x00, 0x00, 0x00
} ;


 //  字符串123+数字00000……000 
int myyAES::iv[4][4]={0x31, 0x00, 0x00, 0x00, 
					  0x32, 0x00, 0x00, 0x00, 
					  0x33, 0x00, 0x00, 0x00, 
					  0x00, 0x00, 0x00, 0x00 } ;


//  我真的太棒了！ 
int myyAES::encrypt(int *text){
	for(int i=0;i<blockCnt;i++){

		for(int j=0;j<4;j++){
			for(int k=0;k<4;k++){
				megBlocks[i][k][j]^=iv[k][j];
			}
		}
	//	print(round);
	
		round=0;
		addRoundKey(i,round);
	//	print(round);
		round++;
		
		
		while(round<10){
			
	//		cout<<"in 字节代换"<<endl; 
			subBytes(i);
	//		print(round);
			
	//		cout<<"in 行移位"<<endl;
			shiftRows(i) ;
	//		print(round);
			
	//		cout<<"in 列混合"<<endl;
			mixColumns(i);
	//		print(round);	
					
	//		cout<<"in 轮密钥加"<<endl;		
			addRoundKey(i,round);
	//		print(round);
			round++;
		}
		
		
	//	cout<<"最后一轮"<<endl;
	//	cout<<"in 字节代换"<<endl; 
		subBytes(i);
	//	print(round);
		
	//	cout<<"in 行移位"<<endl;
		shiftRows(i) ;
	//	print(round);	
					
	//	cout<<"in 轮密钥加"<<endl;
	//	print(round);
		addRoundKey(i,round);
	//	print(round);
		
		
		for(int j=0;j<4;j++){
			for(int k=0;k<4;k++){
				iv[k][j] = megBlocks[i][k][j] ;
			}
		}
		
	}

//	cout<<"密文十六进制"<<endl; 
	int n=0;
	for(int i=0;i<blockCnt;i++){
		for(int j=0;j<4;j++){
			for(int k=0;k<4;k++){
				text[n++] = megBlocks[i][k][j] ;
			//	printf("%02x ",megBlocks[i][k][j]);
			}
		//	cout<<endl;
		}
	//	cout<<endl;
	}
	return n;
}


void myyAES::print(int round){
	cout<<"round="<<round<<endl;
	for(int i=0;i<blockCnt;i++){
		for(int j=0;j<4;j++){
			for(int k=0;k<4;k++){
				printf("%x ",megBlocks[i][j][k]);
			}
			cout<<endl;
		}
		cout<<endl;
	}
	cout<<endl;
}


int myyAES::paiSInv[16][16] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};



int myyAES::MixMatrixInv[4][4]={0x0e, 0x0b, 0x0d, 0x09,
								0x09, 0x0e, 0x0b, 0x0d,
								0x0d, 0x09, 0x0e, 0x0b,
								0x0b, 0x0d, 0x09, 0x0e} ;



void myyAES::groupInv(int *text){
	blockCntInv = lenInv/16 ;
	int t=0;
	
	for(int i=0;i<blockCntInv;i++){
		for(int j=0;j<4;j++){
			for(int k=0;k<4;k++){
				cipherBlocks[i][k][j] = text[t++] ;
			}
		}
	}

}

void myyAES::addRoundKeyInv(int number, int round){

	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){ 
			int t = cipherBlocks[number][j][i] ;
		    cipherBlocks[number][j][i] = t ^ roundKey[4*round+i][j] ;
		}
	}
}

void myyAES::shiftRowsInv(int number){
	int temp[4] ;
	
	for(int i=1;i<4;i++){
		for(int j=0;j<4;j++)   temp[j] = cipherBlocks[number][i][j] ;
		for(int k=0;k<4;k++){
			int pos = (k-i)%4;
			if(pos<0)  pos+=4 ;
			cipherBlocks[number][i][k] = temp[pos] ;
		}
	}
	
}

void myyAES::subBytesInv(int number){
	
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			int t = cipherBlocks[number][i][j] ;
			int a = t/16 ;
			int b = t%16;
			cipherBlocks[number][i][j] = paiSInv[a][b] ;
		}
	}
	
}

void myyAES::mixColumnsInv(int number){
	int bloc[4][4];
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++)
			bloc[i][j] = cipherBlocks[number][i][j] ;
	}
	
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			cipherBlocks[number][i][j] = 0 ;
			for(int k=0;k<4;k++){
				bitset<2*Max> a(MixMatrixInv[i][k]), b(bloc[k][j]) ;
				bitset<2*Max> t = multiple(a,b) ;
				cipherBlocks[number][i][j] ^= t.to_ulong()  ;
			}
		}
	}
	
}

void myyAES::printInv(int round){
//	cout<<"round="<<round<<endl;
	for(int i=0;i<blockCntInv;i++){
		for(int j=0;j<4;j++){
			for(int k=0;k<4;k++){
				printf("%x ",cipherBlocks[i][j][k]);
			}
			cout<<endl;
		}
		cout<<endl;
	}
	cout<<endl;
}



void myyAES::decryption(int *text, int l){
	lenInv = l;
	groupInv(text);
//	cout<<"分组后"<<endl; 
//	printInv(-1);	
	memset(text,0,1024) ;
	
	blockCntInv = lenInv/16 ;

	for(int i=blockCntInv-1;i>=0;i--){
		round = 10;
	//	printInv(round) ;
		addRoundKeyInv(i, round) ;
		round-- ;
//		printInv(round) ;
		
		while(round>=1){
	//		cout<<"逆向行移位"<<endl; 
			shiftRowsInv(i) ;
	//		printInv(round);
	
	//		cout<<"逆向字节替代"<<endl;
			subBytesInv(i) ;
	//		printInv(round);
	
	//		cout<<"逆向轮密钥加"<<endl;
			addRoundKeyInv(i, round) ;
	//		printInv(round);
	
	//		cout<<"逆向列混合"<<endl;
			mixColumnsInv(i) ;
	//		printInv(round);
			round-- ;
		}
		
		//最后一轮
		//	cout<<"round="<<round<<endl;
			shiftRowsInv(i) ;
			subBytesInv(i) ;
			addRoundKeyInv(i, round) ;	

		// CBC
		

		if(i==0){
			ivInv[0][0] = 0x31;
			ivInv[1][0] = 0x32;
			ivInv[2][0] = 0x33;
			ivInv[3][0] = ivInv[0][1] = ivInv[0][2] = ivInv[0][3] = ivInv[1][1] = ivInv[1][2] = ivInv[1][3] = ivInv[2][1] = ivInv[2][2] = ivInv[2][3] = ivInv[3][1] = ivInv[3][2] = ivInv[3][3] = 0x00 ;
		}
		else{
			for(int j=0;j<4;j++){
				for(int k=0;k<4;k++){
					ivInv[j][k] = cipherBlocks[i-1][j][k] ;
				}
			//	cout<<endl;
			}
		}

		for(int j=0;j<4;j++){
			for(int k=0;k<4;k++){
				cipherBlocks[i][j][k] ^= ivInv[j][k] ;
			}
		}

	}
	
//	printInv(round);
	

	int cnt=0;
	for(int i=0;i<blockCntInv;i++){
		for(int j=0;j<4;j++){
			for(int k=0;k<4;k++){
				text[cnt++] = cipherBlocks[i][k][j] ;
			}
		}
	}

	int padding = text[--cnt];
	
	for(int i=0;i<padding;i++){
		if(text[lenInv-i-1]!=padding) {
			cout<<"解密失败！i="<<i<<endl;
			return ;
		}
		text[lenInv-i-1] = '\0' ;
	}
}

