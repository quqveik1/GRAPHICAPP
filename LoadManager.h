#pragma once

struct Image
{
	const char* path = NULL;
	HDC dc = NULL;
    Vector size = {};
};

struct CLoadManager
{
	int currentImagesAmount = 0;

	Image images[1000];


    HDC loadImage(const char* path, Vector size = {});
    void deleteAllImages ();

};


void CLoadManager::deleteAllImages ()
{
    for (int i = 0; i < currentImagesAmount; i++)
    {
        //if (images[i].dc) txDeleteDC (images[i].dc); 
    }
}



HDC CLoadManager::loadImage(const char* path, Vector size /* = {} */)
{
	bool newImage = true;
	int suitableDCNum = -1;
	for (int i = 0; i < currentImagesAmount; i++)
	{
		if (!strcmp(path, images[i].path))
		{
			newImage = false;
			suitableDCNum = i;
			break;
		}
	}

	if (!newImage)
	{
		return images[suitableDCNum].dc;
	}

	
	images[currentImagesAmount].dc = txLoadImage(path);
    if (images[currentImagesAmount].dc == NULL) assert (!path);
	images[currentImagesAmount].path = path;
    images[currentImagesAmount].size = size;
	currentImagesAmount++;

	return images[currentImagesAmount-1].dc;


}
