//  COMP 371 Assignment 2 team Last Minute Formed

#ifndef Model_h
#define Model_h

class Model {
private:
	static const int kNumCubeVertices = 36;
	static const float kShuffleSpeed;       //static const initalized at the bottom of the file
	static const float kEdgeThickness;      //how many cubes border the edge of the wall
	static const float kWallDistance;       //how far does the wall generate from the model by default

	glm::vec3* positions_;                  //Array of centered coordinates of the model
	glm::vec3* positions_default_;          //Array of original coordinates of the model
	glm::vec3 center_;                      //Center of model

	glm::vec3* wall_x_;                       //array of the wall model on the x axis
	glm::vec3* wall_y_;                       //array on y axis
	glm::vec3* wall_z_;                       //array on z axis

	//counters for the number of cubes in the models
	int num_cubes_;
	int num_cube_wall_x_;
	int num_cube_wall_y_;
	int num_cube_wall_z_;

	//For shuffling the model
	float model_front_;                     //Coordinate position of the "front" of the model on the z-axis
	bool unshuffle_;                         //Determines if the model is currently moving towards the shuffled shape or the regular shape

public:
	//transformations on the model
	glm::vec3 model_translation_vector_;
	glm::vec3 model_rotation_vector_;
	glm::vec3 model_rotation_wout_wall_;
	float model_scale_;

	//For selecting which wall to draw
	static enum Axes { xAxis = 1, yAxis = 2, zAxis = 3 };

	//Default constructor
	Model() {
		//initialize private members
		positions_ = new glm::vec3;
		positions_default_ = new glm::vec3;
		center_ = glm::vec3{ 0.0f, 0.0f, 0.0f };

		wall_x_ = new glm::vec3;
		wall_y_ = new glm::vec3;
		wall_z_ = new glm::vec3;

		num_cubes_ = 0;
		num_cube_wall_x_ = 0;
		num_cube_wall_y_ = 0;
		num_cube_wall_z_ = 0;

		model_front_ = 0.0f;
		unshuffle_ = false;
		//initialize public members
		model_translation_vector_ = glm::vec3{ 0.0f, 0.0f, 0.0f };
		model_rotation_vector_ = glm::vec3{ 0.0f, 0.0f, 0.0f };
		model_rotation_wout_wall_ = glm::vec3{ 0.0f, 0.0f, 0.0f };
		model_scale_ = 1;
	}

	//TODO: why is destructor being called when object is not going out of scope
	~Model() {
		/*delete[] positions_;
		delete[] positions_default_;
		delete[] wall_x_;
		delete[] wall_y_;
		delete[] wall_z_;*/
	}

	Model(const char* filepath) {
		//initialize variables
		center_ = glm::vec3{ 0.0f, 0.0f, 0.0f };
		//transformations
		model_translation_vector_ = glm::vec3{ 0.0f, 0.0f, 0.0f };
		model_rotation_vector_ = glm::vec3{ 0.0f, 0.0f, 0.0f };
		model_rotation_wout_wall_ = glm::vec3{ 0.0f, 0.0f, 0.0f };
		model_scale_ = 1;
		//Wall Stuff
		num_cube_wall_x_ = 0;
		num_cube_wall_y_ = 0;
		num_cube_wall_z_ = 0;
		//shuffle
		model_front_ = 0.0f;
		unshuffle_ = false;

		//read in file and count cubes
		std::string file_content = "";
		std::ifstream model_filestream;
		// ensure ifstream objects can throw exceptions:

		// open files to read
		model_filestream.open(filepath);
		//get line count
		num_cubes_ = 0;
		std::string line;
		while (std::getline(model_filestream, line)) {
			++num_cubes_;
			file_content += line;
		}
		// read file's buffer contents into stream
		// close file handlers
		model_filestream.close();

		//Set size of arrays based on number of cubes read in input
		positions_ = new glm::vec3[num_cubes_];
		positions_default_ = new glm::vec3[num_cubes_];

		float x = 0, y = 0, z = 0;
		size_t end;
		std::string token = file_content;

		//parse string to set positions
		//glm::vec3* original_model = new glm::vec3[num_cubes_];
		//string contains coordinates delimited by comma, every 3 values represent xyz coords of one cube
		for (int i = 0; i < num_cubes_; i++) {
			end = token.find(",");					//find next comma
			x = std::stof(token.substr(0, end));	//take substr until comma and convert to float and store
			token = token.substr(end + 1);			//set token equal to new substring without the first value of string 
			//repeat for y and z
			end = token.find(",");
			y = std::stof(token.substr(0, end));
			token = token.substr(end + 1);
			end = token.find(",");
			z = std::stof(token.substr(0, end));
			token = token.substr(end + 1);
			//set the vector for a single cube in the array of vectors
			positions_[i] = glm::vec3(x, y, z);
			positions_default_[i] = glm::vec3(x, y, z);
		}

		SetModelData();

	}

