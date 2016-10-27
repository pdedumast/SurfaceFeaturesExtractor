#include "condylesfeaturesextractor.h"
#include "vtkObjectFactory.h"
#include <vtkSimplePointsWriter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkTetra.h>
#include <string>
#include <fstream>


vtkStandardNewMacro(CondylesFeaturesExtractor);

CondylesFeaturesExtractor::CondylesFeaturesExtractor(){
	this->inputSurface = vtkSmartPointer<vtkPolyData>::New();
	this->outputSurface = vtkSmartPointer<vtkPolyData>::New();
}

CondylesFeaturesExtractor::~CondylesFeaturesExtractor(){}


void CondylesFeaturesExtractor::SetInput(std::string input)
{
	this->inputSurface = readVTKFile(input);
}


void CondylesFeaturesExtractor::init_output()
{
	this->outputSurface = this->inputSurface;
}


void CondylesFeaturesExtractor::compute_normals()
{
	puts("Computing normals");
	vtkSmartPointer<vtkPolyDataNormals> NormalFilter = vtkSmartPointer<vtkPolyDataNormals>::New();
	NormalFilter->SetInputData(this->inputSurface);

	NormalFilter->Update();

	this->outputSurface = NormalFilter->GetOutput();

}


void CondylesFeaturesExtractor::Update()
{
	this->init_output();

	// Compute normal for each vertex
	this->compute_normals();
	puts("Normals computed");

	// Compute distance to each mean groups


}

vtkSmartPointer<vtkPolyData> CondylesFeaturesExtractor::GetOutput()
{
	return this->outputSurface;
}

