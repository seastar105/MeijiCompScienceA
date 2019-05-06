#include<stdio.h>
#include<string.h>

int main() {
	char str[256] = "0,0,0";
	char *ptr = strtok(str,",");
	int col=0,row=0;
	int i,j;
	FILE *a = fopen("filter2.txt","r");
	fgets(str,256,a);
	while(ptr!=NULL) {
		col++;
		ptr = strtok(NULL,",");
	}
	row = 1;
	while(fgets(str,256,a)) row++;
	printf("%d\n",col);
	printf("%d\n",row);
	double filter[3][3];
	rewind(a);
    while(fgets(str,256,a)) {
        i = 0;
        ptr = strtok(str,",");
        j = 0;
        while(ptr != NULL) {
            sscanf(ptr,"%lf,",&filter[i][j]);
			printf("%lf, ",filter[i][j++]);
            ptr = strtok(NULL,",");
        }
        i++;
		printf("\n");
    }

	return 0;
}
