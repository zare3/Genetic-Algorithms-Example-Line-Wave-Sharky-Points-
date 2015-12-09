//
//  HelperFunctions.h
//  Neural Assignment 4
//
//  Created by MacBook Pro on 12/6/15.
//  Copyright (c) 2015 AUC. All rights reserved.
//

#ifndef Neural_Assignment_4_HelperFunctions_h
#define Neural_Assignment_4_HelperFunctions_h



enum shape {LINE,WAVE};
string paths [2] = {"line.txt", "wave.txt"};
string rootPath = "/Users/Home/AUC/Fall 15/Neural/Assignments/Genetic Algorithms With Visualisation/Genetic Algorithms With Visualisation/";
string outputPaths[2] = {"outputLine.txt","outputWave.txt"};
int numLayers;
vector<int> layersDescription;
double EETA, targetMSE, maxEpochCnt;
FitnessNetwork network;
vector<double> targets;
vector<vector<double>> inputs;

sf::Texture texture;
sf::Sprite sprite;
sf::Image image;




double fitnessFunction (Chromosome* ch);
double mutationFunction (double val, int epochIdx, int epochCnt);
void buildNetwork (shape shapeType);
double getMCCR (Chromosome);
double delta (double t, double y, int epochCnt);
int getChromosomeSize();
void train ();
double getRandDouble (double fMin, double fMax);
void visualise (Chromosome ch);




void train (shape shapeType, double maxFitness, int popSize, int epochs, int networkMutationRate, int chromosomeMutationRate, mutationFunctionPtr mutationFunction, fitnessFunctionPtr fitnessFunction, double LB, double UB, ofstream& OUT, vector<int>& layersDescription)
{
    srand(time(NULL));
    
    double MCCR = 0;
    ::layersDescription = layersDescription;
    buildNetwork(shapeType);
    int chromosomeSize = getChromosomeSize(); // 146
    
    
    image.create(400, 400, sf::Color ::White);
    
    
    Evolution evolution (maxFitness, epochs,networkMutationRate);
    
    for (int i=0; i<popSize; i++){
        
        Chromosome ch (chromosomeMutationRate, mutationFunction,fitnessFunction);
        for (int i=0; i<chromosomeSize; i++){
            double rnd = getRandDouble(LB, UB);
            ch.addGene(rnd);
        }
        evolution.addChromosome(ch);
    }
    
    
    
    
    OUT << "numOfLayers: " << layersDescription.size() << endl;
    OUT << "AS FOLLOWS: " << endl;
    for (int i=0; i<layersDescription.size(); i++) OUT << layersDescription[i] << ' ';
    OUT << endl << "POP SIZE: " << popSize << endl;
    OUT << "MAX FITNESS: " << maxFitness << endl;
    
    
    
    Chromosome target = evolution.train(&OUT);
    visualise(target);
    MCCR = getMCCR(target);
    OUT << endl << "--------------------------" << endl;
    
    OUT << "ARCHITECTURE: " << endl;
    OUT << "MCCR: " << MCCR <<endl;
    OUT << "NUM OF LAYERS " << layersDescription.size()<<":  " << endl;
    OUT << '\t';
    for (int i=0; i<layersDescription.size(); i++) OUT << layersDescription[i]<<' ';
    OUT << endl;
    OUT << "MAX EPOCHS: " << epochs << endl;
    OUT << "MAX FITNESS: " << maxFitness << endl;
    OUT << "NETWORK MUTATION RATE: " << networkMutationRate << endl;
    OUT << "CHROMOSOME MUTATION RATE: " << chromosomeMutationRate << endl;
    
    OUT << "---------------------------" << endl;
    OUT << "FINAL CHROMOSOME: " << endl;
    
    for (int i=0; i<target.size(); i++)
    {
        OUT << target.getGeneAt(i) << ' ';
    }
    OUT.close();
    cout << MCCR << endl;
}


double getRandDouble (double fMin, double fMax)
{
    
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}





int getChromosomeSize ()
{
    int size = layersDescription[0] * 3;
    for (int i=0; i<layersDescription.size()-1; i++)
    {
        size += (layersDescription[i+1] * layersDescription[i]  );
    }
    return size;
}


