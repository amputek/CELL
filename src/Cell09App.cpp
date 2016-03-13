#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Cell09App : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void Cell09App::setup()
{
    cout << "poopie poop" << "\n";
}

void Cell09App::mouseDown( MouseEvent event )
{
}

void Cell09App::update()
{
}

void Cell09App::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( Cell09App, RendererGl )
