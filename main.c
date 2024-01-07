#include <stdio.h>
#include "eval/tokenizer.h"
#include "eval/eval.h"
#include "eval/analyzer.h"
#include "eval/substitute.h"
#include "ucel.h"
#include "utils.h"

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("No input file.\nUsage: %s <table [.ucel]>\n", argv[0]);
		return -1;
	}

	table *t = parse_table(argv[1]);

	if (!t) {
		printf("Table parsing failed\n");
		return -1;
	}

	eval_table(t);	
	print_table(t);

	return 0;
}
