#include "condylesfeaturesextractor.hxx"
#include "condylesfileIO.hxx"
#include "condylesfeaturesextractorCLP.h"

#include <dirent.h>
#include <iterator>

// Declaration of getListFile()
void getListFile(std::string path, std::vector<std::string> &list, const std::string &suffix);

int main (int argc, char *argv[])
{
	PARSE_ARGS;

    vtkSmartPointer<CondylesFeaturesExtractor> Filter = vtkSmartPointer<CondylesFeaturesExtractor>::New();

    if (argc < 3)
    {
        cout << "Usage: " << argv[0] << " --input" << " [input surface name <std::string>]" << " --output" << " [output surface name <std::string>]" << " --meanGroup" << " [mean shapes directory <std::vector<std::string>>]" << endl;
        return EXIT_FAILURE;
    }
    // Check input shape
    if(inputSurface.rfind(".vtk")==std::string::npos || inputSurface.empty())
    {
        std::cerr << "Wrong Input File Format, must be a .vtk or .vtp file" << std::endl;
        return EXIT_FAILURE;
    }
    // Check output file
    if(outputSurface.rfind(".vtk")==std::string::npos || outputSurface.empty())
    {
        std::cerr << "Wrong Output File Format, must be a .vtk file" << std::endl;
        return EXIT_FAILURE;
    }
        
    // Get list of group mean shapes
    std::vector<std::string> listMeanFiles;
    // if (!meanGroupDir.empty())
        // getListFile(meanGroupDir, listMeanFiles, "vtk");
    listMeanFiles = meanGroupDir;
    // **********
	Filter->SetInput(inputSurface.c_str(), listMeanFiles);
	Filter->Update();
    writeVTKFile(outputSurface.c_str(),Filter->GetOutput());

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


