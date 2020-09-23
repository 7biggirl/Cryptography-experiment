#include<iostream>
#include<cstring>
#include<cmath>
#include<gmp.h>
#include<gmpxx.h>
#include<time.h>
#include<sstream>
#include"SHA256.h"

using namespace std;

typedef unsigned int UInt32;
typedef unsigned __int64 UInt64;
typedef unsigned char UChar;
#define Max 1000//最大字符数
SHA256 sha256=SHA256();
Message_Digest M_D;
UInt32 W[Max/4];//整型
UInt32 M[16];   //存分组信息

//压缩+显示
void compress(){
    int i;
    M_D = sha256.DEAL(M);

    cout<<" HASH(HEX): ";
    for(i=0;i<8;i++){
        cout<<hex<<M_D.H[i];
    }
    cout<<endl;

}
//添加填充位+添加长度
void PAD(UChar Y[Max]){
    //x+1+d+l=|x|
    UInt32 i,j;
    UInt32 T1=0,T2=0,T3=0,T4=0;
    UChar temp[Max]={0};
    UInt64 x = strlen((char *)Y);//数据长度
    UInt32 d = abs(55-x) ;   //填充长度
    d%=64 ;
    UInt32 n = (x+8)/64+1; //分组数
    UInt32 m = x%64;       //最后组数据长度
    UInt32 l = 8;
	/*
    cout<<"数据长度x:"<<int(x)<<" ";
    cout<<"填充长度d:"<<d<<" ";
    cout<<"分组数量n:"<<n<<" ";
    cout<<"最后长度m:"<<m<<endl;
    */
    //不填充
    for(i=0;i<x;i++){
        temp[i] = Y[i];
    }
    //填充1次1000 0000
        temp[x] = 0x80;
    //填充d次0000 0000
    for(i=x+1;i<x+d+1;i++){
        temp[i] = 0x00;
    }
    //填充长度的63-0位
    for(i=1;i<=l;i++){
        temp[(n*64)-i] = (UChar)(8*x>>(i-1)*8);
    }
    //无符号字符转换为无符号整型
    for(i=0;i<Max/4;i++){
        for(j=0;j<4;j++){
            if(j==0)
                T1 = temp[4*i+j];
            if(j==1)
                T2 = temp[4*i+j];
            if(j==2)
                T3 = temp[4*i+j];
            if(j==3)
                T4 = temp[4*i+j];
        }
        W[i] = (T1<<24) + (T2<<16) + (T3<<8) +T4;
    }
    //显示16进制数据
    /*
    cout<<"16进制数据：";
    for(i=0;i<n*16;i++){
        cout<<hex<<" "<<W[i];
    }

    cout<<endl;
    */

    //分组处理
    for(i=0;i<n;i++){
     //   cout<<"分组处理:"<<i+1<<endl;
        for(j=0;j<16;j++){
            M[j] = W[(i*16)+j];
        }
        compress();//sha-256压缩
    }
}


mpz_class _hash(UChar Y[Max]){
    char str[81] ="";
    PAD(Y);
    //  codeblocks太奇怪了，只能这样子拼接
    char first ;
    int flag = 0 ;
    for(int i=0;i<8;i++){
        char buff[10] ="" ;
        sprintf(buff, "%010lu", M_D.H[i]) ;
        if(i>0)  str[0]=first ;
        for(int j=0;j<10;j++){
            str[flag+j] = buff[j] ;
        }
        flag+=10 ;
        if(i==0)  first = str[0] ;
    }
    flag=80 ;
    while(str[0]=='0'){
        for(int i=0;i<80;i++)   str[i] = str[i+1] ;
        str[flag--]='\0' ;
    }
    mpz_class ans ;
    ans = str ;
    return  ans;
}



mpz_class key_p, key_q, key_alpha, key_a, key_beta ;
mpz_class sig_k, sig_gamma, sig_delta ;
mpz_class _sha256 ;
UChar x[100] = "SchoolofDataandComputerScience,Sunyat-senUniversity" ;


void signature(mpz_class x){
    cout<<"\n----------------------------- signature(message,k) ------------------------------------------------"<<endl;
    start:
    // 选1~q-1的随机数
    mpz_class k;
    gmp_randclass rr(gmp_randinit_default) ;
    rr.seed(time(NULL)) ;
    k = rr.get_z_bits(160) ;
    k += key_q ;
    k %= key_q ;

    cout<<"\n k is "<<k<<endl;

    mpz_powm(sig_gamma.get_mpz_t(), key_alpha.get_mpz_t(), k.get_mpz_t(), key_p.get_mpz_t()) ;
    mpz_mod(sig_gamma.get_mpz_t(), sig_gamma.get_mpz_t(), key_q.get_mpz_t()) ;
    cout<<"\n gamma is "<<sig_gamma<<endl;

    mpz_class invert_k ;
    mpz_invert(invert_k.get_mpz_t(), k.get_mpz_t(), key_q.get_mpz_t()) ;
    sig_delta = ( _sha256+key_a*sig_gamma)*invert_k ;
    sig_delta %= key_q ;
    cout<<"\n delta is "<<sig_delta<<endl;

    if(sig_gamma==0 || sig_delta==0)  goto start ;
    cout<<"\n Neither gamma nor delta is equal to 0, continue"<<endl;

}