double getMCCR (Chromosome ch)
{
    
    
    double CCR1 = 0;
    double CCR2 = 0;
    
    double CCR1Cnt = 0;
    double CCR2Cnt = 0;
    
    for (int i=0; i<targets.size(); i++){
        
        double output = network.test(ch, i);
        
        cout << "OUTPUT: " << output << " TARGET: " << targets[i] << endl;
        
        if (targets[i] == 1){
            CCR1Cnt++;
            CCR1+=(output == 1);
        }
        else {
            
            CCR2Cnt++;
            CCR2+= (output == -1);
        }
    }
    
    cout << CCR1 << ' ' << CCR2 << endl;
    
    return min (CCR1/CCR1Cnt,CCR2/CCR2Cnt) * 100;
}

void visualise (Chromosome ch)
{
    // test 400*400 points and draw them
    vector<double> testInput(3);
    for (int i=0; i<400; i++)
    {
        for (int j=0; j<400; j++)
        {
            testInput[0] = (i/200.0)-1;
            testInput[1] = (j/200.0)-1;
            testInput[2] = 1;
            
            double test = network.test(ch,testInput);
            if (test<0) image.setPixel(i, j, sf::Color::Yellow);
            else image.setPixel(i, j, sf::Color::White);
        }
    }
    
    for (int i=0; i<inputs.size(); i++){
        double x = inputs[i][0];
        double y = inputs[i][1];
        
        x++; x*=200;
        y++; y*=200;
        
        if (targets[i] == 1){
            image.setPixel(x, y, sf::Color::Blue);
            image.setPixel(x+1, y, sf::Color::Blue);
            image.setPixel(x-1, y, sf::Color::Blue);
            image.setPixel(x, y+1, sf::Color::Blue);
            image.setPixel(x, y-1, sf::Color::Blue);
            
        }
        else{
            image.setPixel(x, y, sf::Color::Red);
            image.setPixel(x+1, y, sf::Color::Red);
            image.setPixel(x-1, y, sf::Color::Red);
            image.setPixel(x, y+1, sf::Color::Red);
            image.setPixel(x, y-1, sf::Color::Red);
        }
        
    }
    
}



double fitnessFunction (Chromosome* ch)
{
    return -network.getMSEForChromosome(*ch);
}


double mutationFunction (double val, int epochIdx, int epochCnt){
    
    const double UB = 100;
    const double LB = -100;
    
    double t = epochIdx;
    
    int prob = rand () % 2;
    
    if ( prob == 0 )
        return val + delta (t,UB-val,epochCnt);
    else
        return val - delta (t,val-LB,epochCnt);
    
}

double delta (double t, double y, int epochCnt)
{
    double r = getRandDouble(0,1);
    int T = epochCnt;
    double b = 5;
    return y * (1-pow(r, pow (   (1 - (t/T)),b   ) ) );
}


void buildNetwork (shape shapeType)
{
    string shapeFilePath="";
    double x,y,targ;
    vector<double> singlePattern(3);
    targets.clear();
    inputs.clear();
    ifstream IN;
    
    
    shapeFilePath += rootPath;
    shapeFilePath+=paths[shapeType];
    
    
    IN.open(shapeFilePath.c_str());
    
    if (IN.fail()){
        throw "FAILED TO OPEN FILE";
    }
    
    
    numLayers = layersDescription.size();
    //layersDescription.push_back(5); // 15
    //layersDescription.push_back(1); // 5
    
    EETA = 0.01;
    targetMSE = 0.003;
    maxEpochCnt = 100000;
    
    
    
    IN >> x;
    while (!IN.eof()){
        IN >> y >> targ;
        
        singlePattern[0] = x;
        singlePattern[1] = y;
        singlePattern[2] = 1;
        
        inputs.push_back(singlePattern);
        
        if (targ==2)
            targets.push_back(-1);
        else targets.push_back(1);
        
        IN >> x;
        
    }
    network.init(numLayers, layersDescription, inputs, targets);
}





#endif
