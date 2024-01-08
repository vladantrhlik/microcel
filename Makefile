
adtčka := stack.c list.c
evaluate := analyzer.c eval.c substitute.c tokenizer.c 
microcel := ucel.c utils.c

main: main.c $(microcel:%=ucel/%) $(adtčka:%=adt/%) $(evaluate:%=eval/%) 
	gcc $^ -o $@ -lm -Wall
