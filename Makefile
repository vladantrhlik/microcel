
adtčka := stack.c list.c
evaluate := analyzer.c eval.c substitute.c tokenizer.c 

main: main.c ucel.c utils.c $(adtčka:%=adt/%) $(evaluate:%=eval/%) 
	gcc $^ -o $@ -Wall
