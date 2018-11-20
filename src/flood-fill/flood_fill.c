#include <stdio.h>
#include <stdlib.h>

#define ROWSIZE		25
#define COLSIZE		50

char g_bitmap[ROWSIZE][COLSIZE];

void load_bitmap(const char *path)
{
	FILE *f;
	char buf[COLSIZE + 2];
	int row, col, i;

	for (row = 0; row < ROWSIZE; ++row)
		for (col = 0; col < COLSIZE; ++col)
			g_bitmap[row][col] = ' ';

	if ((f = fopen(path, "r")) == NULL) {
		fprintf(stderr, "cannot open file!..\n");
		exit(EXIT_FAILURE);
	}

	row = 0;
	for (row = 0; fgets(buf, COLSIZE + 2, f) != NULL; ++row)
		for (i = 0; buf[i] != '\0' && buf[i] != '\n'; ++i)
			g_bitmap[row][i] = buf[i];

	if (ferror(f)) {
		fprintf(stderr, "cannot read!..\n");
		exit(EXIT_FAILURE);
	}

	fclose(f);
}

void store_bitmap(const char *path)
{
	FILE *f;
	int row;

	if ((f = fopen(path, "w")) == NULL) {
		fprintf(stderr, "cannot open file!..\n");
		exit(EXIT_FAILURE);
	}

	for (row = 0; row < ROWSIZE; ++row) {
		if (fwrite(g_bitmap[row], 1, COLSIZE, f) != COLSIZE) {
			fprintf(stderr, "cannot write file!..\n");
			exit(EXIT_FAILURE);
		}
		if (fputc('\n', f) == EOF) {
			fprintf(stderr, "cannot write file!..\n");
			exit(EXIT_FAILURE);
		}
	}
	
	fclose(f);
}

void put_bitmap(void)
{
	int row, col;

	for (row = 0; row < ROWSIZE; ++row) {
		for (col = 0; col < COLSIZE; ++col)
			putchar(g_bitmap[row][col]);
		putchar('\n');
	}
}

void flood_fill(int row, int col, char ch)
{
	if (g_bitmap[row][col] == ch || g_bitmap[row][col] == '#')
            return;

	g_bitmap[row][col] = ch;

	flood_fill(row + 1, col, ch);
	flood_fill(row, col + 1, ch);
	flood_fill(row - 1, col, ch);
	flood_fill(row, col - 1, ch);
}

int main(void)
{
	load_bitmap("Pattern.txt");
	//put_bitmap();
	flood_fill(4, 4, '.');
	put_bitmap();
	store_bitmap("PatternFilled.txt");

	return 0;
}
	