/*
* Copyright (c) 2016 Cormac Grindall (Mithreindeir)
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* vrFreely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "../include/vrArray.h"
#include "../include/vr.h"

vrArray * vrArrayAlloc()
{
	return vrAlloc(sizeof(vrArray));
}

vrArray * vrArrayInit(vrArray * arr, int sizeofdata)
{
	arr->sizeof_active = 0;
	arr->sizeof_data = sizeofdata;
	arr->data = NULL;
	arr->sizeof_array = 0;
	arr->size_available = 0;

	return arr;
}

void vrArrayDestroy(vrArray * arr)
{
	vrFree(arr->data);
	vrFree(arr);
}

void vrArrayPush(vrArray* arr, void * object)
{
	if (arr->size_available <= 0)
	{
		arr->sizeof_array++;
		void** buffer = vrRealloc(arr->data, (arr->sizeof_array)*arr->sizeof_data);
		VR_ASSERT(buffer != NULL, "REALLOC RETURNED NULL.");
		arr->data = buffer;
		arr->data[arr->sizeof_active] = object;
		arr->sizeof_active++;
	}
	else
	{
		if (arr->sizeof_active + 1 > arr->sizeof_array)
		{
			printf("Meh\n");
			abort();
		}
		//VR_ASSERT((arr->sizeof_active + 1) < arr->sizeof_array, "Array failure");
		arr->data[arr->sizeof_active] = object;
		arr->size_available--;
		arr->sizeof_active++;
	}
}

void vrArrayPop(vrArray* arr)
{
	arr->sizeof_active--;
	arr->data[arr->sizeof_active] = NULL;
	arr->size_available++;
}

void vrArrayReserve(vrArray * arr, int size)
{
	void** buffer = vrRealloc(arr->data, arr->sizeof_data * ( arr->sizeof_array + size));
	arr->sizeof_array += size;
	arr->size_available += size;
	VR_ASSERT(buffer != NULL, "REALLOC RETURNED NULL.");
	arr->data = buffer;
}

int vrArrayErase(vrArray* arr, int index)
{
	if (index > arr->sizeof_array || index < 0) return -1;

	void** buffer = vrCalloc(arr->sizeof_data, arr->sizeof_array - 1);

	int c = 0;
	for (int i = 0; i < arr->sizeof_active; i++)
	{
		if (i != index)
		{
			buffer[c] = arr->data[i];
			c++;
		}
	}

	vrFree(arr->data);
	arr->data = buffer;
	arr->sizeof_active--;
	arr->sizeof_array--;
	//Correct index if iterating
	//Through data
	return index - 1;
}

void vrArrayCopy(vrArray * dest, vrArray * src)
{
	dest->data = vrRealloc(dest->data, src->sizeof_data * src->sizeof_array);
	for (int i = 0; i < src->sizeof_array; i++)
	{
		dest->data[i] = src->data[i];
	}
	dest->sizeof_active = src->sizeof_active;
	dest->sizeof_array = src->sizeof_array;
	dest->sizeof_data = src->sizeof_data;
}

void vrArrayClear(vrArray * arr)
{
	for (int i = 0; i < arr->sizeof_active; i++)
	{
		vrArrayPop(arr);
	}
}
