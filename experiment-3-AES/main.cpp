#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include"myyAES.h"

using namespace std;


int main(){

				cout<<"\n\n\n AES密码体制\n\n 密钥：mengyayuan\n\n 偏移量：123"
				<<"\n\n 密文编码：十六进制 \n\n 补码方式：PKCS5Padding\n\n\n"
				<<"--------------------------------\n\n"
				<<" 请输入您要加密的密文：";
				string sMEG;  cin>>sMEG ;
				int len=sMEG.length() ;
				
				const char *cMEG ={0} ;
				cMEG = sMEG.data() ;
				
				int iMEG[len+1];
				for(int i=0;i<len;i++)  iMEG[i]=cMEG[i];
			
				myyAES data(iMEG,len) ;
				int text[1024]={-1};
				len = data.encrypt(text);
				cout<<"\n 密文：";
				for(int i=0;i<len;i++)  printf("%02x",text[i]);
				
				cout<<"\n\n\n 解密：";
				data.decryption(text, len);
				for(int i=0;text[i]!='\0';i++)  printf("%c",text[i]);
				cout<<"\n\n\n"<<endl;
				
				system("pause");
				return 0; 

}



// http://www.seacha.com/tools/aes.html


// \
ilearnedhowtocalculatetheamountofpaperneededforaroomwheniwasatschoolyoumultiplythesquarefootageofthewallsbythecubiccontentsofthefloorandceilingcombinedanddoubleityouthenallowhalfthetotalforopeningssuchaswindowsanddoorsthenyouallowtheotherhalfformatchingthepatternthenyoudoublethewholethingagaintogiveamarginoferrorandthenyouorderthepaper

// \
b7f960f03a03be42874a7536c7f7656936c58d9abef969d2cb1d3ab9513ec8e7515bc67e337ced4390de0a1c1b0db8cb568b3644464795db20c6c8e93a7f3b07201bd4a434385b4b1395e0eb7be6fe0531539d969c303d10f8fe40124174121a613ee3be7748e5d1e1e24d1b3da723a22bd9fd880fbb465bdb05f4abacf9db5620efb5cd9225cf2fd058da45087a9dcd389fff356d3be3f6a693a1f92bb58852237a75d8e0c7144207b3f82cbb2f32d9303d6042b964ef1498da08143d25f98bf1e58c39364d1d0955c13eb7c1472fb4e1fba3171e80c81c7c094047659bd56458dba873e48e079c9e56acc2c5913efd08a0110fc30a18997eae921deef3ddf5ca54039a796912a49449f2289eaf500c716482c1c3854c04453efbb46c07a56da075eb2779a3fccec662635ad5171a5fdc67b8be9f1dd79197f4ae8316e3d723e8c635a7499b7edc8b3be7d54c0e8807f68bf63e279ebf07b3194726175aa34f 


