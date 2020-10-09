// GA.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include <iostream>
#include "Header.h"

int main()
{
	double Min[2] = { -1,-1 };
	double Max[2] = { 1,1 };

	Evolution(Rosenbrook, Min, Max,50);

	std::cin.get();
}
