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
private:
    float edgeThickness = 1.f;
    float wallDistance = 5.f;
public:
    glm::vec3* positions;               //Recentered coordinates of the model array pointer
    glm::vec3* positionsDefault;        //Original coordinates of the model array pointer
    glm::vec3* colors;
    glm::vec3 center;
    float front;
    int numCubes;
    bool diffColors;
    bool unshuffle;                     //Determines if the model is currently moving towards the shuffled shape or the regular shape

    glm::vec3* wallX;
    glm::vec3* wallY;
    glm::vec3* wallZ;

    int cubesWallX;
    int cubesWallY;
    int cubesWallZ;
    
    enum Axes{ xAxis = 1, yAxis = 2, zAxis = 3 };
    
    //Default constructor
    Model() {
        positions = new glm::vec3;
        positionsDefault = new glm::vec3;
        center = glm::vec3{ 0.0f, 0.0f, 0.0f };
        front = 0.0f;
        numCubes = 0;
        unshuffle = false;

        wallX = new glm::vec3;
        wallY = new glm::vec3;
        wallZ = new glm::vec3;
        cubesWallX = 0;
        cubesWallY = 0;
        cubesWallZ = 0;
    }

    //Main constructor with parameters
    Model(glm::vec3* originalModel, float size)
    {
        //initialize all variables
        unshuffle = false;
        //Dynmically set array size for new model
        numCubes = (int)size;
        positions = new glm::vec3[numCubes];
        positionsDefault = new glm::vec3[numCubes];

        //variables to keep track of min max in each axis. used to center model (and build wall)
        float maxX = originalModel[0].x;
        float maxY = originalModel[0].y;
        float maxZ = originalModel[0].z;
        float minX = originalModel[0].x;
        float minY = originalModel[0].y;
        float minZ = originalModel[0].z;

        //copy inputs into the objects parameters
        for (int i = 0; i<numCubes; i++)
        {
            positions[i] = originalModel[i];
            positionsDefault[i] = originalModel[i];
            //Calculate the physical center of the model based on the cubes on the outer most edge of each dimension
            //Find min in each dimension, store it
            if (positions[i].x > maxX)
                maxX = positions[i].x;
            if (positions[i].y > maxY)
                maxY = positions[i].y;
            if (positions[i].z > maxZ)
                maxZ = positions[i].z;
            //Find max in each dimension, store it
            if (positions[i].x < minX)
                minX = positions[i].x;
            if (positions[i].y < minY)
                minY = positions[i].y;
            if (positions[i].z < minZ)
                minZ = positions[i].z;
        }
        
        //Calculate average point (middle) from min/max
        float xHalfRange, yHalfRange, zHalfRange;
        xHalfRange = (maxX - minX) / 2;
        yHalfRange = (maxY - minY) / 2;
        zHalfRange = (maxZ - minZ) / 2;
        //Store center position as vector
        center = glm::vec3((minX + xHalfRange), (minY + yHalfRange), (minZ + zHalfRange));
        //For shuffle
        front = zHalfRange;         //sets to zHalfRange because positions are offset by (minZ + zHalfRange) 
                                    //zHalfRange cancels out leaving front to be same coord as minZ

        //===========================================================================
        //procedurally generate a wall model that comes with the model for all 3 axes
        //===========================================================================
        int cubesFlatX = numCubes, cubesFlatY = numCubes, cubesFlatZ = numCubes;    //counter of number of cubes for each wall
        //x wall -> y-z axis    y wall -> x-z axis      z wall -> x-y axis
        glm::vec3 *flatX = new glm::vec3[cubesFlatX];  //a vector that counts cubes that would be in the flattened version of the original model (flattened/shuffled) on X axis
        glm::vec3* flatY = new glm::vec3[cubesFlatY];  //same thing in the Y axis
        glm::vec3* flatZ = new glm::vec3[cubesFlatZ];  //Z axis
        cubesFlatX = cubesFlatY = cubesFlatZ = 0;
        //for each cube in the model
        for (int i = 0; i < numCubes; i++) {
            //add the cube to the flat X model
            flatX[cubesFlatX++] = positions[i];
            //for every existing cube in the flattened model
            for (int j = 0; j < cubesFlatX - 1; j++) {
                //if the cube just added to the flat model already exists in the flat model, decrement the cube counter
                //this way, inside flatX, the only cubes we have left are the unique cubes of the model on the YZ plane
                if (flatX[j].y == positions[i].y && flatX[j].z == positions[i].z) {
                    cubesFlatX--;
                    break;
                }
            }
            //repeat the logic for the flat Y model
            flatY[cubesFlatY++] = positions[i];
            for (int j = 0; j < cubesFlatY - 1; j++) {
                //in Y, only have unique cubes in XZ plane
                if (flatY[j].x == positions[i].x && flatY[j].z == positions[i].z) {
                    cubesFlatY--;
                    break;
                }
            }
            //z wall model
            flatZ[cubesFlatZ++] = positions[i];
            for (int j = 0; j < cubesFlatZ - 1; j++) {
                //unique cubes in XY plane
                if (flatZ[j].x == positions[i].x && flatZ[j].y == positions[i].y) {
                    cubesFlatZ--;
                    break;
                }
            }
        }
        //each wall model is slightly bigger than the original model
        //thus for wall on X axis, (y+edgeThickness)*(z+edgeThickness) in dimension.
        //same thing for Y and Z axis walls. then subtract cubes for size of hole from flat model
        cubesWallX = ((maxY - minY) + (edgeThickness * 2 + 1)) * ((maxZ - minZ) + (edgeThickness * 2 + 1)) - cubesFlatX;
        cubesWallY = ((maxX - minX) + (edgeThickness * 2 + 1)) * ((maxZ - minZ) + (edgeThickness * 2 + 1)) - cubesFlatY;
        cubesWallZ = ((maxX - minX) + (edgeThickness * 2 + 1)) * ((maxY - minY) + (edgeThickness * 2 + 1)) - cubesFlatZ;
        wallX = new glm::vec3[cubesWallX];
        wallY = new glm::vec3[cubesWallY];
        wallZ = new glm::vec3[cubesWallZ];
        //for each wall, add a cube to the model
        //X wall
        int counter = 0;
        for (float i = minY - (edgeThickness); i < maxY + (edgeThickness + 1); i++) {
            for (int j = minZ - (edgeThickness); j < maxZ + (edgeThickness + 1); j++) {
                wallX[counter++] = glm::vec3(center.x , i, j);
                for (int k = 0; k < cubesFlatX; k++) {
                    if (wallX[counter - 1].y == flatX[k].y && wallX[counter - 1].z == flatX[k].z) {
                        counter--;
                        break;
                    }
                }
            }
        }
        //Y wall
        counter = 0;
        for (float i = minX - (edgeThickness); i < maxX + (edgeThickness + 1); i++) {
            for (int j = minZ - (edgeThickness); j < maxZ + (edgeThickness + 1); j++) {
                wallY[counter++] = glm::vec3(i, center.y, j);
                for (int k = 0; k < cubesFlatY; k++) {
                    if (wallY[counter - 1].x == flatY[k].x && wallY[counter - 1].z == flatY[k].z) {
                        counter--;
                        break;
                    }
                }
            }
        }
        //Z wall
        counter = 0;
        for (float i = minX - (edgeThickness); i < maxX + (edgeThickness + 1); i++) {
            for (int j = minY - (edgeThickness); j < maxY + (edgeThickness + 1); j++) {
                wallZ[counter++] = glm::vec3(i, j, center.z);
                for (int k = 0; k < cubesFlatZ; k++) {
                    if (wallZ[counter - 1].x == flatZ[k].x && wallZ[counter - 1].y == flatZ[k].y) {
                        counter--;
                        break;
                    }
                }
            }
        }

        //Modify model coordinates to be relative to it's center point instead
        for (int i = 0; i < numCubes; i++) {
            positions[i] -= center;
            positionsDefault[i] -= center;
        }
        for (int i = 0; i < cubesWallX; i++) {
            //for the walls, displace them by an additional (half width + wall distance)
            wallX[i] -= center + glm::vec3(xHalfRange + wallDistance, .0f, .0f);
        }
        for (int i = 0; i < cubesWallY; i++) {
            wallY[i] -= center + glm::vec3(.0f, yHalfRange + wallDistance, .0f);
        }
        for (int i = 0; i < cubesWallZ; i++) {
            wallZ[i] -= center + glm::vec3(.0f, .0f, zHalfRange + wallDistance);
        }
    }
    
    //Shuffle function which "flattens" the model
    void shuffle(float deltaTime)
    {
        //all cubes at front and shuffled on to each other
        bool allFront = true;
        //all cubes returned to original position?
        bool allDefault = true;
        for (int i = 0; i<numCubes; i++)
        {
            //Move all the cubes back to their original positions
            if(unshuffle)
            {
                allFront = false;
                if (positions[i].z > positionsDefault[i].z)
                {
                    positions[i].z -= shuffleSpeed * deltaTime;
                    allDefault = false;
                }
            }
            //We're shuffling all the cubes on to each other
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
        //if cubes in default position, start shuffling them
        if (allDefault)
        {
            unshuffle = false;
        }
        //if all cubes reached the front, toggle flag to move them back
        if (allFront)
        {
            unshuffle = true;
        }
    }
    
    //Draw the model
    //Pass in each transformation as a set of coordinates except for scale which applies to all dimensions
    void draw(int worldLoc, int startIndex, float scale, glm::vec3 coordinates, glm::vec3 angles)
    {
        for(int i = 0; i<numCubes; i++)
        {
            //set each transformation matrix based on input variables
            glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
            //glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), coordinates + scale*(positions[i]));
            glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), scale * (positions[i]));
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), coordinates);
            glm::mat4 rotationMatrix = 
                glm::rotate(glm::mat4(1.0f), glm::radians(angles.x), glm::vec3(1.f, .0f, .0f)) * 
                glm::rotate(glm::mat4(1.0f), glm::radians(angles.y), glm::vec3(.0f, 1.f, .0f)) *
                glm::rotate(glm::mat4(1.0f), glm::radians(angles.z), glm::vec3(.0f, .0f, 1.f)
            );
            //Scale first, place all cubes relative to center of model, rotate them on center, then move out to proper place
            glm::mat4 worldMatrix = translationMatrix * rotationMatrix * positionMatrix * scalingMatrix;
            glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &worldMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, startIndex, NUM_CUBE_VERTICES); // 36 vertices, starting at index 0
        }
        
    }

    //axis is 1, 2 or 3 matchin x y or z
    void drawWall(int worldLoc, int startIndex, float scale, glm::vec3 coordinates, glm::vec3 angles, int axis) {
        switch (axis) {
        case xAxis:
            for (int i = 0; i < cubesWallX; i++) {
                //set each transformation matrix based on input variables
                glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
                //glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), coordinates + scale*(positions[i]));
                glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), scale * (wallX[i]));
                glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), coordinates);
                glm::mat4 rotationMatrix =
                    glm::rotate(glm::mat4(1.0f), glm::radians(angles.x), glm::vec3(1.f, .0f, .0f)) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(angles.y), glm::vec3(.0f, 1.f, .0f)) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(angles.z), glm::vec3(.0f, .0f, 1.f)
                    );
                //Scale first, place all cubes relative to center of model, rotate them on center, then move out to proper place
                glm::mat4 worldMatrix = translationMatrix * rotationMatrix * positionMatrix * scalingMatrix;
                glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &worldMatrix[0][0]);
                glDrawArrays(GL_TRIANGLES, startIndex, NUM_CUBE_VERTICES); // 36 vertices, starting at index 0
            }
            break;
        case yAxis:
            for (int i = 0; i < cubesWallY; i++) {
                //set each transformation matrix based on input variables
                glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
                //glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), coordinates + scale*(positions[i]));
                glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), scale * (wallY[i]));
                glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), coordinates);
                glm::mat4 rotationMatrix =
                    glm::rotate(glm::mat4(1.0f), glm::radians(angles.x), glm::vec3(1.f, .0f, .0f)) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(angles.y), glm::vec3(.0f, 1.f, .0f)) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(angles.z), glm::vec3(.0f, .0f, 1.f)
                    );
                //Scale first, place all cubes relative to center of model, rotate them on center, then move out to proper place
                glm::mat4 worldMatrix = translationMatrix * rotationMatrix * positionMatrix * scalingMatrix;
                glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &worldMatrix[0][0]);
                glDrawArrays(GL_TRIANGLES, startIndex, NUM_CUBE_VERTICES); // 36 vertices, starting at index 0
            }
            break;
        case zAxis:
            for (int i = 0; i < cubesWallZ; i++) {
                //set each transformation matrix based on input variables
                glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
                //glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), coordinates + scale*(positions[i]));
                glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), scale * (wallZ[i]));
                glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), coordinates);
                glm::mat4 rotationMatrix =
                    glm::rotate(glm::mat4(1.0f), glm::radians(angles.x), glm::vec3(1.f, .0f, .0f)) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(angles.y), glm::vec3(.0f, 1.f, .0f)) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(angles.z), glm::vec3(.0f, .0f, 1.f)
                    );
                //Scale first, place all cubes relative to center of model, rotate them on center, then move out to proper place
                glm::mat4 worldMatrix = translationMatrix * rotationMatrix * positionMatrix * scalingMatrix;
                glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &worldMatrix[0][0]);
                glDrawArrays(GL_TRIANGLES, startIndex, NUM_CUBE_VERTICES); // 36 vertices, starting at index 0
            }
            break;
        default:
            break;
        }
    }

};

#endif /* Model_h */