bool vertify(mpz_class sig_gamma, mpz_class sig_delta){
    cout<<"\n--------------------------- verify(message, gamma, delta) -----------------------------------------"<<endl;
    mpz_class e1, e2 ;

    mpz_class invert_delta ;
    mpz_invert(invert_delta.get_mpz_t(), sig_delta.get_mpz_t(), key_q.get_mpz_t()) ;

    e1 =  _sha256*invert_delta ;
    e1 %= key_q ;
    cout<<"\n e1 is "<<e1<<endl;

    e2 = sig_gamma*invert_delta ;
    e2 %= key_q ;
    cout<<"\n e2 is "<<e2<<endl;

    mpz_class test, t1, t2 ;
    mpz_powm(t1.get_mpz_t(), key_alpha.get_mpz_t(), e1.get_mpz_t(), key_p.get_mpz_t()) ;
    mpz_powm(t2.get_mpz_t(), key_beta.get_mpz_t(), e2.get_mpz_t(), key_p.get_mpz_t()) ;
    test = t1*t2 ;
    test %= key_p ;
    test %= key_q ;

    return test==sig_gamma?true:false ;
}


int main(){
    mpz_init_set_str(key_p.get_mpz_t(),
                    "008746338ba0d0b67ec6dde878f14d624dfe86a4663aaf170208b02b020c09199af1dbaaf0ba5a70d52b3eb715f775b0989ff176d1d6c680042dab48d35d802c598036280c559ffb59ea4c82c01fae4a227847cb715e03511602f7dedd0d0f1556c5e63c9b181a4a7ffb1416a3d6ef69cb9b413746aab5e259c3b12bec3244a7e09f6718ecb721c528020a9ae09568f461512c526593563fad544106fb5fa90922aef456a0f110ccab7628d793ca3c5f6a4c6e22ee05227e1d3c395f99f52418c5dfdee1442d0aa3ca1e9154cae569c25497c3445f2bed1e8de3cada7e2f1825d7341e591234df4053ca5b33c92e3217967ca00211b68c1b0aafa5152b84557a81",
                     16 ) ;
    mpz_init_set_str(key_q.get_mpz_t(),
                     "00bdd727428445e6e4ad64978a4184de1f28ab3f298e79f11147bee060150040e1",
                     16 ) ;
    mpz_init_set_str(key_alpha.get_mpz_t(),
                     "75fd519cb49bcfd504dc7f8bfb100511dee71335ba07d7bf85e0b602441b121fc1b537f29b941254f85703d2aa41e2cf400e3be7d952ad9a32f461174873034631295c2bc86d5134779358e8f4c6d54e591edc645dbd389088f9a7dfa3927fbeedce1bc57fa1e6dd3908daac26914ead4f40e8acab8b57a303d1970a06393789c56decbb504f7ae3d9f8e2cd1850b9e77d4aa64f49218fd0f6c24ccb5aa6af9d97e4d041b0676dda90dfbba0177df89b730ab1b8db5ef19ebd2fed52a790826fc507647576bebed69e5a8a11ec25f89d445b56b738d2a7f713d74527a4e6ed99af43ebcdb2cfe2ccaee76fd543ea429ec6402c273b030a63e6a686b2d299d8d7",
                     16 ) ;

    key_a = "1352189132780742348147690325435974676612066923875027455889316666163952829642123" ;
    mpz_powm(key_beta.get_mpz_t(), key_alpha.get_mpz_t(), key_a.get_mpz_t(), key_p.get_mpz_t()) ;


    cout<<"\n******************************************* public key ********************************************\n"
        <<"\n p is "<<key_p<<endl
        <<"\n q is "<<key_q<<endl
        <<"\n alpha is "<<key_alpha<<endl
        <<"\n beta is "<<key_beta<<endl;
    cout<<"\n******************************************* private key *******************************************\n"
        <<"\n a is "<<key_a<<endl;

  //  cout<<"\n Enter message: ";
  //  scanf("%s",x) ;

    _sha256 = _hash(x) ;
    gmp_printf(" _sha256(message) is (Dec)%Zd\n", _sha256.get_mpz_t()) ;

    signature(_sha256) ;
    if(vertify(sig_gamma, sig_delta)) {
       cout<<"\n Test is passed! Congratulations!"<<endl;
    }
    else  cout<<"\n Validation failed. Invalid signature!"<<endl;
   // system("pause") ;
}





