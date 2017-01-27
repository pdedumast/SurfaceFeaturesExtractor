#include <iostream>

#include "computeMeanCLP.h"
#include "computeMean.hxx"
#include "fileIO.hxx"

#include <dirent.h>
#include <iterator>


// Declaration of getListFile()
void getListFile(std::string path, std::vector<std::string> &list, const std::string &suffix);

int main(int argc, char** argv)
{
    PARSE_ARGS;

    vtkSmartPointer<ComputeMean> Filter = vtkSmartPointer<ComputeMean>::New();

    if(argc < 3)
    {
        std::cout << "Usage " << argv[0] << " [--inputList <std::vector<std::string>>] [--outputSurface <std::string>] " << std::endl;
        return 1;
    }

    // Check input dir & Get shapes list
    std::vector<std::string> listShapes;
    // if (!inputDir.empty())
    //     getListFile(inputDir, listShapes, "vtk");
    listShapes = inputList;   
        
    // Check output file
    if(outputSurface.rfind(".vtk")==std::string::npos || outputSurface.empty())
    {
        std::cerr << "Wrong Output File Format, must be a .vtk file" << std::endl;
        return EXIT_FAILURE;
    }

    int groupNumber = 0;
    // **********
    Filter->SetInput(listShapes, groupNumber);
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





// bool DirectoryExists( const char* pzPath )
// {
//     if ( pzPath == NULL) return false;

//     DIR *pDir;
//     bool bExists = false;

//     pDir = opendir (pzPath);

//     if (pDir != NULL)
//     {
//         bExists = true;    
//         (void) closedir (pDir);
//     }

//     return bExists;
// }
