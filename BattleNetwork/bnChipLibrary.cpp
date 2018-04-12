#include "bnChipLibrary.h"



ChipLibrary::ChipLibrary()
{
}


ChipLibrary::~ChipLibrary()
{
}

ChipLibrary& ChipLibrary::GetInstance()
{
	static ChipLibrary instance;
	return instance;
}

Chip* ChipLibrary::Next()
{
	if (library.empty()) {
		return nullptr;
	}

	int random = rand() % library.size();
	Chip* next = nullptr;

	list<Chip>::iterator it = library.begin();

	for (int i = 0; i < random; i++) {
		it++;
	}

	next = new Chip(*it);
	library.erase(it);

	return next;
}

void ChipLibrary::LoadLibrary()
{
	library.insert(library.begin(), Chip((unsigned)0, '*', 20, "TstChp\0", "test chip\0"));
	library.insert(library.begin(), Chip((unsigned)0, '*', 40, "TstChp2\0", "test chip\0"));
	library.insert(library.begin(), Chip((unsigned)0, '*', 100, "UltPwer\0", "test chip\0"));
	library.insert(library.begin(), Chip((unsigned)0, '*', 10, "Barr\0", "test chip\0"));
	library.insert(library.begin(), Chip((unsigned)0, '*', 30, "Barr2\0", "test chip\0"));
}
