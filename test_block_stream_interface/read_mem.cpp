// ���� ���������� ����������. ���� 4. ������ �� ������
//

#include <stdlib.h>
#include <stdint.h>
#include "stdafx.h"
#include "hard_windows/HardwareInterface.h"

const char help[] = "ReadBpiMem. Test Block Stream Interface. Reading from memory BSI.\n\tUse test_bsi_1.exe ip [bsi_no(hex) [segment_no(hex) [address(hex) [step(hex) [count(hex)]]]]]";
int bm = 0; // ����� �������� ������

// �������������� ������ � �����
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

// ���� ������ � ������
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

// �������� �������� �� ��������� ��� �� ��������� ������
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
//4) ������ �� ������. ����� ������ (12 ���), ��������� �����, ����������, ���������� ������ - ����� ���. ������. ������� �� ����� ����� ������ 16 ������, ��������.  ������ - 64 ���
int main(int argc, char* argv[])
{
	int bsi_no; // ����� ���
	int segment_no; // ����� �������� ������
	int address; // ��������� �����
	int step; // ����������
	int count; // ���������� ������

	if(argc <2)
	{
		printf(help);
		return -1;
	}

	// ������ ������ ��� (��� �� ���������, ��� �� �������)
	bsi_no = (int)read_int64(argc, argv, 2, "Enter BSI number(hex): ");

	// ������ ������ ��� (��� �� ���������, ��� �� �������)
	segment_no = (int)read_int64(argc, argv, 3, "Enter segment memory number(hex): ");

	// ������ ���������� ������
	address = (int)read_int64( argc, argv, 4, "Enter begining address(hex): ");

	// ������ ����������
	step = (int)read_int64( argc, argv, 5, "Enter step(hex): ");

	// ������ ���������� ������ ��� ������
	count = (int)read_int64( argc, argv, 6, "Enter count(hex): ");

	// ���� ������ �������� � ������� ���
	test_4(argv[1], bsi_no, segment_no, address, step, count);
	printf("Read from the BSI memory successful\n");

	return 0;
}

