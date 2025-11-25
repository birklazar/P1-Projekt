
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void Merge(int L[], int start, int end, int mid);
void MergeSort(int L[], int start, int end);
void printList(int L[], int size);

int main(void){
  int L[] = { 5, 3, 8, 1, 6, 10, 7, 2, 4, 9 };
  int L_size = sizeof(L) / sizeof(L[0]);

  printf("Given list \n");
  printList(L, L_size);

  MergeSort(L, 0, L_size - 1);

  printf("The sorted list \n");
  printList(L, L_size);
  return 0;
}

void Merge(int L[], int start, int end, int mid){
    int n1 = mid - start + 1;
    int n2 = end - mid;

    int *left = (int *)malloc(n1 * sizeof(int));
    int *right = (int *)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++)
        left[i] = L[start + i];
    for (int j = 0; j < n2; j++)
        right[j] = L[mid + 1 + j];

    int i = 0, j = 0, k = start;

    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            L[k] = left[i];
            i++;
        } else {
            L[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        L[k] = left[i];
        i++;
        k++;
    }

    while (j < n2) {
        L[k] = right[j];
        j++;
        k++;
    }

    free(left);
    free(right);
}

void MergeSort(int L[], int start, int end){
    if(start < end){
        int mid = floor((start+end)/2);
        MergeSort(L, start, mid);
        MergeSort(L,mid+1,end);
        Merge(L,start,end,mid);
    }
}

void printList(int L[], int size){
  int i;
  for (i = 0; i < size; i++)
    printf("%d ", L[i]);
  printf("\n");
}
