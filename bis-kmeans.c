#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
/*
//initializing pointers of main function

void initialize(int r, int c, int k, float*** list, float** cent, int* element, float** b, float** tcent, float* M, float* m) {
    int i,j,i1;

    //initializing b
    b = (float **)malloc(r*sizeof(float *));
    for(i=0;i<r;i++) {
    	b[i] = (float *)malloc(c*sizeof(float));
    }
    
    //initializing M,m
    m = (float*)malloc(r*sizeof(float));
    M = (float*)malloc(r*sizeof(float));

    //initializing cent
    cent=(float **)malloc(k*sizeof(float *));
	for(i=0;i<k;i++) {
		cent[i] = (float *)malloc(c*sizeof(float));
	}

    //initializing element
    element = (int *)malloc((k+1)*sizeof(int));
    for(i=0;i<=k;i++) {
        element[i]=0;
    }

    //initializing list
    list=(float***)malloc((k+1)*sizeof(float**));
    for(i=0;i<=k;i++) {
        list[i]=(float**)malloc(r*sizeof(float*));
        for(j=0;j<r;j++) {
            list[i][j]=(float*)malloc(c*sizeof(float));
        }
    }

    //initializing tcent
    tcent=(float**)malloc(2*sizeof(float*));
    for(i=0;i<2;i++) {
        tcent[i]=(float*)malloc(c*sizeof(float));
    }
}
*/

int match(float* bc, int k, int c, float*** list, int* elem) {
    int r=0,i,i1,j,j1;
    for(i=0;i<k;i++) {
        for(j=0;j<elem[i];j++) {
            r=0;
            for(i1=0;i1<c;i1++) {
                if(bc[i1]!=list[i][j][i1]) {
                    r=1;
                    break;
                }
            }
            if(r==0) {
                break;
            }
        }
        if(r==0) {
            break;
        }
    }
    return (i+1);
}

float checkd(float* a, float* b, int c) {
    float sse=0;
    for(int i=0;i<c;i++) {
        sse+=(a[i]-b[i])*(a[i]-b[i]);
    }
    return sse;
}

float scale(float x, float M, float m) {
	return ((x-m)/(M-m));
}

float finddist(float x, float y) {
    return (x-y)*(x-y);
}

float checklow(int msv, int size, float*** list, int c) {
    int i;
    float ans=1;
    for(i=0;i<size;i++) {
        if(ans>list[msv][i][c]) {
            ans=list[msv][i][c];
        }
    }
    return ans==1?0:ans;
}

float checkhigh(int msv, int size, float*** list, int c) {
    int i,ans=0;
    for(i=0;i<size;i++) {
        if(ans<list[msv][i][c]) {
            ans=list[msv][i][c];
        }
    }
    return ans==0?1:ans;
}

int makecluster(int r, int c, int k, float*** list, int* element, float** tcen, float** cent, int count, int msv, int iteration) {
    printf("%d\n", iteration);
    float dist1=0,dist0=0;
    int cn0=0,cn1=0,i,j,i1;
    element[count]=0;
    element[count+1]=0;
    for(i=0;i<element[msv];i++) {
        dist0=0;
        dist1=0;
        for(j=0;j<c;j++) {
            dist0+=(list[msv][i][j]-tcen[0][j])*(list[msv][i][j]-tcen[0][j]);
            dist1+=(list[msv][i][j]-tcen[1][j])*(list[msv][i][j]-tcen[1][j]);
        }
        if(dist1>dist0) {
            for(j=0;j<c;j++) {
                list[count][element[count]][j]=list[msv][i][j];
            }
            element[count]++;
        }
        else {
            for(j=0;j<c;j++) {
                list[count+1][element[count+1]][j]=list[msv][i][j];
            }
            element[count+1]++;
        }
    }
    printf("%d %d\n", element[count], element[count+1]);
    float** tcent=(float**)malloc(2*sizeof(float*));
    for(i=0;i<2;i++) {
        tcent[i]=(float *)malloc(c*sizeof(float));
    }
    float sum;
    for(i=0;i<2;i++) {
        for(j=0;j<c;j++) {
            sum=0;
            //if(element[count+i]!=0) {
                for(i1=0;i1<element[count+i];i1++) {
                    sum+=list[count+i][i1][j];
                }
            //}
            if(element[count+i]==0) {
                tcent[i][j]=0;
            }
            else {
                tcent[i][j]=sum/(float)element[count+i];
            }
            
        }
    }

    float sse=0;
    for(i=0;i<2;i++) {
        for(i1=0;i1<element[msv];i1++) {
            for(j=0;j<c;j++) {
                sse+=(tcent[i][j]-list[count+i][i1][j])*(tcent[i][j]-list[count+i][i1][j]);
            }
        }
    }

    printf("SSE value is %f\n", sse);



    int cp=1;
    for(i=0;i<2;i++) {
        for(j=0;j<c;j++) {
            if(tcent[i][j]!=tcen[i][j]) {
                cp=0;
                break;
            }
        }
        if(cp==0) {
            break;
        }
    }
    if(cp==0 && iteration<50) {
        return makecluster(r,c,k,list,element,tcent,cent,count,msv,iteration+1);
    }
    else {
        for(i=msv+1;i<=count+1;i++) {
            for(i1=0;i1<element[i];i1++) {
                for(j=0;j<c;j++) {
                    list[i-1][i1][j]=list[i][i1][j];
                }
            }
        }
        for(i=msv+1;i<count+2;i++) {
            element[i-1]=element[i];
        }
        for(i=msv+1;i<count;i++) {
            for(j=0;j<c;j++) {
                cent[i-1][j]=cent[i][j];
            }
        }
        for(j=0;j<c;j++) {
            cent[count][j]=tcent[0][j];
            cent[count+1][j]=tcent[1][j];
        }
        float sse[count+1], tt=0;
        for(i=0;i<count+1;i++) {
            sse[i]=0;
            for(i1=0;i1<element[i];i1++) {
                for(j=0;j<c;j++) {
                    sse[i]+=finddist(cent[i][j],list[i][i1][j]);
                }
            }
            tt+=sse[i];
            printf("%f\n", sse[i]);
        }
        printf("\n    Final SSE value is %f\n\n", tt);
        int ans=0;
        float val=0;
        for(i=0;i<count+1;i++) {
            tt+=sse[i];
            if(val<sse[i]) {
                ans=i;
                val=sse[i];
            }
        }
        return ans;
    }
}

