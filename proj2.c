#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

double taylor_log(double x, unsigned int n){
	double answer = 1.0, numerator = 1.0, fraction;
	unsigned int i;
	//If X is smaller than 0, end the program
	if(x < 0){
		printf("The argument can't be of a negative type.\n");
		exit(0);
	}
	//algorithm for 0 < x < 1
	if(x > 0 && x <= 1){
		for(i = 1; i < n; i++){
			numerator = numerator * (1.0 - x);
			fraction = numerator / i;
			answer = answer - fraction;
		}
	}
	//algorithm for x > 1
	else{
		for(i = 1; i <= n; i++){
			numerator = numerator * (x - 1.0) / x;
			fraction = numerator / i;
			answer = answer + fraction;
		}
	}
	return answer;
}	
	
double fa(double n, double x){
	n--;
	return n ? n * n * x * x :2.0 * x;
}
double fb(double n){
	return (2.0 * n) - 1.0;
}
double cfrac_log(double x, unsigned int n){
	x = (x - 1.0)/(x + 1.0);
	double cf = 0.0;
	while(n >= 1){
		cf = fa(n, x) / (fb(n) - cf);
		n--;
	}
	return cf;
}

double taylor_pow(double x, double y, unsigned int n){
	double pow_x = 1.0, pow_ln_x = 1.0, denominator = 1.0, fraction, answer = 1.0;
	unsigned int i;
	double log_x = taylor_log(x, n);
	for(i = 1; i < n; i++){
		pow_x = pow_x * y;
		pow_ln_x = pow_ln_x * log_x;
		denominator = denominator * i;
		fraction = pow_x * pow_ln_x / denominator;
		answer = answer + fraction;
	}
	return answer;
}

double taylorcf_pow(double x, double y, unsigned int n){
	double pow_x = 1.0, pow_ln_x = 1.0, denominator = 1.0, fraction, answer = 1.0;
	unsigned int i;
	double log_x = cfrac_log(x, n);
	for(i = 1; i < n; i++){
		pow_x = pow_x * y;
		pow_ln_x = pow_ln_x * log_x;
		denominator = denominator * i;
		fraction = pow_x * pow_ln_x / denominator;
		answer = answer + fraction;
	}
	return answer;
}

int main(int argc, char *argv[]){
	int strcmp(const char *s1, const char *s2);
	if(strcmp(argv[1], "--log")==0){
		if(argc != 4){
			printf("Wrong input of arguments. There are too many or too few arguments.\n");
			return 1;
		}
		else{
			char *p;
			double x = strtod(argv[2], &p);
			double n = strtod(argv[3], &p);
			if(x && n > 0){
				printf("\tlog(%.4g) = %.12g\n", x, log(x));
				printf("  cfrac_log(%.4g) = %.12g\n", x, cfrac_log(x, n));
				printf(" taylor_log(%.4g) = %.12g\n", x, taylor_log(x, n));
			}
			else{
				printf("Wrong input of x or n.\n");
				exit(0);
			}
		}
	}
	else if(strcmp(argv[1], "--pow")==0){
		if(argc != 5){
			printf("Wrong input of arguments. There are too many or too few arguments.\n");
			return 1;
		}
		else{
			char *p;
			double x = strtod(argv[2], &p);
			double y = strtod(argv[3], &p);
			double n = strtod(argv[4], &p);
			if(x && y && n >= 0){
				printf("\t pow(%.2g, %.2g) = %.12g\n", x, y, pow(x, y));
				printf("  taylor_pow(%.2g, %.2g) = %.12g\n", x, y, taylor_pow(x, y, n));
				printf("taylorcf_pow(%.2g, %.2g) = %.12g\n", x, y, taylorcf_pow(x, y, n));
			}
			else{
				printf("Wrong input of x, y or n.\n");
				exit(0);
			}
		}
	}	
	else{
		printf("Wrong input of arguments\n");
		return 1;
	}
	return 0;
}
