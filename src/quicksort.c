#include <stdio.h>
#include <random>
#include <chrono>
#include <pthread.h>
#include <atomic>
#include <queue>
#define NUM_THREADS 8

pthread_mutex_t mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP; //mutex lock for shared resources
pthread_mutex_t task_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP; //mutex lock for shared resources
pthread_cond_t got_task = PTHREAD_COND_INITIALIZER; /* for synchronization */

void swap(int arr[], int i, int j){
	if(i != j){
		arr[i] = arr[i] + arr[j];
		arr[j] = arr[i] - arr[j];
		arr[i] = arr[i] - arr[j];
	}
}

struct task{
	int lo;
	int hi;
};

struct worker_info{
	std::queue<task>* taskq; //queue for tasks
	int* arr; //shared arr
	int processed_elements; //keeps track of the % of array processed
	int len; //length of array
};

// sequential implementation of a quicksort array
void quicksort_seqhelper(int arr[], int lo, int hi){
	int p = arr[hi]; //selecting hi as the pivot
	int start = lo; // pointer for start
	int end = hi - 1; // pointer for end
	int len = hi - lo + 1; // len of the chunk of array
	int new_p;

	if(len < 2) return;
	if(len < 3) {
		if(arr[lo] > arr[hi]) swap(arr, lo, hi);
		return;
	}

	while(true){
		if (arr[start] > p && arr[end] < p){
			swap(arr, start, end);
			start += 1;
			end -= 1;
		}

		if(arr[start] <= p) start += 1;
		if(arr[end] >= p) end -= 1;

		if(end < start){
			break;
		}
	}

	if(arr[end] > arr[start]){
		swap(arr, end, hi);
		new_p = end;	
	}
	else{
		swap(arr, start, hi);
		new_p = start;
	}

	quicksort_seqhelper(arr, lo, new_p-1);
	quicksort_seqhelper(arr, new_p, hi);
	return;

}

void quicksort_seq(int arr[], int len){
	quicksort_seqhelper(arr, 0, len-1);
	return;
}

//helper for parallel implementation of quicksort
void *quicksort_parhelper(void *arg){
	worker_info* thread_data = static_cast<worker_info*>(arg);

	/* number of pending requests, initially none */
	task mytask;
	task newtask;
	int p, start, end, len, new_p, rc;

	while(true){
		//wait
		/* first, lock the mutex */
		rc = pthread_mutex_lock(&mutex);
		if (rc) { /* an error has occurred */
			perror("pthread_mutex_lock");
			pthread_exit(NULL);
		}
		/* check if all things are processed */
		if(thread_data->processed_elements >= thread_data->len){
			//printf("Oh everything's full\n");
			// post quit objects to make all other threads quit
			// task{hi = -1, lo = -1} are quit-objects
			newtask.lo = -1; newtask.hi = -1;
			
			thread_data->taskq->push(newtask);
			rc = pthread_cond_signal(&got_task);
			pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);
		}
		/* mutex is now locked - wait on the condition variable */
		/* if there are no requests to be handled.              */
		rc = 0;
		if (thread_data->taskq->size() == 0)
			rc = pthread_cond_wait(&got_task, &mutex);
		if (thread_data->taskq->size() > 0 && rc == 0) { /* we have a request pending */
			/* do your stuff... */
			mytask = thread_data->taskq->front(); thread_data->taskq->pop(); //get task and empty
			// check if this is a quit-task
			//printf("%d %d %d %d\n", mytask.lo, mytask.hi, thread_data->processed_elements, thread_data->taskq->size());
			if(mytask.lo == -1 && mytask.hi == -1){
				// this is a quit task, lets add another quit task and quit
				thread_data->taskq->push(mytask);
				rc = pthread_cond_signal(&got_task);
				pthread_mutex_unlock(&mutex);
				pthread_exit(NULL);
			}
			p = thread_data->arr[mytask.hi]; //selecting hi as the pivot
			start = mytask.lo; // pointer for start
			end = mytask.hi - 1; // pointer for end
			len = mytask.hi - mytask.lo + 1; // len of the chunk of array
		}
		/* finally, unlock the mutex */
		pthread_mutex_unlock(&mutex);

		if(len < 1){continue;}
		else if(len < 2){
			rc = pthread_mutex_lock(&mutex); 
			thread_data->processed_elements++;
			pthread_mutex_unlock(&mutex);
		} 
		else if(len < 3) {
			if(thread_data->arr[mytask.lo] > thread_data->arr[mytask.hi]) swap(thread_data->arr, mytask.lo, mytask.hi);
			rc = pthread_mutex_lock(&mutex); 
			thread_data->processed_elements += 2;
			pthread_mutex_unlock(&mutex);
		}
		else{
			while(true){
				if (thread_data->arr[start] > p && thread_data->arr[end] < p){
					swap(thread_data->arr, start, end);
					start += 1;
					end -= 1;
				}

				if(thread_data->arr[start] <= p) start += 1;
				if(thread_data->arr[end] >= p) end -= 1;

				if(end < start){
					break;
				}
			}

			if(thread_data->arr[end] > thread_data->arr[start]){
				swap(thread_data->arr, end, mytask.hi);
				new_p = end;	
			}
			else{
				swap(thread_data->arr, start, mytask.hi);
				new_p = start;
			}

			// push new tasks and signal
			//printf("Assigning tasks yo! %d %d\n", mytask.lo, mytask.hi);
			rc = pthread_mutex_lock(&task_mutex);
			newtask.lo = mytask.lo; newtask.hi = new_p-1;
			thread_data->taskq->push(newtask);
			rc = pthread_cond_signal(&got_task);

			newtask.lo = new_p; newtask.hi = mytask.hi;
			thread_data->taskq->push(newtask);
			rc = pthread_cond_signal(&got_task);
			pthread_mutex_unlock(&task_mutex);
		}
	}

	return NULL;
}

