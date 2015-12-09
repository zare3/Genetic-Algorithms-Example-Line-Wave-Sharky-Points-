//
//  Chromosome.h
//  Neural Assignment 4
//
//  Created by MacBook Pro on 12/3/15.
//  Copyright (c) 2015 AUC. All rights reserved.
//

#ifndef Neural_Assignment_4_Chromosome_h
#define Neural_Assignment_4_Chromosome_h

#include <vector>
#include <cstdio>
#include <cctype>
#include <ctime>
using namespace std;


class Chromosome;

typedef double (*mutationFunctionPtr)(double , int , int );
typedef double (*fitnessFunctionPtr)(Chromosome*);


class Chromosome
{
public:
    
    
    Chromosome(double, mutationFunctionPtr, fitnessFunctionPtr);
    Chromosome(){};
    void addGene (double);
    void popGene();
    int size ();
    void mutate(int, int ,int);
    vector<double> getGenes();
    double getGeneAt(int);
    void updateFitness ();
    double getFitness ();
    double getMutationRate ();
    mutationFunctionPtr getMutationFunctionPtr();
    fitnessFunctionPtr getFitnessFunctionPtr();
    bool operator < (const Chromosome&) const;
    
private:
    
    
    int chromosomeSize;
    vector<double> genes;
    double mutationRate;
    mutationFunctionPtr mutationFun;
    fitnessFunctionPtr fitnessFun;
    double fitness;
    
    double randDouble (double,double);
    
    
};

#endif


Chromosome :: Chromosome (double mutationRate, mutationFunctionPtr mutationFun, fitnessFunctionPtr fitnessFun )
{
    
    srand(time(NULL));
    this->chromosomeSize = 0;
    this->mutationRate = mutationRate;
    this-> mutationFun = mutationFun;
    this->fitnessFun = fitnessFun;
}

mutationFunctionPtr Chromosome::getMutationFunctionPtr()
{
    return this->mutationFun;
    
}

fitnessFunctionPtr Chromosome::getFitnessFunctionPtr()
{
    return this->fitnessFun;
}

void Chromosome :: addGene (double gene)
{
    this->genes.push_back(gene);
    this->chromosomeSize++;
}

void Chromosome::popGene(){
    this->genes.pop_back();
    this->chromosomeSize--;
}

int Chromosome::size(){
    return this->chromosomeSize;
}

void Chromosome :: mutate(int idx, int epochIdx, int epochCnt)
{
    double rnd = randDouble(0, 100);
    if (rnd < mutationRate)
        genes[idx] = mutationFun(genes[idx],epochIdx,epochCnt);
}

vector<double> Chromosome::getGenes()
{
    return this->genes;
}

double Chromosome::getGeneAt(int idx)
{
    if (idx>=genes.size())
        throw "GENES ARRAY OUT OF RANGE";
    double ret = genes[idx];
    return ret;
}

void Chromosome::updateFitness()
{
    this->fitness = fitnessFun(this);
}

double Chromosome::getFitness()
{
    return this->fitness;
}

double Chromosome::randDouble(double fMin, double fMax)
{
    
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}



double Chromosome::getMutationRate()
{
    return this->mutationRate;
}

bool Chromosome::operator<(const Chromosome& guest) const
{
    return this->fitness < guest.fitness;
}

