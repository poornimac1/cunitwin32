/*  CUnitWin32 unit testing framework for C/C++ for Microsoft Windows. 

    Copyright (C) 2008 Dushara Jayasinghe.

    CUnitWin32 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CUnitWin32 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CUnitWin32.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

#include "getopt.h"

void inc_fails(void);

#define TEST_ASSERT(expression)		do {														\
	if(!(expression)) {																			\
	_tprintf(TEXT("%s @ %s in line %d failed\n"), TEXT(#expression), TEXT(__FILE__), __LINE__);	\
	fflush(stdout);																				\
	inc_fails();																				\
	}																							\
}while(0)


#include "test_def.h"

#ifndef TEST_SETUP
#define TEST_SETUP()
#endif

#ifndef TEST_TEARDOWN
#define TEST_TEARDOWN()
#endif

/******************************************************************************
	Defines
******************************************************************************/
typedef struct {
	void (*function)(void);
	TCHAR *name;
}test_info_t;

#define LIB_VERSION _TEXT("1.0.0")
#define LIB_NAME	_TEXT("CUnitWin32")

/******************************************************************************
	National prototypes
******************************************************************************/
/******************************************************************************
	Global variables
******************************************************************************/
/******************************************************************************
	National variables
******************************************************************************/
#define DEFINE_TEST(name)	{name, TEXT(#name) },
static const test_info_t test_list[] = {
#include "test_def.h"
};

const test_info_t *ptest;
static int fails;

static struct option opts[] = {
	{ TEXT("help"), 0, 0, _T('h') },
	{ TEXT("test"), required_argument, 0, _T('t') },
	{ TEXT("xml"), 0, 0, _T('x') },
	{ TEXT("version"), 0, 0, _T('v') },
	{ 0, 0, 0, 0 }
};

static int xml_mode;
static const TCHAR *do_test;

/******************************************************************************
	National functions
******************************************************************************/

static TCHAR *strip_path(TCHAR *original)
{
	TCHAR *ret = original;
	for(; original && *original; original++) {
		if(*original == '\\')
			ret = original + 1;
	}

	return ret;
}


static void help(TCHAR *myself)
{
	_ftprintf(stderr,
		TEXT("Usage: %s [OPTION]...\n\n")
		TEXT("Execute the tests ") LIB_NAME _TEXT(" was compiled with.\n\n")
		TEXT(" -h, --help           Print this help, then exit\n")
		TEXT(" -v, --version        Print version number, then exit\n")
		TEXT(" -t, --test TEST_NAME Run only TEST_NAME\n")
		TEXT(" -x, --xml            Output results in XML format\n\n")
		TEXT("For bug reports contact the person you got this utility from :-)\n"), strip_path(myself));
}


static void version(TCHAR *myself)
{
	_tprintf(
		TEXT("%s using ")LIB_NAME _TEXT(" version ") LIB_VERSION _TEXT("\n")
		TEXT("Copyright (C) 2008 Dushara Jayasinghe\n")
		TEXT("This is free software; see the source for copying conditions.\n")
		TEXT("There is NO warranty; not even for MERCHANTABILITY or\n")
		TEXT("FITNESS FOR A PARTICULAR PURPOSE.\n\n"), strip_path(myself));
}


static void print_header(TCHAR *name)
{
	TCHAR *beg;

	for(beg = name; 
		!(*name == '.' && *(name+1) == 'e' && *(name+2) == 'x' && *(name+3) == 'e'); 
		*name++)
	{
		if(*name=='\\')
			beg = name;
	}
	if(*beg == '\\')
		beg++;

	*name = 0;

	if(xml_mode)
	{
		_tprintf(
			TEXT("<?xml version=\"1.0\" encoding=\"utf-8\"?>")
			TEXT("<embedded-test-results>")
			TEXT("<test-suite name=\"%s\">")
			TEXT("<results>\n"), beg);
			fflush(stdout);
	}

	*name = '.';
}


