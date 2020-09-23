#include<iostream>
#include<cmath>
#include<algorithm>
//#include<string>
//#include<stdio.h> 
using namespace std;

//char code[500];
string code;
int length;



int getKeyLen(){
	int keyLen = 1;
	double IC[100] ;
	
	while(1){
		for(int k=0;k<keyLen;k++)  IC[k]=0 ;
		for(int i=0;i<keyLen;i++){  //在第i串
			int cnt[26];
			memset(cnt, 0, 26);
			for(int w=0;w<26;w++)  cnt[w]=0;
			int size=0;
					
			for(int j=0; i+j*keyLen<length; j++)
				cnt[(int)(code[i+j*keyLen]-'A')]++;
				
			for(int k=0;k<26;k++)  size+=cnt[k];

			size*=(size-1);
			double temp=0.00f;
			for(int u=0;u<26;u++){
				if(cnt[u]!=0)
					temp += ((double)cnt[u]*(double)(cnt[u]-1))/(double)size;
			}
			IC[i]=temp;
		}
	
		double avgIC = 0;	
		for(int w=0;w<keyLen;w++)  avgIC+=IC[w];
		avgIC /= keyLen ;
		if(avgIC >= 0.06 && avgIC <= 0.07){
			return keyLen;
		}
		keyLen++;
	}
}

string getKeyword(int keyLen){
	string keyword="", word;
	double MG[26]={0};  //移动 
	memset(MG, 0, 26);
	int cnt[26]={0};
	memset(cnt, 0, 26) ;
	for(int i=0;i<26;i++)  cnt[i]=MG[i]=0 ;

	double p[26]={0.082, 0.015, 0.028, 0.043, 0.127, 0.022, 0.020, 0.061, 0.070, 0.002, 0.008, 0.040, 0.024, 0.067, 0.075, 0.019, 0.001, 0.060, 0.063, 0.091, 0.028, 0.010, 0.023, 0.001, 0.020, 0.001 } ;
	int size=0;
		
		//先从第w串开始，依此算移动k个看
		for(int w=0;w<keyLen;w++){  //w串 
			for(int i=0;i<26;i++)  cnt[i]=MG[i]=0;
			size=0;
			for(int x=0;w+x*keyLen<length;x++){  //统计目前这一串的各个字母的个数 
				cnt[code[w+x*keyLen]-'A']++, size++;
			}
		
			double diff = 1000.0 ;
			int mark = 0 ;
			for(int k=0;k<26;k++){    //移动k个试试看 
				double temp=0.0 ;

				for(int a=0;a<26;a++){
					temp+=(double)(p[a]*cnt[(k+a)%26]) / (double)size ;
				} 
				MG[k] = temp ;
				if(fabs(MG[k]-0.065) <= diff )    diff=fabs(MG[k]-0.065), mark=k ;
			}
			
			word[w]=(char)(mark+'A');
			keyword += word[w];
		}
		
		return keyword;
}

string getMessenge(string keyword, int keyLen){
	char  m[1000];
	string meg="";
	int k=0;

	for(int i=0;i<length;i++){
		m[i]=(code[i]-keyword[k]+26)%26 ;
		m[i]=(char)(m[i]+'a') ;
		k++;
		k%=keyLen ;
		meg += m[i];
	}
	return meg ;
}


int main(){
	cout<<"密文："; 
	cin>>code;
	length = code.length();
	
	const int keyLen = getKeyLen();
	string keyword = getKeyword(keyLen);
	string word = getMessenge(keyword, keyLen);

	cout<<"\n密钥长度为："<<keyLen<<endl;
	cout<<"\n密钥："<<keyword<<endl;
	cout<<"\n明文："<<word<<endl;		

}

