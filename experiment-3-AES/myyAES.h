#ifndef MYYAES_H
#define MYYAES_H


class myyAES{
	private:
		int MEG[1024] ;
		int len ;
		int megBlocks[512][4][4];
		int blockCnt ;
		int roundKey[44][4]; 
		int round;
		static int iv[4][4] ;
		static int MixMatrix[4][4] ; 
		static int paiS[16][16] ;
		const static int rcon[10][4];
		
		int lenInv ;
		int cipherBlocks[512][4][4] ;
		int blockCntInv ;
		int ivInv[4][4] ;
		static int paiSInv[16][16] ;
		static int MixMatrixInv[4][4] ;

	public:
		void print(int round);
		myyAES(int meg[1024], int len);
		~myyAES() ;
		void group(int len) ;
		void creatRoundKey() ;
		void f(const int*, int*, int);
		void addRoundKey(int number, int round);  //state,roundkey
		void subBytes(int number) ; //state
		void shiftRows(int number) ;  //state
		void mixColumns(int number) ;  //state, mixmatrix
		int encrypt(int *text) ;
		
		void decryption(int *text, int);
		void groupInv(int *text) ;
		void addRoundKeyInv(int, int) ;
		void shiftRowsInv(int) ;
		void subBytesInv(int) ;
		void mixColumnsInv(int) ;
		void printInv(int round) ;
};

#endif

