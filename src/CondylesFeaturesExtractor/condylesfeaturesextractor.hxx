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
	this->intermediateSurface = this->inputSurface;
	this->outputSurface = this->inputSurface;
}

/** 
 * Function compute_normals()
 */
void CondylesFeaturesExtractor::compute_normals()
{
	// puts(" :: Function compute_normals");
	vtkSmartPointer<vtkPolyDataNormals> NormalFilter = vtkSmartPointer<vtkPolyDataNormals>::New();
	NormalFilter->SetInputData(this->intermediateSurface);

	NormalFilter->ComputePointNormalsOn();
    NormalFilter->ComputeCellNormalsOff();
    NormalFilter->SetFlipNormals(0);
    NormalFilter->SplittingOff();
	NormalFilter->FlipNormalsOff();
	NormalFilter->ConsistencyOff();

	NormalFilter->Update();
	this->intermediateSurface = NormalFilter->GetOutput();

}

/** 
 * Function compute_positions()
 */
void CondylesFeaturesExtractor::compute_positions()
{
	// std::cout<<" :: Function compute_positions"<<std::endl;

    std::string name = "position";
    int nbPoints = this->intermediateSurface->GetNumberOfPoints();

	vtkFloatArray* position = vtkFloatArray::New();
	position->SetNumberOfComponents(3);
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
	// std::cout<<" :: Function compute_distances"<<std::endl;

	int nbPoints = this->intermediateSurface->GetNumberOfPoints();

	// Load each mean groupe shape & create labels
	std::vector< vtkSmartPointer<vtkPolyData> > meanShapesList;
	std::vector<std::string> meanDistLabels;
	for (int k=0; k<this->listMeanFiles.size(); k++) 
	{
		vtkSmartPointer<vtkPolyData> meanShape = vtkSmartPointer<vtkPolyData>::New();
		meanShape = readVTKFile( this->listMeanFiles[k].c_str() );
		meanShapesList.push_back(meanShape);

		std::string k_char = static_cast<std::ostringstream*>( &( std::ostringstream() << k) )->str();
		meanDistLabels.push_back("distanceGroup"+k_char);
	}
	int trouve = 0;
	for(int k=0; k<meanShapesList.size(); k++)
	{
		vtkSmartPointer<vtkFloatArray> meanDistance = vtkFloatArray::New() ;
		meanDistance->SetName(meanDistLabels[k].c_str());

		for (int i=0; i<nbPoints; i++)
		{
			double* p1 = new double[3];
			p1 = this->intermediateSurface->GetPoint(i);
			
			double* p2 = new double[3];
			p2 = meanShapesList[k]->GetPoint(i);

			double dist = sqrt( (p1[0]-p2[0])*(p1[0]-p2[0]) + (p1[1]-p2[1])*(p1[1]-p2[1]) + (p1[2]-p2[2])*(p1[2]-p2[2]) );
			
			if (i>=1002)	// 1002 = nb de vertices sur un min
				dist = 0;

			meanDistance->InsertNextTuple1(dist);

			this->intermediateSurface->GetPointData()->SetActiveScalars(meanDistLabels[k].c_str());
			this->intermediateSurface->GetPointData()->SetScalars(meanDistance);
		}
	}
}


void CondylesFeaturesExtractor::compute_maxcurvatures()		// Kappa2
{
	// std::cout<<" :: Function compute_curvatures"<<std::endl;

	vtkSmartPointer<vtkCurvatures> curvaturesFilter = vtkSmartPointer<vtkCurvatures>::New();

	curvaturesFilter->SetInputDataObject(this->intermediateSurface);
	curvaturesFilter->SetCurvatureTypeToMaximum();
	curvaturesFilter->Update();

	this->intermediateSurface = curvaturesFilter->GetOutput();
}
void CondylesFeaturesExtractor::compute_mincurvatures()		// Kappa1
{
	// std::cout<<" :: Function compute_curvatures"<<std::endl;

	vtkSmartPointer<vtkCurvatures> curvaturesFilter = vtkSmartPointer<vtkCurvatures>::New();

	curvaturesFilter->SetInputDataObject(this->intermediateSurface);
	curvaturesFilter->SetCurvatureTypeToMinimum();
	curvaturesFilter->Update();

	this->intermediateSurface = curvaturesFilter->GetOutput();	
}
void CondylesFeaturesExtractor::compute_gaussiancurvatures()	// G
{
	// std::cout<<" :: Function compute_gaussiancurvatures"<<std::endl;

	vtkSmartPointer<vtkCurvatures> curvaturesFilter = vtkSmartPointer<vtkCurvatures>::New();

	curvaturesFilter->SetInputDataObject(this->intermediateSurface);
	curvaturesFilter->SetCurvatureTypeToGaussian();
	curvaturesFilter->Update();

	this->intermediateSurface = curvaturesFilter->GetOutput();
}
void CondylesFeaturesExtractor::compute_meancurvatures()		// H
{
	// std::cout<<" :: Function compute_meancurvatures"<<std::endl;
	vtkSmartPointer<vtkCurvatures> curvaturesFilter = vtkSmartPointer<vtkCurvatures>::New();

	curvaturesFilter->SetInputDataObject(this->intermediateSurface);
	curvaturesFilter->SetCurvatureTypeToMean();
	curvaturesFilter->Update();

	this->intermediateSurface = curvaturesFilter->GetOutput();
}

// void CondylesFeaturesExtractor::compute_shapeindex()			// S
// {
// 	// std::cout<<" :: Function compute_shapeindex"<<std::endl;
// 	vtkSmartPointer<vtkCurvatures> curvaturesFilter = vtkSmartPointer<vtkCurvatures>::New();

// 	curvaturesFilter->SetInputDataObject(this->intermediateSurface);
// 	curvaturesFilter->SetCurvatureTypeToMean();
// 	curvaturesFilter->Update();

// 	this->intermediateSurface = curvaturesFilter->GetOutput();
// }

/**
 * Function Update()
 */
void CondylesFeaturesExtractor::Update()
{
	this->init_output();

	// Compute normal for each vertex
	this->compute_normals();
	// puts("Normals computed");

	// Compute position of each point
	this->compute_positions();
	// puts("Positions computed");

	// Compute distance to each mean groups
	this->compute_distances();
	// puts("Distances computed");

	// Compute curvatures at each point
	this->compute_maxcurvatures();
	// puts("Curvatures max computed");

	this->compute_mincurvatures();
	// puts("Curvatures min computed");

	this->compute_gaussiancurvatures();
	// puts("Curvatures gaussian computed");

	this->compute_meancurvatures();
	// puts("Curvatures mean computed");

	this->outputSurface = this->intermediateSurface;
}

/**
 * Function GetOutput()
 */
vtkSmartPointer<vtkPolyData> CondylesFeaturesExtractor::GetOutput()
{
	return this->outputSurface;
}


