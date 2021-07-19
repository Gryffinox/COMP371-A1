//
//  Model.h
//  Assignment
//
//  Created by Amanda Simard on 2021-07-17.
//  Copyright © 2021 Concordia. All rights reserved.
//

#ifndef Model_h
#define Model_h

class Model
{
public:
    glm::vec3 positions[20];
    glm::vec3 positionsDefault[20];
    glm::vec3 colors[20];
    glm::vec3 center;
    float front;
    float numCubes;
    bool diffColors;
    bool unshuffle;
    
    Model()
    {
        glm::vec3 temp[] = {
            glm::vec3( -1.5f,  -1.5f, -5.0f),
            glm::vec3( -0.5f,  -0.5f, -4.0f),
            glm::vec3( 0.5f,  0.5f, -3.0f),
            glm::vec3( 1.5f,  1.5f, -2.f),
            glm::vec3( 1.5f,  0.5f, -1.f),
            glm::vec3( 1.5f,  -0.5f, 0.f),
            glm::vec3( 1.5f,  -1.5f, 1.f),
            glm::vec3( 0.5f,  -0.5f, 2.f),
            glm::vec3( -0.5f,  0.5f, 3.f),
            glm::vec3( -1.5f,  1.5f, 4.f),
        };
        
        
        numCubes = sizeof(temp)/sizeof(temp[0]);
        for (int i = 0; i<numCubes; i++)
        {
            positions[i] = temp[i];
            
            positionsDefault[i] = temp[i];
        }
        
        float maxX = 0.0f;
        float maxY = 0.0f;
        float maxZ = 0.0f;
        float minX = 0.0f;
        float minY = 0.0f;
        float minZ = 0.0f;
        for (int i = 0; i<numCubes; i++)
        {
            float iX = positions[i].x;
            float iY = positions[i].y;
            float iZ = positions[i].z;
            
            if (iX > maxX)
                maxX = iX;
            if (iY > maxY)
                maxY = iY;
            if (iZ > maxZ)
                maxZ = iZ;
            
            if (iX < minX)
                minX = iX;
            if (iY < minY)
                minY = iY;
            if (iZ < minZ)
                minZ = iZ;
        }
        float xHalfRange, yHalfRange, zHalfRange;
        xHalfRange = (maxX-minX)/2;
        yHalfRange = (maxY-minY)/2;
        zHalfRange = (maxZ-minZ)/2;
        center = glm::vec3((minX + xHalfRange), (minY + yHalfRange), (minZ + zHalfRange));
        front = zHalfRange;
        
        for (int i = 0; i<numCubes; i++)
        {
            positions[i].x -= center.x;
            positions[i].y -= center.y;
            positions[i].z -= center.z;
        }
        
        unshuffle = false;
    }
    
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
                    positions[i].z -= 0.5 * 2.5 * deltaTime;
                    allDefault = false;
                }
            }
            else{
                if (positions[i].z < front)
                {
                    allDefault = false;
                    positions[i].z += 0.5 * 2.5 * deltaTime;
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
    
    void draw(glm::vec3 position, int worldLoc, int startIndex, float scale)
    {
        for(int i = 0; i<numCubes; i++)
        {
            glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), (position + scale*(positions[i])));
            
            glm::mat4 worldMatrix =   translationMatrix * scalingMatrix;
            
            glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &worldMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, startIndex, 36); // 3 vertices, starting at index 0
        }
        
    }
};

#endif /* Model_h */
