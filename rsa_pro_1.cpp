#include<iostream>
#include<gmpxx.h>
#include<gmp.h>
#include<bitset>
#include<cstdlib>
#include<ctime>
#include<cstring>
#include<string.h>
#include<sstream>
#include<string>
#include<windows.h>
#include<stdlib.h>

using namespace std;
#define LENGTH 1024

mpz_class key_n, p, q, key_e, key_d, n2;


/*  也可以！
mpz_class get_e(mpz_class d, mpz_class n2){  //求xmodn的逆
    mpz_class x=d, n=n2 ;
    mpz_class s=0, t=0 ;
    mpz_class s0=1, s1=0, t0=0, t1=1 ;
    mpz_class r = n%x, q=n/x ;

    while(r){   //贝祖定理
        s = s0-s1*q ;
        t = t0-t1*q ;
        s0 = s1; s1 = s ;
        t0 = t1; t1 = t ;
        n = x ; x = r; r = n%x; q = n/x ;
    }
    return (t+n2)%n2 ;
}
*/

//#define Prime_P "81248286426143277303966132512035811539421391094025648828564689331605080140859761764333278197129300012501875226675607696256537957535751729603021876998622198518922712769978595071799144205568024691423702338561090512807745395499907747385953212582956284149483666975180542834473227547186323440438670420482550829947"
//#define Prime_Q "119199722656344691804117048692754755964396659487510145864844388768248609580948995696182467414790111954917498729666516350163225826651408314879504174656794404840740672187971734293048269766801484823610640906346267005320111760059691833618126068753639438033587844313384628188785164278313252029270809057781027716287"


int initial_key(){
    q =  "119199722656344691804117048692754755964396659487510145864844388768248609580948995696182467414790111954917498729666516350163225826651408314879504174656794404840740672187971734293048269766801484823610640906346267005320111760059691833618126068753639438033587844313384628188785164278313252029270809057781027716287" ;
  //  p = "40088276464845531965743272044458194990342194657057470025331138676836691575851140659011736836535976181590469188377564717654025381158572193753709755421408266937690787722529439466019692039906201501906010157724991087839789826468061163386692000476131145299094113562044184957827175941213256314411457604124324110339" ;
  //  p = "22636170916640637261194151607114356541762697020463314013025335690649370869486324712740877164029868144221021466245438680830401302153030992692205883271659279318288426857981813851988345906784233873853946551667858407660195289905292606655748437934576200785434525302130160535547813491194115322637071554558276741623" ;
    p = "81248286426143277303966132512035811539421391094025648828564689331605080140859761764333278197129300012501875226675607696256537957535751729603021876998622198518922712769978595071799144205568024691423702338561090512807745395499907747385953212582956284149483666975180542834473227547186323440438670420482550829947" ;

    key_d = "17341127" ;

    key_n = p*q;
    n2 = (p-1)*(q-1) ;

  //  cout<<"n2="<<n2<<endl;
  //  cout<<"gcd(n2,d)="<<gcd(n2, key_d)<<endl;

    mpz_invert(key_e.get_mpz_t(), key_d.get_mpz_t(), n2.get_mpz_t()) ;
   // key_e = get_e(key_d, n2) ;

    cout<<" 公钥："<<"\n n = "<<key_n<<"\n e = "<<key_e<<endl<<endl;
    cout<<" 私钥："<<"\n n = "<<key_n<<"\n d = "<<key_d<<endl<<endl;

    return 0 ;
}

unsigned long rotl(unsigned long x, int shift){
	return (x<<shift)|(x>>(32-shift)) ;
}



