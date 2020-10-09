#pragma once
#include <vector>
#include <algorithm>
#include <time.h>

struct Member 
{
	std::vector<double> DNA = std::vector<double>(2);
	double Fitness;
};
class Population
{
	double _minimum[2], _maximum[2];
	std::vector<Member> _member = std::vector<Member>(100);
	std::vector<Member> _parents;// { _member.at(0), _member.at(1) };

public:
	Population();
	Population(double* , double* );
	~Population();
	void CreateMembers();
	double EvaluateFitness(double(*ObjectiveFunction)(std::vector<double>));
	void NaturalSelection(unsigned);
	void Reproduction(int,int);
	void Display(int);
};

Population::Population() {}
Population::~Population() {}
Population::Population(double Minimum[2], double Maximum[2])
{
	memcpy(_minimum, Minimum, sizeof(double) * 2);
	memcpy(_maximum, Maximum, sizeof(double) * 2);

	for (int i = 0; i < _member.size(); i++)
	{
		_member.at(i).Fitness = 0.0;
	}
}
void Population::CreateMembers()
{
	int P = _member.size();
	int Q = _member.at(0).DNA.size();
	for (int i = 0; i < P; i++)
	{
		for (int j = 0; j < Q; j++)
		{
			_member.at(i).DNA.at(j) = (double)(rand() % (int)(_maximum[j] - _minimum[j]) + _minimum[j]);
		}
	}
}
double Population::EvaluateFitness(double(*ObjectiveFunction)(std::vector<double>))
{
	for (int i = 0; i < _member.size(); i++)
	{
		_member.at(i).Fitness = (*ObjectiveFunction)(_member.at(i).DNA);
	}
	std::sort(_member.begin(), _member.end(), [](Member const &a, Member &b) {return a.Fitness < b.Fitness; });

	return _member.at(0).Fitness;
}
void Population::NaturalSelection(unsigned n )
{
	for (int i = 0; i < n; i++)
	{
		_parents.push_back(_member.at(i));
	}
}
void Population::Reproduction(int MutationRate,int count)
{
	double LR=1.0;
	srand(time(NULL));
	if (count > 10) { LR = 0.1; }
	else if (count > 20) { LR = 0.05; }

	for (int i = 0; i < _member.size(); i++)
	{
		for (int j = 0; j < _member.at(0).DNA.size(); j++)
		{
			int TempSelection = rand() % _parents.size();
			_member.at(i).DNA.at(j) = _parents.at(TempSelection).DNA.at(j);

			if (rand() % 1000 < MutationRate)
			{
				int x = rand() % 3;
				//_member.at(i).DNA.at(j) = (double)(rand() % (int)(_maximum[j] - _minimum[j]) + _minimum[j]);
				if (x == 0) { _member.at(i).DNA.at(j) *= 1.15; }
				else if (x == 1) { _member.at(i).DNA.at(j) *= 0.85; }
				else { _member.at(i).DNA.at(j) += (0.25*LR); }
			}
		}
	}
}
void Population::Display(int gen)
{
	printf("\nGeneration => %d :  Best DNA =>( %f , %f ) :   Function Value => %f ",gen,_member.at(0).DNA.at(0), _member.at(0).DNA.at(1),_member.at(0).Fitness);
}


double Rosenbrook(std::vector<double> X)
{
	double a = 1.0;
	double b = 100.0;
	double x = X.at(0);
	double y = X.at(1);

	return pow(a - x, 2) + b * pow(y - pow(x, 2),2);
}
double Evolution(double(*ObjectiveFunction)(std::vector<double>), double* Minimum, double* Maximum, int MutationRate,double Target)
{
	Population population(Minimum, Maximum);
	bool AchievedTarget = false;
	int generation = 0;
	int count = 0, limit = 50;
	double tol = 1e-6;
	double old_Best, new_Best;

	old_Best = population.EvaluateFitness(*ObjectiveFunction);
	population.Display(generation);
	srand(time(NULL));
	while (!AchievedTarget || count < limit)
	{
		generation++;

		//Selecting fitest parents for Reproduction 
		population.NaturalSelection(3);

		//Reproduction
		population.Reproduction(MutationRate,count);

		//evaluate objective function and sort Members in ascending order i.e lowest to highest
		new_Best = population.EvaluateFitness(*ObjectiveFunction);

		//Incrementing Count to check for Convergence
		if (abs(new_Best - old_Best) <= tol) {count++;}
		else if (abs(new_Best - old_Best) > tol) { count = 0; }
		if (new_Best == Target){AchievedTarget = true;}
		//swapping
		old_Best = new_Best;
		population.Display(generation);
	}

	return new_Best;
}
double Evolution(double(*ObjectiveFunction)(std::vector<double>), double* Minimum, double* Maximum, int MutationRate)
{
	Population population(Minimum, Maximum);
	bool AchievedTarget = false;
	int generation = 0;
	int count = 0, limit = 50;
	double tol = 1e-6;
	double old_Best, new_Best;

	old_Best = population.EvaluateFitness(*ObjectiveFunction);
	population.Display(generation);
	srand(time(NULL));

	while (count < limit)
	{
		generation++;

		//Selecting fitest parents for Reproduction 
		population.NaturalSelection(5);

		//Reproduction
		population.Reproduction(MutationRate,count);

		//evaluate objective function and sort Members in ascending order i.e lowest to highest
		new_Best = population.EvaluateFitness(*ObjectiveFunction);

		//Incrementing Count to check for Convergence
		if (abs(new_Best - old_Best )<= tol) { count++; }
		else if (abs(new_Best - old_Best) > tol) { count = 0; }
		//swapping
		old_Best = new_Best;
		population.Display(generation);
	}

	return new_Best;
}
