#include "ObjLoader.h"
#include <SDL.h>
#include <string>
#include <vector>
#include "Shader.h"

//TODO: Allow .obj files that use sets of 4 vertices for faces to be loaded. Blender exports like this: "f 1/1/1 2/2/1 3/3/1 4/4/1"
/* File open modes for SDL_RWFromFile(file, mode)

r	Open a file for reading. The file must exist.

w	Create an empty file for writing. If a file with the same name already exists its content is erased and the file is treated as a new empty file.

a	Append to a file. Writing operations append data at the end of the file. The file is created if it does not exist.

r+	Open a file for update both reading and writing. The file must exist.

w+	Create an empty file for both reading and writing. If a file with the same name already exists its content is erased and the file is
	treated as a new empty file.

a+	Open a file for reading and appending. All writing operations are performed at the end of the file, protecting the previous
	content to be overwritten. You can reposition (fseek, rewind) the internal pointer to anywhere in the file for reading,
	but writing operations will move it back to the end of file. The file is created if it does not exist.
*/

bool LoadOBJ(const char* aFilePath, GLuint &aVBO, unsigned int &aVertexCount, GLuint &aIBO, unsigned int &aIndexCount)
{
	// Read the file to a SDL_RWops struct
	SDL_RWops* file = SDL_RWFromFile(aFilePath, "r");

	// Check to see if the file was read properly
	if (file == NULL)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "(ObjLoader) File could not be opened: \"%s\"%s", aFilePath, " in function: \"bool LoadOBJ(const char* &aFilePath, GLuint &aVBO, unsigned int &aVertexCount, GLuint &aIBO, unsigned int &aIndexCount)\"");
		return false;
	}
	else
	{
		// Seek to the end of the file
		SDL_RWseek(file, 0, RW_SEEK_END);

		// Get the current offset (how far in the file we have seeked)
		const long size = (long)SDL_RWtell(file);

		// Seek back to the beginning of the file
		SDL_RWseek(file, 0, RW_SEEK_SET);

		if (size > 0)
		{
			char* data;

			// Create a new char array that is one char larger than the file size
			data = new char[size + 1];

			// Set the last element of the char array to a null character, so there are no issues reading the end-of-file
			data[size] = 0;

			// Read the file contents and store it in the char array "data", using "size" as the size will read the entire file
			SDL_RWread(file, data, size, 1);

			// You can use std::stof() to convert string to float, std::strtod() or std::stod to convert string to double, std::stoi to convert string to int
			
			// Store the data buffer in a string so we can perform certain operations on it, such as find() and stof().
			std::string fileString(data);

			// Delete the data buffer after storing it in a string
			delete[] data;

			// size_type ensures that the container will always be large enough to index the string
			//std::string::size_type index;

			//tempIndex is to keep the string index when reading from substrings
			std::string::size_type tempIndex;

			std::vector<glm::vec3> vertices;
			std::vector<glm::vec2> textureCoords;
			std::vector<glm::vec3> vertexNormals; // TODO: Add vertex normals to the VertexFormat struct

			std::vector<VertexFormat> vFormat;

			std::vector<unsigned int> indices;

			// Read the file contents into the appropriate vectors
			for (std::string::size_type index = 0; index < fileString.size();)
			{
				if (fileString.at(index) == 'v')
				{
					if (fileString.at(index + 1) == ' ') // Just 'v' means that we are reading vertices, so it should be followed by three floats
					{
						// Read a float from the string starting after the 'v', passing '&index' will store the new string position in 'index'
						float x = std::stof(fileString.substr(index + 2), &tempIndex);
						index += tempIndex + 2;
						float y = std::stof(fileString.substr(index), &tempIndex);
						index += tempIndex;
						float z = std::stof(fileString.substr(index));

						vertices.push_back(glm::vec3(x, y, z));
						//vFormat.push_back(VertexFormat(glm::vec3(x, y, z))); //TEMP
					}
					else if (fileString.at(index + 1) == 't') // 'vt' means that we are reading texture coordinates, so it should be followed by two floats
					{
						float xCoord = std::stof(fileString.substr(index + 2), &tempIndex);
						index += tempIndex + 2;
						float yCoord = std::stof(fileString.substr(index));

						textureCoords.push_back(glm::vec2(xCoord, yCoord));
					}
					else if (fileString.at(index + 1) == 'n') // 'vn' means that we are reading vertex normals, so it should be followed by three floats
					{
						float x = std::stof(fileString.substr(index + 2), &tempIndex);
						index += tempIndex + 2;
						float y = std::stof(fileString.substr(index), &tempIndex);
						index += tempIndex;
						float z = std::stof(fileString.substr(index));

						vertexNormals.push_back(glm::vec3(x, y, z));
					}
				}
				else if (fileString.at(index) == 'f') // 'f' means that we are reading faces, so we are putting the vectors together
				{
					// Set index to one character after the 'f'
					index += 1; 


                    for (int i = 0; i < 3; ++i, index = fileString.find(' ', index))
                    {
                        bool vertexFound = false;

                        // Read the first number from the set of three
						//TODO: Check if data exists in file. Blender can export objects without any UV data
                        unsigned int vertexIndex(stoi(fileString.substr(index), &tempIndex) - 1); // Subtract one since the file assumes 1 is the first index, instead of 0 in OpenGL                 
                        index += tempIndex + 1; // +1 to skip the '/'
                        unsigned int textureCoordIndex(stoi(fileString.substr(index), &tempIndex) - 1);
                        index += tempIndex + 1;
                        unsigned int vertexNormalIndex(stoi(fileString.substr(index), &tempIndex) - 1);

                        VertexFormat vert(vertices[vertexIndex], textureCoords[textureCoordIndex], vertexNormals[vertexNormalIndex]);

                        // See if the same vertex is already in the vector
                        for (unsigned int j = 0; j < vFormat.size(); ++j)
                        {
                            if (vert == vFormat.at(j))
                            {                    
                                indices.push_back(j);
                                vertexFound = true;
                                break;
                            }
                        }
                        if (!vertexFound)
                        {
                            vFormat.push_back(vert);
                            indices.push_back(vFormat.size() - 1);
                        }
                    }
				}

				// Move to the next line
				index = fileString.find('\n', index);

				// The are no new lines, so we break out of the loop
				if (index == std::string::npos)
				{
					SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Index = EndOfFile");
					break;
				}
				++index;
			}

			aVertexCount = vFormat.size();
			aIndexCount = indices.size();

			//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Size of file: %s = %i bytes", aFilePath, sizeof(VertexFormat) * aVertexCount);

			// Bind the VBO to buffer the data into
			glBindBuffer(GL_ARRAY_BUFFER, aVBO);

			// Buffer the vertex data
			glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * aVertexCount, &vFormat[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);



			// Bind the VBO to buffer the data into
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, aIBO);

			// Buffer the vertex data
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * aIndexCount, &indices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			// Close the SDL_RWops struct
			SDL_RWclose(file);

			return true;
		}
		else
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "(FileReader) File could not be read: \"%s\" %s", aFilePath, SDL_GetError());
		}
	}

	return false;
}

