#pragma once

struct Image
{
	const char* path = NULL;
	HDC dc = NULL;
};

struct CLoadManager
{
	int currentImagesAmount = 0;

	Image images[1000];


	HDC loadImage (const char* path);
    void deleteAllImages ();

};


void CLoadManager::deleteAllImages ()
{
    for (int i = 0; i < currentImagesAmount; i++)
    {
        //if (images[i].dc) txDeleteDC (images[i].dc); 
    }
}



HDC CLoadManager::loadImage(const char* path)
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
	currentImagesAmount++;

	return images[currentImagesAmount-1].dc;


}
