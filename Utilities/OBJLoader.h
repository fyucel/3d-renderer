#pragma once

class IndexedModel
{
public:
	void CalculateNormals();

private:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> textureCoordinates;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	static const int NumEntriesPerIndex = 3;
};

class OBJModel
{
public:
	OBJModel(const std::string& filename);

	struct Index
	{
		unsigned int vertex;
		unsigned int textureCoordinate;
		unsigned int normal;

		bool operator < (const Index& rightHandSide) const;
	};

	IndexedModel& SeparateIndices();

private:
	std::vector<Index> indices;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> textureCoordinates;
	std::vector<glm::vec3> normals;

	IndexedModel indexedModel;

	bool hasTextureCoordinates;
	bool hasNormals;

	static const int LengthLengthToIgnore = 2;

	unsigned int FindLastVertexIndex(const std::vector<Index>& indexLookup,
		const Index& currentIndex, const IndexedModel& result);

	void CreateFace(const std::string& faceLine);
	glm::vec2 ParseVec2(const std::string& vec2Line);
	glm::vec3 ParseVec3(const std::string& vec3Line);
	Index ParseIndex(const std::string& token);

	unsigned int FindNextChar(const char* string, unsigned int stringLength,
		unsigned int startingIndex, char token);
	unsigned int ParseIndexValue(const std::string& token,
		unsigned int startingIndex, unsigned int endingIndex);
	float ParseFloatValue(const std::string& token, unsigned int startingIndex,
		unsigned int endingIndex);
	std::vector<std::string> SplitStringWithDelimeter(
		const std::string& string, char delimeter);
};