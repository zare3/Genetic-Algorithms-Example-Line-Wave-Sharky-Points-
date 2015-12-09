//
//  Evolution.h
//  Neural Assignment 4
//
//  Created by MacBook Pro on 12/3/15.
//  Copyright (c) 2015 AUC. All rights reserved.
//

#ifndef Neural_Assignment_4_Evolution_h
#define Neural_Assignment_4_Evolution_h

#include "Chromosome.h"
#include <vector>
#include <iomanip>
using namespace std;

class Evolution
{
    
    
public:
    
    Evolution (double targetMaximumFitness, int maxEpochCnt, int mutationCnt);
    void addChromosome (Chromosome);
    Chromosome train(ostream*);
    
    
private:
    
    vector<Chromosome> population;
    int popSize;
    double currentMaximumFitness;
    double targetMaximumFitness;
    double averageMaxFitness;
    int maxEpochCnt;
    int currEpochCnt;
    int mutationCnt;
    
    
    pair<Chromosome,Chromosome> marry (Chromosome , Chromosome );
    void generateNewPopulation();
    void updateFitnessValues();
    
    double randDouble(double fMin, double fMax);
};



#endif

Evolution :: Evolution (double targetMaximumFitness, int maxEpochCnt, int mutationCnt)
{
    this -> targetMaximumFitness = targetMaximumFitness;
    this -> maxEpochCnt = maxEpochCnt;
    this->popSize = 0;
    this->averageMaxFitness = -1e9;
    this->currentMaximumFitness = -1e9;
    this->currEpochCnt = 0;
    this -> mutationCnt = mutationCnt;
}

Chromosome Evolution::train(ostream* outS)
{
    ostream& o = *outS;
    
    while (currentMaximumFitness < targetMaximumFitness && currEpochCnt < maxEpochCnt)
    {

        this->population;
        generateNewPopulation();
        o << "EPOCH: " << currEpochCnt << " MAX:  " << setprecision(9) << fixed << currentMaximumFitness << "    " << "AVG:  " << averageMaxFitness << endl;
        cout <<"EPOCH: " << currEpochCnt << " MAX:  " << setprecision(9) << fixed << currentMaximumFitness << "    " << "AVG:  " << averageMaxFitness << endl;
        this->currEpochCnt++;
        
        Chromosome target = population[0];
        
        o << "---------------------------" << endl;
        
        for (int i=0; i<target.size(); i++)
        {
            o << target.getGeneAt(i) << ' ';
        }
        o << endl << endl << endl;

    }
    return population[0];
    
}

void Evolution::generateNewPopulation()
{
    vector<Chromosome> newGeneration;
    for (int i=0; i<popSize; i++){
        
        
        
        int p1,p2,p3,p4;
        int firstParent, secondParent;
        
        p1 = rand () % population.size();
        p2 = rand () % population.size();
        p3 = rand () % population.size();
        p4 = rand () % population.size();
        
        if (population[p1] < population[p2])
            firstParent = p2;
        else firstParent = p1;
        
        if (population[p3] < population[p4])
            secondParent = p4;
        else secondParent = p3;
        pair<Chromosome,Chromosome> children = marry(population[firstParent], population[secondParent]);
        
        
        for (int i=0; i<this->mutationCnt; i++){
            children.first.mutate(rand() % children.first.size(), currEpochCnt, maxEpochCnt);
            children.second.mutate(rand() % children.second.size(), currEpochCnt, maxEpochCnt);
        }
        
        
        population.push_back(children.first);
        population.push_back(children.second);
        
        
    }

    
    for (int i=0; i<newGeneration.size(); i++){
        population.push_back(newGeneration[i]);
    }
    
    sort (population.rbegin(), population.rend());
    population.erase(population.begin()+popSize, population.end());
    updateFitnessValues();
}

void Evolution::updateFitnessValues()
{
    currentMaximumFitness = population[0].getFitness();
    double sum = 0;
    for (int i=0; i<population.size(); i++)
    {
        sum += (population[i].getFitness());
    }
    sum /= population.size();
    this->averageMaxFitness = sum;
}

void Evolution::addChromosome(Chromosome ch)
{
    ch.updateFitness();
    this->popSize++;
    this->population.push_back(ch);
}

pair<Chromosome,Chromosome> Evolution::marry(Chromosome parent1, Chromosome parent2)
{
    
    Chromosome child1 (parent1.getMutationRate(), parent1.getMutationFunctionPtr(),parent1.getFitnessFunctionPtr());
    Chromosome child2 (parent1.getMutationRate(), parent1.getMutationFunctionPtr(),parent1.getFitnessFunctionPtr());
    int pivot = rand() % parent1.size();
    
    for (int i=0; i<parent1.size(); i++){
        
        if (i < pivot)
            child1.addGene(parent1.getGeneAt(i));
        else child1.addGene(parent2.getGeneAt(i));
    }
    
    for (int i=0; i<parent1.size(); i++){
        
        if (i < pivot)
            child2.addGene(parent2.getGeneAt(i));
        else child2.addGene(parent1.getGeneAt(i));
    }
    
    child1.updateFitness();
    child2.updateFitness();
    
    return {child1,child2};
}


double Evolution::randDouble(double fMin, double fMax)
{
    
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}