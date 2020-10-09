//program for implementing db index on the clustered files

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

//driver code
int main(int argc,char *argv[])
{
	if(argc!=2)
		printf("Wrong input.\nSample input ::: <./a.out> <filename(<file's name><.txt>_<cluster number>_<clustering method>_<no. of iterations>)>\n");//the entire file name should be given because otherwise it is becoming very subjective 

	else
	{
		FILE *f;
//		printf("%s\n",argv[1]);
		f=fopen(argv[1],"r");
		char s2[1000],st[1000000];

    long int i,j,p,i1,j1,p1,lenFILE=0,c,r=0,x=-1;
    if(f==NULL)
    {
        printf("File does not exist.\n");
        exit(0);
    }
    long int k;
    do
    {
        ++lenFILE;
        st[lenFILE]=fgetc(f);//extracting the elements of the file
    }while(st[lenFILE+1]!=EOF);
    fclose(f);

    float **arr;//matrix storing the final elements
    arr=(float **)malloc(5000*sizeof(float *));
    for(i=0;i<5000;i++)
	    arr[i]=(float *)malloc(50*sizeof(float));
    
    for(i=0;i<lenFILE;i++)
    {
        if((st[i]>=48 && st[i]<=57)||(st[i]>=65 && st[i]<=90)||(st[i]>=97 && st[i]<=122)||(st[i]==45))//accepting the ascii value of the required characters
        {
            k=-1;
            while((st[i]>=48 && st[i]<=57)||(st[i]>=65 && st[i]<=90)||(st[i]>=97 && st[i]<=122)||(st[i]==46)||(st[i]==45))
            {
                s2[++k]= st[i];
                i++;
            }
            s2[++k]='\0';
            arr[r][++x]=atof(s2);
            i-=1;
        }
        if(st[i]=='\n')
        {
	    c=x;	
            ++r;//we have to move to the next element that is next row
            x=-1;
        }
    }

    //data collected as arr[r][c]

/*	for(i=0;i<r;i++)
	{
		for(j=0;j<=c;j++)
			printf("%lf ",arr[i][j]);
		printf("\n");
	}*/
	
  
	char *f1=argv[1];
//	printf("%s\n",f1);
	char *f2="iris.txt";
//	printf("%s\n",f2);

	int flag=0;
	for(i=0;i<strlen(f2);i++)
	{
		if(f1[i]!=f2[i])
			flag++;
	}
	
	if(flag==0 && strlen(f1)==strlen(f2))
	{
		for(int variable=0;variable<3;variable++)
		{
			for(int i=50*variable;i<(50*variable+49);i++)
			{
				arr[i][c]=variable+1;
			}
		}
	}

  
	char *f3="titanic.txt";
	int flag1=0;
	for(i=0;i<strlen(f3);i++)
	{
		if(f1[i]!=f3[i])
			flag1++;
	}
	if(flag1==0 && strlen(f1)==strlen(f3))
	{
		//printf("1\n");
		for(i=0;i<r;i++)
		{
			if(arr[i][c]==-1)
				arr[i][c]=2;
		}
	}

	char *f4="thyroid.txt";
	int flag2=0;
	for(i=0;i<strlen(f4);i++)
	{
		if(f1[i]!=f4[i])
			flag2++;
	}
	if(flag2==0 && strlen(f1)==strlen(f4))
	{
		//printf("1\n");
		for(i=0;i<r;i++)
		{
//			printf("%lf\n",arr[i][c]);
			if(arr[i][c]==-1)
				arr[i][c]=2;
		}
	}

	char *f5="pima.txt";
	int flag3=0;
	for(i=0;i<strlen(f5);i++)
	{
		if(f1[i]!=f5[i])
			flag1++;
	}
	if(flag3==0 && strlen(f1)==strlen(f5))
	{
		//printf("1\n");
		for(i=0;i<r;i++)
		{
			if(arr[i][c]==0)
				arr[i][c]=2;
		}
	}

/*    for(i=0;i<r;i++)
      {
        for(j=0;j<=c;j++)
        {
            printf("%6.2f  ", arr[i][j]);
        }
        printf("\n");
      }
      printf("\n");
      printf("No. of elements = %ld\n",r);
      printf("No. of attributes = %ld\n",c);*/

		//there are c no. of attributes and the cluster number is the (c+1)-th attribute
    		

//		for(i=0;i<r;i++)
//			arr[i][c]=arr[i][c]/2;

		int K=0;//K stores the number of clusters
		int max_K=arr[0][c];//K will be the maximum element of the last column		
		
		for(i=1;i<r-1;i++)
		{	
				if(arr[i][c]>max_K)
					max_K=arr[i][c];				
		}

		K=max_K;
		
//		printf("%d\n",K);

		int KC2=(K*(K-1))/2;//no of possible combinations of inter-cluster distances

//		printf("%d\n",KC2);

		double *inter;
		double *intra;
//inter stores intercluster distances and intra stores intracluster distances

		inter=(double *)malloc(KC2*sizeof(double));
		intra=(double *)malloc(K*sizeof(double));//there can be K intra-cluster distances

		int cnt1=0,cnt2=0,cnt_inter=0,cnt_intra=0;
		double dist1=0,dist2=0;

		//finding inter-cluster distances
		for(i=1;i<=K;i++)
		{
			cnt1=0;
			for(j=i+1;j<=K;j++)
			{
				for(int i1=0;i1<r;i1++)
				{
					if(arr[i1][c]==(float)i)
					{
						
						for(int i2=0;i2<r;i2++)
						{
							if(arr[i2][c]==(float)j)
							{
								for(int j1=0;j1<c;j1++)
								{
									dist1+=(arr[i1][j1]-arr[i2][j1])*(arr[i1][j1]-arr[i2][j1]);
								}
								cnt1++;
							}
							
						}
					}
				}
//				printf("%lf %d\n",dist1,cnt1);
				inter[cnt_inter++]=dist1/(float)cnt1;
			}
		}

		//finding intra-cluster distances
		for(i=1;i<=K;i++)
		{
			cnt2=0;
			for(int i1=0;i1<r;i1++)
			{
				if(arr[i1][c]==(float)i)
				{
					
					for(int i2=0;i2<r;i2++)
					{
						if(arr[i2][c]==(float)i)
						{
							for(int j1=0;j1<c;j1++)
							{
								dist2+=(arr[i1][j1]-arr[i2][j1])*(arr[i1][j1]-arr[i2][j1]);
							}
							cnt2++;
						}
						
					}
				}
			}
//			printf("%lf %d\n",dist2,cnt2);
			intra[cnt_intra++]=dist2/cnt2;
		}

		double max_db=0,db;
		for(i=1;i<=K;i++)
		{
			int c1=0;
			for(j=i+1;j<=K;j++)
			{
//				printf("%lf %lf %lf\n",intra[i-1],intra[j-1],inter[i-1+i*c1]);
				db=(intra[i-1]+intra[j-1])/inter[i-1+i*c1];
				c1++;
				//for 1st iteration under i we need inter[i-1] then we need inter[i] and so on
			}
			if(max_db<db)
				max_db=db;
		}
		
		printf("The DBindex is %lf.\n",max_db/K);
							
				
		
		return 0;					
	}
}		
