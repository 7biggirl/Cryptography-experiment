#include<iostream>
using namespace std;
#define m 3
// 注释部分可用于校验 

int mod26(int x){
	while(x>=26 || x<0){
		if(x>=26)  x-=26, x%=26;
		else x+=26, x%=26;
  	}
  	return x;
}

void print(int a[m][m]){
	for(int i=0;i<m;i++){
		for(int j=0;j<m;j++)  cout<<a[i][j]<<"  ";
		cout<<endl;
	}
	cout<<endl;
}


int gcd(int x,int y){
	if(y==0)  return x;
	return gcd(y, x%y);
}

int getDetInver(const int det){
	for(int i=0;i<26;i++){
		int temp = i*det ;
		if(temp%26==1)  return  i;
	}
	return -1;
}

int getDet(int a[m][m],const int size){
	if(size==1)  return a[0][0];

	int det=0;
	int temp[m][m] = {0} ;
	
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			for(int k=0;k<size;k++){
				if(k>=i)  temp[j][k] = a[j+1][k+1];
				else temp[j][k] = a[j+1][k];
			}
		}
		int t = getDet(temp, size-1);
		if(i%2==0)  det+=a[0][i]*t;
		else det-=a[0][i]*t;
	}
	return det;
}

void getAd(int a[m][m],int ad[m][m],int size){

	if(size==1){
		ad[0][0]=1;  return ;
	}
	int temp[m][m] = {0};
	
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			for(int k=0;k<size-1;k++){
				for(int u=0;u<size-1;u++){
					temp[k][u] = a[k>=i? k+1:k][u>=j? u+1:u];
				}
			}
			if((i+j)%2==0) ad[j][i] = getDet(temp,size-1);
			else ad[j][i] = -getDet(temp, size-1);  //转置 
			ad[j][i] = mod26(ad[j][i]);
		}
	}

}


void getInverse(int a[m][m], int inver[m][m]){
//	cout<<"before"<<endl;
//	cout<<getDet(a,m)<<endl;
	
	int det=mod26(getDet(a,m));
//	cout<<"行列式: "<<det<<endl;
	if(gcd(det,26)!=1){
		cout<<"行列式与26不互素,不存在逆矩阵!"<<endl;
		return ;
	}

	int DetInver = getDetInver(det);
//	cout<<"行列式的逆: "<<DetInver<<endl;
	int ad[m][m]={0} ;

	getAd(a,ad,m);
//	cout<<"伴随矩阵:"<<endl;
//	print(ad);

	for(int i=0;i<m;i++){
		for(int j=0;j<m;j++){
			inver[i][j] = ad[i][j]*DetInver ;
			inver[i][j] = mod26(inver[i][j]);
		}
	}

//	cout<<"逆矩阵"<<endl;
//	print(inver);
}

//方阵乘法
void multiple(int inver[m][m], int yd[m][m], int key[m][m]){
	for(int i=0;i<m;i++){
		for(int j=0;j<m;j++){
			key[i][j] = 0 ;
			for(int k=0;k<m;k++){
				key[i][j] += inver[i][k]*yd[k][j] ;
			}
			key[i][j] = mod26(key[i][j]) ;
		}
	}

	
	//print(key);
}


void getB(int y[m][m], int x[m][m], int key[m][m], int b[m][m]){
	int product[m][m] ={0} ;
	multiple(x, key, product) ;
//	cout<<"product"<<endl;
//	print(product);
	
	for(int i=0;i<m;i++){
		for(int j=0;j<m;j++){
			b[i][j] = y[i][j] - product[i][j] ;
			b[i][j] = mod26(b[i][j]);
		}
	}

	cout<<"B:"<<endl;
	print(b);
}

bool check(int x[1000][m], int key[m][m], int b[m][m], int y[1000][m],int col){
	int temp[1000][m]={0};
	//加密 
	for(int i=0;i<col;i++){
		for(int j=0;j<m;j++){
			temp[i][j] = 0;
			for(int k=0;k<m;k++){
				temp[i][j] += x[i][k]*key[k][j];
			}
			temp[i][j] += b[0][j] ;
			temp[i][j] = mod26(temp[i][j]) ;
		}
	}
	cout<<"经过以上密钥key,b加密后,密文:"<<endl;
	for(int i=0;i<col;i++){
		for(int j=0;j<m;j++)
			cout<<temp[i][j]<<"  ";
		cout<<endl;
	}
	
	for(int i=0;i<col;i++){
		for(int j=0;j<m;j++){
			if(temp[i][j]!=y[i][j])  return false;
		}
	}
	
	for(int i=0;i<col;i++){
		for(int j=0;j<m;j++)  cout<<(char)(temp[i][j]+'A');
	}
	cout<<endl;
	return true;
}





int main(){

	int x[1000][m], y[1000][m], inver[m][m], key[m][m]={0}, b[m][m] = {0} ;
	int i=0,j=0;
	string word;

	cout<<" 请输入明文:";   cin>>word;
	for(int k=0;k<word.length();k++){
		x[i][j] = (int)(word[k]-'a');
		j++;
		if(j==m)  j=0,i++;
	}

	i=j=0;
	cout<<" 请输入密文:";   cin>>word;
	for(int k=0;k<word.length();k++){
		y[i][j] = (int)(word[k]-'A');
		j++;
		if(j==m)  j=0,i++;
	}
	int size=i;	


	cout<<"x:"<<endl;
	for(int k=0;k<size;k++){
		for(int u=0;u<m;u++){
			cout<<x[k][u]<<"  ";
		}
		cout<<endl;
	}

	cout<<"\ny:"<<endl;
	for(int k=0;k<size;k++){
		for(int u=0;u<m;u++){
			cout<<y[k][u]<<"  ";
		}
		cout<<endl;
	}


	int xdiff[m][m]={0}, ydiff[m][m]={0} ;
	for(int k=0;k<m;k++){
		for(int u=0;u<m;u++){
			xdiff[k][u] = x[k+m][u]-x[k][u] ;
			ydiff[k][u] = y[k+m][u]-y[k][u] ;
		}
	}

//	cout<<"xdiff:"<<endl;
//	print(xdiff);
//	cout<<"ydiff"<<endl;
//	print(ydiff);

	getInverse(xdiff, inver);
	
	multiple(inver, ydiff, key) ;
	cout<<"\nkey:"<<endl;
	print(key);
	 
	getB(y, x, key, b) ;

	if(check(x,key,b,y,size))
		cout<<"\n与原密文一致, 检验通过!"<<endl;
	else cout<<"经检验,密钥不匹配!"<<endl;



}

