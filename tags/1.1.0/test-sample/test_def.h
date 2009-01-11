/*  Sample CUnitWin32 Unit tests

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

#ifndef DEFINE_TEST
#define DEFINE_TEST(name)
#endif

/******************************************************************************
	Defines
******************************************************************************/
#ifndef TEST_DECL_ONCE
#define TEST_DECL_ONCE

#include "sample.h"

#define TEST_SETUP		test_setup
#define TEST_TEARDOWN	test_teardown

/******************************************************************************
	Test functions
******************************************************************************/

void test_setup(void)
{
	// There's no setup code. You can undefine TEST_SETUP and get rid of this
	// function
}


void test_teardown(void)
{
	// There's no teardown code. You can undefine TEST_TEARDOWN and get rid of
	// this function
}


static void pass(void)
{
	TEST_ASSERT(get_incremented_value() == 1);
}


static void fail(void)
{
	// This is to prove that unit tests don't affect each other. If they do,
	//this test will pass.
	TEST_ASSERT(get_incremented_value() == 2);
}


static void exception(void)
{
	// Guess what. The framework also catches system exceptions!
	TEST_ASSERT(access_violation() == 1);
}

#endif

/******************************************************************************
	Test definitions
******************************************************************************/

#if 1	// To take advantage of outlining

DEFINE_TEST( pass )
DEFINE_TEST( fail )
DEFINE_TEST( exception )

#endif

#undef DEFINE_TEST
