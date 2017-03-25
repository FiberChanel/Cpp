// ���� ���������� ����������. ���� 3. ������ � ������
//

#include <stdlib.h>
#include <stdint.h>
#include "stdafx.h"
#include "hard_windows/HardwareInterface.h"

const char help[] = "WriteBpiMem. Test Block Stream Interface. Recording in the BSI memory.\n\tUse test_bsi_1.exe ip [bsi_no(hex) [segment_no(hex) [address(hex) [step(hex) [count(hex) [value(hex64)]]]]]]";
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
void test_3( char *ip, int bsi_no, int segment, int address, int step, int count, uint64_t value)
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
	for( int i = 0; i< count; i++)
		data[i] = value+i;

	result = WriteMem64Fast( hndl, bm, segment, bsi_no, address, step, count, data);
	if(result != brSuccess)
	{
		printf("Error WriteMem64Fast for %s. Result = %d\n", ip, result);
		SpecBuffFree(hndl, count, data);
		exit(-1);
	}
	SpecBuffFree(hndl, count, data);
}
//3) ������ � ������. ����� ������ (12 ���), ��������� �����, ����������, ���������� ������ - ����� ���. ������. ������ ������ - �������, ��������. ������ - 64 ����.

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

int main(int argc, char* argv[])
{
	int bsi_no; // ����� ���
	int segment_no; // ����� �������� ������
	int address; // ��������� �����
	int step; // ����������
	int count; // ���������� ������
	uint64_t value; // ��������� �������� ������

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

	// ������ ���������� �������� ��� ������
	value = read_int64( argc, argv, 7, "Enter begining value(hex): ");

	// ���� ������ �������� � ������� ���
	test_3(argv[1], bsi_no, segment_no, address, step, count, value);
	printf("Recording in the BSI memory successful\n");

	return 0;
}

