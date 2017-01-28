#include "RenVector.h"

RenVector::RenVector() {
	used = 0;
	allocated = 0;
	data = 0;
}

void RenVector::clear() {
	renInfo temp;
	used = 0;
	temp.type = renError;
	temp.asset = 0;
	for(int i = 0; i < allocated;++i)
		data[i] = temp;
}

void RenVector::setSize(int size) {
	renInfo* tempData;
	int smallest;
	if(allocated != 0) {
		tempData = new renInfo[size];
		smallest = size;
		if(smallest > allocated)
			smallest = allocated;
		for(int i = 0;i < smallest;++i)
			tempData[i] = data[i];
		delete [] data;
		data = tempData;
	} else {
		data = new renInfo[size];
	}
	if(used>size)
		used = size;
	allocated = size;
}

void RenVector::add(renInfo& newRen) {
	if(newRen.type < numRenTypes && newRen.type >= 0) {
		if(used >= allocated) {
			setSize(allocated+ren_sizeGrowth);
		}
		data[used].asset = newRen.asset;
		data[used].type = newRen.type;
		data[used].matrix = newRen.matrix;
		++used;
	}
}

renInfo* RenVector::get(int loc) {
	if(loc < allocated&&loc>=0)
		return &data[loc];
	else
		return 0;
}

RenVector::~RenVector() {
	if(data)
		delete [] data;
	allocated = 0;
	used = 0;
}