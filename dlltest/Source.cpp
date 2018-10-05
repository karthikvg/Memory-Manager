/*
* NAME   : V.SAI KARTHIK
* PROJECT: MEMORY MANAGER
* TASK ID: 5
* DATE   : 6-2-2018
*/
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include "myheader.h"

#define MAX_MEMORY 1024

using namespace std;

struct row_node;
struct row_head
{
	void* memory;
	row_head* next_row_head;
	row_node* next_row_node;
};
row_head* create_row_head()
{
	row_head* temp = (row_head*)malloc(sizeof(row_head));
	temp->memory = (void*)malloc(unsigned(MAX_MEMORY));
	temp->next_row_head = NULL;
	temp->next_row_node = NULL;
	return temp;
}
struct row_node
{
	int start;
	int end;
	row_node* next_row_node;
};
row_node* create_row_node(int x, int y)
{
	row_node* temp = (row_node*)malloc(sizeof(row_node));
	temp->start = x;
	temp->end = y;
	temp->next_row_node = NULL;
	return temp;
}
int min(int a, int b){return a < b ? a : b;}
void show_map(row_head* head)
{
	while (head)
	{
		row_node* temp = head->next_row_node;
		while (temp)
		{
			printf("\t\t%d %d\n", temp->start, temp->end);
			temp = temp->next_row_node;
		}
		head = head->next_row_head;
	}
}
void inserting(row_head* head,int start,int end)
{
	if (head->next_row_node == NULL)
	{
		head->next_row_node = create_row_node(start, end);
		return;
	}
	if (head->next_row_node->start - 1 == end)
	{
		head->next_row_node->start = start;
		return;
	}
	else if (head->next_row_node->start > end)
	{
		row_node* temp = create_row_node(start, end);
		temp->next_row_node = head->next_row_node;
		head->next_row_node = temp;
		return ;
	}
	row_node* temp = head->next_row_node;
	while (true)
	{
		if (temp->next_row_node == NULL)
		{
			if (temp->end == start - 1)
				temp->end = end;
			else
				temp->next_row_node=create_row_node(start,end);
			return;
		}
		else
		{
			if (temp->end + 1 == start&&temp->next_row_node->start==end+1)
			{
				row_node* new_node = temp->next_row_node;
				temp->end = new_node->end;
				temp->next_row_node = new_node->next_row_node;
				free(new_node);
				return;
			}
			else if (temp->next_row_node->start - 1 == end)
			{
				temp->next_row_node->start = start;
				return;
			}
			else if (temp->next_row_node->start > end)
			{
				row_node* new_temp = create_row_node(start, end);
				new_temp->next_row_node = temp->next_row_node;
				temp->next_row_node = new_temp;
				return;
			}
		}
		temp = temp->next_row_node;
	}
}
int get_segment_size(int end, int start)
{
	return end - start;
}
int removing(row_head* head, int size)
{
	if (head == NULL)
		return -1;
	if (head->next_row_node == NULL)
		return -1;
	row_node* temp = head->next_row_node;
	if (size==get_segment_size(temp->end,temp->start))
	{
		head->next_row_node = temp->next_row_node;
		int location = temp->start;
		free(temp);
		return location;
	}
	else if (size < get_segment_size(temp->end, temp->start))
	{
		int location = temp->start;
		temp->start += size;
		return location;
	}
	while (temp->next_row_node)
	{
		if (size < get_segment_size(temp->next_row_node->end, temp->next_row_node->start))
		{
			int location = temp->start;
			temp->next_row_node->start += size;
			return location;
		}
		else if (size == get_segment_size(temp->next_row_node->end, temp->next_row_node->start))
		{
			row_node* new_temp = temp->next_row_node;
			int location = new_temp->start;
			temp->next_row_node = new_temp->next_row_node;
			free(new_temp);
			return location;
		}
		temp = temp->next_row_node;
	}
	return -1;
}
row_head* data_page_node;
void* kar_malloc(unsigned int _size)
{
	_size = min(_size, 1024);
	row_head* temp = data_page_node;
	while (true)
	{
		int pos = removing(temp, _size);
		if (pos == -1)
		{
			if (temp->next_row_head == NULL)
			{
				temp->next_row_head = create_row_head();
				inserting(temp->next_row_head, 0, MAX_MEMORY);
			}
			temp = temp->next_row_head;
			continue;
		}
		else
		{
			*(short int*)((char*)temp->memory + (int)_size) = _size;
			return (void*)((char*)temp->memory + (int)_size+2);
		}
		
		temp = temp->next_row_head;
	}
	return kar_malloc(_size);
}
void kar_free(void* ptr)
{
	row_head* temp = data_page_node;
	while (temp)
	{
		if ((char*)ptr - (char*)(temp->memory) >= 0 && (char*)ptr - (char*)(temp->memory) <= 1024)
		{
			int x = *(short*)((char*)(ptr)-2)+2;
			int y = (char*)ptr - (char*)(temp->memory)-2;
			inserting(temp,y, x + y);
			return;
		}
		temp = temp->next_row_head;
	}
}
int main()
{
	data_page_node = create_row_head();
	inserting(data_page_node, 0, 1024);
	int* p =(int*)kar_malloc(1020);
	p[1] = 5;
	p[3] = 20;
	printf("%d", p[1]+p[3]);
	int* c = (int*)kar_malloc(100);
	system("pause");
	return 0;
}