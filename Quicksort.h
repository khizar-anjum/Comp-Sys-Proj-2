#ifndef _QUICK_SORT_H
#define _QUICK_SORT_H

#ifdef __cplusplus
extern "C" {
#endif

int partition(int* nums, int lo, int hi);
int quickSort(int* nums, int lo, int hi);

#ifdef __cplusplus
}
#endif

#endif