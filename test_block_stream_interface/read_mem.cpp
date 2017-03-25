// Блок потокового интерфейса. Тест 4. Чтение из памяти
//

#include <stdlib.h>
#include <stdint.h>
#include "stdafx.h"
#include "hard_windows/HardwareInterface.h"

const char help[] = "ReadBpiMem. Test Block Stream Interface. Reading from memory BSI.\n\tUse test_bsi_1.exe ip [bsi_no(hex) [segment_no(hex) [address(hex) [step(hex) [count(hex)]]]]]";
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

// Тест записи в память
void test_4( char *ip, int bsi_no, int segment, int address, int step, int count)
{
	void *hndl;

	int result = OpenInterface( ip, &hndl);
	if(result != brSuccess)
	{
		printf("Error open interface for %s. Result = %d\n", ip, result);
		exit(-1);
	}

	uint64_t *data;
	SpecBuffAlloc(hndl, count, (void**)&data);

	result = ReadMem64Fast( hndl, bm, segment, bsi_no, address, step, count, data);
	if(result != brSuccess)
	{
		printf("Error ReadMem64Fast for %s. Result = %d\n", ip, result);
		SpecBuffFree(hndl, count, data);
		exit(-1);
	}
	printf("Read %d values from memory:\n",count);
	int cnt_displ = count;
	if(cnt_displ > 16) cnt_displ = 16;

	for( int i = 0; i< cnt_displ; i++)
		printf("\t%2d: %08llX\n", i+1, data[i]);
	FILE *f = fopen("out.txt","wt");
	if(!f)
	{
		printf("Error open 'out.txt' for write\n");
	}
	else
	{
		for( int i = 0; i< count; i++)
			fprintf(f,"%08llX\n",data[i]);
		fclose(f);
	}
	SpecBuffFree(hndl, count, data);
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
//4) Чтение из памяти. Номер памяти (12 бит), начальный адрес, приращение, количество данных - через ком. строку. Вывести на экран можно первые 16 данных, например.  Данные - 64 бит
int main(int argc, char* argv[])
{
	int bsi_no; // Номер БПИ
	int segment_no; // номер сегмента памяти
	int address; // Начальный адрес
	int step; // приращение
	int count; // Количество данных

	if(argc <2)
	{
		printf(help);
		return -1;
	}

	// Чтение номера БПИ (или из аргумента, или из консоли)
	bsi_no = (int)read_int64(argc, argv, 2, "Enter BSI number(hex): ");

	// Чтение номера БПИ (или из аргумента, или из консоли)
	segment_no = (int)read_int64(argc, argv, 3, "Enter segment memory number(hex): ");

	// Чтение начального адреса
	address = (int)read_int64( argc, argv, 4, "Enter begining address(hex): ");

	// Чтение приращения
	step = (int)read_int64( argc, argv, 5, "Enter step(hex): ");

	// Чтение количества данных для записи
	count = (int)read_int64( argc, argv, 6, "Enter count(hex): ");

	// Тест записи значения в регистр БПИ
	test_4(argv[1], bsi_no, segment_no, address, step, count);
	printf("Read from the BSI memory successful\n");

	return 0;
}

