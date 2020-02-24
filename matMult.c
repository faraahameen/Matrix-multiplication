#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>


void readfile();
void elementThread();
void*elementCalc(void*data);
void* rowCalc(void* data);
void rowthread();
  FILE*file;
  FILE*file2; 


int **mat1,**mat2,**resMat;
int r1,c1,r2,c2;
struct threadinfo
{
    int rowi;
    int coli;
  };

void readfile()
{
    int i,j;
    FILE*file = fopen("input.txt","r");
    if(file == NULL)
    {
        printf("Cannot open file");
        error(0);
    }
    
    fscanf(file,"%d",&r1);
    fscanf(file,"%d",&c1);
    mat1 = malloc(r1*sizeof(int*));
    for(i=0; i<r1; i++)
    {
        mat1[i]=(int*)malloc(c1*sizeof(int));
        for (j=0; j<c1 ; j++){
            fscanf(file,"%d",&mat1[i][j]);
         }
    }


    //scanf(file,"%d %d",&r2,&c2);
    fscanf(file,"%d",&r2);
    fscanf(file,"%d",&c2);
    mat2 = malloc(r2*sizeof(int*));
    for(i=0; i<r2; i++)
    {
        mat2[i]=(int*)malloc(c2*sizeof(int));
        for (j=0; j<c2 ; j++)
            fscanf(file,"%d",&mat2[i][j]);

    }

    resMat =malloc(r1*sizeof(int*));
    for(i=0; i<r1; i++)
    {
        resMat[i] = (int*)malloc(c2*sizeof(int));
    }
}


void elementThread()
{
    pthread_t thread[r1*c2];
    struct threadinfo*data =(struct threadinfo*)malloc(sizeof(struct threadinfo));
    for(int i=0; i<r1; i++)
    {
        for(int j=0; j<c2; j++)
        {
            data->rowi=i;
            data->coli=j;
            pthread_create(&thread[i*c2+j], NULL, elementCalc,(void*) data);
        }
    }

    for(int i=0; i< r1*c2; i++){

        pthread_join(thread[i], NULL);

    }
}

void*elementCalc(void*info)
{
    struct threadinfo *index = (struct threadinfo *) info;
   
    resMat[index->rowi][index->coli] = 0;

    for(int k=0; k<c1; k++)
    {

        resMat[index->rowi][index->coli] += mat1[index->rowi][k] * mat2[k][index->coli];

    }
 //free(index);
 pthread_exit(NULL);

}






void rowthread()
{
    pthread_t thread[r1];
    struct threadinfo*data =(struct threadinfo*)malloc(sizeof(struct threadinfo));
    for (int i = 0; i < r1; i++)
    {
        data->rowi=i;
        data->coli=0;
        pthread_create(&thread[i], NULL, rowCalc,(void*)data);

    }
    for (int i = 0; i < r1; i++)
    {
        pthread_join(thread[i], NULL);
    }

}




void* rowCalc(void* info)
{
    struct threadinfo *index =/*(struct threadinfo*)*/ info;
   
    for(int k =0; k<c2; k++)
    {
        resMat[index->rowi][k]=0;
        for(int j=0; j<c1; j++)
        {
            resMat[index->rowi][k]+=mat1[index->rowi][j] * mat2[j][k];
        }

    }
//free(index);

  pthread_exit(NULL);
}








int main()
{
    clock_t start,end;
    double time1,time2;
    readfile();
    start = clock();
    rowthread();
    end = clock();
    FILE*file2 = fopen("output.txt","w");
    fprintf(file2,"RESULT: \n");
     for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            fprintf(file2, "%d ", resMat[i][j]);
        }
        fprintf(file2, "\n");
    }
    time1 =  (double) (end - start) / CLOCKS_PER_SEC;
    fprintf(file2, "Time(row thread) [%lf]\n", time1);

    start = clock();
    elementThread();
    end = clock();
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            fprintf(file2, "%d ", resMat[i][j]);
        }
        fprintf(file2, "\n");
    }
    time2 =  (double) (end - start) / CLOCKS_PER_SEC;
    fprintf(file2, "Time(row*col thread) [%lf]\n", time2);

    fclose(file);
    fclose(file2);

return 0;
}



