#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int str_is_number(const char *str) {
	if (!str || !*str) return 0;
	int i = 0;
	if (*str == '-') {
		if (strlen(str) < 2) return 0;
		i++;
	}
	while (*(str + i)) {
		if (!isdigit(*(str + i))) return 0;
		i++;
	}
	return 1;
}

void append_zero_chars(char **str, int n) {
	if (n <= 0) return;
	int size_str = strlen(*str);
	*str = realloc(*str, size_str + n + 1);
	int i = size_str;
	while (i < size_str + n) {
		(*str)[i] = '0';
		i++;
	}
	(*str)[i] = '\0';
}

int max(int n1, int n2) {
	return n1 > n2 ? n1 : n2;
}

char *multiply_number_by_one_digit(const char *n, int d) {
	if (!n) return NULL;
	if (d == 0) return strdup("0");
	int size_n = strlen(n);
	char *res = malloc(sizeof(char) * (size_n + 1));
	int rest = 0;
	int i = size_n - 1;
	int mult;
	while (i >= 0) {
		mult = (n[i] - 48) * d + rest;
		rest = mult / 10;
		res[i] = (mult % 10) + 48;
		i--;
	}
	res[size_n] = '\0';

	if (rest) {
		res = realloc(res, sizeof(char) * (size_n + 2));
		memcpy(res + 1, res, size_n + 1);
		res[0] = rest + 48;
	}

	return res;
}

char *add_numbers(const char *n1, const char *n2) {
	if (!n1 && !n2) return NULL;
	if (!n1) return strdup(n2);
	if (!n2) return strdup(n1);

	int len_n1 = strlen(n1);
	int len_n2 = strlen(n2);
	int len_max = max(len_n1, len_n2);

	char *res = malloc(sizeof(char) * (len_max + 1));

	int i = 0;
	int rest = 0;
	char c1;
	char c2;
	char add;
	while (i < len_max) {
		c1 = i < len_n1 ? n1[len_n1 - i - 1] : '0';
		c2 = i < len_n2 ? n2[len_n2 - i - 1] : '0';
		add = 48 + (c1 - 48) + (c2 - 48) + rest;
		if (add > '9') {
			rest = 1;
			add -= 10;
		} else {
			rest = 0;
		}
		res[len_max - i - 1] = add;
		i++;
	}
	res[len_max] = '\0';

	if (rest) {
		res = realloc(res, sizeof(char) * (len_max + 2));
		memcpy(res + 1, res, len_max + 1);
		res[0] = '1';
	}

	return res;
}

int main(int ac, char **av) {
	if (ac != 3 || !str_is_number(av[1]) || !str_is_number(av[2])) {
		printf("usage: ./infinite-multiplication <number> <number>");
		return 1;
	}

	char *n1 = strdup(av[1]);
	char *n2 = strdup(av[2]);

	char *res = NULL;
	int res_is_neg = *n1 == '-' ^ *n2 == '-';

	if (*n1 == '-') *n1 = '0';
	if (*n2 == '-') *n2 = '0';

	char *tmp;
	char *mult;
	int size_n1 = strlen(n1);
	int i = size_n1 - 1;
	int zero_offset;

	while (i >= 0) {
		mult = multiply_number_by_one_digit(n2, n1[i] - 48);
		zero_offset = size_n1 - 1 - i;
		append_zero_chars(&mult, zero_offset);
		tmp = res;
		res = add_numbers(res, mult);
		free(tmp);
		free(mult);
		i--;
	}
	
	if (res_is_neg) {
		res = realloc(res, sizeof(char) * (strlen(res) + 2));
		memcpy(res + 1, res, strlen(res) + 1);
		res[0] = '-';
	}
	
	printf("%s x %s = %s\n", av[1], av[2], res);

	free(n1);
	free(n2);
	free(res);

	return 0;
}
