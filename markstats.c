
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"
#include "markstats.h"

void merge_tree(TNODE **rootp1, TNODE **rootp2) {
// your implementation
// use recursive algorithm to traverse tree rootp2,
// get data of each node and insert into rootp1

	if ((*rootp2) == NULL) {
		return;
	}

	insert(rootp1, (*rootp2)->data.name, (*rootp2)->data.score);

	if ((*rootp2)->left) {
		merge_tree(rootp1, &(*rootp2)->left);
	}

	if ((*rootp2)->right) {
		merge_tree(rootp1, &(*rootp2)->right);
	}





}

void merge_data(MARKS *ds1, MARKS *ds2) {
// your implementation
// call the merge_tree function to merge ds2->bst into da1->bst
// update the stats of the merged data set using the stats of ds1 and ds2.

	merge_tree(&ds1->bst, &ds2->bst);
	int count = ds1->count + ds2->count;
	float mean = (ds1->mean * ds1->count + ds2->mean * ds2->count) / count;
	float stddev = sqrt(
			(((ds1->stddev * ds1->stddev * ds1->count)
					+ (ds1->mean * ds1->mean * ds1->count)
					+ (ds2->stddev * ds2->stddev * ds2->count)
					+ (ds2->mean * ds2->mean * ds2->count)) / count)
					- (mean * mean));

	ds1->count = count;
	ds1->mean = mean;
	ds1->stddev = stddev;
}

// the following are adapted from markstats.c of a7q2

#include "markstats.h"

void add_data(MARKS *dsp, char *name, float score) {
	if (search(dsp->bst, name) == NULL) {
		insert(&dsp->bst, name, score);
		int count = dsp->count;
		float mean = dsp->mean;
		float stddev = dsp->stddev;
		dsp->count = count + 1;
		dsp->mean = (mean * count + score) / (count + 1);
		dsp->stddev = sqrt(
				score * score / (count + 1.0)
						+ (stddev * stddev + mean * mean)
								* (count / (count + 1.0))
						- dsp->mean * dsp->mean);
	} else {
		printf("record exit");
	}
}

void remove_data(MARKS *dsp, char *name) {
	TNODE *np = NULL;
	if ((np = search(dsp->bst, name)) != NULL) {
		float score = np->data.score;
		delete(&dsp->bst, name);

		//recompute statistics summary by removing an old one
		int count = dsp->count;
		float mean = dsp->mean;
		float stddev = dsp->stddev;
		dsp->count = count - 1;
		dsp->mean = (mean * count - score) / (count - 1.0);
		dsp->stddev = sqrt(
				(stddev * stddev + mean * mean) * (count / (count - 1.0))
						- score * score / (count - 1.0)
						- dsp->mean * dsp->mean);
	} else {
		printf("record not exit");
	}
}

void display_marks(MARKS *dsp) {
	printf("grade report\n");
	display_inorder(dsp->bst);
	printf("\nstatistics summary\n");
	printf("%-15s%d\n", "count", dsp->count);
	printf("%-15s%3.1f\n", "mean", dsp->mean);
	printf("%-15s%3.1f\n", "stddev", dsp->stddev);
}

void clean_marks(MARKS *dsp) {
	clean_tree(&dsp->bst);
	dsp->count = 0;
	dsp->mean = 0;
	dsp->stddev = 0;
}

void import_data(MARKS *ds, char *filename) {
	char line[40], name[20];
	FILE *fp = fopen(filename, "r");
	char *result = NULL;
	float score = 0;
	char delimiters[] = ",\n";

	if (fp == NULL) {
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}

	while (fgets(line, sizeof(line), fp) != NULL) {
		result = strtok(line, delimiters);
		if (result) {
			strcpy(name, result);
			result = strtok(NULL, delimiters);
			score = atof(result);
			add_data(ds, name, score);
		}
	}
	fclose(fp);
}

void print_to_file_inorder(TNODE *root, FILE *fp) {
	if (root) {
		if (root->left)
			print_to_file_inorder(root->left, fp);
		fprintf(fp, "%-15s%3.1f%4c\n", root->data.name, root->data.score,
				letter_grade(root->data.score));
		if (root->right)
			print_to_file_inorder(root->right, fp);
	}
}

void report_data(MARKS *dsp, char *filename) {
	FILE *fp = fopen(filename, "w");
	fprintf(fp, "grade report\n");
	print_to_file_inorder(dsp->bst, fp);
	fprintf(fp, "\nstatistics summary\n");
	fprintf(fp, "%-15s%d\n", "count", dsp->count);
	fprintf(fp, "%-15s%3.1f\n", "mean", dsp->mean);
	fprintf(fp, "%-15s%3.1f\n", "stddev", dsp->stddev);
	fclose(fp);
}

char letter_grade(float s) {
	char r = 'F';
	if (s >= 85)
		r = 'A';
	else if (s >= 70)
		r = 'B';
	else if (s >= 60)
		r = 'C';
	else if (s >= 50)
		r = 'D';
	else
		r = 'F';
	return r;
}
