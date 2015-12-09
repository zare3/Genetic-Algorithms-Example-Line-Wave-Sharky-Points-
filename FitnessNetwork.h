//
//  FitnessNetwork.h
//  Neural_Assignment_1_Take_3
//
//  Created by MacBook Pro on 10/10/15.
//  Copyright (c) 2015 Neural. All rights reserved.
//

#ifndef Neural_Assignment_1_Take_3_FitnessNetwork_h
#define Neural_Assignment_1_Take_3_FitnessNetwork_h

#include "Neuron.h"
#include "Evolution.h"
#include <fstream>
#include <ctime>


class FitnessNetwork
{
    
    
public:
    FitnessNetwork ();
    void init (int, vector<int>, vector<vector<double> > , vector<double>);
    double getMSEForChromosome(Chromosome);
    double test (Chromosome, int);
    double test (Chromosome, vector<double> );
    
    
private:
    int numLayers;
    vector<int> layersDescription;
    vector<double> targets;
    vector<vector<double> > inputs;
    vector<vector<Neuron> > neurons;
    double currentMSE;
    
    double getRandNumber (int,int);
    void updateMSE(int);
    
    Chromosome chromosome;
    
    
    /* Multi Layer Functions*/
    void initMultiLayer();
    void buildFitnessNetwork();
    void forwardPass(int);
    void forwardPass(vector<double>);
    void initInputLayer();
    /* ----------------------*/
    

    
    
    
};



FitnessNetwork :: FitnessNetwork ()
{
 
}

void FitnessNetwork :: init (int numLayers, vector<int> layersDescription, vector<vector<double> > inputs, vector<double> targets)
{
    srand(time(NULL));
    this->numLayers = numLayers;
    this->layersDescription = layersDescription;
    this->inputs = inputs;
    this->targets = targets;
    
    this->currentMSE = 1e9;
    
    
    if (numLayers>1){
        
        initMultiLayer();
    }
    else
    {
        
        initInputLayer();
    }
}




void FitnessNetwork :: initMultiLayer ()
{
    
    initInputLayer();
    buildFitnessNetwork();
    
}

void FitnessNetwork :: initInputLayer()
{
    neurons.resize(numLayers);
    
    for (int i=0; i<neurons.size(); i++)
    {
        neurons[i].resize(layersDescription[i]);
    }
    
    for (int i=0; i<neurons[0].size(); i++)
    {
        for (int j=0; j<inputs[0].size(); j++)
        {
            neurons[0][i].addEdge(inputs[0][j],getRandNumber(0,1));
        }
    }
}

void FitnessNetwork :: buildFitnessNetwork ()
{
    
    
    
    for (int i=1; i<neurons.size(); i++)
    {
        for (int j=0; j<neurons[i].size(); j++)
        {
           // neurons[i][j].addEdge(1, getRandNumber(0, 1));
        }
    }
    
    
    // propagate output from each layer to make output layer ready
    for (int i=0; i<neurons.size()-1; i++)
    {
        for (int j=0; j<neurons[i].size(); j++)
        {
            double neuronOut = neurons[i][j].fireNeuron();
            for (int k=0; k<neurons[i+1].size(); k++)
            {
                neurons[i+1][k].addEdge(neuronOut,getRandNumber(0,1));
            }
            
        }
    }
    
    // fire output layer
    for (int i=0; i<neurons[neurons.size()-1].size(); i++)
    {
        neurons[neurons.size()-1][i].fireNeuron();
    }
    
    
}




void FitnessNetwork :: forwardPass (int patternIdx)
{
    
    
    // first layer inputs
    int chromosomeIdx = 0;
    for (int i=0; i<neurons[0].size(); i++)
    {
        for (int j=0; j<inputs[patternIdx].size(); j++)
        {
            neurons[0][i].setEdge(j,inputs[patternIdx][j],this->chromosome.getGeneAt(chromosomeIdx++));
        }
    }
    // compute forward pass till last layer
    for (int i=0; i<neurons.size()-1; i++)
    {
        for (int j=0; j<neurons[i].size(); j++)
        {
            double neuronOut = neurons[i][j].fireNeuron();
            for (int k=0; k<neurons[i+1].size(); k++)
            {
                neurons[i+1][k].setEdge(j,neuronOut,this->chromosome.getGeneAt(chromosomeIdx++));
            }
        }
    }
    
    // fire output layer
    for (int i=0; i<neurons[neurons.size()-1].size(); i++)
    {
        neurons[neurons.size()-1][i].fireNeuron();
    }
}




void FitnessNetwork :: forwardPass (vector<double> inputData)
{
    
    
    // first layer inputs
    int chromosomeIdx = 0;
    for (int i=0; i<neurons[0].size(); i++)
    {
        for (int j=0; j<inputData.size(); j++)
        {
            neurons[0][i].setEdge(j,inputData[j],this->chromosome.getGeneAt(chromosomeIdx++));
        }
    }
    // compute forward pass till last layer
    for (int i=0; i<neurons.size()-1; i++)
    {
        for (int j=0; j<neurons[i].size(); j++)
        {
            double neuronOut = neurons[i][j].fireNeuron();
            for (int k=0; k<neurons[i+1].size(); k++)
            {
                neurons[i+1][k].setEdge(j,neuronOut,this->chromosome.getGeneAt(chromosomeIdx++));
            }
        }
    }
    
    // fire output layer
    for (int i=0; i<neurons[neurons.size()-1].size(); i++)
    {
        neurons[neurons.size()-1][i].fireNeuron();
    }
}




double FitnessNetwork:: getRandNumber(int LO, int HI)
{
    return(LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO))));
}

void FitnessNetwork :: updateMSE(int patternIdx)
{
    double sum = currentMSE;
    if (sum == 1e9) sum = 0;
    for (int i=0; i<neurons[neurons.size()-1].size(); i++)
    {
        double output = neurons[neurons.size()-1][i].getOutput();
        double target = targets[patternIdx];
        sum += (( output - target ) * (output - target) );
    }
    
    sum/=neurons[neurons.size()-1].size();
    
    //sum/= (patternIdx+1);
    currentMSE = sum;
    
}



double FitnessNetwork::test (Chromosome chromosome, int patternIdx)
{
    this->chromosome = chromosome;
    forwardPass(patternIdx);
    if ( neurons[neurons.size()-1][0].getOutput()>0) return 1;
    else return -1;
}


double FitnessNetwork::test (Chromosome chromosome, vector<double> testInput)
{
    this->chromosome = chromosome;
    forwardPass(testInput);
    if ( neurons[neurons.size()-1][0].getOutput()>0) return 1;
    else return -1;
}

double FitnessNetwork :: getMSEForChromosome(Chromosome chromosome)
{
    this->currentMSE = 0;
    this->chromosome = chromosome;
    for (int patternIdx=0; patternIdx<this->targets.size(); patternIdx++){
        forwardPass(patternIdx);
        updateMSE(patternIdx);
    }
    currentMSE/=targets.size();
    return this->currentMSE;
}






#endif
