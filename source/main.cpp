// Example program:
// Using SDL2 to create an application window

#include <SDL.h>
#include <SDL_syswm.h>
#include <iostream>

#include <OgreRoot.h>
#include <OgreManualObject.h>
#include <OgreRenderWindow.h>


void createQuad (Ogre::SceneManager* sceneMgr)
{
	Ogre::ManualObject* manual = sceneMgr->createManualObject();

	// Use identity view/projection matrices to get a 2d quad
	manual->setUseIdentityProjection(true);
	manual->setUseIdentityView(true);

	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("mat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	manual->begin(material->getName());

	manual->position(-1, -1, 0.0);
	manual->textureCoord(0, 1);
	manual->colour(Ogre::ColourValue(1,0,0,1));

	manual->position(1, -1, 0.0);
	manual->textureCoord(1, 1);
	manual->colour(Ogre::ColourValue(1,1,0,1));

	manual->position(1, 1, 0.0);
	manual->textureCoord(1, 0);
	manual->colour(Ogre::ColourValue(1,1,1,1));

	manual->position(-1, 1, 0.0);
	manual->textureCoord(0, 0);
	manual->colour(Ogre::ColourValue(1,0,1,1));

	manual->quad(0,1,2,3);

	manual->end();

	manual->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);

	sceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(manual);
}


int main()
{

  SDL_Init(SDL_INIT_VIDEO);   // Initialize SDL2
  
  SDL_Window *window;
  
  // Create an application window with the following settings:
  window = SDL_CreateWindow( 
    "An SDL2 window",                  //    window title
    SDL_WINDOWPOS_UNDEFINED,           //    initial x position
    SDL_WINDOWPOS_UNDEFINED,           //    initial y position
    640,                               //    width, in pixels
    480,                               //    height, in pixels
    SDL_WINDOW_SHOWN //    flags - see below
  );
  
  // Check that the window was successfully made
  if(window==NULL){   
    // In the event that the window could not be made...
    std::cout << "Could not create window: " << SDL_GetError() << '\n';
    return 1;
  }
  
	Ogre::Root* root = new Ogre::Root("", "", "");
	root->loadPlugin (OGRE_PLUGIN_DIR_REL + std::string("/RenderSystem_GL"));
	root->setRenderSystem(root->getRenderSystemByName("OpenGL Rendering Subsystem"));

	root->initialise(false);


	//get the native whnd
	struct SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);

	if (SDL_GetWindowWMInfo(window, &wmInfo) == SDL_FALSE)
		throw std::runtime_error("Couldn't get WM Info!");

	Ogre::String winHandle;

	switch (wmInfo.subsystem)
	{
	case SDL_SYSWM_X11:
		winHandle = Ogre::StringConverter::toString((unsigned long)wmInfo.info.x11.window);
		break;
	default:
		throw std::runtime_error("Unexpected WM!");
		break;
	}

    Ogre::NameValuePairList params;
    params.insert(std::make_pair("title", "OGRE Window"));
    params.insert(std::make_pair("FSAA", "0"));
    params.insert(std::make_pair("vsync", "false"));
    params.insert(std::make_pair("parentWindowHandle",  winHandle));

	Ogre::RenderWindow* ogreWindow = Ogre::Root::getSingleton().createRenderWindow("OGRE Window", 640, 480, false, &params);
	ogreWindow->setVisible(true);
	
	Ogre::SceneManager* sceneMgr = root->createSceneManager(Ogre::ST_GENERIC);
	Ogre::Camera* camera = sceneMgr->createCamera("cam");
	Ogre::Viewport* vp = ogreWindow->addViewport(camera);
	vp->setBackgroundColour(Ogre::ColourValue(1,0,0,1));

	createQuad(sceneMgr);


while(1){
	root->renderOneFrame();
	
    SDL_Event event;
    if(SDL_PollEvent(&event)){
		if (event.type == SDL_QUIT)
			break;
    }

}

	delete root;

  
  // Close and destroy the window
  SDL_DestroyWindow(window); 
   
  // Clean up
  SDL_Quit();


  
  return 0;   
 
}
