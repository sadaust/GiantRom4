/*=======================
=RenVector.h/.cpp		=
=by Tyler van Gastel	=
=latest update 11/7/2015=
=vector for render info	=
=========================*/

#pragma once
#include "ResourceDefines.h"
#define ren_startSize 100
#define ren_sizeGrowth 10

class RenVector {
private:
	int used;
	int allocated;
	renInfo* data;
public:
	RenVector();
	void clear();
	void setSize(int);
	void add(renInfo&);
	int getSize(){return used;}
	renInfo* get(int);
	~RenVector();
};