// TEMPORARY
bool DisplayTextFile(const char* aFilePath)
{
	// Read the file to a SDL_RWops struct
	SDL_RWops* file = SDL_RWFromFile(aFilePath, "r");

	// Check to see if the file was read properly
	if (file == NULL)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "(ObjLoader) File could not be opened: \"%s\"%s", aFilePath, " in function: \"bool LoadOBJ(const char* &aFilePath, GLuint &aVBO, unsigned int &aVertexCount, GLuint &aIBO, unsigned int &aIndexCount)\"");
		return false;
	}
	else
	{
		// Seek to the end of the file
		SDL_RWseek(file, 0, RW_SEEK_END);

		// Get the current offset (how far in the file we have seeked)
		const long size = (long)SDL_RWtell(file);

		// Seek back to the beginning of the file
		SDL_RWseek(file, 0, RW_SEEK_SET);

		std::string lineHeader;

		if (size > 0)
		{
			char* data;

			// Create a new char array that is one char larger than the file size
			data = new char[size + 1];

			// Read the file contents and store it in the char array "data", using "size" as the size will read the entire file
			SDL_RWread(file, data, size, 1);

			// Set the last element of the char array to a null character, so there are no issues reading the end-of-file
			data[size] = 0;

			// Close the SDL_RWops struct
			SDL_RWclose(file);

			//float num = std::stof(data);

			std::string test(data);

			std::string::size_type index;

			index = test.find('\n');

			std::string test2 = test.substr(test.size());

			int x = std::stoi("1/2/3");
			
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "(FileReader) File contents: \"%i\"", x); // Displaying doubles seems to not work

			delete[] data;

			return true;
		}
		else
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "(FileReader) File could not be read: \"%s\"%s", aFilePath, SDL_GetError());
		}
	}

	return false;
}