#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>


bool isValid(int i, int j, int width, int height){
	if (i < 0 || i >= width || j < 0 || j >= height)
		return false;
	else
		return true;
}

void applyFilter(int ** pixels, int ** newpixels, int filter[], int width, int height){
	
	int i, j, m, n, k, total;
		
	for (i=0; i<width; i++){
		for (j=0; j<height; j++){
			k=0;
			total=0;
			for(m=-1; m<2; m++){
				for(n=-1; n<2; n++){
					if (isValid(i+m, j+n, width, height))
						total += pixels[i+m][j+n] * filter[k];
					k++;
				}
			}
			newpixels[i][j] = total;
		}
	}
			
}

void proportion(int **pixels, int width, int height, int maxvalue){
	
	int i, j, min, max;
	
	min = 9999;
	max = -9999;
	
	for(i=0; i<width; i++){
		for(j=0; j<height; j++){
			if (pixels[i][j] > max)
				max = pixels[i][j];
			if (pixels[i][j] < min)
				min = pixels[i][j];
		}
	}
	
	max -= min;
	
	for(i=0; i<width; i++){
		for(j=0; j<height; j++){
			pixels[i][j] -= min;
		}
	}
	
	for(i=0; i<width; i++){
		for(j=0; j<height; j++){
			pixels[i][j] = (pixels[i][j] * maxvalue) / max;
		}
	}
}


void write_pgm(int **pixels, char *line, char *line1, char *line2, int width, int height, int maxrange, FILE *fp, char filename[]){
	
	char *temp;
	int i,j;
	
	temp = (char*) calloc (100, sizeof(char));
	
	fp = fopen(filename, "wb" );
	
	sprintf(line,"%d",height);
	sprintf(temp, "%d", width);
	strcat(line," ");
	strcat(line, temp);
	strcat(line, "\n");
	
	fwrite(line1 , 1 , strlen(line1) , fp );
	fwrite(line2 , 1 , strlen(line2) , fp );
	fputs(line,fp);
	
	sprintf(temp, "%d", maxrange);
	strcat(temp, "\n");
	fputs(temp, fp);

	
	for(i=0; i< width; i++){
		pixels[i][0] = 0;
		pixels[i][height-1] = 0;
	}
	
	for(i=0; i< height; i++){
		pixels[0][i] = 0;
		pixels[width-1][i] = 0;
	}
		
	if(line1[1] == '2'){
		temp = (char *) calloc (1000, sizeof(char));
		
		for (i=0; i<width; i++){
			for (j=0; j<height; j++){
				sprintf(temp, "%d", pixels[i][j]);
				strcat(temp," ");
				fputs(temp, fp);			
			}
		}
	}
	
	else{
		for (i=0; i<width; i++){
			for (j=0; j<height; j++){
				fputc(pixels[i][j], fp);
			}	
		}
	}
	
	fclose(fp);
	
}


int main()
{
	
    FILE *fp;
    unsigned char c;
    unsigned char *buffer;
    char *s, *line, *line1, *line2, *temp, filename[100];
    int width, height, maxrange, counter, counter2, pixel, i, j;
    int ** pixels, **horizontalPixels, **verticalPixels;
    
    int horizontalFilter[9] = {1, 2, 1, 0, 0, 0,-1, -2, -1};
    
    int verticalFilter[9] = {-1, 0, 1,-2, 0, 2, -1, 0, 1};
    
    printf("Enter the name of the file to be edge filtered (in \"filename.pgm\" format): ");
    scanf("%s", filename);
    
    fp = fopen(filename, "rb");
    if (fp == NULL)
        printf("Error: unable to open input image file ");

	line = (char *) calloc (1000, sizeof(char));
	line1 = (char *) calloc (1000, sizeof(char));
	line2 = (char *) calloc (1000, sizeof(char));
	temp = (char *) calloc (1000, sizeof(char));
	buffer = (unsigned char *) calloc (1000, sizeof(unsigned char));
	
	fgets(line1, 1000, fp);
	fgets(line2, 1000, fp);
	fscanf(fp,"%d %d", &height, &width);
    fgets(line, 1000, fp);
    fscanf(fp, "%d", &maxrange);
    fgets(line, 1000, fp);
	counter = 0;
	counter2 = 0;
	
	printf("Pgm format:%sComment:%sResolution:%dx%d\nMaximum pixel value:%d\n", line1, line2, height, width, maxrange);
	
	pixels = (int**) calloc (width, sizeof(int*));
	horizontalPixels = (int**) calloc (width, sizeof(int*));
	verticalPixels = (int**) calloc (width, sizeof(int*));
	
	for (i=0; i< width; i++){
		pixels[i] = (int*) calloc (height, sizeof(int));
		horizontalPixels[i] = (int*) calloc (height, sizeof(int));
		verticalPixels[i] = (int*) calloc (height, sizeof(int));
	}
	
	if (line1[1] == '2'){
		for (i=0; i<width; i++){
			for (j=0; j<height; j++){
				fscanf(fp, "%s", line);
				pixel = atoi(line);
				pixels[i][j] = pixel;
			}
		}
	}
	
	else if (line1[1] == '5'){
		for (i=0; i<width; i++){
			for (j=0; j<height; j++){
				counter += fread(&c, 1, 1, fp);
				pixels[i][j] = (int) c;
			}
		}
		
		printf("%d pixels read.\n", counter);
	}
	
	else{
		printf("File could not read. Make sure it is a pgm file.");
		return 0;
	}
	
	fclose(fp);
	fp = fopen( "result.pgm", "wb" );
	
	sprintf(line,"%d",height);
	sprintf(temp, "%d", width);
	strcat(line," ");
	strcat(line, temp);
	strcat(line, "\n");
	
	fwrite(line1 , 1 , strlen(line1) , fp );
	fwrite(line2 , 1 , strlen(line2) , fp );
	fputs(line,fp);
	
	temp = (char*) calloc (100, sizeof(char));
	sprintf(temp, "%d", maxrange);
	strcat(temp, "\n");
	fputs(temp, fp);
	
	applyFilter(pixels, horizontalPixels, horizontalFilter, width, height);
	applyFilter(pixels, verticalPixels, verticalFilter, width, height);
	
	for (i=0; i< width; i++){
		for(j=0; j<height; j++){
			pixels[i][j] = sqrt(pow(horizontalPixels[i][j], 2) + pow(verticalPixels[i][j], 2));
		}
	}
	
	proportion(pixels, width, height, maxrange);
	write_pgm(pixels, line, line1, line2, width, height, maxrange, fp, "result.pgm");
	
	proportion(horizontalPixels, width, height, maxrange);
	write_pgm(horizontalPixels, line, line1, line2, width, height, maxrange, fp, "horizontal.pgm");
	
	proportion(verticalPixels, width, height, maxrange);
	write_pgm(verticalPixels, line, line1, line2, width, height, maxrange, fp, "vertical.pgm");
	
	printf("New image (result.pgm) created.");
	
    return 0;
}
