#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

namespace veg {
	class VegPipeline
	{
	public:
		VegPipeline(const std::string &vertFilepath, const std::string & fragFilepath);
		~VegPipeline();


	private:
		
		static std::vector<char> readFile(const std::string& filepath);

		void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath);

	};

}