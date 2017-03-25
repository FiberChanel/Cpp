// Блок потокового интерфейса. Тест 1. Запись в регистр блока
//

#include <stdlib.h>
#include <stdint.h>
#include "stdafx.h"
#include "hard_windows/HardwareInterface.h"

const char help[] = "WriteBpiReg. Test Block Stream Interface. Recording in the register.\n\tUse test_bsi_1.exe ip [bsi_no(hex) [[reg_no(hex)] value(hex)]]";
int bm = 0; // Номер базового модуля

// Преобразование строки в число
int64_t getInt( const char *ps)
{
	int64_t r;
	int cnt = sscanf_s(ps,"%llx",&r);
	if(!cnt)
	{
		printf("Error scanf '%s'",ps);
		printf(help);
		exit(-1);
	}
	return r;
}
void test_1( char *ip, int bsi, int rg, int64_t vl)
{
	void *hndl;
	int result = OpenInterface( ip, &hndl);
	if(result != brSuccess)
	{
		printf("Error open interface for %s. Result = %d\n", ip, result);
		exit(-1);
	}

	result = WriteDMCReg64( hndl, bm, rg, bsi, vl);
	if(result != brSuccess)
	{
		printf("Error WriteDMCReg64 for %s. Result = %d\n", ip, result);
		exit(-1);
	}
}

// Получить величину из аргумента или из командной строки
uint64_t read_int64(int argc, char *argv[], int arg_no, const char *message)
{
	char *ps=0;
	char buf[1024];

	if(argc >= arg_no+1)
		ps = argv[arg_no];
	else
	{
		printf(message);
		ps = gets_s(buf,1024);
	}
	return( getInt(ps));
}

int main(int argc, char* argv[])
{
	int bsi_no; // Номер БПИ
	int reg_no; // Номер регистра БПИ
	int64_t value; // Значение для записи в регистр БПИ

	if(argc <2)
	{
		printf(help);
		return -1;
	}

	// Чтение номера БПИ (или из аргумента, или из консоли)
	bsi_no = (int)read_int64(argc, argv, 2, "Enter BSI number(hex): ");

	// Чтение номера регистра БПИ (или из аргумента, или из консоли)
	reg_no = (int)read_int64(argc, argv, 3, "Enter register BSI number(hex): ");

	// Чтение значения для записи в регистр БПИ (или из аргумента, или из консоли)
	value = read_int64(argc, argv, 4, "Enter value for write(hex): ");
	
	// Тест записи значения в регистр БПИ
	test_1(argv[1], bsi_no, reg_no, value);

	printf("Write bsi reg successful\n");

	return 0;
}

