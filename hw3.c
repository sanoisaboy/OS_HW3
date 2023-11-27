#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

unsigned int bitmap[4];
pthread_t threads[100];
int thread_count = 0;

void list_bitmap(void)
{

	for (int i = 0; i < 4; i++)
	{
		printf("bitmap[%d] = %u\n", i, bitmap[i]);
	}

	printf("------------------------------------------\n");

	return;
}

int allocate_map(void)
{

	if (bitmap == NULL)
	{
		return -1;
	}

	for (int i = 0; i < 4; i++)
	{
		bitmap[i] = 0;
	}

	list_bitmap();

	return 1;
}

void release_pid(int pid)
{

	unsigned int test_bit = 1;

	test_bit = test_bit << (pid % 32);

	if (bitmap[pid / 32] & test_bit)
	{
		bitmap[pid / 32] = bitmap[pid / 32] & ~test_bit;
	}
	else
	{
		printf("This Pid is not allocated\n");
	}

	// list_bitmap();
}

void *threadFunc(void *arg)
{
	int sleep_time = (rand() % 2) + 1;
	printf("Thread ID: %lu\npid=%d,this thread will sleep %d seconds\nRelease Pid %d\n", pthread_self(), (int)arg, sleep_time, (int)arg);
	// printf("pid=%d,this thread will sleep %d seconds\n", (int)arg, sleep_time);

	// printf("Release Pid %d\n", (int)arg);
	// sleep(sleep_time);
	release_pid((int)arg);

	pthread_exit(NULL);
}

void create_thread(int PID)
{

	int rc;
	void *reBuf;
	rc = pthread_create(&threads[thread_count], NULL, threadFunc, (void *)PID);

	if (rc)
	{
		printf("error:%d\n", rc);
		exit(-1);
	}

	// rc = pthread_join(thread, &reBuf);

	if (rc)
	{
		printf("rrror:%d\n", rc);
		exit(-1);
	}

	// printf("return value:%s\n", (char *)reBuf);
	thread_count++;
	return;
}

int allocate_pid(void)
{

	if ((bitmap[0] & bitmap[1] & bitmap[2] & bitmap[3]) == 4294967295)
	{
		list_bitmap();
		return -1;
	}

	int PID_number = 0;
	int add_set = 1;
	for (int i = 0; i < 4; i++)
	{
		unsigned int set_bit = 1;
		for (int j = 0; j < 32; j++)
		{
			if (bitmap[i] & set_bit)
			{
				set_bit = set_bit << 1;
			}
			else if (add_set)
			{
				bitmap[i] = bitmap[i] | set_bit;
				add_set = 0;
				PID_number = i * 32 + j;
			}
		}
	}

	// printf("Successful to allocate PID. The PID of new process : %d\n", PID_number);

	create_thread(PID_number);
	// list_bitmap();

	return 0;
}

int main(int argc, char *argv[])
{

	int control_mode = 1;
	int remove_PID_number;
	srand(time(NULL));
	printf("------------Allocating bitmap-------------\n");
	allocate_map();
	// create_thread();
	while (control_mode != 3)
	{
		printf("(1)create 100 process (2)exit\n");
		printf(" Please enter your choise :");
		scanf("%d", &control_mode);

		if (control_mode == 1)
		{
			for (int i = 0; i < 100; i++)
			{

				allocate_pid();
			}
			for (int i = 0; i < thread_count; i++)
			{
				pthread_join(threads[i], NULL);
			}
			thread_count = 0;
		}
		else if (control_mode == 2)
		{
			return 0;
		}
	}

	printf("Exit this Process !\n");

	return 0;
}
