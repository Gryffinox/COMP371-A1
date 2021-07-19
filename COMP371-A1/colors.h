//
//  colors.h
//  Assignment
//
//  Created by Amanda Simard on 2021-07-17.
//  Copyright © 2021 Concordia. All rights reserved.
//

#ifndef colors_h
#define colors_h

float rgbValue(float c);

const glm::vec3 WHITE = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 DARK_BLUE = glm::vec3(rgbValue(38), rgbValue(70), rgbValue(83));
const glm::vec3 YELLOW = glm::vec3(rgbValue(233), rgbValue(196), rgbValue(106));
const glm::vec3 ORANGE = glm::vec3(rgbValue(244), rgbValue(162), rgbValue(97));
const glm::vec3 BLUE = glm::vec3(rgbValue(0), rgbValue(0), rgbValue(255));
const glm::vec3 RED = glm::vec3(rgbValue(255), rgbValue(0), rgbValue(0));
const glm::vec3 GREEN = glm::vec3(rgbValue(0), rgbValue(255), rgbValue(0));

float rgbValue(float c)
{
    return c/255.0f;
}


#endif /* colors_h */
