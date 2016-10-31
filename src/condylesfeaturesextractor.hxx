#include "condylesfeaturesextractor.h"



vtkStandardNewMacro(CondylesFeaturesExtractor);

/**
* Constructor CondylesFeaturesExtractor()
*/
CondylesFeaturesExtractor::CondylesFeaturesExtractor(){
	this->inputSurface = vtkSmartPointer<vtkPolyData>::New();
	this->outputSurface = vtkSmartPointer<vtkPolyData>::New();

	this->intermediateSurface = vtkSmartPointer<vtkPolyData>::New();
}

/**
* Destructor CondylesFeaturesExtractor()
*/
CondylesFeaturesExtractor::~CondylesFeaturesExtractor(){}

/**
* Function SetInput() for CondylesFeaturesExtractor
*/
void CondylesFeaturesExtractor::SetInput(std::string input, std::vector<std::string> list)
{
	this->listMeanFiles = list;
	this->inputSurface = readVTKFile(input);
}

/**
 * Function init_output() for CondylesFeaturesExtractor
 */
void CondylesFeaturesExtractor::init_output()
{
	this->outputSurface = this->inputSurface;
}

/** 
 * Function compute_normals()
 */
void CondylesFeaturesExtractor::compute_normals()
{
	puts("Function compute_normals");
	vtkSmartPointer<vtkPolyDataNormals> NormalFilter = vtkSmartPointer<vtkPolyDataNormals>::New();
	NormalFilter->SetInputData(this->inputSurface);

	NormalFilter->Update();

	this->intermediateSurface = NormalFilter->GetOutput();

}

/** 
 * Function compute_positions()
 */
void CondylesFeaturesExtractor::compute_positions()
{

    std::string name = "position";
    int nbPoints = this->intermediateSurface->GetNumberOfPoints();

	vtkFloatArray* position = vtkFloatArray::New();
	position->SetNumberOfComponents(3);

	std::cout<<"---Computing Scalar "<<name<<std::endl;
	position->SetName(name.c_str());

	for(int i=0; i<nbPoints; i++)
	{
		double* p = new double[3];
		p = this->intermediateSurface->GetPoint(i);

		position->InsertNextTuple3(p[0],p[1],p[2]);		
	}
	
	this->intermediateSurface->GetPointData()->SetActiveVectors(name.c_str());
	this->intermediateSurface->GetPointData()->SetVectors(position);

}

/** 
 * Function compute_distances()
 */
void CondylesFeaturesExtractor::compute_distances()
{
	std::cout<<"Function compute_distances"<<std::endl;
	int nbCells = this->inputSurface->GetNumberOfCells();

	if(nbCells <= 0)		// Pas de surface ds le vtkfile
		throw itk::ExceptionObject("Empty input fiber");

	std::vector<std::string>::iterator it = this->listMeanFiles.begin(), it_end = this->listMeanFiles.end();
	for (; it != it_end; it++) {}
}

/**
 * Function Update()
 */
void CondylesFeaturesExtractor::Update()
{
	this->init_output();

	// Compute normal for each vertex
	this->compute_normals();
	puts("Normals computed");

	// Compute position of each point
	this->compute_positions();
	puts("Positions computed");

	// Compute distance to each mean groups
	// this->compute_distances();
	// puts("Distances computed");
	
	this->outputSurface = this->intermediateSurface;
}

/**
 * Function GetOutput()
 */
vtkSmartPointer<vtkPolyData> CondylesFeaturesExtractor::GetOutput()
{
	return this->outputSurface;
}


