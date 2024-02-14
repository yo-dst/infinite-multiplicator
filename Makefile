all:
	@gcc main.c -o infinite-multiplication

run:
	@gcc main.c -o infinite-multiplication
	@./infinite-multiplication 12 345