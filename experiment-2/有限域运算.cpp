#include<iostream>
#include<bitset>
//#include<cmath>
#include<stdlib.h>
#include<stdio.h>
#define MAX 127 

using namespace std;
bitset<MAX+1> px; //长度为128，前面用0补充
bitset<MAX> rx;


//  构造有限域GF(2^127)：不可约多项式为f(x)=x^127+x+1
//给出GF(2^127)上的加法、乘法、平方以及求逆的子程序；
//将自己的学号转为二进制字符串，看作有限域中的元素a，计算元素的逆元素以及a^20190911


int degree(const bitset<2*MAX> &a){
	for(int i=2*MAX-1;i>=0;i--){
		if(a[i]==1)  return i;
	}
	return 0;
}



bitset<MAX> mod(bitset<2*MAX> &a){
	
	bitset<2*MAX> _r = bitset<2*MAX>(rx.to_string()) ,
			      ans = bitset<2*MAX>(a.to_string().substr(MAX)) ;

		
	while(degree(a)>=MAX-1){
		ans.reset();
		ans = bitset<2*MAX>(a.to_string().substr(MAX+1)) ;
		for(int i=MAX;i<2*MAX;i++){
			if(a[i])   ans ^= _r<<(i-MAX) ;
		}
		a=ans;
	}

	return bitset<MAX>(ans.to_string().substr(MAX)) ;
	
} 




void add(){  
	string a,b;
	cout<<"\n 请输入两个加数："<<endl;
	cout<<"\n a: ";  cin>>a;
	cout<<"\n b: ";  cin>>b;
	
	bitset<MAX> x(a), y(b);
	bitset<MAX> ans = x^y ;
	printf("\n answre（十六进制）: 0x%x\n\n\n",ans);
//	cout<<"ans="<<ans<<endl;
}




bitset<MAX> multiple(bitset<MAX> a, bitset<MAX> b){  //完成！
	bitset<2*MAX> ans;

	bitset<2*MAX> temp(a.to_string());   //注意每次temp都是等于a 
	
	for(int i=0;i<MAX;i++){
		if(b[i])  ans^=temp<<i;
	}
	return mod(ans);
}



bitset<MAX> square(bitset<MAX> a){   //完成！
	bitset<2*MAX> ans ;	
	
	for(int i=0;i<MAX;i++){
		if(a[i]){
			ans[2*i] = 1 ; 
		}
	}

	return mod(ans);
}


bitset<MAX> inverse(bitset<MAX> _a){
	//贝祖定理   拓展欧几里得
	bitset<2*MAX> ans, s, a;
	bitset<2*MAX> p=bitset<2*MAX>(px.to_string()) ;
	ans[0] = 1; 
	
	a = bitset<2*MAX>(_a.to_string());
	
	while(degree(a)){
		int diff = degree(a)-degree(p);
		if(diff<0){
			bitset<2*MAX> temp=a;
			a=p;
			p=temp;
			
			temp=s;
			s=ans;
			ans=temp; 
			
			diff = -diff ;
		}
		a ^= p<<diff ;
		ans ^= s<<diff ;
	}
	
	bitset<MAX> t=bitset<MAX>(ans.to_string().substr(MAX)) ;
//	cout<<"与原来的乘积："<<multiple(t,_a)<<endl;
	return  t;

} 

void getB(int value, int array[],int &len){
	int i=0;
	while(value!=0){
		array[i++] = value % 2 ;
		value /= 2;
		len++ ;
	}
}


bitset<MAX> getExp(bitset<MAX> a){
	int exp = 20190911 , len=0;
	int array[MAX] ={0} ;
	memset(array, 0, MAX ) ;
	getB(exp,array,len) ;

	bitset<MAX> ans(1);
	
	int i=len-1;
	
	while(i>=0){
		ans = square(ans) ;
		if(array[i--])  
			ans = multiple(ans, a) ;
	}

	return ans ;
}


void studentId(){
	bitset<2*MAX> id;
	cout<<" 学号（十六进制）：";
	scanf("%x",&id);
	cout<<"\n your id is "<<id<<endl;
	
	bitset<MAX> a = mod(id) ;
	cout<<"\n 化成有限域中的元素a："<<a<<endl;
	printf(" (十六进制：0x%x)\n\n",a) ;
	
	bitset<MAX> t = inverse(a) ;
	cout<<"\n 学号的逆："<<t<<endl;
	printf(" (十六进制：0x%x)\n\n",t) ;
	cout<<"与原来的乘积："<<multiple(inverse(a),a)<<endl;
	
	cout<<"\n a^20190911："<<getExp(a)<<endl ; 
	printf(" (十六进制：0x%x)\n\n\n",getExp(a)) ;
}
 
 

int main(){
	px[127] =px[0]=px[1]= 1;
	rx[0] = rx[1] = 1;
//	cout<<"px="<<px<<"\nrx="<<rx<<endl<<endl; 
	for(int i=MAX;i>=0;i--)  if(px.test(i))  cout<<"x^"<<i<<"+";
	 while(1){
	 	system("cls");
	 	cout<<"\n\n 请选择你要进行的计算："<<endl;
	 	cout<<"\n 1-加法    2-乘法    3-平方    4-求逆    5-学号计算    6-退出"<<endl;
	 	cout<<"\n choice: "; 
	 	int choice;
	 	cin>>choice;
	 	switch(choice){
	 		case 1:{
	 			system("cls");
	 			cout<<"\n\n 加法"<<endl;
	 			add();
	 			system("pause") ;
	 			break;
	 		}
	 		case 2:{
	 			system("cls");
	 			cout<<"\n\n 乘法"<<endl;
	 			bitset<MAX> a,b;
	 			cout<<"\n 请输入两个乘数："<<endl;
				cout<<"\n a: ";  cin>>a;
				cout<<"\n b: ";  cin>>b;
				printf("\n 乘积（十六进制）：0x%x\n\n\n",multiple(a,b)) ;
			//	cout<<"\n乘积:"<<multiple(a,b)<<endl<<endl;
				
				system("pause") ;
	 			break;
	 		}
			case 3:{
				 system("cls");
	 			 cout<<"\n\n 平方"<<endl;
	 			 bitset<MAX> a;
			     cout<<"\n 请输入底数：";  cin>>a;
			     printf("\n 平方（十六进制）：0x%x\n\n",square(a)) ;
			//	 cout<<"\n平方："<<square(a)<<endl;
				 system("pause") ;
			     break;
			}
			case 4:{
				system("cls");
	 			cout<<"\n\n 求逆"<<endl;
	 			bitset<MAX> a ;
	 			cout<<"\n 输入a：";   cin>>a;
	 			bitset<MAX> t = inverse(a) ;
	 			printf("\n 逆（十六进制）：0x%x",t) ;
			//	cout<<"\n逆："<<inverse(a)<<endl;
				printf("\n\n 与原来的乘积（十六进制）：0x%x\n\n",multiple(t,a)) ;
				system("pause") ;
				break;
			}
			case 5:{
				system("cls");
	 			cout<<"\n\n 学号计算\n\n"<<endl;
				studentId();
				system("pause") ;
				break;
			}
			default:
				system("cls");
				cout<<"\n\n 退出"<<endl; 
				return 0;
	 	}
	 }
}

