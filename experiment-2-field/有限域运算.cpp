#include<iostream>
#include<bitset>
//#include<cmath>
#include<stdlib.h>
#include<stdio.h>
#define MAX 127 

using namespace std;
bitset<MAX+1> px; //����Ϊ128��ǰ����0����
bitset<MAX> rx;


//  ����������GF(2^127)������Լ����ʽΪf(x)=x^127+x+1
//����GF(2^127)�ϵļӷ����˷���ƽ���Լ�������ӳ���
//���Լ���ѧ��תΪ�������ַ����������������е�Ԫ��a������Ԫ�ص���Ԫ���Լ�a^20190911


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
	cout<<"\n ����������������"<<endl;
	cout<<"\n a: ";  cin>>a;
	cout<<"\n b: ";  cin>>b;
	
	bitset<MAX> x(a), y(b);
	bitset<MAX> ans = x^y ;
	printf("\n answre��ʮ�����ƣ�: 0x%x\n\n\n",ans);
//	cout<<"ans="<<ans<<endl;
}




bitset<MAX> multiple(bitset<MAX> a, bitset<MAX> b){  //��ɣ�
	bitset<2*MAX> ans;

	bitset<2*MAX> temp(a.to_string());   //ע��ÿ��temp���ǵ���a 
	
	for(int i=0;i<MAX;i++){
		if(b[i])  ans^=temp<<i;
	}
	return mod(ans);
}



bitset<MAX> square(bitset<MAX> a){   //��ɣ�
	bitset<2*MAX> ans ;	
	
	for(int i=0;i<MAX;i++){
		if(a[i]){
			ans[2*i] = 1 ; 
		}
	}

	return mod(ans);
}


bitset<MAX> inverse(bitset<MAX> _a){
	//���涨��   ��չŷ�����
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
//	cout<<"��ԭ���ĳ˻���"<<multiple(t,_a)<<endl;
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
	cout<<" ѧ�ţ�ʮ�����ƣ���";
	scanf("%x",&id);
	cout<<"\n your id is "<<id<<endl;
	
	bitset<MAX> a = mod(id) ;
	cout<<"\n �����������е�Ԫ��a��"<<a<<endl;
	printf(" (ʮ�����ƣ�0x%x)\n\n",a) ;
	
	bitset<MAX> t = inverse(a) ;
	cout<<"\n ѧ�ŵ��棺"<<t<<endl;
	printf(" (ʮ�����ƣ�0x%x)\n\n",t) ;
	cout<<"��ԭ���ĳ˻���"<<multiple(inverse(a),a)<<endl;
	
	cout<<"\n a^20190911��"<<getExp(a)<<endl ; 
	printf(" (ʮ�����ƣ�0x%x)\n\n\n",getExp(a)) ;
}
 
 

int main(){
	px[127] =px[0]=px[1]= 1;
	rx[0] = rx[1] = 1;
//	cout<<"px="<<px<<"\nrx="<<rx<<endl<<endl; 
	for(int i=MAX;i>=0;i--)  if(px.test(i))  cout<<"x^"<<i<<"+";
	 while(1){
	 	system("cls");
	 	cout<<"\n\n ��ѡ����Ҫ���еļ��㣺"<<endl;
	 	cout<<"\n 1-�ӷ�    2-�˷�    3-ƽ��    4-����    5-ѧ�ż���    6-�˳�"<<endl;
	 	cout<<"\n choice: "; 
	 	int choice;
	 	cin>>choice;
	 	switch(choice){
	 		case 1:{
	 			system("cls");
	 			cout<<"\n\n �ӷ�"<<endl;
	 			add();
	 			system("pause") ;
	 			break;
	 		}
	 		case 2:{
	 			system("cls");
	 			cout<<"\n\n �˷�"<<endl;
	 			bitset<MAX> a,b;
	 			cout<<"\n ����������������"<<endl;
				cout<<"\n a: ";  cin>>a;
				cout<<"\n b: ";  cin>>b;
				printf("\n �˻���ʮ�����ƣ���0x%x\n\n\n",multiple(a,b)) ;
			//	cout<<"\n�˻�:"<<multiple(a,b)<<endl<<endl;
				
				system("pause") ;
	 			break;
	 		}
			case 3:{
				 system("cls");
	 			 cout<<"\n\n ƽ��"<<endl;
	 			 bitset<MAX> a;
			     cout<<"\n �����������";  cin>>a;
			     printf("\n ƽ����ʮ�����ƣ���0x%x\n\n",square(a)) ;
			//	 cout<<"\nƽ����"<<square(a)<<endl;
				 system("pause") ;
			     break;
			}
			case 4:{
				system("cls");
	 			cout<<"\n\n ����"<<endl;
	 			bitset<MAX> a ;
	 			cout<<"\n ����a��";   cin>>a;
	 			bitset<MAX> t = inverse(a) ;
	 			printf("\n �棨ʮ�����ƣ���0x%x",t) ;
			//	cout<<"\n�棺"<<inverse(a)<<endl;
				printf("\n\n ��ԭ���ĳ˻���ʮ�����ƣ���0x%x\n\n",multiple(t,a)) ;
				system("pause") ;
				break;
			}
			case 5:{
				system("cls");
	 			cout<<"\n\n ѧ�ż���\n\n"<<endl;
				studentId();
				system("pause") ;
				break;
			}
			default:
				system("cls");
				cout<<"\n\n �˳�"<<endl; 
				return 0;
	 	}
	 }
}

