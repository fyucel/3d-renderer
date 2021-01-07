#include "Utilities.h"
#include "OBJLoader.h"

void IndexedModel::CalculateNormals()
{
	for (auto index = 0; index < indices.size(); index += NumEntriesPerIndex)
	{
		int vertexIndex = indices[index];
		int textureCoordinateIndex = indices[index + 1];
		int normalIndex = indices[index + 2];

		glm::vec3 firstVector = positions[textureCoordinateIndex]
			- positions[vertexIndex];
		glm::vec3 secondVector = positions[normalIndex]
			- positions[vertexIndex];

		glm::vec3 normal = glm::normalize(glm::cross(
			firstVector, secondVector));

		normals[vertexIndex] += normal;
		normals[textureCoordinateIndex] += normal;
		normals[normalIndex] += normal;
	}

	for (auto position = 0; position < positions.size(); position++)
		normals[position] = glm::normalize(normals[position]);
}

OBJModel::OBJModel(const std::string& filename) :
	hasTextureCoordinates{ false },
	hasNormals{ false }
{
	std::ifstream input(filename);

	if (input.is_open())
	{
		// TO DO: throw exception
	}

	std::string line = "";
	unsigned int lineLength = 0;

	while (input.good())
	{
		std::getline(input, line);
		lineLength = line.length();

		if (lineLength < LengthLengthToIgnore) continue;

		switch (line[0])
		{
		case 'v':
			switch (line[1])
			{
			case 't':
				textureCoordinates.push_back(ParseVec2(line));
				break;
			case 'n':
				normals.push_back(ParseVec3(line));
				break;
			default:
				positions.push_back(ParseVec3(line));
				break;
			}
		case 'f':
			CreateFace(line);
			break;
		}
	}
}

IndexedModel& OBJModel::SeparateIndices()
{

}

unsigned int OBJModel::FindLastVertexIndex(
	const std::vector<Index>& indexLookup,
	const Index& currentIndex,
	const IndexedModel& result)
{

}

void OBJModel::CreateFace(const std::string& faceLine)
{
	std::vector<std::string> tokens = SplitStringWithDelimeter(faceLine, ' ');

	indices.push_back(ParseIndex(tokens[1]));
	indices.push_back(ParseIndex(tokens[2]));
	indices.push_back(ParseIndex(tokens[3]));
}

glm::vec2 OBJModel::ParseVec2(const std::string& vec2Line)
{

}

glm::vec3 OBJModel::ParseVec3(const std::string& vec3Line)
{

}

OBJModel::Index OBJModel::ParseIndex(const std::string& token)
{

}

unsigned int OBJModel::FindNextChar(const char* string,
	unsigned int stringLength, unsigned int startingIndex, char token)
{

}

unsigned int OBJModel::ParseIndexValue(const std::string& token,
	unsigned int startingIndex, unsigned int endingIndex)
{

}

float OBJModel::ParseFloatValue(const std::string& token,
	unsigned int startingIndex, unsigned int endingIndex)
{

}

std::vector<std::string> OBJModel::SplitStringWithDelimeter(
	const std::string& string, char delimeter)
{

}

bool OBJModel::Index::operator<(const Index& rightHandSide) const
{
	return vertex < rightHandSide.vertex;
}
