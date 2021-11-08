// #define RVINTRIN_EMULATE
#include <rvintrin.h>

#undef VERBOSE_TESTS
#define SINGLE_TEST ""
#include "tests/tests.h"

extern int32_t shtst();

void printc(int c)
{
	volatile char *p = (void*)0x10000000;
	*p = c;
}

void printsn(char *p, int n)
{
	while (*p)
		printc(*(p++)), n--;
	while (n-- > 0)
		printc(' ');
}

void prints(char *p)
{
	while (*p)
		printc(*(p++));
}

void printh(unsigned int v)
{
	prints("0x");
	for (int i = 0; i < 8; i++, v <<= 4) {
		int d = v >> 28;
		if (d < 10)
			printc('0' + d);
		else
			printc('A' + d - 10);
	}
}

int streq(const char *p, const char *q)
{
	while (*p || *q)
		if (*(p++) != *(q++))
			return 0;
	return 1;
}

#ifdef VERBOSE_TESTS

int check(uint32_t *s, uint32_t *r, int n)
{
	int errcnt = 0;
	while (n--) {
		prints("SIG=");
		printh(*s);
		prints(" REF=");
		printh(*r);
		prints(*s == *r ? " OK\n" : " ERROR\n");
		errcnt += *s != *r;
		s++, r++;
	}
	return errcnt;
}

#define RUNTEST(_name) if (!SINGLE_TEST[0] || streq(SINGLE_TEST, #_name)) { \
  prints("--- " #_name " ---\n"); testcode_##_name(); \
  errcnt += check(signature_##_name, reference_##_name, sizeof(reference_##_name)/4); }

#else

int check(uint32_t *s, uint32_t *r, int n)
{
	int errcnt = 0;
	while (n--) errcnt += (*(s++) != *(r++));
	prints(errcnt ? " ERROR\n" : " OK\n");
	return errcnt;
}

#define RUNTEST(_name) if (!SINGLE_TEST[0] || streq(SINGLE_TEST, #_name)) { \
  printsn("" #_name, 17); testcode_##_name(); \
  errcnt += check(signature_##_name, reference_##_name, sizeof(reference_##_name)/4); }

#endif

int main()
{
	int errcnt = 0;
	int tmp;

	prints("Hello World!\n");

	tmp = _rv32_grev(_rv32_clmul(123456789, 12345678), -1);
	printh(tmp);
	prints(tmp == 0x696ECAD1 ? " OK\n" : (errcnt++, " ERROR\n"));

	tmp = _rv32_clmulr(_rv32_grev(123456789, -1), _rv32_grev(12345678, -1));
	printh(tmp);
	prints(tmp == 0x696ECAD1 ? " OK\n" : (errcnt++, " ERROR\n"));

	tmp = _rv_cmix(tmp ^ _rv32_pack(-1, 0), tmp, ~tmp);
	printh(tmp);
	prints(tmp == 0xFFFF0000 ? " OK\n" : (errcnt++, " ERROR\n"));

	tmp = _rv32_rol(_rv32_slo(255 << 8, 8), 12);
	printh(tmp);
	prints(tmp == 0xF00FF00F ? " OK\n" : (errcnt++, " ERROR\n"));

	tmp = shtst();
	printh(tmp);
	prints(tmp == 0x14549256 ? " OK\n" : (errcnt++, " ERROR\n"));

	RUN_ALL_TESTS

	prints(errcnt ? "*** FAIL ***\n" : "*** PASS ***\n");

	return 0;
}
