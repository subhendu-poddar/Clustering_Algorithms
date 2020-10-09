//Project for mini-project(K-means clustering)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

double scale(double x,double M,double m)
{
	return ((x-m)/(M-m));
}

int checkdistance(float** b, float ***centlist, int m2, int c, int k, int r)
{
	float dist=0,mindist=100000;
	int i,j,l;
	for(i=0;i<k;i++)
	{
		dist =0;
		for(j=0;j<c;j++)
		{
			dist += ((b[r][j]-centlist[m2][i][j]) * (b[r][j]-centlist[m2][i][j]));
		}
		if(mindist>dist)
		{
			mindist = dist;
			l=i;
		}
	}

	return (l+1);

}

float** finddistance(int r, int c, int k, float **b, float **cent, float **cent1, int iteration, float **arr) {
	
	int i , j , i1, j1 ,d1 = 0 ,d2 = 0;
	
	//here cl is cluster centroid;
	float **cl;
	cl = (float **)malloc(k*sizeof(float *));
	for(i=0;i<k;i++) {
		cl[i] = (float *)malloc(c*sizeof(float));
	}

	//allocating an array to create the cluster;
	float ***list;
	list = (float ***)malloc(k*sizeof(float **));
	for(i=0;i<k;i++) {
		list[i] =(float **)malloc(r*sizeof(float *));
		for(j=0;j<r;j++) {
			list[i][j] = (float *)malloc(c*sizeof(float));
		}
	}
	//number of objects in each clustered will be stored in "element";
	int *element = (int *)malloc(k*sizeof(int));
	for(i=0;i<k;i++) {
		element[i] = 0;
	}
	//for checking the distance from a point to all the clustered centroids;
	float *dist;
	dist = (float *)malloc(k*sizeof(float));

	for(i=0;i<r;i++) {

		for(i1=0;i1<k;i1++) {
			dist[i1] = 0;
			for(j=0;j<c;j++) {
				dist[i1] += ((b[i][j] - cent[i1][j])*(b[i][j] - cent[i1][j]));
			}
		}
		//distances are taken in "dist";

		float min_dist = dist[0];
		d1 = 0;
		for(j=0;j<k;j++) {
			if(min_dist > dist[j]) {
				min_dist = dist[j];
				d1 = j;
			}
		}
		for(j=0;j<c;j++) {
			d2 = element[d1];
			list[d1][d2][j] = b[i][j];
		}
		element[d1]++;

	}
	//number of elements in each cluster has been stored;

	float sum = 0;
	for(i=0;i<k;i++) {

		for(j=0;j<c;j++) {
			sum = 0;
			for(j1=0;j1<element[i];j1++) {
				sum += list[i][j1][j];
			}
			if(element[i] == 0)
				cl[i][j] = 0;
			else
				cl[i][j] = sum/(float)element[i];
		}
	}
	//cluster centroids are found;

	cent1 = cent ;
	cent = cl ;

	int cl_count = 0;
	for(i=0;i<k;i++) {
		for(j=0;j<c;j++) {
			if(cent[i][j] == cent1[i][j]) {
				cl_count++;
			}
		}
	}
	//calculating sum of square errors;
	float sse = 0;
	for(i=0;i<k;i++) {
		for(j=0;j<element[i];j++) {
			for(j1=0;j1<c;j1++) {
				sse += ((cl[i][j1]-list[i][j][j1]) * (cl[i][j1]-list[i][j][j1]));
			}
		}
	}
	cent[0][c]=sse;

	if((cl_count != k*c) && iteration <50 )  {
		for(i=0;i<k;i++) {
			printf("%d  ", element[i]);
		}
		printf("\n");
		iteration++;
		printf("\nsse value of iteration %d is %7.4f\n\n",iteration,sse);
		return finddistance(r,c,k,b,cent,cent1,iteration,arr);
	}
	else {
		cent[0][c] = sse;
		return cent;
	}


}



