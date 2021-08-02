// COMP 371 Assignment 2 team Last Minute Formed
// some colors selected using coolors.co

#ifndef colors_h
#define colors_h

glm::vec3 rgb(float r, float g, float b);

const glm::vec3 WHITE = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 BLACK = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 BLUE = rgb(0,0,255);
const glm::vec3 RED = rgb(255,0,0);
const glm::vec3 GREEN = rgb(0,255,0);
const glm::vec3 DARK_BLUE = rgb(38,70,83);
const glm::vec3 YELLOW = rgb(233,196,106);
const glm::vec3 ORANGE = rgb(244,162,97);
const glm::vec3 DARK_ORANGE = rgb(231,111,81);
const glm::vec3 TEAL = rgb(42, 157, 143);
const glm::vec3 LIGHT_GREEN = rgb(138,177,125);
const glm::vec3 FUSCHIA = rgb(255, 0, 128);
const glm::vec3 LIGHT_BLUE = rgb(95,204,232);
const glm::vec3 JET = rgb(52,52,52);
const glm::vec3 SPACE_CADET = rgb(50,50,75);
const glm::vec3 LIGHT_SPACE_CADET = rgb(47,48,97);
const glm::vec3 ST_PATS_BLUE = rgb(31,50,129);
const glm::vec3 EGYPT_BLUE = rgb(14,52,160);
const glm::vec3 PURPLE_NAVY = rgb(95,89,128);
const glm::vec3 MANATEE = rgb(159,156,176); //light purple


glm::vec3 rgb(float r, float g, float b)
{
    return glm::vec3(r/255,g/255,b/255);
}


#endif /* colors_h */
