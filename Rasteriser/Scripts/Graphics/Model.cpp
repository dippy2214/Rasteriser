#include "Model.h"
#include "../Core/Maths.h"

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
    const int TILE_SIZE = 32;

    float2 screenSize = renderTarget->Size();
    int width  = (int)screenSize.x;
    int height = (int)screenSize.y;

    int numTriangles = points.size() / 3;

    int tilesX = (width  + TILE_SIZE - 1) / TILE_SIZE;
    int tilesY = (height + TILE_SIZE - 1) / TILE_SIZE;
    int numTiles = tilesX * tilesY;

    struct Tile
    {
        int minX, maxX;
        int minY, maxY;
        std::vector<int> triangles;
    };

    std::vector<Tile> tiles(numTiles);

    //initialize tiles
    for (int ty = 0; ty < tilesY; ++ty)
    {
        for (int tx = 0; tx < tilesX; ++tx)
        {
            int index = ty * tilesX + tx;

            int minX = tx * TILE_SIZE;
            int minY = ty * TILE_SIZE;
            int maxX = std::min(minX + TILE_SIZE - 1, width - 1);
            int maxY = std::min(minY + TILE_SIZE - 1, height - 1);

            tiles[index].minX = minX;
            tiles[index].maxX = maxX;
            tiles[index].minY = minY;
            tiles[index].maxY = maxY;
        }
    }

    //transform + bin triangles
    struct ScreenTri
    {
        float3 a, b, c;
        int triIndex;
    };

    std::vector<ScreenTri> screenTris(numTriangles);

    for (int tri = 0; tri < numTriangles; ++tri)
    {
        int i = tri * 3;

        float3 a = cam->VertexToScreen(points[i + 0], this, screenSize);
        float3 b = cam->VertexToScreen(points[i + 1], this, screenSize);
        float3 c = cam->VertexToScreen(points[i + 2], this, screenSize);

        if (a.z <= 0 || b.z <= 0 || c.z <= 0)
            continue;

        screenTris[tri] = { a, b, c, tri };

        float minX = Maths::SmallestOfThree(a.x, b.x, c.x);
        float maxX = Maths::LargestOfThree(a.x, b.x, c.x);
        float minY = Maths::SmallestOfThree(a.y, b.y, c.y);
        float maxY = Maths::LargestOfThree(a.y, b.y, c.y);

        int tileMinX = std::max(0, (int)minX / TILE_SIZE);
        int tileMaxX = std::min(tilesX - 1, (int)maxX / TILE_SIZE);
        int tileMinY = std::max(0, (int)minY / TILE_SIZE);
        int tileMaxY = std::min(tilesY - 1, (int)maxY / TILE_SIZE);

        for (int ty = tileMinY; ty <= tileMaxY; ++ty)
        {
            for (int tx = tileMinX; tx <= tileMaxX; ++tx)
            {
                tiles[ty * tilesX + tx].triangles.push_back(tri);
            }
        }
    }

    // parallel tile rasterization
    std::atomic<int> nextTile(0);
    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> workers;

    auto worker = [&]()
    {
        while (true)
        {
            int tileIndex = nextTile.fetch_add(1);
            if (tileIndex >= numTiles)
                break;

            Tile& tile = tiles[tileIndex];

            for (int tri : tile.triangles)
            {
                const ScreenTri& st = screenTris[tri];
                float3 a = st.a;
                float3 b = st.b;
                float3 c = st.c;

                int i = tri * 3;

                int startX = std::max(tile.minX,
                    (int)Maths::SmallestOfThree(a.x, b.x, c.x));
                int endX = std::min(tile.maxX,
                    (int)Maths::LargestOfThree(a.x, b.x, c.x));

                int startY = std::max(tile.minY,
                    (int)Maths::SmallestOfThree(a.y, b.y, c.y));
                int endY = std::min(tile.maxY,
                    (int)Maths::LargestOfThree(a.y, b.y, c.y));

                for (int y = startY; y <= endY; ++y)
                {
                    for (int x = startX; x <= endX; ++x)
                    {
                        float2 p(x, y);
                        float3 triWeights;

                        if (!Maths::IsInsideTriangle(
                                a.xy(), b.xy(), c.xy(), p, &triWeights))
                            continue;

                        float3 depths(a.z, b.z, c.z);
                        float depth = 1.0f /
                            ((1.0f / depths).Dot(triWeights));

                        if (depth > renderTarget->GetDepth(x, y))
                            continue;

                        if (!hasTexture)
                        {
                            renderTarget->SetPixel(x, y,
                                triColours[i / 3]);
                            renderTarget->SetDepth(x, y, depth);
                        }
                        else
                        {
                            float2 textureCoord(0,0);

                            textureCoord += textureCoords[i + 0] /
                                            depths.x * triWeights.x;
                            textureCoord += textureCoords[i + 1] /
                                            depths.y * triWeights.y;
                            textureCoord += textureCoords[i + 2] /
                                            depths.z * triWeights.z;
                            textureCoord *= depth;

                            float4 textureColor =
                                modelTexure->get_pixel(
                                    textureCoord.x,
                                    textureCoord.y);

                            float lightIntensity =
                                std::max(0.0f,
                                    normals[i].Dot(float3(0,1,1)));

                            float4 lightingColor =
                                float4(1,1,1,1) * lightIntensity;

                            float4 finalColor =
                                lightingColor * textureColor;

                            renderTarget->SetPixel(x, y, finalColor);
                            renderTarget->SetDepth(x, y, depth);
                        }
                    }
                }
            }
        }
    };

    for (int t = 0; t < numThreads; ++t)
        workers.emplace_back(worker);

    for (auto& t : workers)
        t.join();
}
