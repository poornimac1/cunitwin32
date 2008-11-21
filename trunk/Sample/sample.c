/*  Sample source tested under CUnitWin32

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

#include "sample.h"

/******************************************************************************
	Defines
******************************************************************************/
/******************************************************************************
	National prototypes
******************************************************************************/
static int value;

/******************************************************************************
	Global variables
******************************************************************************/
/******************************************************************************
	National variables
******************************************************************************/
/******************************************************************************
	National functions
******************************************************************************/
/******************************************************************************
	Global functions
******************************************************************************/

int get_incremented_value(void)
{
	return ++value;
}


int access_violation(void)
{
	return *(int*)0 = value;
}
