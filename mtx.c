/*
 * MTX, a BLAS library (level 3) for ANSI-C v3.4
 *  	Copyright (C) 2013 Eng. Juan Camilo Gómez C. MSc. (kmilo17pet@gmail.com)
 *
 *	MTX is free software: you can redistribute it and/or modify it
 *	under the terms of the GNU Lesser General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	MTX is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */
#ifdef _CVI_
    #include "toolbox.h"  
#endif
#include "mtx.h"
/*============================================================================*/
matrix mtx_new(const unsigned char rows,const unsigned char cols){
    if (cols<=0 || rows<=0) return NULL;
    matrix m;
    int i;
    m=(matrix)malloc(sizeof(_Matrix));
    if (m==NULL) return NULL;
    
    m->pos =(double**) calloc(rows,sizeof(double*));
    if (m->pos==NULL){
        free(m);
        return NULL;
    }
    
    for (i=0;i<rows;i++){
        m->pos[i]=(double*)calloc(cols,sizeof(double));
        if (m->pos[i]==NULL){
            free(m->pos);
            free(m);
            return NULL;
        }
    }
    m->rows = rows;
    m->cols = cols;
    return m;
}
/*============================================================================*/
void _mtx_del(const matrix M){
    if (M==NULL) return;
    int i=0;
    for (i=0;i<M->rows;i++)
        free(M->pos[i]);
    free(M->pos);
    free(M);    
}
/*============================================================================*/
matrix mtx_cpy(const matrix M){
    matrix C = mtx_new(M->rows,M->cols);
    int i,j;
    for(i=0;i<C->rows;i++){
        for(j=0;j<C->cols;j++)
            C->pos[i][j]=M->pos[i][j];
    }
    return C;
}
/*============================================================================*/
matrix mtx_eye(unsigned char n, const double alpha){
    if (n<=0) return;
    matrix I = mtx_new(n,n);
    while(n--)  I->pos[n][n]=alpha;
    return I;
}
/*============================================================================*/
matrix mtx_diag(const matrix m){
    if(m==NULL) return NULL;
    unsigned char n,i;
    n=((m->rows)<(m->cols))? (m->rows) : (m->cols);
    matrix md = mtx_new(n,1);
    for (i=0;i<n;i++) md->pos[i][0] = m->pos[i][i]; 
    return md;
}
/*============================================================================*/
double mtx_trace(const matrix A){
    if(A==NULL) return NAN;
    if (A->rows!=A->cols) return NAN; 
    int n = A->rows;
    double trace=0;
    while(n--)  trace += A->pos[n][n];
    return trace;
}
/*============================================================================*/
void mtx_disp(const matrix M){
    if (M==NULL || (int)((intptr_t)M)==0) {puts("(0,0)=| |\n");return;}   
    if (((M->rows)==0)||((M->cols)<=0)) {puts("(0,0)=| |\n");return;}
    double mvalue;
    int i,j;
    printf("(%d,%d)=\n",M->rows,M->cols);
    for (i=0;i<M->rows;i++){
        for (j=0;j<M->cols;j++){
        mvalue=M->pos[i][j];
        if (fabs(mvalue)<4e-4) mvalue=0;
        printf(MTX_PRINT_FORMAT,mvalue);
        }
    putchar('\n');
    }
    putchar('\n');
}
/*============================================================================*/
matrix mtx_ndisp(int n, ...){
   int v;
   va_list param_pt;
   va_start(param_pt, n);
   for (v=0; v<n; v++){
        matrix m = va_arg(param_pt, matrix);
            if ((int)((intptr_t)m)==-1) break;
        mtx_disp(m);
   }
   return NULL;
}
/*============================================================================*/
matrix mtx_t(const matrix A){
    if(A==NULL) return NULL;
    int i,j;
    matrix At=mtx_new(A->cols,A->rows);
    for(i=0;i<At->rows;i++){
        for(j=0;j<At->cols;j++){
            At->pos[i][j]=A->pos[j][i];
        }
    }
    return At;
}
/*============================================================================*/
matrix mtx_gadd(const double alpha, const matrix A, const double beta, const matrix B){
    if(A==NULL || B==NULL) return NULL;
    if ((A->rows != B->rows) || (A->cols != B->cols)) return NULL;
    matrix C = mtx_new(A->rows, A->cols);
    int i,j;
    for (i=0;i<A->rows;i++){
        for (j=0;j<A->cols;j++)
            C->pos[i][j]=alpha*A->pos[i][j] + beta*B->pos[i][j];
    }    
    return C;
}
/*============================================================================*/
matrix mtx_ptpprod(const matrix A, const matrix B){     
    if(A==NULL || B==NULL) return NULL;
    if ((A->rows != B->rows) || (A->cols != B->cols)) return NULL;
    matrix C = mtx_new(A->rows, A->cols);
    int i,j;
    for (i=0;i<A->rows;i++){
        for (j=0;j<A->cols;j++)
            C->pos[i][j]=A->pos[i][j] * B->pos[i][j];
    }
    return C;
}
/*============================================================================*/
matrix mtx_ptpdiv(const matrix A, const matrix B){  
    if(A==NULL || B==NULL) return NULL;
    if ((A->rows != B->rows) || (A->cols != B->cols)) return NULL;
    matrix C = mtx_new(A->rows, A->cols);
    int i,j;
    for (i=0;i<A->rows;i++){
        for (j=0;j<A->cols;j++)
            C->pos[i][j]=A->pos[i][j] / B->pos[i][j];
    }    
    return C;
}
/*============================================================================*/
matrix mtx_koper(const matrix A, const char oper, const double k){
    if(A==NULL) return NULL;
    matrix C = mtx_new(A->rows, A->cols);
    int i,j;
    switch (oper){
        case '*':
            for (i=0;i<A->rows;i++){
                for (j=0;j<A->cols;j++) C->pos[i][j] = k*A->pos[i][j];
            } 
            break;
        case '+':
            for (i=0;i<A->rows;i++){
                for (j=0;j<A->cols;j++) C->pos[i][j] = k+A->pos[i][j];
            }             
            break;
        case '-':
            for (i=0;i<A->rows;i++){
                for (j=0;j<A->cols;j++) C->pos[i][j] = A->pos[i][j]-k;
            } 
            break;
        case '/':
            for (i=0;i<A->rows;i++){
                for (j=0;j<A->cols;j++) C->pos[i][j] = A->pos[i][j]/k;
            } 
            break;
        case '^':
            for (i=0;i<A->rows;i++){
                for (j=0;j<A->cols;j++) C->pos[i][j] = pow( A->pos[i][j], k);
            } 
            break;
        default:
            break;
    }
    return C;        
}
/*============================================================================*/
matrix mtx_rand(unsigned rows, unsigned char cols){
    static int flagseed=0;
    if ((rows<=0)||(cols<=0)) return(NULL);
    if (!flagseed){
        srand(time(NULL));
        flagseed=1;
    }
    int i,j;
    matrix M=mtx_new(rows,cols);
    for (i=0;i<M->rows;i++){
        for(j=0;j<M->cols;j++)
            M->pos[i][j]=(double)rand()/(double)RAND_MAX;
    }
    return(M);
}
/*============================================================================*/
matrix mtx_prod(const double alpha, const matrix A, const matrix B){
    matrix C=mtx_new(A->rows,B->cols);
    if (mtx_OUT_equal_AxB(C,alpha,A,B)==-1){ 
        mtx_del(C);
        return C;
    }
    return(C);
}
/*============================================================================*/
int mtx_OUT_equal_AxB(matrix OUT, const double alpha, const matrix A, const matrix B){ 
    if(A==NULL || B==NULL) return -1;
    if(A->cols != B->rows) return -1;
    int i,j,y;
    for(i =0;i<A->rows; i++){
        for(j=0;j<B->cols; j++){
            OUT->pos[i][j] = 0.0;
            for(y=0;y<A->cols; y++)
                OUT->pos[i][j] += A->pos[i][y]*B->pos[y][j];
            OUT->pos[i][j]*=alpha;
        }
    }
    return 0;
}
/*============================================================================*/
int mtx_dgemm(const double alpha, const matrix a, char transa, const matrix b, char transb, const double beta, matrix c){ //mtx_dgemm(0, NULL, '.', NULL, '.', k , A)
    double temp;
    int i,j,l;
    unsigned char nota=(transa!='T')&(transa!='t'),notb=(transb!='T')&(transb!='t');
    int n=c->rows,m=c->cols,k;
    
    if (c==NULL) return -1;
    
    if ( alpha == 0.0 ){
        if ( beta == 0.0 ){
            for ( j = 0; j < n; j++ ){
                for ( i = 0; i < m; i++ ) c->pos[i][j] = 0.0;
            }
        }
        else{
            for ( j = 0; j < n; j++ ){
                for ( i = 0; i < m; i++ ) c->pos[i][j] *= beta;
            }
        }
        return 0;
    }
    if (a==NULL || b==NULL) return -1;
    if (notb){
        if(nota){
            /*=================== Form  C = alpha*A*B + beta*C =======================*/
            if ((a->rows != c->rows)  || (b->cols != c->cols) || (a->cols != b->rows)) return -1;
            m=a->rows;
            n=b->cols;
            k=a->cols;
            for ( j = 0; j < n; j++ ){
                if ( beta == 0.0 ){
                    for ( i = 0; i < m; i++ ) c->pos[i][j] = 0.0;
                }
                else if ( beta != 1.0 ){
                    for ( i = 0; i < m; i++ ) c->pos[i][j] *= beta;
                }
                for ( l = 0; l < k; l++ ){
                    if ( b->pos[l][j] != 0.0 ){
                        temp = alpha * b->pos[l][j];
                        for ( i = 0; i < m; i++ ) c->pos[i][j] +=  temp*a->pos[i][l];
                    }
                }
            }
        }
        else{
            /*=================== Form  C = alpha*A'*B + beta*C =======================*/
            if ((a->cols != c->rows)  || (b->cols != c->cols) || (a->rows != b->rows)) return -1;
            m=a->cols;
            n=b->cols;
            k=a->rows;
            for ( j = 0; j < n; j++ ){
                for ( i = 0; i < m; i++ ){
                    temp = 0.0;
                    for ( l = 0; l < k; l++ ) temp += a->pos[l][i]*b->pos[l][j];
                    if ( beta == 0.0 )  c->pos[i][j] = alpha*temp;
                    else                c->pos[i][j] = alpha*temp + beta*c->pos[i][j];
                }
            }
        }
    }
    else{
        if(nota){
            /*=================== Form  C = alpha*A*B' + beta*C =======================*/
            if ((a->rows != c->rows)  || (b->rows != c->cols) || (a->cols != b->cols)) return -1;
            m=a->rows;
            n=b->rows;
            k=a->cols;
            for ( j = 0; j < n; j++ ){
                if ( beta == 0.0 ){
                    for ( i = 0; i < m; i++ )   c->pos[i][j] = 0.0;
                }
                else if ( beta != 1.0 ){
                    for ( i = 0; i < m; i++ )   c->pos[i][j] *= beta;
                }

                for ( l = 0; l < k; l++ ){
                    if ( b->pos[j][l] != 0.0 ){
                        temp = alpha*b->pos[j][l];
                        for ( i = 0; i < m; i++ )   c->pos[i][j] +=  temp*a->pos[i][l];
                    }
                }
            }
        }
        else{
            /*=================== Form  C = alpha*A'*B' + beta*C =======================*/
            if ((a->cols != c->rows)  || (b->rows != c->cols) || (a->rows != b->cols)) return -1;
            m=a->cols;
            n=b->rows;
            k=a->rows;
            for ( j = 0; j < n; j++ ){
                for ( i = 0; i < m; i++ ){
                    temp = 0.0;
                    for ( l = 0; l < k; l++ )   temp += a->pos[l][i]*b->pos[j][l];
                    if ( beta == 0.0 )  c->pos[i][j] = alpha*temp;
                    else                c->pos[i][j] = alpha*temp + beta*c->pos[i][j];
                }
            }
        }
    }
    return 0;
}
/*============================================================================*/
int mtx_dgema(const double alpha, const matrix a, char transa, const double beta, const matrix b, char transb){
    int i,j;
    unsigned char nota=(transa!='T')&(transa!='t'),notb=(transb!='T')&(transb!='t');
    int m,n;
    if (notb){
        if(nota){
            /*=================== Form  A = alpha*A + beta*B =======================*/
            if( (a->rows != b->rows) || (a->cols != b->cols) ) return -1;
            for(i=0;i<a->rows;i++){
                for(j=0;j<a->cols;j++)  a->pos[i][j] = alpha*a->pos[i][j] + beta*b->pos[i][j];                
            }
        }
        else{
            /*=================== Form  A = alpha*A' + beta*B =======================*/
            if( (a->rows != b->cols) || (a->cols != b->rows) ) return -1;
            for(i=0;i<b->rows;i++){
                for(j=0;j<b->cols;j++)  a->pos[j][i] = alpha*a->pos[j][i] + beta*b->pos[i][j];                
            }
        }
    }
    else{
        if(nota){
            /*=================== Form  A = alpha*A + beta*B' =======================*/
        }
        else{
            /*=================== Form  A = alpha*A' + beta*B' =======================*/
            if( (a->rows != b->rows) || (a->cols != b->cols) ) return -1;
            for(i=0;i<a->rows;i++){
                for(j=0;j<a->cols;j++)  a->pos[i][j] = alpha*a->pos[j][i] + beta*b->pos[j][i];                
            }
        }
    }
    return 0;
}
/*============================================================================*/
int mtx_A_equal_A_plus_B(matrix A, const matrix B){ 
    if(A==NULL || B==NULL) return -1;
    if((A->cols != B->cols) ||  (A->rows != B->rows)) return -1;
    int i,j;
    for(i=0;i<A->rows;i++){
        for(j=0;j<A->cols;j++){
            A->pos[i][j] += B->pos[i][j];
        }
    }
    return 0;
}
/*============================================================================*/
int mtx_A_equal_A_sub_B(matrix A, const matrix B){
    if(A==NULL || B==NULL) return -1;
    if((A->cols != B->cols) ||  (A->rows != B->rows)) return -1;
    int i,j;
    for(i=0;i<A->rows;i++){
        for(j=0;j<A->cols;j++){
            A->pos[i][j] = A->pos[i][j] - B->pos[i][j];
        }
    }
    return 0;
}
/*============================================================================*/
int mtx_A_equal_kA(matrix A, const double k){ 
    if(A==NULL) return -1;
    int i,j;
    for(i=0;i<A->rows;i++){
        for(j=0;j<A->cols;j++){
            A->pos[i][j] = A->pos[i][j]*k;
        }
    }
    return 0;
}
/*============================================================================*/
int mtx_OUT_equal_kA(matrix OUT,const matrix A, const double k){
    if(OUT==NULL || A==NULL) return -1;
    int i,j;
    for(i=0;i<A->rows;i++){
        for(j=0;j<A->cols;j++){
            OUT->pos[i][j] = A->pos[i][j]*k;
        }
    }
    return 0;     
}
/*============================================================================*/
matrix mtx_inv(const matrix X){
    if(X==NULL) return NULL;
    if(X->rows!=X->cols) return NULL; 
    matrix A=mtx_cpy(X); 
    matrix C=mtx_eye(X->rows,1.0);
    double tol = sqrt(DBL_EPSILON);
    int l,p,k,j; //must be signed 
    double y,m,n;

    for(j=0; j<A->cols; j++){
        p=j;
        while(fabs(A->pos[p][j]) < tol){
            p++;
            if (p >= A->rows){
                mtx_del(A);
                return NULL; 
            }
        }
        y=A->pos[p][j];
        for(k=0; k<A->cols; k++){
            m=A->pos[p][k];
            A->pos[p][k]=A->pos[j][k];
            A->pos[j][k]=m/y;
            n=C->pos[p][k];
            C->pos[p][k]=C->pos[j][k];
            C->pos[j][k]=n/y;
        }
        
        for(l=j+1; l<A->cols; l++){
            m=A->pos[l][j];
            for(k=0; k<A->cols; k++){
                A->pos[l][k] = A->pos[l][k] - m*A->pos[j][k];
                C->pos[l][k] = C->pos[l][k] - m*C->pos [j][k];
            }
        }
    }
      
    for (j=A->cols-1; j>=0; j--) 
        for (k=j-1; k>=0; k--){
            m=A->pos[k][j]; 
            for(l=0; l<A->cols; l++){
                A->pos[k][l] = A->pos[k][l] - m*A->pos[j][l];
                C->pos[k][l] = C->pos[k][l] - m*C->pos[j][l];
            }
    }
    
    mtx_del(A);
    return C;
}
/*============================================================================*/
matrix mtx_linsolve(const matrix A, const matrix B){
    if (A==NULL || B==NULL) return NULL;
    if ((A->rows != A->cols) || (B->cols !=1) || (B->rows != A->rows)) return NULL;
    matrix X = mtx_inv(A);
    if (X==NULL) return NULL;
    matrix y = mtx_new(A->rows ,1);    
    y = mtx_prod(1.0,X,B);
    mtx_del(X);
    return y;
}
/*============================================================================*/
matrix mtx_fxptp(const matrix A, double (*fx)(double)){
    if (A==NULL) return NULL;
    matrix C = mtx_new(A->rows, A->cols);
    int i,j;
    for (i=0;i<A->rows;i++){
        for (j=0;j<A->cols;j++)
            C->pos[i][j] = (*fx)(A->pos[i][j]);
    }    
    return C;
}
/*============================================================================*/
int mtx_memcpy(matrix dst, matrix scr){
    if (dst==NULL || scr==NULL) return -1;
    if ((dst->rows!=scr->rows) || (dst->cols!=scr->cols)) return -1;
    int i,j;
    for(i=0;i<dst->rows;i++){
        for(j=0;j<dst->cols;j++){
            dst->pos[i][j]=scr->pos[i][j];
    }}
    return 0;
}
/*============================================================================*/
matrix mtx_powui(const matrix m, unsigned int power){
    if(m==NULL) return NULL;
    if(m->rows!=m->cols) return NULL; 
    unsigned int k;
    matrix md=mtx_eye(m->rows, 1.0);
    if (power==0) return(md);
    else{
        matrix temp=mtx_new(m->rows,m->cols);
        mtx_memcpy(md,m);
        for (k=1;k<power;k++){
            mtx_OUT_equal_AxB(temp,1.0,md,m);
            mtx_memcpy(md,temp);
        }
        mtx_del(temp);
    }
    return(md);
}
/*============================================================================*/
matrix mtx_hcat(const matrix m1, const matrix m2){
    if( m1==NULL && m2==NULL) return NULL;
    if( m1==NULL) return m2;
    if( m2==NULL) return m1;
    if( m1->rows != m2->rows) return NULL;
    unsigned char f, c, nc=(m1->cols)+(m2->cols), c1=(m1->cols);
    matrix cm=mtx_new(m1->rows, nc);
    for (f=0;f<(m1->rows);f++){
        for (c=0;c<nc;c++)
            cm->pos[f][c] =(c>=c1)? m2->pos[f][c-c1] : m1->pos[f][c];           
    }
    return cm;
}
/*============================================================================*/
matrix mtx_vcat(const matrix m1, const matrix m2){
    if( m1==NULL && m2==NULL) return NULL;
    if( m1==NULL) return m2;
    if( m2==NULL) return m1;
    if( m1->cols != m2->cols) return NULL;
    unsigned char f, c, nf=(m1->rows)+(m2->rows), f1=(m1->rows);
    matrix cm=mtx_new(nf, m1->cols);
    for (f=0;f<nf;f++){
        for (c=0;c<m1->cols;c++)
            cm->pos[f][c] =(f>=f1)? m2->pos[f-f1][c] : m1->pos[f][c];
    }
    return cm;
}
/*============================================================================*/
matrix mtx_nvcat(int n, ...){
    int v;
    va_list param_pt;
    va_list margs;
    va_start(param_pt, n);
    int nf=0, nc=-1, nm=0;
    matrix m;
    int i,j,k=0;
    for (v=0; v<n; v++){
        m = va_arg(param_pt, matrix);
        if ((int)((intptr_t)m)==0 || m==NULL) break;
        nf+=m->rows;
        if (nc==-1){
            nc=m->cols;
        }
        else{
            if (nc!=m->cols) return NULL;
        }       
        nm++;
   }
   if (nm==0) return NULL;
   
   matrix C=mtx_new(nf,nc);
   va_start(margs, n);
   for (v=0; v<nm; v++){
       m = va_arg(margs, matrix);
       for(i=0;i<m->rows;i++){
           for(j=0;j<m->cols;j++){
               C->pos[i+k][j] = m->pos[i][j];
           }
       }
       k+=i;
   }
   return C;    
}
/*============================================================================*/
matrix mtx_nhcat(int n, ...){
    int v;
    va_list param_pt;
    va_list margs;
    va_start(param_pt, n);
    int nf=-1, nc=0, nm=0;
    matrix m;
    int i,j,k=0;
    for (v=0; v<n; v++){
        m = va_arg(param_pt, matrix);
        if ((int)((intptr_t)m)==0 || m==NULL) break;
        nc+=m->cols;
        if (nf==-1){
            nf=m->rows;
        }
        else{
            if (nf!=m->rows) return NULL;
        }       
        nm++;
   }
   if (nm==0) return NULL;
   
   matrix C=mtx_new(nf,nc);
   va_start(margs, n);
   for (v=0; v<nm; v++){
       m = va_arg(margs, matrix);
       for(i=0;i<m->rows;i++){
           for(j=0;j<m->cols;j++){
               C->pos[i][j+k] = m->pos[i][j];
           }
       }
       k+=i;
   }
   return C;    
}
/*============================================================================*/
matrix mtx_getsubset(const matrix m, int f1, int f2, int c1, int c2){
    if( m == NULL) return NULL;
    int ft,ct,f,c;
    ft=f2-f1+1;
    ct=c2-c1+1;
    if (ft<=0) ft=1;
    if (ct<=0) ct=1;
    matrix outm=mtx_new(ft,ct);
    for (f=f1;f<=f2;f++){
        for (c=c1;c<=c2;c++)
            outm->pos[f-f1][c-c1] = m->pos[f][c];
    }   
    return (outm);
}
/*============================================================================*/
void mtx_setsubset(matrix m1, const matrix m2,int f1,int f2,int c1,int c2){
    if( m1 == NULL || m2 == NULL) return;
    if( (m2->rows > m1->rows)  || (m2->cols > m1->cols) ) return;
    int f,c;
    for (f=f1;f<=f2;f++){
        for (c=c1;c<=c2;c++)
            m1->pos[f][c] = m2->pos[f-f1][c-c1];
    }   
}
/*============================================================================*/
double mtx_det(const matrix M){
    if(M->rows!=M->cols) return NAN;
    int n=M->rows;
    matrix L = mtx_new(M->rows,M->cols);
    matrix U = mtx_new(M->rows,M->cols);
    double det=1,signo=1.0;
    signo=mtx_lu(L,U,M); //determinant usign LU factorization
#ifdef _CVI_ 
    #define isnan(_x_)  IsNotANumber(_x_)
#endif
    if(isnan(signo)) {
        mtx_del(L);
        mtx_del(U);
        return ((double)0.0);
    }
    while(n--) det*=U->pos[n][n];
    mtx_del(L);
    mtx_del(U);
    return signo*det;
}
/*============================================================================*/
int mtx_swaprows(matrix A, int r1, int r2){
    if (A==NULL) return -1;
    int i;
    int n=A->cols;
    double temp;
    for(i=0;i<n;i++){
        temp = A->pos[r1][i]; 
        A->pos[r1][i] = A->pos[r2][i]; 
        A->pos[r2][i] = temp; 
    }
    return 0;
}
/*============================================================================*/
int mtx_swapcols(matrix A, int c1, int c2){
    if (A==NULL) return -1;
    int i;
    int n=A->rows;
    double temp;
    for(i=0;i<n;i++){
        temp = A->pos[i][c1]; 
        A->pos[i][c1] = A->pos[i][c2]; 
        A->pos[i][c2] = temp; 
    }
    return 0;
}
/*============================================================================*/
double mtx_lu(matrix L, matrix U, const matrix M){
    if (M==NULL || U==NULL || L==NULL) return -1;
    if (M->rows!=M->cols || U->rows!=U->cols || L->rows!=L->cols || L->rows!=M->rows || U->rows!=M->rows) return -1;
    int k,i,j,r,z; 
    int n = M->rows;
    matrix A = mtx_cpy(M);
    matrix P = mtx_eye(n, 1.0); //permutation matrix
    double tol = sqrt(DBL_EPSILON);
    double temp,signo=1.0;
    for(i=0;i<n;i++){ //just to be sure L is eye and U zeros
        for(j=0;j<n;j++){
            L->pos[i][j]=(i==j)? 1.0 : 0.0;
            U->pos[i][j]=0.0;
        }
    }  
    for(k=0;k<n;k++){
        if (fabs(A->pos[k][k]) < tol){
            for(r=k;r<n;r++){
                if(fabs(A->pos[r][k]) >= tol) break;
                if (r==(n-1)){
                    mtx_del(A);
                    mtx_del(P);
                    return NAN; //singular matrix
                }
            }
            mtx_swaprows(A,r,k);
            mtx_swaprows(P,r,k);
            if(k>0){ 
                for(z=0;z<k-1;z++){
                    temp = L->pos[r][z];
                    L->pos[r][z] = L->pos[k][z]; 
                    L->pos[k][z] = temp;
                } 
            }
            signo = -signo;
        }
        for(i=k+1;i<n;i++){
            L->pos[i][k]=A->pos[i][k]/A->pos[k][k];
            for(j=k+1;j<n;j++)  A->pos[i][j]=A->pos[i][j] - L->pos[i][k]*A->pos[k][j];
        }
        for(j=k;j<n;j++)    U->pos[k][j] = A->pos[k][j];         
    }
    mtx_del(A);
    mtx_del(P);
    return signo;
}
/*============================================================================*/
double mtx_cumsum(const matrix m){
    if (m==NULL) return(0.0);
    double cums=0;
    int i,j;
    for (i=0;i<m->rows;i++){
        for (j=0;j<m->cols;j++){
            cums+=m->pos[i][j];
        }
    }
    return(cums);
}
/*============================================================================*/
double mtx_cummax(const matrix m){
    if (m==NULL) return(0.0);
    double maxval=m->pos[0][0];
    int i,j;
    for (i=0;i<m->rows;i++){
        for (j=0;j<m->cols;j++){
            maxval = (m->pos[i][j]>maxval)?  m->pos[i][j] : maxval;
        }
    }
    return(maxval);    
}
/*============================================================================*/
double mtx_cummin(const matrix m){
    if (m==NULL) return(0.0);
    double minval=m->pos[0][0];
    int i,j;
    for (i=0;i<m->rows;i++){
        for (j=0;j<m->cols;j++){
            minval = (m->pos[i][j]<minval)?  m->pos[i][j] : minval;
        }
    }
    return(minval);    
}
/*============================================================================*/
matrix mtx_mean(const matrix m){
    if (m==NULL) return (NULL);
    if (mtx_isvector(m)){
        matrix s=mtx_new(1,1);
        s->pos[0][0]=mtx_cumsum(m)/mtx_numel(m);
        return(s);
    }
    matrix sm=mtx_new(1,m->cols);
    int f,c;
    double sum;
    for (c=0;c<(m->cols);c++){
        sum=0.0;
        for (f=0;f<(m->rows);f++) sum+=m->pos[f][c]; 
        sm->pos[0][c]=sum/(m->rows);
    }
    return(sm);
}
/*============================================================================*/
matrix mat_rpinv(const matrix A){
    if(A==NULL) return NULL; 
    matrix AT=mtx_t(A);
    matrix AxAT=mtx_prod(1.0,A,AT);
    matrix iAxAT=mtx_inv(AxAT);
    matrix res = mtx_prod(1.0,AT,iAxAT);
    mtx_del(AT);
    mtx_del(AxAT);
    mtx_del(iAxAT);
    return res; //AT*(A*AT)^-1
}
/*============================================================================*/
matrix mat_lpinv(const matrix A){
    matrix AT=mtx_t(A);
    matrix AxAT=mtx_prod(1.0,AT,A);
    matrix iATxA=mtx_inv(AxAT);
    matrix res = mtx_prod(1.0,iATxA,AT);
    mtx_del(AT);
    mtx_del(AxAT);
    mtx_del(iATxA);
    return res; //(AT*A)^-1 *AT
}
/*============================================================================*/
double mtx_cumprod(const matrix m){
    if (m==NULL) return(NAN);
    double cump=1.0;
    int i,j;
    for (i=0;i<m->rows;i++){
        for (j=0;j<m->cols;j++){
            cump*=m->pos[i][j];
        }
    }
    return(cump);
}
/*============================================================================*/
matrix mtx_produ(const matrix m){
    if (m==NULL) return NULL;
    if (mtx_isvector(m)){
        matrix s=mtx_new(1,1);
        s->pos[0][0]=mtx_cumprod(m);
        return(s);
    }
    matrix sm=mtx_new(1,m->cols);
    int f,c;
    double prd;
    for (c=0;c<(m->cols);c++){
        prd=1.0;
        for (f=0;f<(m->rows);f++) prd*=m->pos[f][c];
        sm->pos[0][c]=prd;
    }
    return(sm);
}
/*============================================================================*/
matrix mtx_max(const matrix m){
    if (m==NULL) return NULL;
    if (mtx_isvector(m)){
        matrix s=mtx_new(1,1);
        s->pos[0][0]=mtx_cummax(m);
        return(s);
    }
    matrix sm=mtx_new(1,m->cols);
    int f,c;
    double maxval;
    for (c=0;c<(m->cols);c++){
        maxval=m->pos[0][c];
        for (f=0;f<(m->rows);f++){
            maxval = (m->pos[f][c]>maxval)? m->pos[f][c] : maxval;
        }
        sm->pos[0][c]=maxval;
    }
    return(sm);
}
/*============================================================================*/
matrix mtx_min(const matrix m){
    if (m==NULL) return NULL;
    if (mtx_isvector(m)){
        matrix s=mtx_new(1,1);
        s->pos[0][0]=mtx_cummin(m);
        return(s);
    }
    matrix sm=mtx_new(1,m->cols);
    int f,c;
    double minval;
    for (c=0;c<(m->cols);c++){
        minval=m->pos[0][c];
        for (f=0;f<(m->rows);f++){
            minval = (m->pos[f][c]<minval)? m->pos[f][c] : minval;
        }
        sm->pos[0][c]=minval;
    }
    return(sm);
}
/*============================================================================*/
matrix mtx_sum(const matrix m){
    if (m==NULL) return NULL;
    if (mtx_isvector(m)){
        matrix s=mtx_new(1,1);
        s->pos[0][0]=mtx_cumsum(m);
        return(s);
    }
    matrix sm=mtx_new(1,m->cols);
    int f,c;
    double prd;
    for (c=0;c<(m->cols);c++){
        prd=0.0;
        for (f=0;f<(m->rows);f++){
            prd+=m->pos[f][c];
        }
        sm->pos[0][c]=prd;
    }
    return(sm);
}
/*============================================================================*/
matrix mtx_2d2mtx(const double *array2d,const int nf,const int nc){
    matrix mout=mtx_new(nf,nc);
    int f,c;
    for (f=0;f<nf;f++){
        for (c=0;c<nc;c++)
            mout->pos[f][c]=array2d[f*nc+c];
    }
    return(mout);
}
/*============================================================================*/
matrix mtx_1d2mtx(const double *array1d, const int arraylength){
    matrix mout=mtx_new(arraylength,1);
    int i;
    for (i=0;i<arraylength;i++)
        mout->pos[i][0]=array1d[i];        
    return(mout);
}
/*============================================================================*/
double mtx_colspprod(const matrix A, const int j, const int k){
    int i;
    double sum=0.0;
    for(i=0;i<A->rows;i++)
        sum+= A->pos[i][j]*A->pos[i][k];
    return sum;
}
/*============================================================================*/
matrix mtx_grams(const matrix M, matrix R){
    if (M==NULL || R==NULL) return NULL;
    if (M->rows != M->cols) return NULL;
    matrix A=mtx_cpy(M);
    matrix Q=mtx_new(M->rows,M->cols);
    matrix Rt=NULL;
    matrix Qt=NULL;
    int i,j,k,n=M->rows;
    double mult;
    double tol = sqrt(DBL_EPSILON);
    double norm;
    for(i=0;i<n;i++){ //just to be sure R is zeros
        for(j=0;j<n;j++) R->pos[i][j]=0.0;
    }  

    for(j=0;j<n;j++){
        for(k=0;k<=(j-1);k++){
            mult = mtx_colspprod(A,j,k)/mtx_colspprod(A,k,k);
            for(i=0;i<A->rows;i++)  A->pos[i][j] = A->pos[i][j] - mult*A->pos[i][k]; 
        }
    }
    
    for(j=0;j<n;j++){
        norm=0.0;
        for(i=0;i<A->rows;i++) norm+=pow(fabs(A->pos[i][j]),2.0); norm=sqrt(norm); //vectorial norm for j column
        if(norm<tol){
            mtx_del(Q);
            mtx_del(A);
            return NULL; // Columns are linearly dependent.
        }
        for(i=0;i<A->rows;i++) Q->pos[i][j]=A->pos[i][j]/norm;
    }
    Qt=mtx_t(Q); 
    Rt=mtx_prod(1.0,Qt,M); 
    mtx_memcpy(R,Rt);
    mtx_del(A);
    mtx_del(Rt);
    mtx_del(Qt);
    return Q;
}
/*============================================================================*/
double mtx_norm_inf(const matrix M){
    int i,j;
    double maxm,sum;
    for(i=0;i<M->cols;i++){
        sum=0.0;
        for(j=0;j<M->rows;j++)
            sum+=fabs(M->pos[i][j]);
        if(i==0) maxm=sum;
        maxm = (sum>maxm)? sum : maxm;
    }
    return maxm;
}
/*============================================================================*/
matrix mtx_expm(const matrix M, const double alpha){
    if (M==NULL) return NULL;
    if (M->rows != M->cols) return NULL;
    int e,s;
    unsigned char p=1, q=6, k;
    double c=0.5;
    matrix B  = mtx_koper(M, '*', alpha);
    frexp(mtx_norm_inf(B),&e);
    s = ((e+1)>0)? e+1 : 0;
    matrix A = mtx_koper(B,'/' ,pow(2.0, (double)s));
    mtx_del(B);
    matrix X = mtx_cpy(A);
    matrix I = mtx_eye(A->rows , 1.0);
    matrix E = mtx_gadd(1, I,  c, A); //E = eye(size(A)) + c*A;
    matrix D = mtx_gadd(1, I, -c, A); //D = eye(size(A)) - c*A;  
    matrix cX = mtx_new(A->rows, A->cols);
    matrix iD = mtx_new(A->rows, A->cols);
    for(k=2;k<=q;k++,p=!p){
        c = c*(q-k+1)/(k*(2*q-k+1));
        mtx_dgemm(1.0, A, '.', X, '.', 0, X); // X = A*X
        mtx_OUT_equal_kA(cX, X, c); // cX = c*X
        mtx_A_equal_A_plus_B(E, cX); // E = E + cX;
        mtx_dgema(1.0, D, '.', ((p==1)? 1.0: -1.0) , cX, '.');  // D = D + cX  or D = D - cX  (p depends)
    }
    iD = mtx_inv(D);
    mtx_dgemm(1.0, iD, '.', E, '.', 0, I); // I = iD*E
    mtx_memcpy(E,I);
    for(k=1;k<=s;k++){
        mtx_dgemm(1.0, E, '.', E, '.', 0, I); //I = E*E
        mtx_memcpy(E,I);
    } 
    mtx_del(A);mtx_del(X);mtx_del(I);mtx_del(D);mtx_del(cX);mtx_del(iD);    
    return E;
}
/*============================================================================*/
matrix mtx_lspcf(double *X, double *Y, int n, int m){ //least-squares polynomial curve fitting
    if (n<1) return NULL;    
    double *Sx = (double*)malloc((2*n+1)*sizeof(double));
    matrix Q = mtx_new(n+1,1);
    matrix M = mtx_new(n+1,n+1);
    int i,j;
    for(i=0;i<=2*n;i++){
        Sx[i]=0;
        for(j=0;j<m;j++) Sx[i]+=pow(X[j],i);   
    }
    for(i=0;i<n+1;i++){
        Q->pos[i][0]=0;
        for(j=0;j<m;j++) Q->pos[i][0]+=pow(X[j],i)*Y[j];
        for(j=0;j<n+1;j++) M->pos[i][j]=Sx[j+i];
    }
    matrix R = mtx_linsolve(M,Q);
    free(Sx);
    mtx_del(M);mtx_del(Q);
    return R;
}
/*============================================================================*/
matrix mtx_dot(matrix A, matrix B){
    if (A==NULL|| B==NULL) return NULL;
    int i;
    int n,m;
    matrix C;
    if (mtx_isvector(A) && mtx_isvector(B)){
        if(mtx_length(A)!=mtx_length(B)) return NULL;
        C=mtx_new(1,1);
        n = (mtx_isrow(A))? 0 : 1;
        m = (mtx_isrow(B))? 0 : 1;
        for(i=0;i<mtx_length(A);i++)  C->pos[0][0]+= A->pos[i*n][i*(!n)]*B->pos[i*m][i*(!m)];
        return C;
    }
    if (A->rows != B->rows && A->cols != B->cols) return NULL;
    C = mtx_new(1,mtx_length(A));
    for(n=0;n<A->cols;n++){
        for(m=0;m<A->rows;m++)   C->pos[0][n]+= A->pos[m][n]*B->pos[m][n];
    }
    return C;
}
/*============================================================================*/