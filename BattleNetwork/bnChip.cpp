#include "bnChip.h"

Chip::Chip(unsigned id, char code, unsigned damage, string sname, string desc) : ID(id), code(code), damage(damage), shortname(sname), description(desc)
{
}

Chip::Chip(const Chip & copy)
{
	ID = copy.ID;
	code = copy.code;
	damage = copy.damage;
	shortname = copy.shortname;
	description = copy.description;
}

Chip::~Chip()
{
}

const string Chip::GetDescription()
{
	return description;
}

const string Chip::GetShortName()
{
	return shortname;
}

const char Chip::GetCode()
{
	return code;
}

const unsigned Chip::GetDamage()
{
	return damage;
}

const unsigned Chip::GetID()
{
	return ID;
}
