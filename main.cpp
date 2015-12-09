
//
// Disclamer:
// ----------
//

// This code needs SFML to run
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resource, use the helper
// method resourcePath() from ResourcePath.hpp
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

// Here is a small helper for you ! Have a look.
#include "ResourcePath.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include "FitnessNetwork.h"
#include "HelperFunctions.h"
// Update the paths of the wave/line input text files in "HelperFunctions.h"

using namespace std;



int main(int, char const**)
{
    
    shape shapeType = WAVE;
    vector<int> layersDescription;
    layersDescription.push_back(7);
    layersDescription.push_back(1);
    ofstream OUT (rootPath + outputPaths[shapeType] );
    train(shapeType,-0.01,1000,500,2,100,mutationFunction,fitnessFunction, -1,1,OUT,layersDescription);
    
    /*
     
     void train (shape shapeType, double maxFitness, int popSize, int epochs, int networkMutationRate, int chromosomeMutationRate, mutationFunctionPtr mutationFunction, fitnessFunctionPtr fitnessFunction, double LB, double UB, ofstream& OUT, vector<int> layersDescription)
     
     */

    
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(400, 400), "SFML window");


    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }

        // Clear screen
        window.clear();
        
        
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        
        window.draw(sprite);

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}





