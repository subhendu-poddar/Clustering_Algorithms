#include <bits/stdc++.h>
using namespace std;

float distance(float* a, float* b, int c) {
    float x=0;
    for(int i=0;i<c;i++) {
        x+=(a[i]-b[i])*(a[i]-b[i]);
    }
    return x;
}

int main(int argc, char* argv[]) {
	//collect data from file given (filename) by the user;
    FILE *fr;
    char s[1000000],s2[1000000];
    strcpy(s,argv[1]);
    strcat(s, ".data");
    fr=fopen(s, "r");
    char st[1000000];
    char ch;
    int i,j=-1,len=0,w,p=0,q=-1,x1,x2,g;
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

    float **data;
    data = (float **)malloc(1000000*sizeof(float *));
    for(i=0;i<1000000;i++) {
    	data[i] = (float *)malloc(100*sizeof(float));
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
            data[p][++q]=atof(s2);
            i-=1;
        }
        if(st[i]=='\n')
        {
	    	w=q;
            ++p;
            q=-1;
        }
    }
    int r=p,c=w,i1,j1,ind,k=0;
    for(i=0;i<r;i++) {
        if(data[i][c]>k) {
            k=data[i][c];
        }
    }



    float*** arr=new float** [50];
    for(i=0;i<50;i++) {
        arr[i]=new float* [r];
        for(j=0;j<r;j++) {
            arr[i][j]=new float [c];
        }
    }
    int* elem=new int [k];
    memset(elem,0,sizeof(elem));

    for(i=0;i<r;i++) {
        ind=data[i][c];
        for(j=0;j<c;j++) {
            arr[ind-1][elem[ind-1]][j]=data[i][j];
        }
        elem[ind-1]++;
    }






    float temp,intra=0,inter=100000000;

    for(i=0;i<k;i++) {
        for(j=0;j<elem[i];j++) {
            temp=0;
            for(j1=0;j1<elem[i];j1++) {
                if(j1==j) {
                    continue;
                }
                temp+=distance(arr[i][j],arr[i][j1],c);
            }
            temp/=elem[i];
            intra=max(intra,temp);
        }
    }

    for(i=0;i<k;i++) {
        for(i1=0;i1<elem[i];i1++) {
            for(j=0;j<k;j++) {
                if(i==j) {
                    continue;
                }
                temp=0;
                for(j1=0;j1<elem[j];j1++) {
                    temp+=distance(arr[i][i1],arr[j][j1],c);
                }
                temp/=elem[j];
                inter=min(inter,temp);
            }
        }
    }

    cout << inter/intra << endl;

    return 0;

}