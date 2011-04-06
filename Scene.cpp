/*
    This file belongs to the Ray tracing tutorial of http://www.codermind.com/
    It is free to use for educational purpose and cannot be redistributed
    outside of the tutorial pages.
    Any further inquiry :
    mailto:info@codermind.com
 */

#include "Config.cpp"
#include <iostream>
using namespace std;


const SimpleString emptyString("");
const SimpleString diffuseString ("Diffuse");
const SimpleString intensityString ("Intensity");

void GetMaterial(const Config &sceneFile, Material &currentMat)
{    
    // diffuse color
    {
        float fScalar;
        fScalar =  float(sceneFile.GetByNameAsFloat(diffuseString, 0.0f)); 
        Point vColor;
	vColor.x = fScalar;
	vColor.y = fScalar;
	vColor.z = fScalar;
        vColor = sceneFile.GetByNameAsPoint(diffuseString, vColor);
        currentMat.r   = vColor.x;
        currentMat.g = vColor.y;
	currentMat.b  = vColor.z;
    }

    // Reflection color
    {
        float fScalar;
        fScalar =  float(sceneFile.GetByNameAsFloat("Reflection", 0.0f)); 
		currentMat.reflection = fScalar;
    }
}

void GetSphere(const Config &sceneFile, Sphere &currentSph)
{   Point Origin;
    Origin.x = 0.0;
    Origin.y = 0.0;
    Origin.z = 0.0;
  
    currentSph.c = sceneFile.GetByNameAsPoint("Center", Origin); 

    currentSph.r =  float(sceneFile.GetByNameAsFloat("Size", 0.0f)); 

    currentSph.materialId = sceneFile.GetByNameAsInteger("Material.Id", 0); 
    
    currentSph.transformed = 0;
}

void GetTriangle(const Config &sceneFile, Triangle &currentTri)
{   Point Origin;
    Origin.x = 0.0;
    Origin.y = 0.0;
    Origin.z = 0.0;
  
    currentTri.v0 = sceneFile.GetByNameAsPoint("V0", Origin); 

    currentTri.v1 =  sceneFile.GetByNameAsPoint("V1", Origin); 

    currentTri.v2 =  sceneFile.GetByNameAsPoint("V2", Origin); 
    
    currentTri.materialId = sceneFile.GetByNameAsInteger("Material.Id", 0); 
    
}

void GetAffine(const Config &sceneFile, Sphere &currentSph)
{
    Point Origin;
    Origin.x = 0.0;
    Origin.y = 0.0;
    Origin.z = 0.0;
    
    float fScalar = 0.0;
    currentSph.c = sceneFile.GetByNameAsPoint("Center", Origin); 

    currentSph.r =  float(sceneFile.GetByNameAsFloat("Size", 0.0f)); 
    
    currentSph.materialId = sceneFile.GetByNameAsInteger("Material.Id", 0); 
    
    currentSph.transformed = 1;
    
    Point tlate;
    tlate.x = fScalar;
    tlate.y = fScalar;
    tlate.z = fScalar;
    tlate = sceneFile.GetByNameAsPoint("Translate", Origin);
    currentSph.tlate.x = tlate.x;
    currentSph.tlate.y = tlate.y;
    currentSph.tlate.z = tlate.z;
    
    tlate.x = fScalar;
    tlate.y = fScalar;
    tlate.z = fScalar;
    tlate = sceneFile.GetByNameAsPoint("Scalex", Origin);
    currentSph.mat.m[0][0] = tlate.x;
    currentSph.mat.m[0][1] = tlate.y;
    currentSph.mat.m[0][2] = tlate.z;
    tlate.x = fScalar;
    tlate.y = fScalar;
    tlate.z = fScalar;
    tlate = sceneFile.GetByNameAsPoint("Scaley", Origin);
    currentSph.mat.m[1][0] = tlate.x;
    currentSph.mat.m[1][1] = tlate.y;
    currentSph.mat.m[1][2] = tlate.z;
    tlate.x = fScalar;
    tlate.y = fScalar;
    tlate.z = fScalar;
    tlate = sceneFile.GetByNameAsPoint("Scalez", Origin);
    currentSph.mat.m[2][0] = tlate.x;
    currentSph.mat.m[2][1] = tlate.y;
    currentSph.mat.m[2][2] = tlate.z;
    
    currentSph.mat.m[0][3] = 0;
    currentSph.mat.m[1][3] = 0;
    currentSph.mat.m[2][3] = 0;
    
    currentSph.mat.m[3][0] = 0;
    currentSph.mat.m[3][1] = 0;
    currentSph.mat.m[3][2] = 0;
    currentSph.mat.m[3][3] = 1;
}

