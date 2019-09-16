#include "SPArrayList.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


SPArrayListNode spArrayListNewNode (int sRow, int sCol, int dRow, int dCol, char capturedpiece){
	SPArrayListNode n;
	n.sRow = sRow;
	n.sCol = sCol;
	n.dRow = dRow;
	n.dCol = dCol;
	n.capturedpiece = capturedpiece;
	return n;
}

SPArrayList* spArrayListCreate(int maxSize){
	if (maxSize <= 0){
		return NULL;
	}
	SPArrayList* lst = (SPArrayList*) malloc(sizeof(SPArrayList));
	if (!lst){
		return NULL;
	}
	lst->maxSize = maxSize;
	lst->actualSize = 0;
	lst->elements = (SPArrayListNode*)malloc(sizeof(SPArrayListNode) * maxSize);
	if (lst->elements == NULL){
		free(lst);
		return NULL;
	}
	return lst;
}

SPArrayList* spArrayListCopy(SPArrayList* src){
	if (src==NULL){
		return NULL;
	}
	SPArrayList* copy = spArrayListCreate(src->maxSize);
	if (!copy){
		return NULL;
	}
	for (int i = 0; i < src->actualSize ; i++){
		copy->elements[i] = src->elements[i];
	}
	copy->actualSize = src->actualSize;
	return copy;
}

void spArrayListDestroy(SPArrayList* src){
	if (src!=NULL){
		free(src->elements);
		free(src);
	}
}

SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src){
	if (src==NULL){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	src->actualSize = 0;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, SPArrayListNode elem, int index){
	if (src==NULL){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if (index<0 || index > src->actualSize){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if (src->actualSize == src->maxSize){
		return SP_ARRAY_LIST_FULL;
	}
	for (int i = src->actualSize -1; i>index-1 ; i--){
		src->elements[i+1 ] = src->elements[i];
	}
	src->actualSize+=1;
	src->elements[index] = elem;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, SPArrayListNode elem){
	return spArrayListAddAt(src, elem, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, SPArrayListNode elem){
	return spArrayListAddAt(src, elem, src->actualSize);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index){
	if (src==NULL){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if (index<0 || index>=src->actualSize){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if (src->actualSize == 0){
		return SP_ARRAY_LIST_EMPTY;
	}
	for (int i = index+1; i<src->actualSize ; i++){
		src->elements[i-1] = src->elements[i];
	}
	src->actualSize--;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src){
	return spArrayListRemoveAt(src, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src){
	return spArrayListRemoveAt(src, src->actualSize-1);
}

SPArrayListNode spArrayListGetAt(SPArrayList* src, int index){
	if (index < 0 || index > src->actualSize){
		return (SPArrayListNode){-1,-1,-1,-1,-1};
	}
	return src->elements[index];
}

SPArrayListNode spArrayListGetFirst(SPArrayList* src){
	if (src->actualSize == 0){
		return (SPArrayListNode){-1,-1,-1,-1,-1};
	}
	return spArrayListGetAt(src, 0);
}

SPArrayListNode spArrayListGetLast(SPArrayList* src){
	if (src->actualSize == 0){
		return (SPArrayListNode){-1,-1,-1,-1,-1};
	}
	return spArrayListGetAt(src, src->actualSize-1);
}

int spArrayListMaxCapacity(SPArrayList* src){
	return src->maxSize;
}

int spArrayListSize(SPArrayList* src){
	return src->actualSize;
}

bool spArrayListIsFull(SPArrayList* src){
	if (src==NULL){
		return false;
	}
	return src->actualSize==src->maxSize;
}

bool spArrayListIsEmpty(SPArrayList* src){
	if (src==NULL){
		return false;
	}
	return src->actualSize==0;
}




