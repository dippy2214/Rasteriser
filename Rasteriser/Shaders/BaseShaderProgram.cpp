#include "BaseShaderProgram.h"

void BaseShaderProgram::Render(void* inputParams, int inputSize)
{
	std::vector<ShaderOutputs> outputs;
	ShaderDetails* input = (ShaderDetails*)inputParams;
	/*for (int i = 0; i < inputSize; i+=3)
	{
		ShaderDetails p1 = *(ShaderDetails*)vertexShader->RunShader(&input[i], sizeof(ShaderDetails));
		ShaderDetails p2 = *(ShaderDetails*)vertexShader->RunShader(&input[i+1], sizeof(ShaderDetails));
		ShaderDetails p3 = *(ShaderDetails*)vertexShader->RunShader(&input[i+2], sizeof(ShaderDetails));
		ShaderDetails rasteriserInput[3];
		rasteriserInput[0] = p1;
		rasteriserInput[1] = p2;
		rasteriserInput[2] = p3;
		
		void* temp = rasteriser.RunShader(&rasteriserInput[0], sizeof(rasteriserInput));
		if (temp == nullptr)
		{
			continue;
		}
		RasteriserOutputs pixelShaderInput = *(RasteriserOutputs*)temp;

		for (int i = 0; i < pixelShaderInput.count; i++)
		{
			outputs.emplace_back((ShaderOutputs*)pixelShader->RunShader(&pixelShaderInput.outputs[i], sizeof(outputs)));
		}

	}
	
	for (int i = 0; i < outputs.size(); i++)
	{
		renderer->SetPixel(outputs[i]->pixelPosition, outputs[i]->pixelColor);
	}*/

	int numThreads = std::thread::hardware_concurrency();
	int numTris = inputSize / 3;
	int workPerThread =  numTris / numThreads;
	int remainder = numTris % numThreads;
	int start = 0;
	std::cout << numTris << std::endl;
	std::vector<std::thread> threads;
	for (int i = 0; i < numThreads; i++)
	{
		int end = start + workPerThread;
		if (i == numThreads - 1)
		{
			end += remainder;
		}
		threads.emplace_back(&BaseShaderProgram::ThreadWorker, this, start, end, &outputs, input);
		//std::cout << start << ", " << end << std::endl;
		start = end;
	}

	for (int i = 0; i < threads.size(); i++)
	{
		threads[i].join();
	}

	for (int i = 0; i < outputs.size(); i++)
	{
		//std::cout << outputs[i]->pixelPosition.ToString() << " : " << outputs[i]->pixelColor.ToString() << std::endl;
		renderer->SetPixel(outputs[i].pixelPosition, outputs[i].pixelColor);
		//delete outputs[i];
	}

	//delete input; 
	outputs.clear();
}

void BaseShaderProgram::ThreadWorker(int start, int end, std::vector<ShaderOutputs>* outputs, ShaderDetails* input)
{
	for (int i = start * 3; i < end * 3; i += 3)
	{
		char* p1 = vertexShader->RunShader(&input[i], sizeof(ShaderDetails));
		char* p2 = vertexShader->RunShader(&input[i + 1], sizeof(ShaderDetails));
		char* p3 = vertexShader->RunShader(&input[i + 2], sizeof(ShaderDetails));
		delete[] p1;
		delete[] p2;
		delete[] p3;
		/*ShaderDetails rasteriserInput[3];
		memcpy(&rasteriserInput[0], p1, sizeof(ShaderDetails));
		memcpy(&rasteriserInput[1], p2, sizeof(ShaderDetails));
		memcpy(&rasteriserInput[2], p3, sizeof(ShaderDetails));*/
		

		/*void* temp = rasteriser.RunShader(&rasteriserInput[0], sizeof(rasteriserInput));
		if (temp == nullptr)
		{
			delete temp;
			continue;
		}
		RasteriserOutputs pixelShaderInput = *(RasteriserOutputs*)temp;
		delete temp;*/

		//for (int i = 0; i < pixelShaderInput.count; i++)
		//{
		//	ShaderOutputs pixelResult = *(ShaderOutputs*)pixelShader->RunShader(&pixelShaderInput.outputs[i], sizeof(pixelShaderInput.outputs[i]));
		//	if (pixelResult != nullptr)
		//	{
		//		std::lock_guard<std::mutex> lock(mtx);
		//		outputs->emplace_back(pixelResult);
		//	}
		//	//delete pixelResult;
		//}
		//delete[] pixelShaderInput.outputs;
		//delete pixelShaderInput;
	}
}

void BaseShaderProgram::SetShaderParameters(BaseShader* vertex, BaseShader* pixel, RenderTarget* renderTarget, Camera* camera)
{
	renderer = renderTarget;

	vertexShader = vertex;
	//rasteriser = new Rasteriser();
	rasteriser.SetShaderParameters(renderTarget, camera);
	pixelShader = pixel;
	
}

