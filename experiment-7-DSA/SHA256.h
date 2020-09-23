#ifndef _SHA_256_H
#define _SHA_256_H
#include<iostream>
using namespace std;
typedef unsigned int UInt32;
//�����߼�����
#define Conditional(x,y,z) ((x&y)^((~x)&z))
#define Majority(x,y,z) ((x&y)^(x&z)^(y&z))
#define LSigma_0(x) (ROTL(x,30)^ROTL(x,19)^ROTL(x,10))
#define LSigma_1(x) (ROTL(x,26)^ROTL(x,21)^ROTL(x,7))
#define SSigma_0(x) (ROTL(x,25)^ROTL(x,14)^SHR(x,3))
#define SSigma_1(x) (ROTL(x,15)^ROTL(x,13)^SHR(x,10))
//��ϢժҪ�ṹ
struct Message_Digest{
    UInt32 H[8];
};
//SHA256��
class SHA256
{
public:
    SHA256(){INIT();};
    ~SHA256(){};
    Message_Digest DEAL(UInt32 W[16]);//����512�������ݣ�������ϢժҪ
private:
    void INIT();                //��ʼ�Ӵ�ֵ
    UInt32 ROTR(UInt32 W,int n);//����ת
    UInt32 ROTL(UInt32 W,int n);//����ת
    UInt32 SHR(UInt32 W,int n); //����λ
private:
    //��ϢժҪ
    Message_Digest MD;
};

#endif
