//Project for mini-project(K-medoid clustering)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>

double scale(double x,double M,double m) {
	return ((x-m)/(M-m));
}
int checkdistance(float** b, float **final, int c, int k, int r) {
	//this function finds which centroid is closest from a particular point
	float dist=0,mindist=100000,d;
	int i,j,l;
	for(i=0;i<k;i++) {
		dist =0;
		for(j=0;j<c;j++)
		{
			d = b[r][j]-final[i][j];
			if(d>=0)
				dist+=d;
			else
				dist-=d;
		}
		if(mindist>dist)
		{
			mindist = dist;
			l=i;
		}
	}
	return (l+1);
}
int findnrstcent(int curr, int c, int k, float **b, float **cent) {
	//this function finds the nearest centroid from curr point of b
	float dist, mindist = 2e15;
	int cpoint=0,i,j;
	for(i=0;i<k;i++) {
		dist = 0;
		for(j=0;j<c;j++) {
			if(b[curr][j]>=cent[i][j])
				dist += (b[curr][j]-cent[i][j]);
			else
				dist += (cent[i][j]-b[curr][j]);	
		}
		if(mindist>dist) {
			mindist = dist;
			cpoint=i;
		}
	}
	return cpoint;
}
void makecluster(int r, int c, int k, float **b, float** cent, float*** centlist, int* elem) {
	//this fuction makes clusters based on the centroids stored in cent
	int i,j,pt,d;
	for(i=0;i<k;i++) {
		elem[i] = -1; // should be -1
	}
	for(i=0;i<r;i++) {
		pt = findnrstcent(i,c,k,b,cent);
		++elem[pt];
		for(j=0;j<c;j++) {
			centlist[pt][elem[pt]][j] = b[i][j];
		}
	}
	return;
}
float costfn(int r, int c, int k, int* elem, float** cent, float***centlist) {
	//return the costvalue
	int i,j,l,m;
	float dif,cost=0;
	for(i=0;i<k;i++) {
		for(j=0;j<=elem[i];j++) {
			for(l=0;l<c;l++) {
				dif = cent[i][l] - centlist[i][j][l];
				if(dif>=0)
					cost += dif;
				else
					cost -= dif;	
			}
		}
	}
	return cost;
}
bool checkfn(int r, int c, int k, float** cent, float*** centlist, int* elem, int curr, bool* check) {
	//this function return false if 
	int i,j;
	for(i=0;i<=elem[curr];i++) {
		if(check[i]==false) {
			check[i]=true;
			return false;
		}
	}
	return true;
}
float findmincost(int r, int c, int k, float **b, float **cent, float **final, float ***centlist, int *elem, float precost) {
	
	int i , j , i1, j1 ,d1 = 0 ,d2 = 0, as1,as2;
	float **new, postcost;
	makecluster(r,c,k,b,cent,centlist,elem);
	for(i=0;i<k;i++) printf("%d		", elem[i]+1); printf("\n");//elem[i] number of clusters and the centroid itself
	for(i=0;i<k;i++) {
		new = cent;
		bool *check=(bool *)malloc((elem[i]+1)*sizeof(bool));
		memset(check,false,sizeof(check));
		for(j=0;j<=elem[i];j++) {
			if(!checkfn(r,c,k,new,centlist,elem,i,check)) {
				//printf("%d	%d	",i,j);
				for(j1=0;j1<c;j1++) {
					new[i][j1] = centlist[i][j][j1];
				}
				postcost = costfn(r,c,k,elem,new,centlist);
				//printf("%f\n", postcost);
				if(precost>postcost) {
					precost = postcost;
					final = new;
				}
			}
		}
	}
	cent = final;
	return precost;
	
}
float continueloop(int r, int c, int k, float** b, float**cent, float**final, float*** centlist, int *elem, float precost) {
	//this function continues the loop for a single test
	float costvalue; precost = 2e15;
	int iteration=0;
	while(iteration<=50) {
		iteration++;
		costvalue = findmincost(r,c,k,b,cent,final,centlist,elem,precost);
		printf("cost value in iteration %d is %f\n",iteration, costvalue);
		if(costvalue>=precost)
			break;
		precost = costvalue;
		
	}
	return precost;
}
int main(int argc, char *argv[]) {
	int f = atoi(argv[3]);
	long int k = atoi(argv[2]);
	if(argc!=4 || k==0 || f==0) {
		printf("format:\n./a.out <filename> <no. of clusters> <no. of test>\n\n");
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
    if(fr==NULL) {
        printf("File does not exists\n");
        exit(0);
    }
    do {
        ++len;
        st[len]=fgetc(fr);
    }while(st[len+1]!=EOF);
    fclose(fr);
    float **arr;
    arr = (float **)malloc(100000*sizeof(float *));
    for(i=0;i<100000;i++) {
    	arr[i] = (float *)malloc(100*sizeof(float));
    }
    for(i=0;i<len;i++) {
        if((st[i]>=45 && st[i]<=122)) {
            g=-1;
            while((st[i]>=45 && st[i]<=122)) {
                s2[++g]= st[i];
                i++;
            }
            s2[++g]='\0';
            arr[p][++q]=atof(s2);
            i-=1;
        }
        if(st[i]=='\n') {
	    	w=q;	
            ++p;
            q=-1;
        }
    }
    
    //collected data from file as arr[p][w] ; 

    //scaling of data;

    float *m,*M;
    m = (float*)malloc(p*sizeof(float));
    M = (float*)malloc(p*sizeof(float));
    float **b;
    b = (float **)malloc(p*sizeof(float *));
    for(i=0;i<p;i++) {
    	b[i] = (float *)malloc(w*sizeof(float));
    }
    for(j=0;j<w;j++) {
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
	for(i=0;i<p;i++) {
		for(j=0;j<w;j++) {
			b[i][j]=scale(arr[i][j],M[j],m[j]);//scaling of data in matrix b;
		}
	}
	//Data scaled in the matrix b;
	float **cent, **cent1;
	cent = (float **)malloc(k*sizeof(float *));
	for(i=0;i<k;i++) {
		cent[i] = (float *)malloc(w*sizeof(float));
	}
	int **elemlist = (int **)malloc(f*sizeof(int *));
	for(i=0;i<f;i++) elemlist[i]=(int *)malloc(k*sizeof(int));
	int m1,random,m2, iteration,*elem;
	elem = (int *)malloc(k*sizeof(int));
	float *cost,***centlist, min =1000000, **final,precost;
	cost = (float *)malloc(f*sizeof(float));
	centlist = (float***)malloc(k*sizeof(float**));
	for(i=0;i<k;i++) {
		centlist[i] = (float**)malloc(p*sizeof(float*));
		for(j=0;j<p;j++)
			centlist[i][j] = (float*)malloc(w*sizeof(float));
	}

	final = (float **)malloc(k*sizeof(float *));
	for(i=0;i<k;i++) final[i] = (float *)malloc(w*sizeof(float));
	
	for(m1=0;m1<f;m1++) {
		printf("Generated random numbers:-  ");
		precost = 2e15;
		for(i=0;i<k;i++) {
			random = rand()%p;
			printf("%d ", random);
			for(j=0;j<w;j++) {
				cent[i][j] = b[random][j];
			}
		}
		printf("\nTest %d\n", m1+1);
		final = cent;
		cost[m1] = continueloop(p,w,k,b,cent,final,centlist,elem,precost);
		for(i=0;i<k;i++) elemlist[m1][i]=elem[i];



	}
	precost = cost[0];
	m1=0;
	for(i=0;i<f;i++) if(precost>cost[i]) precost = cost[i], m1=i;
	printf("Lowest cost value found in test %d and cost value is %f\n",m1+1, precost);
	printf("Numbers of elements in each cluster \n");
	for(i=0;i<k;i++) printf("%d  ",elemlist[m2][i]+1); printf("\n");
	for(i=0;i<k;i++) {
		for(j=0;j<w;j++) printf("%f  ", final[i][j]);
		printf("\n");
	}
	
	//creating a new file to save cluster numbers along with the original data;
	char sh[20];
	sprintf(sh, "_%ld_kmedoids.data",k);
	strcat(argv[1],sh);
	FILE *fp = fopen(argv[1], "w");
	if(fp==NULL) {
		printf("\nUnable to store the datas\n\n");
	 	exit(0);
	}
	for(i=0;i<p;i++) {
		for(j=0;j<w;j++)
		{
			fprintf(fp, "%2.1f,", arr[i][j]);
		}
		fprintf(fp, "%d\n", checkdistance(b,final,w,k,i));
	}
	fclose(fp);
	return 0;
}
