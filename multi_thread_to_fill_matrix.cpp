// MultiThreadFillMatrix.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h>
#include <unordered_map>
#include <process.h>

//Fill a [32 * 32] frame buffer, with size [4 * 4] tiles and 16 threads.
#define THREAD_NUM 16
#define TILE_ROW  8
#define TILE_COL 8
#define BUF_WIDTH 32
#define BUF_HEIGHT 32
#define TILE_SIZE 4

template<typename T>
struct HashNode
{
	HashNode<T>* next = nullptr;
	DWORD key = 0xffffffff;
	T data;
};

template<typename T>
struct MyHash
{
	HashNode<T> table[100];
	void Insert(DWORD key, T data)
	{
		DWORD k = key % 100;
		HashNode<T>* h = &table[k];
		// head insert
		HashNode<T>* t = new HashNode<T>();
		t->next = h->next;
		t->key = key;
		t->data = data;
		h->next = t;
	}

	T* Get(DWORD key)
	{
		DWORD k = key % 100;
		HashNode<T>* h = table[k].next;
		while (h)
		{
			if (h->key == key)
				return &(h->data);
			h = h->next;
		}
		return NULL;
	}

	
	void DestroyHash()
	{
		for (int i = 0; i < 100; ++i)
		{
			HashNode<T>* h = table[i].next;
			while (h)
			{
				HashNode<T>* t = h;
				h = h->next;
				delete t;
			}
		}
	}
};

DWORD Matrix[BUF_WIDTH * BUF_HEIGHT];
HANDLE threads[THREAD_NUM];
DWORD threads_id[THREAD_NUM];
MyHash<HANDLE> id_handle_map;
HANDLE begin_evt[THREAD_NUM];
MyHash<HANDLE> id_begnin_evt_handle_map;
HANDLE end_evt[THREAD_NUM];
MyHash<HANDLE> id_handle_end_evt_map;
DWORD WINAPI ThreadProc(LPVOID param);
INT tile_index = 0;

struct tile
{
	DWORD left, top, right, bottom;
	POINT pts[TILE_SIZE * TILE_SIZE];
	MyHash<POINT> thread_pixel_mapping;
};

tile Tiles[TILE_ROW * TILE_COL];
CRITICAL_SECTION cs;
DWORD thread_index = -1;

void Print_Frame_Buffer()
{
	for (int i = 0; i < BUF_HEIGHT; ++i)
	{
		for (int j = 0; j < BUF_WIDTH; ++j)
		{
			printf("%d ", Matrix[i * BUF_WIDTH + j]);
		}
		printf("\n");
	}
	printf("\n");
}

DWORD WINAPI ThreadProc(LPVOID param)
{
	while (true)
	{
 		DWORD thread_id = GetCurrentThreadId();
 		HANDLE begin_evt = *id_begnin_evt_handle_map.Get(thread_id);
 		HANDLE end_evt = *id_handle_end_evt_map.Get(thread_id);
		// can begin?
		// wait begin trigger on
		WaitForSingleObject(*id_begnin_evt_handle_map.Get(thread_id), INFINITE);
		tile& t = Tiles[tile_index];
		POINT* pt = t.thread_pixel_mapping.Get(thread_id);
		Matrix[pt->y * BUF_WIDTH + pt->x] = 1;
		printf("Thread %d Write elem: %d %d\n", 0, pt->x, pt->y);
		// set begin trigger off
		ResetEvent(*id_begnin_evt_handle_map.Get(thread_id));
		// set end trigger on
		SetEvent(*id_handle_end_evt_map.Get(thread_id));
	}
}

int main()
{
	for (int i = 0; i < THREAD_NUM; ++i)
	{
		threads[i] = CreateThread(NULL, 0, ThreadProc, NULL, CREATE_SUSPENDED, &threads_id[i]);
		id_handle_map.Insert(threads_id[i], threads[i]);

		begin_evt[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		id_begnin_evt_handle_map.Insert(threads_id[i], begin_evt[i]);

		end_evt[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		id_handle_end_evt_map.Insert(threads_id[i], end_evt[i]);
	}

	InitializeCriticalSection(&cs);

	DWORD x = 0;
	DWORD y = 0;
	for (int i = 0; i < TILE_ROW; ++i)
	{
		for (int j = 0; j < TILE_COL; ++j)
		{
			tile& t = Tiles[i * TILE_COL + j];
			t.left = x;
			t.top = y;
			t.right = t.left + TILE_SIZE - 1;
			t.bottom = t.top + TILE_SIZE - 1;

			// thread-pixel map
			int _thread_index = 0;
			for (int m = 0; m < TILE_SIZE; ++m)
			{
				for (int n = 0; n < TILE_SIZE; ++n)
				{
					POINT pt;
					pt.x = t.left + m;
					pt.y = t.top + n;
					t.thread_pixel_mapping.Insert(threads_id[_thread_index], pt);
					++_thread_index;
				}
			}

			x += TILE_SIZE;
		}
		x = 0;
		y += TILE_SIZE;
	}

	// wake up
	for (int i = 0; i < THREAD_NUM; ++i)
		ResumeThread(threads[i]);

	while (true)
	{
		ZeroMemory(Matrix, sizeof(Matrix));
		Print_Frame_Buffer();
		while (tile_index < TILE_ROW * TILE_COL)
		{
			// set all begin trigger on
			for (int i = 0; i < THREAD_NUM; ++i)
				SetEvent(begin_evt[i]);
			// wait for ends
			WaitForMultipleObjects(THREAD_NUM, end_evt, TRUE, INFINITE);
			// no end
			for (int i = 0; i < THREAD_NUM; ++i)
				ResetEvent(end_evt[i]);
			++tile_index;
			Print_Frame_Buffer();
		}
		tile_index = 0;
		Print_Frame_Buffer();
	}
	
    return 0;
}

