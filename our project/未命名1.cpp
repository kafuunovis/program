#include<stdio.h>
int main()
{
	int a,b;
	unsigned char map[1024][1024];
	FILE *fp= fopen("map.bin","w+");
	scanf("%d %d",&a,&b);
	fprintf(fp,"%d %d ",a,b);
	for (int j=0; j<b;j++)
	{
		for (int i=0; i<a; i++)
		{
			scanf("%d",&map[i][j]);
			fprintf(fp,"%c",map[i][j]);
		}
	}
	fclose(fp);
}
// 1 1 1 1 1 1 1 1
