#include "../include/vrAlignedArray.h"

vrAlignedArray * vrAlignedArrayAlloc()
{
	return vrAlloc(sizeof(vrAlignedArray));
}

vrAlignedArray * vrAlignedArrayInit(vrAlignedArray * arr, int sizeofdata)
{
	arr->sizeof_active = 0;
	arr->sizeof_data = sizeofdata;
	arr->data = NULL;
	arr->sizeof_array = 0;
	arr->size_available = 0;
	return arr;
}

void vrAlignedArrayPush(vrAlignedArray* arr, void * object)
{
	if (1)
	{
		arr->sizeof_array++;
		void** buffer = realloc(arr->data, (arr->sizeof_array)*arr->sizeof_data);
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
		VR_ASSERT((arr->sizeof_active + 1) < arr->sizeof_array, "Array failure");
		arr->data[arr->sizeof_active] = object;
		arr->size_available--;
		arr->sizeof_active++;
	}
}

void vrAlignedArrayPop(vrAlignedArray* arr)
{
	arr->sizeof_active--;
	arr->size_available++;
}

void vrAlignedArrayReserve(vrAlignedArray * arr, int size)
{
	void** buffer = realloc(arr->data, arr->sizeof_data * ( arr->sizeof_array + size));
	arr->sizeof_array += size;
	arr->size_available += size;
	VR_ASSERT(buffer != NULL, "REALLOC RETURNED NULL.");
	arr->data = buffer;
}

void vrAlignedArrayErase(vrAlignedArray* arr, int index)
{
	if (index > arr->sizeof_array || index < 0) return;

	void** buffer = calloc(arr->sizeof_data, arr->sizeof_array - 1);

	int c = 0;
	for (int i = 0; i < arr->sizeof_active; i++)
	{
		if (i != index)
		{
			buffer[c] = arr->data[i];
			c++;
		}
	}
	arr->data = buffer;
	arr->sizeof_active--;
	arr->sizeof_array--;
}