int main(int argc, char *argv[])
{
	int f = atoi(argv[3]);
	long int k = atoi(argv[2]);
	if(argc!=4 || k==0 || f==0) {
		printf("./a.out <filename> <no. of clusters> <no. of test>\n\n");
		return 0;
	}
	//collect data from file given (filename) by the user;
    FILE *fr;
    char s[1000000],s2[1000000];
    strcpy(s,argv[1]);
    strcat(s, ".data");
    fr=fopen(s, "r");
    char st[1000000];
    char ch;
    int i,j=-1,len=0,w,p=0,q=-1,r,x1,x2,g;
    if(fr==NULL)
    {
        printf("File does not exists\n");
        exit(0);
    }
    do
    {
        ++len;
        st[len]=fgetc(fr);
    }while(st[len+1]!=EOF);
    fclose(fr);

    float **arr;
    arr = (float **)malloc(100000*sizeof(float *));
    for(i=0;i<100000;i++) {
    	arr[i] = (float *)malloc(100*sizeof(float));
    }

    for(i=0;i<len;i++)
    {
        if((st[i]>=45 && st[i]<=122))
        {
            g=-1;
            while((st[i]>=45 && st[i]<=122))
            {
                s2[++g]= st[i];
                i++;
            }
            s2[++g]='\0';
            arr[p][++q]=atof(s2);
            i-=1;
        }
        if(st[i]=='\n')
        {
	    	w=q;	
            ++p;
            q=-1;
        }
    }
    
    //collected data from file as arr[p][w] ; 

    //scaling of data;
	//printf("%d %d\n", p,w);
    float *m,*M;
    m = (float*)malloc(p*sizeof(float));
    M = (float*)malloc(p*sizeof(float));
    float **b;
    b = (float **)malloc(p*sizeof(float *));
    for(i=0;i<p;i++) {
    	b[i] = (float *)malloc(w*sizeof(float));
    }

    for(j=0;j<w;j++)
	{
		M[j]=arr[0][j];
		m[j]=arr[0][j];
		for(i=0;i<p;i++)
		{
			if(arr[i][j]<m[j])
				m[j]=arr[i][j];
			else if(arr[i][j]>M[j])
				M[j]=arr[i][j];
		}
	}
	for(i=0;i<p;i++)
	{
		for(j=0;j<w;j++)
		{
			b[i][j]=scale(arr[i][j],M[j],m[j]);
		}
	}

	//Data scaled in the matrix b;

	float **cent, **cent1;
	float g1;
	
	cent = (float **)malloc(k*sizeof(float *));
	for(i=0;i<k;i++) {
		cent[i] = (float *)malloc(w*sizeof(float));
	}

	cent1 = (float **)malloc(k*sizeof(float *));
	for(i=0;i<k;i++) {
		cent1[i] = (float *)malloc(w*sizeof(float));
	}

	x1 = p-1;
	//Generating k cluster centroids of random numbers;

	long m1,random,m2;
	float sse[50],***centlist, min =100000000;
	centlist = (float***)malloc(50*sizeof(float**));
	for(i=0;i<50;i++)
	{
		centlist[i] = (float**)malloc(k*sizeof(float*));
		for(j=0;j<w;j++)
			centlist[i][j] = (float*)malloc(w*sizeof(float));
	}
	for(m1=0;m1<50;m1++)
	{
		random = (rand()%2900)+100;

		printf("\nGenerated random number is %ld\n", random);

		for(i=0;i<k;i++) {
			for(j=0;j<w;j++) {
			g1 =((float)(rand() % random));
			cent[i][j] = g1/random;
			}
		}


		int iteration = 0;

		cent = finddistance(p,w,k,b,cent,cent1,iteration,arr);

		for(i=0;i<k;i++)
		{
			for(j=0;j<w;j++)
			{
				centlist[m1][i][j] = cent[i][j];
			}
		}
		sse[m1] = cent[0][w];

		printf("\n			sse value in test %ld is  %f\n", m1+1, cent[0][w]);

	}
	for(m1=0;m1<50;m1++)
	{
		if(min>sse[m1])
		{
			m2 = m1;
			min = sse[m1];
		}
	}

	printf("\nlowest SSE value found in test   %ld  and the SSE value is	%f\n", m2+1, min);

	printf("\nFinal centroids are\n\n");
	for(i=0;i<k;i++)
	{
		for(j=0;j<w;j++)
		{
			printf("%7.3f ", centlist[m2][i][j]);
		}
		printf("\n");
	}
	printf("\n\n	Final SSE value is %f\n", min);

	//creating a new file to save cluster numbers along with the original data;

	char sh[200];
	sprintf(sh, "_%ld_kmeans.data",k);
	strcat(argv[1],sh);
	FILE *fp = fopen(argv[1], "w");
	if(fp==NULL) {
		printf("\nUnable to store the datas\n\n");
	 	exit(0);
	}

	for(i=0;i<p;i++)
	{
		for(j=0;j<w;j++)
		{
			fprintf(fp, "%2.1f,", arr[i][j]);
		}
		fprintf(fp, "%d\n", checkdistance(b,centlist,m2,w,k,i));
	}
	fclose(fp);

	return 0;
}
