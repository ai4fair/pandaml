using namespace std;

// C++ includes
#include <iostream>
#include <fstream>

// ROOT includes
#include <TROOT.h>
#include <TGeoManager.h>
#include <TGeoTube.h>

// Functions
void sttRootToCvs(string inputRootFile);
string removePathAndExtension(const string& filepath);

/**
 * @brief Writes STT tube parameters from a ROOT file to a .csv file
 * Gets the STT tube parameters from a ROOT file and writes them to a .csv file
 * 
 * @param inputRootFile Input ROOT file with the STT geometry
 */
void sttRootToCvs(string inputRootFile)
{
	// Create a ROOT geometry manager and import the geometry from the input ROOT file
	TGeoManager *geoManager = new TGeoManager("geoManager", "geoManager");
	geoManager = geoManager->Import(inputRootFile.c_str());

	// Get the full STT volume
	TGeoVolume *fullSttVolume = geoManager->GetVolume("stt01assembly");
	
	// Get the nodes of the full STT volume (frame + tubes)
	TObjArray *sttNodes = fullSttVolume->GetNodes();
	
	// Get the number of nodes/ STT parts
	int numSttParts = sttNodes->GetEntries();
	printf("Number of sttParts: %i \n",numSttParts);
	
	// The geometry matrix includes information about the translation and rotation of the STT parts
	TGeoMatrix *geoMatrix;

	// The volume of a STT part includes information about the shape of the part
	TGeoVolume *Volume;

	// The sttNode of a STT part includes information about the name of the part
	TGeoNode *sttNode;

	// Rotation matrix of a STT part used to define the orientation of the part (9 elements)
	const double *rotationMatrix;
	
	// Translation vector of a STT part used to define the position of the part (3 elements)
	const double *translationVector;

	// The specific shape used for the STT tubes
	TGeoTube *tube;

	// Number of unique rotation matrices
	int numRotMatrices = 0;

	// Boolien to indecate if a new rotation matrix is found
	bool newRotMatrix = false;

	// Vector with all unique rotation matrices
	vector<const double*> rotMatrixVec;

	// The name of the output .csv file will be the same as the input file
	string outputCsvFileName = removePathAndExtension(inputRootFile) + ".csv";
	printf("Output file: %s \n",outputCsvFileName.c_str());
	
	// Create and open the output .csv file
	std::ofstream outCsvFile(outputCsvFileName.c_str());

	// Write the header of the .csv file
	outCsvFile << "name, x, y, z, rMin, rMax, dz, angle \n";
	
	// Iterate over all STT parts
	for (int i = 0; i < numSttParts; i++)
	{
		// Get the STT part node and its name
		sttNode = (TGeoNode*)sttNodes->At(i);
		string nodeName = sttNode->GetName();

		// Only save the node part parameters when the part is a tube
		if (nodeName.find("tube") != std::string::npos)
		{
			// Get all the information from the tube
			geoMatrix = sttNode->GetMatrix();
			Volume = sttNode->GetVolume();
			translationVector = geoMatrix->GetTranslation();
			tube = (TGeoTube*)Volume->GetShape();
			
			// Save the information in the .csv file
			outCsvFile 	<< nodeName 			<< ", " << translationVector[0] << ", "	<< translationVector[1] << ", " 
						<< translationVector[2] << ", " << tube->GetRmin() 		<< ", " << tube->GetRmax() 		<< ", " 
						<< tube->GetDz() << ", ";

			// Get information about the skewed tubes
			if(geoMatrix->IsRotation())
			{
				// Get the rotation matrix and calculate the rotation angle and vector
				rotationMatrix = geoMatrix->GetRotationMatrix();
				double traceRotationMatrix = rotationMatrix[0]+rotationMatrix[4]+rotationMatrix[8];
				double rotationAngle = acos((traceRotationMatrix-1)/2);
				double rotationAngleInDegree = rotationAngle/(2*M_PI)*360.0;
				double rotationVector[3] = {rotationMatrix[7]-rotationMatrix[5], rotationMatrix[2]-rotationMatrix[6], rotationMatrix[3]-rotationMatrix[1]};

				// check if the rotation is in the same direction as the translation vector to decide the sign of the angle
				if(	(std::signbit(rotationVector[0]) == std::signbit(translationVector[0])) && 
					(std::signbit(rotationVector[1]) == std::signbit(translationVector[1]) ||
					 rotationVector[1] == 0))
				{
					outCsvFile << rotationAngleInDegree << "\n";
				}
				else
				{
					outCsvFile << -1.*rotationAngleInDegree << "\n";
				}
			}
			else
			{
				outCsvFile << "0 \n";
			}
		}

		// Find out how many and which different rotation matrices are used

		// First check if the part is a tube and is rotated
		if (geoMatrix->IsRotation() && nodeName.find("tube") != std::string::npos )
		{
			rotationMatrix = geoMatrix->GetRotationMatrix();

			// If this is the first rotation matrix its always unique so add it to the vector
			if (numRotMatrices == 0)
			{
				rotMatrixVec.push_back(rotationMatrix);
				numRotMatrices++;
			}
			else // Check if the rotation matrix is already in the vector
			{
				newRotMatrix = false;

				// Compare every rotation matrix with the new one
				for (int j = 0; j < numRotMatrices; j++)
				{
					// Compare the 9 elements of the rotation matrices
					bool newRotElement = false;
					for (int k = 0; k < 9; k++)
					{
						if (fabs(rotationMatrix[k] - rotMatrixVec[j][k]) > 1e-6)
						{
							newRotElement = true;
							break;
						}
					}
					if (!newRotElement) // If all elements are the same the rotation matrix is not new
					{
						newRotMatrix = false;
						break;
					}
					else // If two elements are different the next rotation matrix is checked
					{
						newRotMatrix = true;
					}

				}
				if (newRotMatrix) // If new elements are in each rotation matrix add it to the vector as a unique rotation matrix
				{
					rotMatrixVec.push_back(rotationMatrix);
					numRotMatrices++;
				}
			}
		}
	}

	// Print the unique rotation matrices
	for (std::vector<const double*>::iterator it = rotMatrixVec.begin(); it != rotMatrixVec.end(); ++it)
	{
		rotationMatrix = *it;
		printf("The rotation matrix is: \n");
		for (int j = 0; j < 3; j++)
		{
			printf("%f %f %f \n",rotationMatrix[j*3],rotationMatrix[j*3+1],rotationMatrix[j*3+2]);
		}
		double traceRotationMatrix = rotationMatrix[0]+rotationMatrix[4]+rotationMatrix[8];
		double rotationAngle = acos((traceRotationMatrix-1)/2);
		double rotationVector[3] = {rotationMatrix[7]-rotationMatrix[5],
									rotationMatrix[2]-rotationMatrix[6], 
									rotationMatrix[3]-rotationMatrix[1]};
		double rotationVectorNorm = 2.*sin(rotationAngle);
		double normalizedRotationVector[3] = {rotationVector[0]/rotationVectorNorm, 
											  rotationVector[1]/rotationVectorNorm, 
											  rotationVector[2]/rotationVectorNorm};

		printf("The rotation angle is %f degrees \n",rotationAngle/(2*M_PI)*360.0);
		printf("The rotation vector is: %f %f %f \n",normalizedRotationVector[0],normalizedRotationVector[1],normalizedRotationVector[2]);
	}

	std::cout << "Number of different rotation matrices: " << numRotMatrices << std::endl;

	outCsvFile.close();

	exit(0);
}

/**
 * @brief Removes path and file extensions
 * Removes every caracter before the last "/" and after the last '.' in a given string
 * 
 * @param filename Input string
 * @return string Output the input string without the path and the file extension
 */
string removePathAndExtension(const string& filepath) 
{
    // Find the last directory separator
    size_t lastSlashPos = filepath.find_last_of("/");
    string baseFilename = (lastSlashPos == string::npos) ? filepath : filepath.substr(lastSlashPos + 1);

    // Find the last dot in the base filename
    size_t lastDotPos = baseFilename.find_last_of('.');
    if (lastDotPos != string::npos) {
        baseFilename = baseFilename.substr(0, lastDotPos);
    }

    return baseFilename;
}