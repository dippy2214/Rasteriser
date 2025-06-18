#include "Model.h"
#include "Maths.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <thread>

Model::Model(std::string fileName)
{
	points = LoadObjFile(fileName);
	for (int i = 0; i < points.size()/3; i++)
	{
		triColours.emplace_back(Color(rand() % 255, rand() % 255, rand() % 255, 255.0f));
	}
	hasTexture = false;
}

Model::Model(std::string fileName, std::string textureFileName)
{
	points = LoadObjFile(fileName);
	modelTexure = new BMPImage(textureFileName.c_str());
	hasTexture = true;
}

float3 Model::ParseVertexOrNormals(std::string str, bool isNormal)
{
	float3 output;
	std::string del = " ";
	int it = 0;
	if (isNormal) { str.erase(0, 3); }
	else { str.erase(0, 2); }
	auto pos = str.find(del);

	while (pos != std::string::npos)
	{
		switch (it)
		{
		case 0:
			output.x = std::atof(str.substr(0, pos).c_str());
			break;
		case 1:
			output.y = std::atof(str.substr(0, pos).c_str());
			break;
		default:
			std::cout << str << "\n" << it << "\n";
			throw std::runtime_error("unexpected amount of data in normal");
		}
		str.erase(0, pos + del.length());
		pos = str.find(del);
		it++;
	}
	output.z = std::atof(str.c_str());

	return output;
}

float2 Model::ParseTextureCoords(std::string str)
{
	float2 output;
	std::string del = " ";
	int it = 0;
	str.erase(0, 3);
	auto pos = str.find(del);

	output.x = std::atof(str.substr(0, pos).c_str());
	str.erase(0, pos + del.length());
	output.y = std::atof(str.c_str());

	return output;
}

std::vector<PointData> Model::ParseFaces(std::string str)
{
	std::vector<PointData> output;
	std::string del = " ";

	std::vector<std::string> faceIndexGroups;

	str.erase(0, 2);
	auto pos = str.find(del);

	while (pos != std::string::npos)
	{
		faceIndexGroups.emplace_back(str.substr(0, pos));
		str.erase(0, pos + del.length());
		pos = str.find(del);
		
	}
	faceIndexGroups.emplace_back(str);

	for (int i = 0; i < faceIndexGroups.size(); i++)
	{
		PointData currentFaceIndexGroup;

		int slashIndex = faceIndexGroups[i].find("/");
		currentFaceIndexGroup.vertexIndex = std::atoi(faceIndexGroups[i].substr(0, slashIndex).c_str()) - 1;
		faceIndexGroups[i].erase(0, slashIndex + 1);

		slashIndex = faceIndexGroups[i].find("/");
		currentFaceIndexGroup.textureCoordIndex = std::atoi(faceIndexGroups[i].substr(0, slashIndex).c_str()) - 1;
		faceIndexGroups[i].erase(0, slashIndex + 1);

		currentFaceIndexGroup.normalIndex = std::atoi(faceIndexGroups[i].c_str()) - 1;
		//std::cout << std::atoi(faceIndexGroups[i].c_str()) - 1 << std::endl;

		output.emplace_back(currentFaceIndexGroup);
		if (i > 2)
		{
			output.emplace_back(output[output.size() - 1 - (3 * i - 6)]);
			output.emplace_back(output[output.size() - 3]);
		}
	}

	return output;
}

std::pair<int, ShaderDetails*> Model::GetShaderDetails()
{
	std::vector<ShaderDetails> shaderDetails;
	for (int i = 0; i < points.size(); i++)
	{
		ShaderDetails currentPoint;
		currentPoint.vertex = points[i];
		currentPoint.textureCoord = textureCoords[i];
		currentPoint.normal = normals[i];
		shaderDetails.emplace_back(currentPoint);
	}
	std::pair<int, ShaderDetails*> output = { points.size(), &shaderDetails[0] };
	return output;
}

