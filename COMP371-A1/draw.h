//
// COMP 371 PrOJeCt asSiGnMenT by team LastMinuteFormed
//
// Built on in-class Lab Framework
// And built on ressources from https://learnopengl.com
//

#ifndef draw_h
#define draw_h

void DrawAll(Shader theShader) {
	/*glBindVertexArray();
	DrawModel();*/
}

void DrawModel(Shader theShader) {
	//draw from global array
	tempModel.draw(theShader, 0);
}

#endif