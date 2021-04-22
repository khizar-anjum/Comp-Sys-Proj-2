int partition(int* nums, int lo, int hi) {
    int idx = lo;

    for(int i = lo+1; i <= hi; i++) {
        if(nums[i] <= nums[lo] && i != ++idx) {
            nums[i] ^= nums[idx] ^= nums[i] ^= nums[idx];
        }
    }
    if(lo != idx) {
        nums[lo] ^= nums[idx] ^= nums[lo] ^= nums[idx];
    }

    return idx;
}

int quickSort(int* nums, int lo, int hi) {
    if(lo <= hi) {
        int pidx = partition(nums,lo,hi);
        quickSort(nums,lo,pidx-1);
        quickSort(nums,pidx+1,hi);
    }
}