void cal_w(unsigned long block[64], unsigned long w[80]){  //64*8 = 512bit
	//每4个block(8位)组成一个w(32位)
	int cnt=0;
	for(int i=0;i<64;i+=4){
		bitset<32> t1(block[i]) , t2(block[i+1]), t3(block[i+2]), t4(block[i+3]) ;
		t1 = t1<<24;
		t2 = t2<<16;
		t3 = t3<<8 ;
		bitset<32> ans = t1^t2^t3^t4;

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



void h(const unsigned long x[128], unsigned long y[]){ // x 经过hash得到 y
    unsigned long h[5] ;
    unsigned long w[80] ;  // 一共有80个，每个32位，也就是4个字节
    unsigned long k[4] ;


	h[0] = 0x67452301 ;
	h[1] = 0xefcdab89 ;
	h[2] = 0x98badcfe ;
	h[3] = 0x10325476 ;
	h[4] = 0xc3d2e1f0 ;

	k[0] = 0x5a827999 ;
	k[1] = 0x6ed9eba1 ;
	k[2] = 0x8f1bbcdc ;
	k[3] = 0xca62c1d6 ;

    y[0] = y[1] = 0 ;  //在前面补0  (1024-960)/32=2
    int flag = 2 ;

    for(int u=0;u<6;u++){
        // cal(x, mac) ;  真的计算hash值
    	unsigned long block[64] ;  //64 *8 = 512bit
        int blockCnt = 1024/512 ;
        int cnt=0;

        for(int i=0;i<blockCnt;i++){
            for(int j=0;j<64;j++){  // 521/8=64
                block[j] = x[cnt++] ;
            }

            cal_w(block, w);

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

        //一次迭代时候，得到160bit的散列值
        // 160*6/32 = 30
        for(int v=0;v<5;v++){
            y[flag] = h[v] ;
         //   printf("%08x",y[flag]) ;
            flag++ ;
        }
    }


}
/*
void printmm(unsigned long m[32]){
    for(int i=0;i<32;i++){
        bitset<32> t(m[i]) ;
        cout<<t<<" ";

    }
}

void printkk(unsigned long k[128]){
    for(int i=0;i<128;i++){
        bitset<32> t(k[i]) ;
        cout<<t<<" ";
        if((i+1)%32==0)  cout<<endl;
    }
}
*/

void OAEP_Padding_Encrption(const char m[], unsigned long plaint_text[]){

    unsigned long left1[32] ;  //满满的32bit  1024/32=32

    //  满满的32bit
    int len = 6 ;
    for(int i=0;i+len<32;i++)  left1[i]=0 ;
    left1[26] = (m[0]<<8)^m[1] ;
    int flag=1 ;
    for(int i=27;i<32;i++){
        left1[i] = (m[++flag]<<24)^(m[++flag]<<16)^(m[++flag]<<8)^m[++flag] ;
    }

    //生成一个1024bit的随机数  1024/8=128  空空的32bit  1024/8=128
    unsigned long right1[128] ;
    unsigned long temp_right[32] ;  //之后用来和left2异或的满满的32
    unsigned seed = time(0) ;
    srand(seed) ;

    for(int j=0;j<128;j++){  //空空的
        bitset<9> t ;
        for(int k=0;k<8;k++){
            if(!j && !k) t[k] = 1;  //1024bit
            else t[k] = rand()%2 ;
        }
        right1[j] = t.to_ulong() ;
    }

    for(int j=0;j<128;j+=4){  //满满的
        temp_right[j/4] = (right1[j]<<24)^(right1[j+1]<<16)^(right1[j+2]<<8)^(right1[j+3]) ;
    }

    //hash函数
    unsigned long right2[32] ;  //hash出来之后是满满的32bit  1024/32=32
    h(right1, right2) ;

    for(int j=0;j<32;j++){
        left1[j] ^= right2[j] ;
    }

    unsigned long left2[32] ;   //这是hash之后的满满的32bit  1024/32=32
    unsigned long temp_left[128] ;   //hash之前是空空的32bit
    for(int i=0;i<128;i+=4){  //满满变空空
        unsigned long t = left1[i/4] ;
        temp_left[i] = t>>24 ;
        temp_left[i+1] = (t>>16)^(temp_left[i]<<8) ;
        temp_left[i+2] = (t>>8)^(temp_left[i+1]<<8)^(temp_left[i]<<16) ;
        temp_left[i+3] = t^(temp_left[i]<<24)^(temp_left[i+1]<<16)^(temp_left[i+2]<<8) ;
    }

    h(temp_left, left2) ;

    for(int j=0;j<32;j++){
        left2[j] ^= temp_right[j] ;
    }


    for(int j=0;j<32;j++){
        plaint_text[j] = left1[j] ;
        plaint_text[32+j] = left2[j] ;
    }

}




void OAEP_Padding_Decryption(unsigned long buff[64], char ans[]){
    // 分成两个1024bit  满满的
    unsigned long left1[32], right1[32] ;
    for(int i=0;i<32;i++){
        left1[i] = buff[i] ;
        right1[i] = buff[i+32] ;
    }
    unsigned long left2[32], right2[32] ;


    unsigned long temp_left[128] ;
    for(int i=0;i<128;i+=4){  //满满变空空
        unsigned long t = left1[i/4] ;
        temp_left[i] = t>>24 ;
        temp_left[i+1] = (t>>16)^(temp_left[i]<<8) ;
        temp_left[i+2] = (t>>8)^(temp_left[i+1]<<8)^(temp_left[i]<<16) ;
        temp_left[i+3] = t^(temp_left[i]<<24)^(temp_left[i+1]<<16)^(temp_left[i+2]<<8) ;
        bitset<32> ppp(t) ;
        bitset<8> a1(temp_left[i]), a2(temp_left[i+1]), a3(temp_left[i+2]), a4(temp_left[i+3]) ;
    }

    h(temp_left, left2) ;

    //满满left2 异或 满满right1
    for(int i=0;i<32;i++){
        right1[i] ^= left2[i] ;
    }

    // 满满right1要先变成空空right1，再hash
    unsigned long temp_right[128] ;
    for(int i=0;i<128;i+=4){  //满满变空空
        unsigned long t = right1[i/4] ;
        temp_right[i] = t>>24 ;
        temp_right[i+1] = (t>>16)^(temp_right[i]<<8) ;
        temp_right[i+2] = (t>>8)^(temp_right[i+1]<<8)^(temp_right[i]<<16) ;
        temp_right[i+3] = t^(temp_right[i]<<24)^(temp_right[i+1]<<16)^(temp_right[i+2]<<8) ;
    }

    h(temp_right, right2) ;

    // 满满left1 异或 满满right2
    for(int i=0;i<32;i++){
        left1[i] ^= right2[i] ;
    }


    char b[128] ;
    for(int i=0;i<128;i+=4){  //满满变空空
        unsigned long t = left1[i/4] ;
        b[i] = t>>24 ;
        b[i+1] = (t>>16)^(b[i]<<8) ;
        b[i+2] = (t>>8)^(b[i+1]<<8)^(b[i]<<16) ;
        b[i+3] = t^(b[i]<<24)^(b[i+1]<<16)^(b[i+2]<<8) ;
    }

    int flag=0, cnt=0;
    for(int i=0;i<128;i++){
        if(b[i]!=0 && !flag)  flag=1;
        if(flag)  ans[cnt++] = (char)b[i] ; // cout<<ans[cnt-1];
    }

}
/*
mpz_class exp_mod(mpz_class base, mpz_class exp){

    mpz_class n = key_n ;
    mpz_class e = exp, b = base ;
    mpz_class temp = 1, ans = 0 ;

    while(e>=1){
        if(e==1){
            ans = (temp*b)%n ;
            return ans ;
        }
        else if(e%2==0){  //指数为偶数，底数平方取模，指数/2
            e /= 2 ;
            b = (b*b)%n ;
        }
        else if(e%2==1){  //指数为奇数，先提取一个底数相乘并取模，指数-1
            temp = (b*temp)%n ;
            e--;
        }
    }

}
*/

int main(){

    if(initial_key())  return 0;

    char m[] = "Sun Yat-sen University" ;  //22个字节
    cout<<" 明文: ";  puts(m) ;

    unsigned long padding[256] ;  //  2048/8=256
    OAEP_Padding_Encrption(m, padding) ;

    int flag = -1 ;
    char num[700]  = "" ;
  //  cout<<"in 移位"<<endl;
    // 用sha-1算出来的散列值是算得的答案串联起来的，所以把每一串数字僵硬的转成大的数字就可以，对了，变成你以为的样子
    for(int i=0;i<64;i++){  //数组转成大数  前两个是0，不管了
        char str[11] ;
        if(flag==-1 && padding[i])  flag=i ;
        if(flag>=0){
            if(i==flag)  _snprintf(str, sizeof(str), "%lu", padding[i]) ;
            else _snprintf(str, sizeof(str), "%010lu", padding[i]) ;
            strcat(num, str) ;
        }

    }

    mpz_class plaint_text(num) ;
    cout<<"\n 填充之后：\n"<<plaint_text<<endl;
  //  mpz_class cipher_text = exp_mod(plaint_text, key_e) ;  //2048bit   对的
    mpz_class cipher_text ;
    mpz_powm(cipher_text.get_mpz_t(), plaint_text.get_mpz_t(), key_e.get_mpz_t(), key_n.get_mpz_t()) ;

    cout<<"\n 加密后的密文(十进制）：\n"<<cipher_text<<endl;
  //  mpz_class meg = exp_mod(cipher_text, key_d) ;  //2048bit  对的
    mpz_class meg ;
    mpz_powm(meg.get_mpz_t(), cipher_text.get_mpz_t(), key_d.get_mpz_t(), key_n.get_mpz_t()) ;
    meg=plaint_text ;
    cout<<"\n 解密之后，还没有去填充得到的："<<endl;
    cout<<meg<<endl;


    // 把大数mpz变成十进制数组
    unsigned long buff[64] ;
    char str[2048] ;
    mpz_get_str(str, 10, meg.get_mpz_t()) ;
   // cout<<"十进制："<<endl;
  //  puts(str) ;

    flag=strlen(str)-1 ;
    int first = (flag+1)%10 ;
    if(!first)  first=10 ;
    int i=63;
    for(i;i>=0;i--){
        char s[11] = "0000000000" ;
        if(flag<=9){  //第一块
            for(int j=0;j<first;j++){
                s[j] = str[j] ;
            }
            flag=-1;
            s[first]='\0' ;
            if(i>0){
                for(int k=0;k<i;k++)  buff[k]=0 ;
            }


        }
        else{  //其他块
            for(int j=9;j>=0;j--){
                s[j] = str[flag--];
            }
            s[10] = '\0' ;
        }

        sscanf(s,"%ul",&buff[i]) ;
        if(flag<0)  break ;
    }

    char b[32] ="" ;
    OAEP_Padding_Decryption(buff, b) ;  //去掉填充
    cout<<"\n 对以上密文解密：\n";  puts(b) ;

    if(strcmp(b, m)==0){
        cout<<"\n\n加解密一致，实验成功！\n"<<endl;
    }

    else   cout<<"实验失败！"<<endl ;

	system("pause") ;

}













