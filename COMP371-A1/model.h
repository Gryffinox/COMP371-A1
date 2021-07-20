//
//  Model.h
//  Assignment
//
//  Created by Amanda Simard on 2021-07-17.
//  Copyright © 2021 Concordia. All rights reserved.
//

#ifndef Model_h
#define Model_h

const float shuffleSpeed = 2.5f;
const int NUM_CUBE_VERTICES = 36;

class Model
{
public:
    glm::vec3* positions;               //Recentered coordinates of the model array pointer
    glm::vec3* positionsDefault;        //Original coordinates of the model array pointer
    glm::vec3* colors;
    glm::vec3 center;
    float front;
    float numCubes;
    bool diffColors;
    bool unshuffle;                     //Determines if the model is currently moving towards the shuffled shape or the regular shape
    
    Model() {
        positions = new glm::vec3;
        positionsDefault = new glm::vec3;
        colors = new glm::vec3;
        front = 0.0f;
        numCubes = 0.0f;
        diffColors = false;
        unshuffle = false;
    }

    Model(glm::vec3* temp, float size)
    {
        colors = new glm::vec3;
        diffColors = false;
        //Dynmically set array size for new model
        numCubes = size;
        positions = new glm::vec3[numCubes];
        positionsDefault = new glm::vec3[numCubes];
        //copy inputs into the objects parameters
        for (int i = 0; i<numCubes; i++)
        {
            positions[i] = temp[i];
            positionsDefault[i] = temp[i];
        }
        
        //Calculate the physical center of the model based on the cubes on the outer most edge of each dimension
        float maxX = positions[0].x;
        float maxY = positions[0].y;
        float maxZ = positions[0].z;
        float minX = positions[0].x;
        float minY = positions[0].y;
        float minZ = positions[0].z;
        //Find min and max cube in each dimension, store it
        for (int i = 0; i<numCubes; i++)
        {
            if (positions[i].x > maxX)
                maxX = positions[i].x;
            if (positions[i].y > maxY)
                maxY = positions[i].y;
            if (positions[i].z > maxZ)
                maxZ = positions[i].z;
            
            if (positions[i].x < minX)
                minX = positions[i].x;
            if (positions[i].y < minY)
                minY = positions[i].y;
            if (positions[i].z < minZ)
                minZ = positions[i].z;
        }
        //Calculate average point (middle) from min/max
        float xHalfRange, yHalfRange, zHalfRange;
        xHalfRange = (maxX-minX)/2;
        yHalfRange = (maxY-minY)/2;
        zHalfRange = (maxZ-minZ)/2;
        //Store center position as vector
        center = glm::vec3((minX + xHalfRange), (minY + yHalfRange), (minZ + zHalfRange));
        //For shuffle
        front = zHalfRange;         //sets to zHalfRange because positions are offset by (minZ + zHalfRange) 
                                    //zHalfRange cancels out leaving front to be same coord as minZ
        
        //Modify model coordinates to be relative to it's center point instead "normalize" the coordinates
        for (int i = 0; i<numCubes; i++)
        {
            positions[i] -= center;
            positionsDefault[i] -= center;
        }
        
        unshuffle = false;
    }
    
    //Shuffle function which "flattens" the model
    void shuffle(float deltaTime)
    {
        bool allFront = true;
        bool allDefault = true;
        for (int i = 0; i<numCubes; i++)
        {
            if(unshuffle)
            {
                allFront = false;
                if (positions[i].z > positionsDefault[i].z)
                {
                    positions[i].z -= shuffleSpeed * deltaTime;
                    allDefault = false;
                }
            }
            else
            {
                if (positions[i].z < front)
                {
                    allDefault = false;
                    positions[i].z += shuffleSpeed * deltaTime;
                    allFront = false;
                }
            }
        }
        if (allDefault)
        {
            unshuffle = false;
        }
        if (allFront)
        {
            unshuffle = true;
        }
    }
    
    void draw(int worldLoc, int startIndex, float scale, glm::vec3 coordinates, glm::vec3 angles) //add angle
    {
        for(int i = 0; i<numCubes; i++)
        {
            //set each transformation matrix based on input variables
            glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), (coordinates + scale*(positions[i])));
            glm::mat4 rotationMatrix = 
                glm::rotate(glm::mat4(1.0f), glm::radians(angles.x), glm::vec3(1.f, .0f, .0f)) * 
                glm::rotate(glm::mat4(1.0f), glm::radians(angles.y), glm::vec3(.0f, 1.f, .0f)) *
                glm::rotate(glm::mat4(1.0f), glm::radians(angles.z), glm::vec3(.0f, .0f, 1.f)
            );
            //calculate final transformation
            glm::mat4 worldMatrix = rotationMatrix * translationMatrix * scalingMatrix;
            glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &worldMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, startIndex, NUM_CUBE_VERTICES); // 3 vertices, starting at index 0
        }
        
    }

};

#endif /* Model_h */