//main program for quicksort parallel implementation
void quicksort_par(int arr[], int len){
	task mytask;
	int rc;
	worker_info thread_data;
	std::queue<task> taskq;

	mytask.lo = 0; mytask.hi=len-1;
	thread_data.arr = arr;
	thread_data.taskq = &taskq;
	thread_data.processed_elements = 0; 
	thread_data.len = len;

	//create those threads
	pthread_t* tid = new pthread_t[NUM_THREADS];
	// dispatch those threads
	for (int j=0; j  < NUM_THREADS; j++){
		pthread_create(&tid[j], NULL, &quicksort_parhelper, &thread_data);
	}
	//do stuff
	thread_data.taskq->push(mytask); //push the first task
	rc = pthread_cond_signal(&got_task); //signal to process it
	// join them back
	for (int j=0; j  < NUM_THREADS; j++)
		pthread_join(tid[j], NULL);
	//destroy them
	delete [] tid;

	return;
}


int main(){
	
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;
	std::random_device rd;
	std::mt19937 e2(rd());
	std::uniform_int_distribution<> dist(200, 399);
	// sequential quicksort works okay, experiment with bigger arrays!
	
	const int len[6] = {1024, 16384, 262144, 4194304, 67108864, 134217728}; //powers of 2
	
	for(int j = 0; j < 2; j++){
		if(j == 0)printf("TESTING SEQUENTIAL IMPLEMENTATION\n");
		else printf("TESTING PARALLEL IMPLEMENTATION\n");
		for (int i = 0; i < 6; i++){
			// declare the array
			int* arr = new int[len[i]];

			//populate the array
			for(int i = 0; i < len[i]; i++){
				arr[i] = dist(e2);
			}

			//sort the array and measure time
			if(j == 0){
			begin = std::chrono::steady_clock::now();
			quicksort_seq(arr, len[i]);
			end = std::chrono::steady_clock::now();
			printf ("It took me %d milli-seconds for an array of length %d\n" , \
				std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count(), len[i]);
			}
			else{
			begin = std::chrono::steady_clock::now();
			quicksort_par(arr, len[i]);
			end = std::chrono::steady_clock::now();
			printf ("It took me %d milli-seconds for an array of length %d\n" , \
				std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count(), len[i]);
			}
			//delete the array to free memory
			delete [] arr;
		}
		printf("\n");
	}
	/*
	int arr[8] = {6,3,7,83,9,22,5,7};
	quicksort_par(arr, 8);
	for(int i = 0; i < 8; i++)
		printf("%d ", arr[i]);
	printf("\n");
	*/
	return 0;
}