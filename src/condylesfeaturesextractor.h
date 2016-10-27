#ifndef CONDYLESFEATURESEXTRACTOR_H
#define CONDYLESFEATURESEXTRACTOR_H

#include <vtkSmartPointer.h>
#include <vtkPolyDataAlgorithm.h>
#include <vtkPolyDataNormals.h>
// #include <vtkPolyDataReader.h>
// #include <vtkPolyDataWriter.h>
#include <fstream>
#include "condylesfileIO.h"


class CondylesFeaturesExtractor : public vtkPolyDataAlgorithm
{
public:
    /** Conventions for a VTK Class*/
    vtkTypeMacro(CondylesFeaturesExtractor,vtkPolyDataAlgorithm);
    static CondylesFeaturesExtractor *New(); 

    /** Set the inputs data of the filter */
    void SetInput(std::string input);

    // Update the filter and process the output
    void Update();

    /**
     * Return the output of the Filter
     * @return       output of the Filter
     */
    vtkSmartPointer<vtkPolyData> GetOutput();

private:
    /** Variables */
    vtkSmartPointer<vtkPolyData> inputSurface;
    vtkSmartPointer<vtkPolyData> outputSurface;

    vtkSmartPointer<vtkPolyData> intermediateSurface;

    /**
     * Initialize outputSurface
     */
    void init_output();

    /**
     * Compute normals
     */
    void compute_normals();





protected:
    /** Constructor & Destructor */
    CondylesFeaturesExtractor();
    ~CondylesFeaturesExtractor();

};

#endif // CONDYLESFEATURESEXTRACTOR_H