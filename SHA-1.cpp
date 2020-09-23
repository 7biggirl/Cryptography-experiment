#include<iostream>
#include<stdio.h>
#include<bitset>
#include<windows.h>
using namespace std;

unsigned long h[5] ;
unsigned long w[80] ;  // 一共有80个，每个32位，也就是4个字节 
unsigned long k[4] ;

unsigned long rotl(unsigned long x, int shift){
	return (x<<shift)|(x>>(32-shift)) ;
}

int padding(char m[3072], unsigned long meg[3072]){
	int len = strlen(m) ;
	for(int i=0;i<len;i++)  meg[i] = m[i] ;

	//  每一字母（数字）化成二进制的8位
	int i=1;
	if(len%64!=56){
		meg[len] = 0x80;
		for(i;(i+len)%64!=56;i++){
			meg[i+len]=0x0 ;
		}
	}

	// 最后的64bit用于放数据长度  64/8=8；也就是8字节
	bitset<64> temp(len*8) ;
	string lenPad = temp.to_string() ;
	int j=0;
	for(j;j<8;j++){
		bitset<8> t(lenPad,j*8,8) ;
		meg[len+i+j] = t.to_ulong() ;
	}

	len = len+i+j; 
	
	return len ;
}

void initial(){
	h[0] = 0x67452301 ;
	h[1] = 0xefcdab89 ;
	h[2] = 0x98badcfe ;
	h[3] = 0x10325476 ;
	h[4] = 0xc3d2e1f0 ;

	k[0] = 0x5a827999 ;
	k[1] = 0x6ed9eba1 ;
	k[2] = 0x8f1bbcdc ;
	k[3] = 0xca62c1d6 ;
}

void cal_w(unsigned long block[64]){  //64*8 = 512bit
	//每4个block(8位)组成一个w(32位) 
	int cnt=0;
	for(int i=0;i<64;i+=4){
		bitset<32> t1(block[i]) , t2(block[i+1]), t3(block[i+2]), t4(block[i+3]) ;
		cout<<block[i]<<" "<<block[i+1]<<" "<<block[i+2]<<" "<<block[i+3]<<endl;
		t1 = t1<<24;
		t2 = t2<<16;
		t3 = t3<<8 ;
		bitset<32> ans = t1^t2^t3^t4;
		//cout<<ans.to_ulong()<<endl;
		printf("%x\n", ans.to_ulong()) ;
		w[cnt++] = ans.to_ulong() ;
	} 

	for(int i=16;i<80;i++){
		unsigned long t = w[i-3]^w[i-8]^w[i-14]^w[i-16] ;
		w[i] = rotl(t,1) ;
	}

}

unsigned long f(unsigned long t, unsigned long b, unsigned long c, unsigned long d){
	if(t/20==0){
		return (b&c) | (~b&d) ;
	}
	if(t/20==2){
		return (b&c) | (b&d) | (c&d) ;
	}
	return b^c^d ;
}

void cal(unsigned long meg[3072], int len, unsigned long mac[5]){
	unsigned long block[64] ;  //64 *8 = 512bit
	int blockCnt = len/64 ;
	int cnt=0;

	for(int i=0;i<blockCnt;i++){
		for(int j=0;j<64;j++){
			block[j] = meg[cnt++] ;
		}

		cal_w(block);

		unsigned long a=h[0], b=h[1], c=h[2], d=h[3], e=h[4] ;	
		
		for(int t=0;t<80;t++){
			unsigned long fans = f(t,b,c,d) ;
			unsigned long temp = rotl(a,5) + fans + e + w[t] + k[t/20] ;
			e = d ;
			d = c ;
			c = rotl(b,30) ;
			b = a ;
			a = temp ;
		}

		h[0] += a ;
		h[1] += b ;
		h[2] += c ;
		h[3] += d ;
		h[4] += e ;

	}

	mac[0] = h[0] ;
	mac[1] = h[1] ;
	mac[2] = h[2] ;
	mac[3] = h[3] ;
	mac[4] = h[4] ;
}

void sha_1(char m[3072], unsigned long mac[5]){
	unsigned long meg[3072] ;

	int len = padding(m, meg) ;// 
	initial() ;  //初始化h k
	cal(meg, len, mac) ;

}


int main(){
	char meg[3072];
	unsigned long mac[5];
	
	while(1){
		cout<<"请输入SHA-1加密字符串：\n";
		gets(meg) ;
		sha_1(meg, mac);
		printf("\n散列值：%08x%08x%08x%08x%08x\n\n\n\n",mac[0],mac[1],mac[2],mac[3],mac[4]) ;
	}
	
//	system("pause") ;
	 
}


