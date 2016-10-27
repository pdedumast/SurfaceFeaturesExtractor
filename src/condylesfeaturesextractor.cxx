#include "condylesfeaturesextractor.hxx"
#include "condylesfileIO.hxx"
#include "condylesfeaturesextractorCLP.h"

int main (int argc, char *argv[])
{
	PARSE_ARGS;

    if (argc < 3)
    {
        cout << "Usage: " << argv[0] << " --input" << " [input surface name]" << " --output" << " [output surface name]" << endl;
        return EXIT_FAILURE;
    }
    if(inputSurface.rfind(".vtk")==std::string::npos || inputSurface.empty())
    {
        std::cerr << "Wrong Input File Format, must be a .vtk or .vtp file" << std::endl;
        return EXIT_FAILURE;
    }
    if(outputSurface.rfind(".vtk")==std::string::npos || outputSurface.empty())
    {
        std::cerr << "Wrong Output File Format, must be a .vtk file" << std::endl;
        return EXIT_FAILURE;
    }


	vtkSmartPointer<CondylesFeaturesExtractor> Filter = vtkSmartPointer<CondylesFeaturesExtractor>::New();

	Filter->SetInput(inputSurface.c_str());
	Filter->Update();
    writeVTKFile(outputSurface.c_str(),Filter->GetOutput());

	return EXIT_SUCCESS;
}