static void print_test(const TCHAR *name)
{
	if(xml_mode)
	{
		_tprintf(
			TEXT("<test-case name=\"%s\" assertion=\""),
			name
			);
	}
	else
	{
		_tprintf(TEXT("%s\n"), name);
	}
	fflush(stdout);
}


static void print_results(BOOL success)
{
	if(xml_mode)
	{
		_tprintf(
			TEXT("\" success=\"%s\">")
			TEXT("\n</test-case>\n"),
			(success)?TEXT("True"):TEXT("False")
			);
		fflush(stdout);
	}
}


static void print_summary(int total, int fails)
{
	if(xml_mode)
	{
		_tprintf(
			TEXT("</results>\n")
			TEXT("<summary executed=\"%d\" failed=\"%d\">")
			TEXT("</summary>")
			TEXT("</test-suite>")
			TEXT("</embedded-test-results>"), total, fails);
	}
	else
	{
		_tprintf(TEXT("\n\ntotal = %d, failed = %d\n"), total, fails);
	}
}


void inc_fails(void)
{
	fails++;
}


static int fake_fork(TCHAR *myself, TCHAR *target)
{
	TCHAR buf[1024];

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
	DWORD error;

	_stprintf_s(buf, sizeof(buf)/sizeof(buf[0]), 
		TEXT("%s %s --test=%s"), 
		myself, (xml_mode)?TEXT("--xml"):TEXT(""), target);

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // Start the child process. 
    if( !CreateProcess( myself,   // No module name (use command line). 
        buf,			  // Command line. 
        NULL,             // Process handle not inheritable. 
        NULL,             // Thread handle not inheritable. 
        FALSE,            // Set handle inheritance to FALSE. 
        0,                // No creation flags. 
        NULL,             // Use parent's environment block. 
        NULL,             // Use parent's starting directory. 
        &si,              // Pointer to STARTUPINFO structure.
        &pi )             // Pointer to PROCESS_INFORMATION structure.
    ) 
    {
        _tprintf( _TEXT("CreateProcess failed (%d).\n"), GetLastError() );
        return -1;
    }

	// Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

	//
	if(GetExitCodeProcess(pi.hProcess, &error))
	{
		if(error)
			fails++;
	} 
	else
	{
		error = GetLastError();
		fails++;
	}

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

	print_results(error==0);
	return 0;
}


static LONG WINAPI vxl_exception_filter( struct _EXCEPTION_POINTERS *ExceptionInfo )
{
	// CPU exception (e.g. Access violation div 0 etc).
	TEST_ASSERT(0);
	TEST_TEARDOWN();
	return EXCEPTION_EXECUTE_HANDLER;
}


static void do_teardown(void)
{
	TEST_TEARDOWN();
}

/******************************************************************************
	Global functions
******************************************************************************/
int find_test(const TCHAR *name)
{
	int i;

	for(i=0;i<sizeof(test_list)/sizeof(test_list[0]);i++) {
		if(!(wcscmp(name, test_list[i].name))) {
			ptest = &test_list[i];
			return 1;
		}
	}

	return 0;
}

int _tmain( int argc, TCHAR *argv[] )
{
	int skip_test = 0;

	while (!skip_test) {
		int c, option_index = 0;
		c = getopt_long(argc, argv, "ht:x", opts, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 'h':
			help(argv[0]);
			skip_test = 1;
			break;
		case 'v':
			version(argv[0]);
			skip_test = 1;
			break;
		case 't':
			do_test = optarg;
			break;
		case 'x':
			xml_mode = 1;
			break;
		default:
			help(argv[0]);
			skip_test = 1;
		}
	}

	if(!skip_test)
	{
		if(do_test && find_test(do_test))
		{
			SetUnhandledExceptionFilter( vxl_exception_filter );
			atexit( do_teardown );

			print_test(do_test);
			TEST_SETUP();
			ptest->function();
		}
		else
		{
			int i;
			print_header(argv[0]);

			for(i=0;i<sizeof(test_list)/sizeof(test_list[0]);i++)
				fake_fork(argv[0], test_list[i].name);

			print_summary(i, fails);
		}
	}

	return (fails)?-1:0;
}

