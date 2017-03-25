// ���� ���������� ����������. ���� 1. ������ � ������� �����
//

#include <stdlib.h>
#include <stdint.h>
#include "stdafx.h"
#include "hard_windows/HardwareInterface.h"

const char help[] = "WriteBpiReg. Test Block Stream Interface. Recording in the register.\n\tUse test_bsi_1.exe ip [bsi_no(hex) [[reg_no(hex)] value(hex)]]";
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
	int reg_no; // ����� �������� ���
	int64_t value; // �������� ��� ������ � ������� ���

	if(argc <2)
	{
		printf(help);
		return -1;
	}

	// ������ ������ ��� (��� �� ���������, ��� �� �������)
	bsi_no = (int)read_int64(argc, argv, 2, "Enter BSI number(hex): ");

	// ������ ������ �������� ��� (��� �� ���������, ��� �� �������)
	reg_no = (int)read_int64(argc, argv, 3, "Enter register BSI number(hex): ");

	// ������ �������� ��� ������ � ������� ��� (��� �� ���������, ��� �� �������)
	value = read_int64(argc, argv, 4, "Enter value for write(hex): ");
	
	// ���� ������ �������� � ������� ���
	test_1(argv[1], bsi_no, reg_no, value);

	printf("Write bsi reg successful\n");

	return 0;
}