	void SetModelData() {
		//===========================================================================
		//Calculate model center and recenter model
		//===========================================================================
		//variables to keep track of min max in each axis. used to center model (and build wall)
		float max_x = positions_[0].x;
		float max_y = positions_[0].y;
		float max_z = positions_[0].z;
		float min_x = positions_[0].x;
		float min_y = positions_[0].y;
		float min_z = positions_[0].z;

		//Calculate the physical center of the model based on the cubes on the outer most edge of each dimension
		for (int i = 0; i < num_cubes_; i++) {
			//Find min in each dimension, store it
			if (positions_[i].x > max_x)
				max_x = positions_[i].x;
			if (positions_[i].y > max_y)
				max_y = positions_[i].y;
			if (positions_[i].z > max_z)
				max_z = positions_[i].z;
			//Find max in each dimension, store it
			if (positions_[i].x < min_x)
				min_x = positions_[i].x;
			if (positions_[i].y < min_y)
				min_y = positions_[i].y;
			if (positions_[i].z < min_z)
				min_z = positions_[i].z;
		}

		//Calculate average point (middle) from min/max
		float x_half_range, y_half_range, z_half_range;
		x_half_range = (max_x - min_x) / 2;
		y_half_range = (max_y - min_y) / 2;
		z_half_range = (max_z - min_z) / 2;

		//Store center position as vector
		center_ = glm::vec3((min_x + x_half_range), (min_y + y_half_range), (min_z + z_half_range));

		//For shuffle
		model_front_ = z_half_range;         //sets to zHalfRange because positions are offset by (minZ + zHalfRange)
		//zHalfRange cancels out leaving front to be same coord as minZ

		//===========================================================================
		//procedurally generate a wall model that comes with the model for all 3 axes
		//===========================================================================
		//each wall model is slightly bigger than the original model
		//thus the wall on X axis is of (y + kEdgeThickness)*(z + kEdgeThickness) dimension.
		//same thing for Y and Z axis walls.
		num_cube_wall_x_ = ((max_y - min_y) + (kEdgeThickness * 2 + 1)) * ((max_z - min_z) + (kEdgeThickness * 2 + 1));
		num_cube_wall_y_ = ((max_x - min_x) + (kEdgeThickness * 2 + 1)) * ((max_z - min_z) + (kEdgeThickness * 2 + 1));
		num_cube_wall_z_ = ((max_x - min_x) + (kEdgeThickness * 2 + 1)) * ((max_y - min_y) + (kEdgeThickness * 2 + 1));
		//set wall model to new array of appropriate size dimension1 * dimension2 size
		wall_x_ = new glm::vec3[num_cube_wall_x_];
		wall_y_ = new glm::vec3[num_cube_wall_y_];
		wall_z_ = new glm::vec3[num_cube_wall_z_];
		//for each wall, add a cube to the model
		//X wall
		int counter = 0;
		//runtime is O(d1 * d2 * n) where d1 * d2 is some dimensions pertaining to the model when flat
		//in worse case, d1*d2 is of n*3, and in best case, sqrt(n) so runtime is about O(n^2)
		//traverse along the Y axis
		for (float i = min_y - (kEdgeThickness); i < max_y + (kEdgeThickness + 1); i++) {
			//traverse along the z axis
			for (float j = min_z - (kEdgeThickness); j < max_z + (kEdgeThickness + 1); j++) {
				//add the cube at (y, z) to the wall model
				wall_x_[counter++] = glm::vec3(center_.x, i, j);
				//if the cube we just added is in the flat model, remove it from the wall model so that there's a hole
				for (int k = 0; k < num_cubes_; k++) {
					if (wall_x_[counter - 1].y == positions_[k].y && wall_x_[counter - 1].z == positions_[k].z) {
						counter--;
						break;
					}
				}
			}
		}
		//counter counted the number of cubes we added to the wall
		num_cube_wall_x_ = counter;
		//resize wall array
		glm::vec3* temp = new glm::vec3[num_cube_wall_x_];
		memcpy(temp, wall_x_, num_cube_wall_x_ * sizeof(glm::vec3));
		delete[] wall_x_;
		wall_x_ = temp;
		//Y wall
		counter = 0;
		for (float i = min_x - (kEdgeThickness); i < max_x + (kEdgeThickness + 1); i++) {
			for (float j = min_z - (kEdgeThickness); j < max_z + (kEdgeThickness + 1); j++) {
				wall_y_[counter++] = glm::vec3(i, center_.y, j);
				for (int k = 0; k < num_cubes_; k++) {
					if (wall_y_[counter - 1].x == positions_[k].x && wall_y_[counter - 1].z == positions_[k].z) {
						counter--;
						break;
					}
				}
			}
		}
		num_cube_wall_y_ = counter;
		temp = new glm::vec3[num_cube_wall_y_];
		memcpy(temp, wall_y_, num_cube_wall_y_ * sizeof(glm::vec3));
		delete[] wall_y_;
		wall_y_ = temp;
		//Z wall
		counter = 0;
		for (float i = min_x - (kEdgeThickness); i < max_x + (kEdgeThickness + 1); i++) {
			for (float j = min_y - (kEdgeThickness); j < max_y + (kEdgeThickness + 1); j++) {
				wall_z_[counter++] = glm::vec3(i, j, center_.z);
				for (int k = 0; k < num_cubes_; k++) {
					if (wall_z_[counter - 1].x == positions_[k].x && wall_z_[counter - 1].y == positions_[k].y) {
						counter--;
						break;
					}
				}
			}
		}
		num_cube_wall_z_ = counter;
		temp = new glm::vec3[num_cube_wall_z_];
		memcpy(temp, wall_z_, num_cube_wall_z_ * sizeof(glm::vec3));
		delete[] wall_z_;
		wall_z_ = temp;

		//Modify model coordinates to be relative to it's center point instead
		for (int i = 0; i < num_cubes_; i++) {
			positions_[i] -= center_;
			positions_default_[i] -= center_;
		}
		for (int i = 0; i < num_cube_wall_x_; i++) {
			//for the walls, displace them by an additional (half width + wall distance)
			wall_x_[i] -= center_ + glm::vec3(x_half_range + kWallDistance, .0f, .0f);
		}
		for (int i = 0; i < num_cube_wall_y_; i++) {
			wall_y_[i] -= center_ + glm::vec3(.0f, y_half_range + kWallDistance, .0f);
		}
		for (int i = 0; i < num_cube_wall_z_; i++) {
			wall_z_[i] -= center_ + glm::vec3(.0f, .0f, z_half_range + kWallDistance);
		}
	}

