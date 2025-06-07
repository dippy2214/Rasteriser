#include "Model.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <random>

Model::Model(std::string fileName)
{
	points = LoadObjFile(fileName);
	for (int i = 0; i < points.size()/3; i++)
	{
		triColours.emplace_back(Color(rand() % 255, rand() % 255, rand() % 255, 255.0f));
	}
}

float3 Model::ParsePoints(std::string str)
{
	float3 output;
	std::string del = " ";
	int it = 0;
	str.erase(0, 2);
	auto pos = str.find(del);
	//std::cout << str << "\n";
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
			throw std::runtime_error("unexpected amount of data in vertex");
		}
		str.erase(0, pos + del.length());
		pos = str.find(del);
		it++;
	}
	output.z = std::atof(str.c_str());
	//std::cout << output.ToString() << "\n";

	return output;
}

std::vector<int> Model::ParseFaces(std::string str)
{
	std::vector<int> output;
	std::string del = " ";

	std::vector<std::string> faceIndexGroups;

	str.erase(0, 2);
	auto pos = str.find(del);

	while (pos != std::string::npos)
	{
		faceIndexGroups.emplace_back(str.substr(0, pos));
		str.erase(0, pos + del.length());
		//std::cout << pos << "\n";
		pos = str.find(del);
		
	}
	faceIndexGroups.emplace_back(str);

	/*for (int i = 0; i < faceIndexGroups.size(); i++)
	{
		std::cout << faceIndexGroups[i] << "\n";
	}*/

	for (int i = 0; i < faceIndexGroups.size(); i++)
	{
		
		output.emplace_back(std::atoi(faceIndexGroups[i].substr(0, 1).c_str())-1);
		//std::cout << std::atoi(faceIndexGroups[i].substr(0, pos).c_str()) - 1 << "\n";
		if (i >= 3)
		{
			output.emplace_back(output[output.size() - (3 * i - 6)]);
			output.emplace_back(output.size() - 2);
		}
	}
	/*for (int i = 0; i < output.size(); i++)
	{
		std::cout << output[i] << "\n";
	}*/
	return output;
}

std::vector<float3> Model::LoadObjFile(std::string fileName)
{
	std::ifstream objFile(fileName);
	std::string storedFileContent;

	std::vector<float3> allPoints;
	std::vector<float3> trianglePoints;

	if (!objFile.is_open() || fileName.substr(fileName.size()-4, 4) != ".obj")
	{
		throw std::runtime_error("Failed to open obj file with name: '" + fileName + "'");
	}
	while (std::getline(objFile, storedFileContent))
	{
		if (storedFileContent.substr(0, 2) == "v ")
		{
			float3 point = ParsePoints(storedFileContent);
			allPoints.emplace_back(point);
			
		}
		else if (storedFileContent.substr(0, 2) == "f ")
		{
			std::vector<int> faces = ParseFaces(storedFileContent);
			for (int i = 0; i < faces.size(); i++)
			{
				trianglePoints.emplace_back(allPoints[faces[i]]);
			}
		}
	}

	/*for (int i = 0; i < trianglePoints.size(); i++)
	{
		std::cout << trianglePoints[i].ToString() << "\n";
	}*/

	return trianglePoints;
}
