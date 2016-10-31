#ifndef CONDYLESFEATURESEXTRACTOR_H
#define CONDYLESFEATURESEXTRACTOR_H

#include <vtkSmartPointer.h>
#include <vtkPolyDataAlgorithm.h>
#include <vtkPolyDataNormals.h>
#include <vtkPointData.h>
#include <vtkFloatArray.h>
#include "condylesfileIO.h"


class CondylesFeaturesExtractor : public vtkPolyDataAlgorithm
{
public:
    /** Conventions for a VTK Class*/
    vtkTypeMacro(CondylesFeaturesExtractor,vtkPolyDataAlgorithm);
    static CondylesFeaturesExtractor *New(); 

    /** Function SetInput(std::string input, std::vector<std::string> list)
    * Set the inputs data of the filter
    * @param input : input shape
    * @param list : list of group mean shapes
    */
    void SetInput(std::string input, std::vector<std::string> list);

    /** Function Update()
     * Update the filter and process the output
     */
    void Update();

    /**
     * Return the output of the Filter
     * @return : output of the Filter CondylesFeaturesExtractor
     */
    vtkSmartPointer<vtkPolyData> GetOutput();

private:
    /** Variables */
    vtkSmartPointer<vtkPolyData> inputSurface;
    vtkSmartPointer<vtkPolyData> outputSurface;

    vtkSmartPointer<vtkPolyData> intermediateSurface;
    std::vector<std::string> listMeanFiles;

    /** Function init_output()
     * Initialize outputSurface
     */
    void init_output();

    /** Function compute_normals()
     * Compute normals of the input surface
     */
    void compute_normals();

    /** Function compute_positions()
     * Compute position of each point of the shape
     */
    void compute_positions();

    /** Function compute_distances()
     * Compute distance to each mean group model
     */
    void compute_distances();

    



protected:
    /** Constructor & Destructor */
    CondylesFeaturesExtractor();
    ~CondylesFeaturesExtractor();

};

#endif // CONDYLESFEATURESEXTRACTOR_H