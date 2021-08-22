//
// COMP 371 PrOJeCt asSiGnMenT by team LastMinuteFormed
//
// Built on in-class Lab Framework
// And built on ressources from https://learnopengl.com
//

#ifndef texture_h
#define texture_h

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//Globals
bool drawTextures;
enum Texture { Glossy, Concrete, SkyboxT, SkyboxL, SkyboxFr, SkyboxR, SkyboxB, SkyboxFl, Metal, Road, Border };

//Texture ints
GLuint glossyTexture, concreteTexture, skyboxTextureT, skyboxTextureL, skyboxTextureFr, skyboxTextureR, skyboxTextureB, skyboxTextureFl, metalTexture, roadTexture, borderTexture;

//Functions
unsigned int loadTexture(char const* path, unsigned int* textureID) {
	glGenTextures(1, textureID);
	glBindTexture(GL_TEXTURE_2D, *textureID);
	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data) {

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}
	stbi_image_free(data);

	return *textureID;
}

// set Texture and changing it depending on input
void setTexture(Texture text, Shader aShader) {
	aShader.use();
	switch (text) {
	case Texture::Glossy:
		aShader.setBool("colorOn", !drawTextures);
		aShader.setBool("textureOn", drawTextures);
		aShader.setInt("tex", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, glossyTexture);
		break;
	case Texture::Concrete:
		aShader.setBool("colorOn", !drawTextures);
		aShader.setBool("textureOn", drawTextures);
		aShader.setInt("tex", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, concreteTexture);
        break;
    case Texture::SkyboxT:
        aShader.setBool("colorOn", !drawTextures);
        aShader.setBool("textureOn", drawTextures);
        aShader.setInt("tex", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, skyboxTextureT);
        break;
    case Texture::SkyboxL:
        aShader.setBool("colorOn", !drawTextures);
        aShader.setBool("textureOn", drawTextures);
        aShader.setInt("tex", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, skyboxTextureL);
        break;
    case Texture::SkyboxFr:
        aShader.setBool("colorOn", !drawTextures);
        aShader.setBool("textureOn", drawTextures);
        aShader.setInt("tex", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, skyboxTextureFr);
        break;
    case Texture::SkyboxR:
        aShader.setBool("colorOn", !drawTextures);
        aShader.setBool("textureOn", drawTextures);
        aShader.setInt("tex", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, skyboxTextureR);
        break;
    case Texture::SkyboxB:
        aShader.setBool("colorOn", !drawTextures);
        aShader.setBool("textureOn", drawTextures);
        aShader.setInt("tex", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, skyboxTextureB);
        break;
    case Texture::SkyboxFl:
        aShader.setBool("colorOn", !drawTextures);
        aShader.setBool("textureOn", drawTextures);
        aShader.setInt("tex", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, skyboxTextureFl);
        break;
    case Texture::Road:
        aShader.setBool("colorOn", !drawTextures);
        aShader.setBool("textureOn", drawTextures);
        aShader.setInt("tex", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, roadTexture);
        break;
    case Texture::Metal:
        aShader.setBool("colorOn", !drawTextures);
        aShader.setBool("textureOn", drawTextures);
        aShader.setInt("tex", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, metalTexture);
        break;
    case Texture::Border:
        aShader.setBool("colorOn", !drawTextures);
        aShader.setBool("textureOn", drawTextures);
        aShader.setInt("tex", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, borderTexture);
        break;
	default:
		std::cout << "Unknown or no textures specified, defaulting to colors only";
		aShader.setBool("colorOn", true);
		aShader.setBool("textureOn", false);
		break;
	}
}

#endif