	//Shuffle function which "flattens" the model
	void Shuffle(float delta_time) {
		//all cubes at front and shuffled on to each other
		bool all_front = true;
		//all cubes returned to original position?
		bool all_default = true;
		for (int i = 0; i < num_cubes_; i++) {
			//Move all the cubes back to their original positions
			if (unshuffle_) {
				all_front = false;
				if (positions_[i].z > positions_default_[i].z) {
					positions_[i].z -= kShuffleSpeed * delta_time;
					all_default = false;
				}
			}
			//We're shuffling all the cubes on to each other
			else {
				if (positions_[i].z < model_front_) {
					positions_[i].z += kShuffleSpeed * delta_time;
					all_default = false;
					all_front = false;
				}
			}
		}
		//if cubes in default position, start shuffling them
		if (all_default) {
			unshuffle_ = false;
		}
		//if all cubes reached the front, toggle flag to move them back
		if (all_front) {
			unshuffle_ = true;
		}
	}

	void ScaleUp(float scale_increase_) {
		model_scale_ += scale_increase_;
	}

	void ScaleDown(float scale_decrease_) {
		model_scale_ -= scale_decrease_;
		if (model_scale_ < 0)
			model_scale_ = 0;
	}

	void MoveLeft(float speed_) {
		model_translation_vector_.x += speed_;
	}
	void MoveRight(float speed_) {
		model_translation_vector_.x -= speed_;
	}
	void MoveForward(float speed_) {
		model_translation_vector_.z += speed_;
	}
	void MoveBackward(float speed_) {
		model_translation_vector_.z -= speed_;
	}


