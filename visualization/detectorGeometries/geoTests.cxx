#include "FairRunSim.h"
#include "PndCave.h"
#include "PndStt.h"
#include "FairGenericVMCConfig.h"
#include "FairRootFileSink.h"
#include "TGeoManager.h"
#include "TGeoTube.h"
#include <cmath>
#include "PndMasterRunSim.h"
#include "PndSttMapCreator.h"

void geoTests()
{
	// -----   Create geometry from geo file   ----------------------------------------
	// std::string outFile = "test.root";
	// FairRunSim *run = new FairRunSim();
	// run->SetName("TGeant4");   // Transport engine
	// FairGenericVMCConfig *config = new FairGenericVMCConfig();
	// run->SetSimulationConfig(std::move(config));
	// run->SetIsMT(true);   // Multi-threading mode (Geant4 only)
	// FairRootFileSink* sink = new FairRootFileSink(outFile);
	// run->SetSink(sink);
	// FairRuntimeDb* rtdb = run->GetRuntimeDb();

	// run->SetMaterials("media_pnd.geo");
	
	// FairModule* cave = new PndCave("CAVE");
	// cave->SetGeometryFileName("pndcave.geo");
	// run->AddModule(cave);

	// FairDetector* stt = new PndStt("STT", kTRUE);
	// stt->SetGeometryFileName("straws_skewed_blocks_35cm_pipe.geo");
	// run->AddModule(stt);
	// run->Init();
	// run->CreateGeometryFile("sttGeo.root");

	TGeoManager *geom = new TGeoManager("geom", "geom");
	geom = geom->Import("sttGeo.root");

	TObjArray *volArray = geom->GetListOfVolumes();
	// for (int i = 0; i < volArray->GetEntries(); i++)
	// {
	//     TGeoVolume *vol = (TGeoVolume *)volArray->At(i);
	//     std::cout << vol->GetName() << std::endl;
	// }

	// volArray->At(1)->Print();

	TGeoVolume *fullStt = geom->GetVolume("stt01assembly");
	TObjArray *nodes = fullStt->GetNodes();
	int numEntries = nodes->GetEntries();
	TGeoMatrix *matrix;
	TGeoVolume *vol;
	TGeoNode *node = (TGeoNode*)nodes->At(0);
	std::cout << nodes->At(1)->GetName() << std::endl;
	std::cout << nodes->At(2)->GetName() << std::endl;
	std::cout << nodes->At(3)->GetName() << std::endl;
	std::cout << nodes->At(4)->GetName() << std::endl;
	std::cout << nodes->At(5)->GetName() << std::endl;
	std::cout << nodes->At(6)->GetName() << std::endl;
	std::cout << nodes->At(7)->GetName() << std::endl;
	std::cout << nodes->At(8)->GetName() << std::endl;
	std::cout << nodes->At(9)->GetName() << std::endl;

	std::string nodeName = node->GetName();
	const double *rotMatrix;

	int numRotMatrices = 0;

	std::vector<const double*> rotMatrixVec;

	bool newRotMatrix = false;

	const double *transMatrix;

	TGeoTube *tube;

	std::ofstream outfile ("tubePos.csv");
	outfile << "name, x, y, z, rMin, rMax, dz, angle \n";
	
	printf("Number of nodes: %i \n",numEntries);
	for (int i = 0; i < numEntries; i++)
	{
		node = (TGeoNode*)nodes->At(i);
		matrix = node->GetMatrix();
		vol = node->GetVolume();
		std::string nodeName = node->GetName();

		if (nodeName.find("tube") != std::string::npos)
		{
			transMatrix = matrix->GetTranslation();
			tube = (TGeoTube*)vol->GetShape();
			
			outfile << nodeName << ", " << transMatrix[0] << ", " << transMatrix[1] << ", " << transMatrix[2] << ", " << tube->GetRmin() << ", " << tube->GetRmax() << ", " << tube->GetDz() << ", ";

			if(matrix->IsRotation())
			{
				rotMatrix = matrix->GetRotationMatrix();
				double traceRotMatrix = rotMatrix[0]+rotMatrix[4]+rotMatrix[8];
				double rotAngle = acos((traceRotMatrix-1)/2);
				double rotVec[3] = {rotMatrix[7]-rotMatrix[5], rotMatrix[2]-rotMatrix[6], rotMatrix[3]-rotMatrix[1]};
				double rotAngleDeg = rotAngle/(2*M_PI)*360.0;

				if((std::signbit(rotVec[0]) == std::signbit(transMatrix[0])) && (std::signbit(rotVec[1]) == std::signbit(transMatrix[1]) || rotVec[1] == 0))
				{
					outfile << rotAngleDeg << "\n";
				}
				else
				{
					outfile << -1.*rotAngleDeg << "\n";
				}
			}
			else
			{
				outfile << "0 \n";
			}
		}


		if (matrix->IsRotation() && nodeName.find("tube") != std::string::npos )
		{
			rotMatrix = matrix->GetRotationMatrix();
			if (numRotMatrices == 0)
			{
				rotMatrixVec.push_back(rotMatrix);
				numRotMatrices++;
			}
			else
			{
				newRotMatrix = false;
				for (int j = 0; j < numRotMatrices; j++)
				{
					bool newRotElement = false;
					for (int k = 0; k < 9; k++)
					{
						if (fabs(rotMatrix[k] - rotMatrixVec[j][k]) > 1e-6)
						{
							newRotElement = true;
							break;
						}
					}
					if (!newRotElement)
					{
						newRotMatrix = false;
						break;
					}
					else
					{
						newRotMatrix = true;
					}

				}
				if (newRotMatrix)
				{
					rotMatrixVec.push_back(rotMatrix);
					numRotMatrices++;
				}
			}
		}
	}

	for (std::vector<const double*>::iterator it = rotMatrixVec.begin(); it != rotMatrixVec.end(); ++it)
	{
		rotMatrix = *it;
		printf("The rotation matrix is: \n");
		for (int j = 0; j < 3; j++)
		{
			printf("%f %f %f \n",rotMatrix[j*3],rotMatrix[j*3+1],rotMatrix[j*3+2]);
		}
		double traceRotMatrix = rotMatrix[0]+rotMatrix[4]+rotMatrix[8];
		double rotAngle = acos((traceRotMatrix-1)/2);
		double rotVec[3] = {rotMatrix[7]-rotMatrix[5], rotMatrix[2]-rotMatrix[6], rotMatrix[3]-rotMatrix[1]};
		double rotVecNorm = 2.*sin(rotAngle);
		double normRotVec[3] = {rotVec[0]/rotVecNorm, rotVec[1]/rotVecNorm, rotVec[2]/rotVecNorm};

		printf("The rotation angle is %f degrees \n",rotAngle/(2*M_PI)*360.0);
		printf("The rotation vector is: %f %f %f \n",normRotVec[0],normRotVec[1],normRotVec[2]);
	}

	std::cout << "Number of different rotation matrices: " << numRotMatrices << std::endl;

	outfile.close();


	exit(0);
}