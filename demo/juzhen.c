#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#define MAXSIZE 1500
int am,an,bm,bn;
typedef int ElemType;
typedef int Status;
//定义三元组类型
typedef struct t{
	int i;
	int j;
	int v;
}syz;
// 通过三元组类型定义出以三元组形式存储的稀疏矩阵类型
typedef struct {
	syz data[MAXSIZE+1];
	int mu,nu,tu;
}TsMatrix;
/*矩阵快速转置*/
void zhuanzhi(TsMatrix M,TsMatrix *T){
	int q,col,p;
	int num[M.tu+1],cpot[M.tu+1];
	//初始化T
	T->mu=M.nu;	T->nu=M.mu;	T->tu=M.tu;
	if(T->tu > 0){
		for(col=1;col<=M.nu;col++){
			num[col]=0;
		}
		for(p=1;p<=M.nu;p++){
			++num[M.data[p].j];
		}
		cpot[1]=1;
		for(col=2;col<=M.nu;col++){
			cpot[col]=cpot[col-1]+num[col-1];
		}

		for(p=1;p<=M.tu;p++){
			col=M.data[p].j; q=cpot[col];
			T->data[q].i=M.data[p].j;
			T->data[q].j=M.data[p].i;
			T->data[q].v=M.data[p].v;
			cpot[col]++;
		}
	}
}
/*输出三元组*/
void printmatrix(TsMatrix T){
	for(int i=1;i<=T.tu;i++)
		printf("%d %d %d\n",T.data[i].i,T.data[i].j,T.data[i].v);
}
/*输出矩阵*/
void print(ElemType **a,int m,int n){
	for(int i=1;i<=m;i++){

		for(int j=1;j<=n;j++){
			if(j==1){
				//printf("！");
			}
			printf("%d ",a[i][j]);
			if(j==n){
				//printf("！");
			}
		}

		printf("\n");
	}
}
/*---生成随机矩阵---*/
ElemType** randzz(int m,int n){
	m++,n++;
	ElemType **a;
	a=(ElemType**)malloc(m*sizeof(int*));//0号单元不用 ，所以m+1
	for(int i=1;i<=m;i++){
		a[i]=(ElemType*)malloc(n*sizeof(int));//同m
	}
	for(int i=1;i<=m;i++){
		for(int j=1;j<n;j++){
			if(rand()%7>1){//为了使矩阵稀疏
				a[i][j]=0;
			}else{
				a[i][j]=rand()%3+1;
			}

		}
	}
	return a;
}
/*稀疏矩阵 转换成 三元组*/
TsMatrix zh1(ElemType **sj,TsMatrix *M){

	int k=0;
	for(int i=1;i<=M->mu;i++){
		for(int j=1;j<=M->nu;j++){
			if(sj[i][j]!=0){
				M->data[++k].v=sj[i][j];
				M->data[k].i=i;
				M->data[k].j=j;
				M->tu++;
			}

		}
	}
}
/*三元组 转换为 稀疏矩阵 */
ElemType** zh2(TsMatrix M){
	int m=M.mu+1,n=M.nu+1;
	ElemType **a;
	a=(ElemType**)malloc(m*sizeof(int*));//0号单元不用 ，所以m+1
	for(int i=0;i<m;i++){
		a[i]=(ElemType*)malloc(n*sizeof(int));//同m
	}
	/*将矩阵初始化为0*/
	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			a[i][j]=0;
		}
	}
	for(int i=1;i<=M.tu;i++){
		if(M.data[i].v!=0){
			a[M.data[i].i][M.data[i].j]=M.data[i].v;
		}
	}
	return a;
}
void print2(TsMatrix M){

	for(int i=1;i<=M.tu;i++){
		printf("( %d, %d, %d)\n",M.data[i].i,M.data[i].j,M.data[i].v);
	}
}

void swap(syz *C,syz A){
	C->i=A.i;
	C->j=A.j;
	C->v=A.v;
}
/*矩阵加法*/
TsMatrix add(TsMatrix A,TsMatrix B,TsMatrix *C){
	C->nu=A.nu,C->mu=A.mu;
	int i=1,j=1,k;
	for(k=1;i<=A.tu&&j<=B.tu;k++){
		if(A.data[i].i < B.data[j].i){
			swap(&(C->data[k]),A.data[i++]);
		}else if(A.data[i].i > B.data[j].i){
			swap(&(C->data[k]),B.data[j++]);
		}else{
			if(A.data[i].j < B.data[j].j){
				swap(&(C->data[k]),A.data[i++]);
			}else if(A.data[i].j > B.data[j].j){
				swap(&(C->data[k]),B.data[j++]);
			}else{
				C->data[k].i=A.data[i].i;
				C->data[k].j=A.data[i].j;
				C->data[k].v=A.data[i].v+B.data[j].v;
				j++,i++;
			}
		}
			C->tu++;
	}
	while(i<=A.tu){
		swap(&(C->data[k++]),A.data[i++]);C->tu++;
	}
	while(j<=B.tu){
		swap(&(C->data[k++]),B.data[j++]);C->tu++;
	}
}

int main(){
	srand((int)time(NULL));
	//初始化M
TsMatrix T,A,B,C;
	ElemType **a=NULL,**b;

	printf("请输入矩阵 A 的行数：");
	scanf("%d",&am);
	printf("清输入矩阵 A 的列数：");
	scanf("%d",&an);
	printf("请输入矩阵 B 的行数：");
	scanf("%d",&bm);
	printf("清输入矩阵 B 的列数：");
	scanf("%d",&bn);

	A.nu=an,A.mu=am,A.tu=0;
	B.nu=bn,B.mu=bm,B.tu=0;

	a=randzz(am,an);
	b=randzz(bm,bn);
	printf("\n随机生成的矩阵 A：\n");
	print(a,am,an);
	zh1(a,&A);
	printf("A 的三元组形式：\n");
	print2(A);
   zhuanzhi(A,&T);
    printf("A 的转置 T：\n");
	print(zh2(T),an,am);
	printf("T 的三元组形式：\n");
	print2(T);
	printf("\n随机生成的矩阵 B：\n");
	print(b,bm,bn);
	zh1(b,&B);
	printf("B 的三元组形式：\n");
	print2(B);
	C.tu=0;
	add(A,B,&C);
	int cm=am,cn=an;
	printf("矩阵 C=A+B :\n");
	print(zh2(C),cm,cn);
	printf("C 的三元组形式：\n");
	print2(C);
}