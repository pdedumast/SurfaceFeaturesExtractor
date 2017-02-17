#include "condylesfeaturesextractor.hxx"
#include "condylesfileIO.hxx"
#include "condylesfeaturesextractorCLP.h"

#include "argio.hh"

#include <dirent.h>
#include <iterator>

// Declaration of getListFile()
void getListFile(std::string path, std::vector<std::string> &list, const std::string &suffix);

int main (int argc, char *argv[])
{
	PARSE_ARGS;

    const char** argv_const = const_cast<const char**>(argv);
    
    vtkSmartPointer<CondylesFeaturesExtractor> Filter = vtkSmartPointer<CondylesFeaturesExtractor>::New();

    vtkSmartPointer<vtkPolyData> inputShape = readVTKFile(inputMesh.c_str());
    int num_points = inputShape->GetNumberOfPoints();

    // Get list of group mean shapes
    std::vector<std::string> listMeanFiles;
    // if (!meanGroupDir.empty())
        // getListFile(meanGroupDir, listMeanFiles, "vtk");
    listMeanFiles = distMesh;

    // Load each mean groupe shape & create labels
    std::vector< vtkSmartPointer<vtkPolyData> > meanShapesList;
    std::vector<std::string> meanDistLabels;
    for (int k=0; k<listMeanFiles.size(); k++) 
    {
        vtkSmartPointer<vtkPolyData> meanShape = vtkSmartPointer<vtkPolyData>::New();
        meanShape = readVTKFile( listMeanFiles[k].c_str() );

        if (meanShape->GetNumberOfPoints() != num_points)
        {
            std::cerr << "All the shapes must have the same number of points" << std::endl;
            return EXIT_FAILURE;
        } 
        meanShapesList.push_back(meanShape);
    }


    // **********
	Filter->SetInput(inputShape, meanShapesList);
	Filter->Update();
    writeVTKFile(outputMesh.c_str(),Filter->GetOutput());

	return EXIT_SUCCESS;
}


/** Function  getListFile(std::string path, std::vector<std::string> &list, const std::string &suffix)
 * Recupere la liste des fichier dans un dossier
 * @param path : directory of group mean shapes
 * @param list : list of group mean shape
 * @param suffix : files extension
 */

void getListFile(std::string path, std::vector<std::string> &list, const std::string &suffix)
{
    DIR *dir = opendir(path.c_str());
    if (dir != NULL)
    {
        while (dirent *entry = readdir(dir))
        {
            std::string filename = entry->d_name;
            if (filename.size() >= suffix.size() && equal(suffix.rbegin(), suffix.rend(), filename.rbegin()))
            list.push_back(path + "/" + filename);
        }
    }
    closedir(dir);
    sort(list.begin(), list.begin() + list.size());
}