std::vector<float3> Model::LoadObjFile(std::string fileName)
{
	std::ifstream objFile(fileName);
	std::string storedFileContent;

	std::vector<float3> allVertexes;
	std::vector<float3> allNormals;
	std::vector <float2> allTexCoords;
	std::vector<float3> trianglePoints;

	if (!objFile.is_open() || fileName.substr(fileName.size()-4, 4) != ".obj")
	{
		throw std::runtime_error("Failed to open obj file with name: '" + fileName + "'");
	}
	while (std::getline(objFile, storedFileContent))
	{
		if (storedFileContent.substr(0, 2) == "v ")
		{
			float3 vertex = ParseVertexOrNormals(storedFileContent); 
			allVertexes.emplace_back(vertex); 
			
		}
		else if (storedFileContent.substr(0, 3) == "vt ")
		{
			float2 UV = ParseTextureCoords(storedFileContent);
			allTexCoords.emplace_back(UV);
		}
		else if (storedFileContent.substr(0, 3) == "vn ")
		{
			float3 normal = ParseVertexOrNormals(storedFileContent, true);
			allNormals.emplace_back(normal);
		}
		else if (storedFileContent.substr(0, 2) == "f ")
		{
			std::vector<PointData> faces = ParseFaces(storedFileContent);
			for (int i = 0; i < faces.size(); i++)
			{
				trianglePoints.emplace_back(allVertexes[faces[i].vertexIndex]);
				textureCoords.emplace_back(allTexCoords[faces[i].textureCoordIndex]);
				normals.emplace_back(allNormals[faces[i].normalIndex]);
			}
		}
	}

	return trianglePoints;
}

void Model::Render(RenderTarget* renderTarget, Camera* cam)
{
	for (int i = 0; i < points.size(); i += 3)
	{
		//auto startTri = std::chrono::high_resolution_clock::now();
		float2 screenSize = renderTarget->Size();
		float3 a = cam->VertexToScreen(points[i + 0], this, screenSize);
		float3 b = cam->VertexToScreen(points[i + 1], this, screenSize);
		float3 c = cam->VertexToScreen(points[i + 2], this, screenSize);

		//std::cout << a.ToString() << " : " << b.ToString() << " : " << c.ToString() << std::endl;

		if (a.z <= 0 || b.z <= 0 || c.z <= 0) { continue; }

		float maxX = Maths::LargestOfThree(a.x, b.x, c.x);
		float minX = Maths::SmallestOfThree(a.x, b.x, c.x);
		float maxY = Maths::LargestOfThree(a.y, b.y, c.y);
		float minY = Maths::SmallestOfThree(a.y, b.y, c.y);

		int blockStartX = Maths::Clamp((int)minX, 0, screenSize.x - 1);
		int blockEndX = Maths::Clamp((int)ceilf(maxX), 0, screenSize.x - 1);
		int blockStartY = Maths::Clamp((int)minY, 0, screenSize.y - 1);
		int blockEndY = Maths::Clamp((int)ceilf(maxY), 0, screenSize.y - 1);

		for (int y = blockStartY; y <= blockEndY; y++)
		{
			for (int x = blockStartX; x <= blockEndX; x++)
			{
				float2 p(x, y);
				float3 triWeights;

				if (!Maths::IsInsideTriangle(a.xy(), b.xy(), c.xy(), p, &triWeights)) { continue; }

				float3 depths(a.z, b.z, c.z);
				float depth = 1 / ((1.0f / depths).Dot(triWeights));

				if (depth > renderTarget->GetDepth(x, y)) { continue; }

				if (!hasTexture)
				{
					renderTarget->SetPixel(x, y, triColours[i / 3]);
					renderTarget->SetDepth(x, y, depth);
				}
				else
				{
					float2 textureCoord;
					textureCoord += textureCoords[i + 0] / depths.x * triWeights.x;
					textureCoord += textureCoords[i + 1] / depths.y * triWeights.y;
					textureCoord += textureCoords[i + 2] / depths.z * triWeights.z;
					textureCoord *= depth;

					float4 textureColor = modelTexure->get_pixel(textureCoord.x, textureCoord.y);
					float lightIntensity = std::max(0.0f, normals[i].Dot(float3(0, 1, 1)));
					float4 lightingColor = float4(1, 1, 1, 1) * lightIntensity;

					//std::cout << "light intensity: " << lightIntensity << " : lighting Color: " << lightingColor.ToString() << std::endl;

					float4 finalColor = lightingColor * textureColor;
					
					renderTarget->SetPixel(x, y, finalColor);
					renderTarget->SetDepth(x, y, depth);
				}
			}
		}
	}
}