	//Draw the model
	//Pass in each transformation as a set of coordinates except for scale which applies to all dimensions
	void Draw(Shader shader, int start_index, glm::vec3 offset) {
		int world_loc = shader.getUniform("worldMatrix");
		int rotation_loc = shader.getUniform("rotationMatrix");

		//set each transformation matrix based on input variables
		//all cubes share scaling, rotation and translation matrices
		glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(model_scale_, model_scale_, model_scale_));
		glm::mat4 rotation_matrix =
			glm::rotate(glm::mat4(1.0f), glm::radians(model_rotation_wout_wall_.x), glm::vec3(1.f, .0f, .0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(model_rotation_wout_wall_.y), glm::vec3(.0f, 1.f, .0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(model_rotation_wout_wall_.z), glm::vec3(.0f, .0f, 1.f)
			);
		glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), model_translation_vector_ + offset);

		//set rotation for norm -- once per draw call
		//does not need to be set again for wall, assuming wall ALWAYS drawn WITH model.
		glUniformMatrix4fv(rotation_loc, 1, GL_FALSE, &rotation_matrix[0][0]);
		for (int i = 0; i < num_cubes_; i++) {
			//set matrix for position of each cube relative to model
			glm::mat4 position_matrix = glm::translate(glm::mat4(1.0f), model_scale_ * (positions_[i]));
			//Scale first, place all cubes relative to center of model, rotate them on center, then move out to proper place
			glm::mat4 world_matrix = translation_matrix * rotation_matrix * position_matrix * scaling_matrix;
			glUniformMatrix4fv(world_loc, 1, GL_FALSE, &world_matrix[0][0]);
			glDrawArrays(GL_TRIANGLES, start_index, kNumCubeVertices); // 36 vertices, starting at index 0
		}
	}

	void Draw(Shader shader, int start_index) {
		Draw(shader, start_index, glm::vec3(0, 0, 0));
	}

	//axis is 1, 2 or 3 matching x y or z
	void DrawWall(Shader shader, int start_index, glm::vec3 offset, int axis) {

		int world_loc = shader.getUniform("worldMatrix");

		//set each transformation matrix based on input variables
		glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(model_scale_, model_scale_, model_scale_));
		//glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), coordinates + scale*(positions[i]));
		glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), offset);
		glm::mat4 rotation_matrix =
			glm::rotate(glm::mat4(1.0f), glm::radians(model_rotation_vector_.x), glm::vec3(1.f, .0f, .0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(model_rotation_vector_.y), glm::vec3(.0f, 1.f, .0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(model_rotation_vector_.z), glm::vec3(.0f, .0f, 1.f)
			);
		glm::mat4 world_matrix;
		switch (axis) {
		case xAxis:
			for (int i = 0; i < num_cube_wall_x_; i++) {
				glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), model_scale_ * (wall_x_[i]));
				//Scale first, place all cubes relative to center of model, rotate them on center, then move out to proper place
				world_matrix = translation_matrix * rotation_matrix * positionMatrix * scaling_matrix;
				glUniformMatrix4fv(world_loc, 1, GL_FALSE, &world_matrix[0][0]);
				glDrawArrays(GL_TRIANGLES, start_index, kNumCubeVertices); // 36 vertices, starting at index 0
			}
			break;
		case yAxis:
			for (int i = 0; i < num_cube_wall_y_; i++) {
				glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), model_scale_ * (wall_y_[i]));
				//Scale first, place all cubes relative to center of model, rotate them on center, then move out to proper place
				world_matrix = translation_matrix * rotation_matrix * positionMatrix * scaling_matrix;
				glUniformMatrix4fv(world_loc, 1, GL_FALSE, &world_matrix[0][0]);
				glDrawArrays(GL_TRIANGLES, start_index, kNumCubeVertices); // 36 vertices, starting at index 0
			}
			break;
		case zAxis:
			for (int i = 0; i < num_cube_wall_z_; i++) {
				glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), model_scale_ * (wall_z_[i]));
				//Scale first, place all cubes relative to center of model, rotate them on center, then move out to proper place
				world_matrix = translation_matrix * rotation_matrix * positionMatrix * scaling_matrix;
				glUniformMatrix4fv(world_loc, 1, GL_FALSE, &world_matrix[0][0]);
				glDrawArrays(GL_TRIANGLES, start_index, kNumCubeVertices); // 36 vertices, starting at index 0
			}
			break;
		default:
			break;
		}
	}
	void DrawWall(Shader shader, int start_index, int axis) {
		DrawWall(shader, start_index, glm::vec3(0, 0, 0), axis);
	}

};

const float Model::kShuffleSpeed = 2.5f;
const float Model::kEdgeThickness = 1;
const float Model::kWallDistance = 5;

#endif /* Model_h */
