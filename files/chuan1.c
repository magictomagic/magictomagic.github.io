#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#define maxna 1000
#define maxnb 2000
#define maxnc 800
int a[maxna][maxnb];
int b[maxnb][maxnc];
int c[maxna][maxnc];
void Init(int n, int m, int a[n][m])
{
    int i,j;
    srand(time(NULL));//随机种子
    for(i=0; i<n; i++)
        for(j=0; j<m; j++)
            a[i][j]= rand()%10;
}
void Print(int n, int m, int a[n][m])
{
    int i,j;
    for(i=0; i<n; i++)
    {
        for(j=0; j<m; j++)
            printf("%d\t",a[i][j]);
        printf("\n");
    }
    printf("\n");
}
int main()
{
    char ch;
        Init(maxna, maxnb, a);
        Init(maxnb, maxnc, b);
        int i,j,k;
        memset(c,0,sizeof(c));
        time_t tt0=clock();
        struct timeval tim;
        gettimeofday(&tim,NULL);
        double t1 = tim.tv_sec +(tim.tv_usec / 1000000.0);
        for(i=0; i<maxna; i++)
        {
            for(j=0; j<maxnb; j++)
            {
                for(k=0; k<maxnc; k++)
                    c[i][j]+=a[i][k]*b[k][j];
            }
        }
        time_t tt1=clock();
        gettimeofday(&tim,NULL);
        printf("CPU 时间:%.6lf seconds\n",(double)(tt1-tt0)/CLOCKS_PER_SEC);
        double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
        printf("实际时间:%.6lf seconds \n", t2-t1);

    return 0;
}