int main(int argc, char *argv[]) {
	int f = atoi(argv[3]);
	int k = atoi(argv[2]);
	if(argc!=4 || k==0) {
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
    int i,j=-1,len=0,w,p=0,q=-1,x1,x2,g;
    if(fr==NULL) {
        printf("File does not exists\n");
        exit(0);
    }
    do {
        ++len;
        st[len]=fgetc(fr);
    }while(st[len+1]!=EOF);
    fclose(fr);

    //intitializing arr
    float **arr;
    arr = (float **)malloc(100000*sizeof(float *));
    for(i=0;i<100000;i++) {
    	arr[i] = (float *)malloc(100*sizeof(float));
    }

    for(i=0;i<len;i++) {
        if((st[i]>=45 && st[i]<=122)) {
            g=-1;
            x2=i;
            while((st[i]>=45 && st[i]<=122)) {
                s2[++g]= st[i];
                i++;
            }
            s2[++g]='\0';
            arr[p][++q]=atof(s2);
            if(st[x2]==45) arr[p][q]=(-1)*arr[p][q];
            i-=1;
        }
        if(st[i]=='\n') {
	    	w=q;	
            ++p;
            q=-1;
        }
    }
    int r=p,c=w,*element;
    float ***list, **cent, **b, **tcent, *M, *m;
    //initialize(r,c,k,list,cent,element,b,tcent,M,m);
    b = (float **)malloc(r*sizeof(float *));
    for(i=0;i<r;i++) {
    	b[i] = (float *)malloc(c*sizeof(float));
    }
    
    //initializing M,m
    m = (float*)malloc(r*sizeof(float));
    M = (float*)malloc(r*sizeof(float));

    //initializing cent
    cent=(float **)malloc((k+1)*sizeof(float *));
	for(i=0;i<=k;i++) {
		cent[i] = (float *)malloc(c*sizeof(float));
	}

    //initializing element
    element = (int *)malloc((k+1)*sizeof(int));
    for(i=0;i<=k;i++) {
        element[i]=0;
    }

    //initializing list
    list=(float***)malloc((k+1)*sizeof(float**));
    for(i=0;i<=k;i++) {
        list[i]=(float**)malloc((r+1)*sizeof(float*));
        for(j=0;j<=r;j++) {
            list[i][j]=(float*)malloc(c*sizeof(float));
        }
    }

    //initializing tcent
    tcent=(float**)malloc(2*sizeof(float*));
    for(i=0;i<2;i++) {
        tcent[i]=(float*)malloc(c*sizeof(float));
    }
    //collected data from file as arr[p][w];
    //scaling of data;

    for(j=0;j<w;j++) {
		M[j]=arr[0][j];
		m[j]=arr[0][j];
		for(i=0;i<p;i++) {
			if(arr[i][j]<m[j])
				m[j]=arr[i][j];
			else if(arr[i][j]>M[j])
				M[j]=arr[i][j];
		}
	}
	for(i=0;i<p;i++) {
		for(j=0;j<w;j++) {
			b[i][j]=scale(arr[i][j],M[j],m[j]);
		}
	}
	//Data scaled in the matrix b;
    
    int random=(rand()%2900)+100;
    for(j=0;j<w;j++) {
        float val=(float)(rand()%random);
        cent[0][j]=(float)(val/random);
    }
    int count=1;
    int msv=0;
    for(i=0;i<p;i++) {
        for(j=0;j<w;j++) {
            list[0][i][j]=b[i][j];
        }
    }
    element[msv]=r;
    int high,low;
    float lw,hg,val1,val2;
    int iteration=0;
    while(count<k) {
        for(j=0;j<w;j++) {
            iteration=0;
            lw=checklow(msv,element[msv],list,j);
            hg=checkhigh(msv,element[msv],list,j);
            low=100*lw;
            high=100*hg;
            val1=(float)((rand()%(high-low))+low)/100;
            val2=(float)((rand()%(high-low))+low)/100;
            tcent[0][j]=val1;
            tcent[1][j]=val2;
        }
        msv=makecluster(p,w,k,list,element,tcent,cent,count,msv,iteration);
        count++;
        printf("\n");
    }
    printf("\n");
    printf("Final Centroids are:-\n\n");
    for(i=0;i<k;i++) {
        for(j=0;j<c;j++) {
            printf("%f ", cent[i][j]);
        }
        printf("\n");
    }
    printf("Cluster distributions are as follows:-\n\n");
    for(i=0;i<k;i++) {
        printf("%d ", element[i]);
    }

    float sse=0;
    for(i=0;i<k;i++) {
        for(j=0;j<element[i];j++) {
            sse+=checkd(cent[i],list[i][j],c);
        }
    }
    printf("\nFinal sse value is %f",sse);

    printf("\n");



    char sh[20];
	sprintf(sh, "_%d_bis-kmeans.data",k);
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
		fprintf(fp, "%d\n", match(b[i],k,c,list,element));
	}
	fclose(fp);










    return 0;
}