void GetLight(const Config &sceneFile, Light &currentLight)
{
    Point Origin;
    Origin.x = 0.0;
    Origin.y = 0.0;
    Origin.z = 0.0;
    
    currentLight.pos = sceneFile.GetByNameAsPoint("Position", Origin); 

    // light color
    {
        float fScalar;
        fScalar =  float(sceneFile.GetByNameAsFloat(intensityString, 0.0f)); 
        Point vColor;
	vColor.x = fScalar;
	vColor.y = fScalar;
	vColor.z = fScalar;
        vColor = sceneFile.GetByNameAsPoint(intensityString, vColor);
	currentLight.col.r   = vColor.x;
        currentLight.col.g = vColor.y;
        currentLight.col.b  = vColor.z;
    }
}

bool init(char* inputName, Scene &myScene)
{
	int nbMats, nbSpheres, nbAffines, nbTriangles, nbLights;
	int i;
	Config sceneFile(inputName);
    if (sceneFile.SetSection("Scene") == -1)
    {
		cout << "Mal formed Scene file : No Scene section." << endl;
		return false;
    }


    myScene.width = sceneFile.GetByNameAsInteger("Image.Width", 640);
    myScene.height = sceneFile.GetByNameAsInteger("Image.Height", 480);

    nbMats = sceneFile.GetByNameAsInteger("NumberOfMaterials", 0);
    nbSpheres = sceneFile.GetByNameAsInteger("NumberOfSpheres", 0);
    nbAffines = sceneFile.GetByNameAsInteger("NumberOfAffines", 0);
    nbTriangles = sceneFile.GetByNameAsInteger("NumberOfTriangles", 0);
    nbLights = sceneFile.GetByNameAsInteger("NumberOfLights", 0);
    

	myScene.materials.resize(nbMats);
	myScene.spheres.resize(nbSpheres+nbAffines);
	myScene.lights.resize(nbLights);
	myScene.triangles.resize(nbTriangles);
	myScene.trianglecount = nbTriangles;
	myScene.spherecount = nbSpheres+nbAffines;
	myScene.lightcount = nbLights;
	myScene.materialcount = nbMats;

	for (i=0; i<nbMats; ++i)
    {   
        Material &currentMat = myScene.materials[i];
        SimpleString sectionName("Material");
        sectionName.append((unsigned long) i);
        if (sceneFile.SetSection( sectionName ) == -1)
        {
		cout << "Mal formed Scene file : Missing Material section." << endl;
		return false;
        }
        GetMaterial(sceneFile, currentMat);
    }
    
    
	for (i=0; i<nbSpheres; ++i)
    {   
        Sphere &currentSphere = myScene.spheres[i];
        SimpleString sectionName("Sphere");
        sectionName.append((unsigned long) i);
        if (sceneFile.SetSection( sectionName ) == -1)
        {
			cout << "Mal formed Scene file : Missing Sphere section." << endl;
		    return false;
        }
        GetSphere(sceneFile, currentSphere);
        if (currentSphere.materialId >= nbMats)
        {
			cout << "Mal formed Scene file : Material Id not valid." << endl;
		    return false;
        }

    }
	
	for (i=nbSpheres; i<(nbSpheres+nbAffines); ++i)
    {   
        Sphere &currentSphere = myScene.spheres[i];
        SimpleString sectionName("AffinedSphere");
        sectionName.append((unsigned long) i-nbSpheres);
        if (sceneFile.SetSection( sectionName ) == -1)
        {
			cout << "Mal formed Scene file : Missing AffinedSphere section." << endl;
		    return false;
        }
        GetAffine(sceneFile, currentSphere);
        if (currentSphere.materialId >= nbMats)
        {
			cout << "Mal formed Scene file : Material Id not valid." << endl;
		    return false;
        }

    }
    
       for (i=0; i<nbTriangles; ++i)
    {   
        Triangle &currentTriangle = myScene.triangles[i];
        SimpleString sectionName("Triangle");
        sectionName.append((unsigned long) i);
        if (sceneFile.SetSection( sectionName ) == -1)
        {
			cout << "Mal formed Scene file : Missing Triangle section." << endl;
		    return false;
        }
        GetTriangle(sceneFile, currentTriangle);
        if (currentTriangle.materialId >= nbMats)
        {
			cout << "Mal formed Scene file : Material Id not valid." << endl;
		    return false;
        }

    }
    
	for (i=0; i<nbLights; ++i)
    {   
        Light &currentLight = myScene.lights[i];
        SimpleString sectionName("Light");
        sectionName.append((unsigned long) i);
        if (sceneFile.SetSection( sectionName ) == -1)
        {
			cout << "Mal formed Scene file : Missing Light section." << endl;
		    return false;
        }
        GetLight(sceneFile, currentLight);
        
    }

	return true;
}

