#include<iostream>
#include<gmp.h>
#include<gmpxx.h>
#include<map>
using namespace std;
mpz_class p, g, ya, yb, xa, xb, ord_g ;
bool ready = 0 ;
map<mpz_class, mpz_class> couple ;

void shanks(mpz_class y, mpz_class &x){
    mpz_class m, ans ;
    mpz_sqrt(m.get_mpz_t(), ord_g.get_mpz_t()) ;
    m++;
    if(ready==false){
        for(mpz_class j=0; j<m-1; j++){
            ans = j*m;
            mpz_powm(ans.get_mpz_t(), g.get_mpz_t(), ans.get_mpz_t(), p.get_mpz_t()) ;
            couple[ans] = j ;
        }
        ready = 1 ;
    }
 //   for(map<mpz_class, mpz_class>::iterator it=couple.begin(); it!=couple.end(); it++){
  //     cout<<"("<<it->first<<" , "<<it->second<<endl;
 //  }
    for(mpz_class i=0; i<m-1; i++){
        mpz_powm(ans.get_mpz_t(), g.get_mpz_t(), i.get_mpz_t(), p.get_mpz_t()) ;
        mpz_invert(ans.get_mpz_t(), ans.get_mpz_t(), p.get_mpz_t()) ;
        ans = y*ans ;
        mpz_mod(ans.get_mpz_t(), ans.get_mpz_t(), p.get_mpz_t()) ;

        if(couple[ans]){
            mpz_class j = couple[ans] ;
            x = m*j+i ;
            mpz_mod(x.get_mpz_t(), x.get_mpz_t(), ord_g.get_mpz_t()) ;
            return ;
         }
    }
}

int main(){
    p = "5682549022748424631339131913370125786212509227588493537874673173634936008725904358935442101466555561124455782847468955028529037660533553941399408331331403379" ;
    g = "2410497055970432881345493397846112198995088771364307195189734031205605186951241875096796459061741781667380437076874705300974836586165283741668656930807264789" ;
    ya = "973768284341326272301553751114322685324340805902069613339667142187801529585352406975030927008752571917079716318221077758236884342662829402529734009607531649" ;
    yb = "4149822765985031146554298777122732051870868431387323913747785791685310508836836283702926446817000114868007555717546362425841865173929670156568682745060708314" ;
    ord_g = "4309874666" ;
    cout<<" p = "<<p<<"\n g = "<<g<<"\n ya = "<<ya<<"\n yb = "<<yb<<endl;
    shanks(ya, xa) ;
    shanks(yb, xb) ;
    cout<<"\n 经过shanks算法得到\n xa is "<<xa<<"\n xb is "<<xb<<endl;

    mpz_class ta, tb ;
    mpz_powm(ta.get_mpz_t(), g.get_mpz_t(), xa.get_mpz_t(), p.get_mpz_t()) ;
    mpz_powm(tb.get_mpz_t(), g.get_mpz_t(), xb.get_mpz_t(), p.get_mpz_t()) ;
    if(ta!=ya || tb!=yb){
        cout<<" 实验失败！ ";
        if(ta!=ya)   cout<<xa<<"不是正确解！"<<endl;
        else cout<<xb<<"不是正确解！"<<endl;
        return 1 ;
    }
    cout<<"\n 经验证，计算正确！"<<endl;
    mpz_class same_a, same_b;
    mpz_powm(same_a.get_mpz_t(), yb.get_mpz_t(), xa.get_mpz_t(), p.get_mpz_t()) ;
    mpz_powm(same_b.get_mpz_t(), ya.get_mpz_t(), xb.get_mpz_t(), p.get_mpz_t()) ;

    if(same_a==same_b)   cout<<"\n 验证通过！实验成功！\n 共同信息为 "<<same_a<<endl<<endl;
    else { cout<<"\n 实验失败,共同信息错误！"<<endl<<endl; return 1; }
    system("pause") ;
}
