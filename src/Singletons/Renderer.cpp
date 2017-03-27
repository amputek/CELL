#include "Renderer.hpp"

CellRenderer :: CellRenderer(){
    
    planktonImgs[0] = loadPngImage( "plankton/a" );
    planktonImgs[1] = loadPngImage( "plankton/b" );
    planktonImgs[2] = loadPngImage( "plankton/c" );
    planktonImgs[3] = loadPngImage( "plankton/d" );
    planktonImgs[4] = loadPngImage( "plankton/e" );
    
    beamImg =       loadPngImage( "environment/beam3" );
    bubbleImgs[0] = loadPngImage( "environment/bubble/0" );
    bubbleImgs[1] = loadPngImage( "environment/bubble/1" );
    bubbleImgs[2] = loadPngImage( "environment/bubble/2" );
    splashImg =     loadPngImage( "environment/splash" );
    maskImg =       loadPngImage( "environment/mask-1" );
    
    cursorImg = loadPngImage( "cursor" );
   
    sporeImgs[0][0] = loadPngImage( "spore/orange_0" );
    sporeImgs[0][1] = loadPngImage( "spore/orange_1" );
    sporeImgs[0][2] = loadPngImage( "spore/orange_2" );

    sporeImgs[1][0] = loadPngImage( "spore/purple_0" );
    sporeImgs[1][1] = loadPngImage( "spore/purple_1" );
    sporeImgs[1][2] = loadPngImage( "spore/purple_2" );
    
    sporeImgs[2][0] = loadPngImage( "spore/green_0" );
    sporeImgs[2][1] = loadPngImage( "spore/green_1" );
    sporeImgs[2][2] = loadPngImage( "spore/green_2" );

    playerImgs[0] = loadPngImage( "player/zero2" );
    playerImgs[1] = loadPngImage( "player/one" );
    playerImgs[2] = loadPngImage( "player/two" );
    
    jellyImgs[0] = loadPngImage( "jelly/blue" );
    jellyImgs[1] = loadPngImage( "jelly/green" );
    jellyImgs[2] = loadPngImage( "jelly/pink" );
    
    friendlyImg = loadPngImage( "friendly" );
    urchinImg = loadPngImage( "twirl" );
    eggImg = loadPngImage( "egg" );
    sparkImg = loadPngImage( "spark" );
    
    title1 = loadPngImage( "title/1" );
    title2 = loadPngImage( "title/2" );
    title3 = loadPngImage( "title/3" );
    title4 = loadPngImage( "title/4" );



    gl::Fbo::Format format;
    miniMap = gl::Fbo::create( 800, 600, format );
    


    
//    loadPngImage( "mask-3"            );
}

void CellRenderer::drawBeam( const vec2 & global, float depth, float op ){
    
    
    float x = toLocal( global, depth ).x;
    
    entityDrawCount++;
    gl::color(ColorA(1.0,1.0,1.0,op));
    gl::ScopedModelMatrix modelScope;
    gl::translate(vec2(x,0));
    
    
    float scale = beamImg->getHeight() / cinder::app::getWindowHeight();
    Rectf scaledRect = Rectf(0, 0, beamImg->getWidth() * scale, beamImg->getHeight() * scale );
    
    gl::draw( beamImg, scaledRect );
}


void CellRenderer::drawSplash( const vec2 & global, float currentLife, float maxLife, float startSize )
{
    float radius = startSize + currentLife;
    
    vec2 pos = toLocal(global,1);
    
    bool isOnScreen = onScreen( pos, radius + 10 );
    
    if(isOnScreen){
        
        entityDrawCount++;
        
        
        float opacityRadiusMod = 1.0f;
        
        opacityRadiusMod = (radius-14.0f) * 0.002f;
        if(opacityRadiusMod >= 1.0f) opacityRadiusMod = 1.0f;
        if(opacityRadiusMod <= 0.4f) opacityRadiusMod = 0.4f;
        
        float opacity = maxLife - currentLife;
        opacity *= opacityRadiusMod;
        
        
        gl::color(ColorA8u(255,255,255, opacity));
        gl::ScopedModelMatrix modelScope;
        gl::translate( pos );
        gl::draw( splashImg, Rectf(-radius, -radius, radius, radius) );
        
    }
}

void CellRenderer::drawBubble( const vec2 & global , float radius, float depth )
{
    entityDrawCount++;
    
    vec2 pos = toLocal(global, depth);
    
    bool isOnScreen = onScreen( pos, radius + 10 );
    
    if(isOnScreen){
        int img = 0;
        if( depth < 0.8 || depth > 1.2 ) img = 1;
        if( depth < 0.5 || depth > 1.5 ) img = 2;
        
        gl::color(ColorA( 1.0, 1.0, 1.0, depth * 0.6 ));
        gl::ScopedModelMatrix modelScope;
        gl::translate( pos );
        gl::draw( bubbleImgs[img], Rectf(-radius,-radius,radius,radius) );
    }
}

void CellRenderer::drawPlayer( const vec2 & global, float direction, float radius, int bodyType )
{
    
    entityDrawCount++;
    
    gl::color(Color(1,1,1));
    gl::ScopedBlendAlpha alpha;
    gl::pushModelView();
    gl::translate( toLocal(global, 1.0f) );
    gl::rotate( -direction + M_PI );
    gl::draw( playerImgs[bodyType] , Rectf( -radius*2, -radius*2, radius*2, radius*2) );
    gl::popModelView();
    
    addToMiniMap( global, Color(1,1,1), radius, true);
}

void CellRenderer::drawTail( vector<vec2> positions, bool wideTail, bool fins, float width, float direction )
{
    
    Path2d mPath;
    Path2d lPath;
    Path2d rPath;
    
    float rotMod = M_PI/2.5;
    
    vector<Path2d> finPaths;
    vector<Path2d> ridgePaths;
    
    for( int i = 0; i < positions.size(); i++ )
    {
        positions[i] = toLocal(positions[i], 1);
    }
    
    //initilase the path2Ds
    if(positions.size() > 0)
    {
        mPath.moveTo(positions[0]);
        if( wideTail ){
            lPath.moveTo(positions[0]);
            rPath.moveTo(positions[0]);
        }
    }
    
    
    //draw Line to each part of the tail
    int index = 0;
    
    for(int i = 1; i < positions.size(); i++ )
    {
        
        mPath.lineTo( positions[i] );
        
        if(wideTail){
            //width of the tail increases towards body
            //points on the 'wide' parts of the tail are points on the tail + a sin/cos vector
            float sizeMod = width * index / positions.size();
            lPath.lineTo(vec2(positions[i].x + sin(direction + rotMod) * sizeMod, positions[i].y + cos(direction + rotMod) * sizeMod));
            rPath.lineTo(vec2(positions[i].x + sin(direction - rotMod) * sizeMod, positions[i].y + cos(direction - rotMod) * sizeMod));
            
            
            //add ridges along the tail, keep these in a collection
            Path2d sPath;
            sPath.moveTo(lPath.getCurrentPoint());
            sPath.curveTo(mPath.getCurrentPoint(), mPath.getCurrentPoint(), rPath.getCurrentPoint());
            ridgePaths.push_back( sPath );
            
            
        }
        index++;
    }
    
    //add Fins
    if(fins){
        finPaths.clear();
        if(positions.size() > 5){
            Path2d fin1 = drawFin( positions[3],positions[2],positions[1],positions[2], width);
            gl::draw( fin1 );
        }
        if(positions.size() > 18){
            Path2d fin2 = drawFin( positions[15],positions[14],positions[12],positions[13], width);
            gl::draw( fin2 );
        }
    }
    
    
    
    gl::color(ColorA8u(255,255,255,255));
    gl::draw(mPath);
    
    //draw side paths if tail is wide
    if(wideTail == true){
        gl::draw(lPath);
        gl::draw(rPath);
        for(int i = 0; i < ridgePaths.size(); i++){
            gl::draw(ridgePaths.at(i) );
        }
    }
    
    
}



void CellRenderer::drawSpore( const vec2 & global, float radius, float depth, int sporeType )
{
    entityDrawCount++;
    
    int blurLevel = 2;
    if( depth > 0.5) blurLevel = 1;
    if( depth > 0.8) blurLevel = 0;
    
    vec2 pos = toLocal( global, depth );
    bool isOnScreen = onScreen( pos, 30 );
    if( isOnScreen )
    {
        
        gl::pushModelView();
        gl::color( ColorA(1.0,1.0,1.0,1.0 * depth) );
        gl::ScopedModelMatrix modelScope;
        gl::translate( pos );
        gl::draw( sporeImgs[sporeType][blurLevel], Rectf( -radius, -radius, radius, radius ) );
        gl::popModelView();
        
    }
    
    addToMiniMap( global, Color(0.5,1,0.5), radius, isOnScreen);
}

void CellRenderer::drawFriendly( const vec2 & global, float direction, float radius )
{
    vec2 pos = toLocal(global,1);
    
    bool isOnScreen = onScreen( pos, radius + 10 );
    
    if( isOnScreen )
    {
        entityDrawCount++;
        gl::pushModelView();
        gl::color(1,1,1);
        gl::translate( pos );
        gl::rotate( -direction + M_PI );
        gl::draw( friendlyImg , Rectf( -radius*2, -radius*2, radius*2, radius*2) );
        gl::popModelView();
    }
    
    addToMiniMap( global, Color(0.2,1,0.2), radius, isOnScreen);
}


void CellRenderer::drawSpark( const vec2 & global, float radius, int type, const vector<vec2> & positions, const vector<float> & sizes )
{
    vec2 pos = toLocal(global,1);
    
    bool isOnScreen = onScreen( pos, radius );
    
    if(isOnScreen){
        
        entityDrawCount++;
        
        if(type == 0){ gl::color(ColorA8u( 255         , 80+radius*3  , 20+radius*4,     220 ) ); }
        if(type == 1){ gl::color(ColorA8u( 108+radius*3, 5+radius*3   , 255        ,     220 ) ); }
        if(type == 2){ gl::color(ColorA8u( 75+radius*4 , 228	      , 178	       ,     220 ) ); }
        
        //draw a few circles at each Finite position
        for(int i = 0; i < positions.size(); i++){
            gl::ScopedModelMatrix modelScope;
            gl::translate( toLocal(positions[i],1) );
            float r = sizes[i];
            gl::draw( sparkImg, Rectf(-r, -r, r, r ));
        }
    
        
        gl::ScopedModelMatrix modelScope;
        gl::translate( pos );
        gl::draw( sparkImg, Rectf(-radius, -radius, radius, radius ));
        gl::draw( sparkImg, Rectf(-radius/2, -radius/2, radius/2, radius/2 ));
        
        
    }
    
    addToMiniMap( global, Color(1,1,0.2), radius, isOnScreen);
}

void CellRenderer::drawPlankton( const vec2 & global, float depth, float radius, int planktonType, float rotation ){
    
    entityDrawCount++;
    
    vec2 pos = toLocal( global, depth );
    bool isOnScreen = onScreen( pos, 10 );
    
    if( isOnScreen )
    {
        color(1,1,1);
        gl::pushModelView();
        gl::translate( pos );
        gl::rotate( rotation );
        gl::draw( planktonImgs[planktonType], Rectf(-radius,-radius,radius,radius) );
        gl::popModelView();
    }
    
    addToMiniMap( global, Color(0.5,0.8,1), 4, isOnScreen);
}


void CellRenderer::drawEgg( const vec2 & global, float ratio, float counter, const vector<vec2> & positions )
{
    
    vec2 local = toLocal(global,1);
    
    
    bool isOnScreen = onScreen( local, ratio );
    
    if( isOnScreen )
    {
        
        entityDrawCount++;
        
        
        gl::color(ColorA(1,1,1,0.5 + (sin(counter)*0.20f)) );
        gl::draw( eggImg, Rectf(local.x - ratio, local.y - ratio, local.x + ratio, local.y + ratio) );
        
        
        vector<vec2> drawPositions;
        for(int i = 0; i < positions.size(); i++)
        {
            int after = i + 1;
            if( after == (int)positions.size() ) after = 0;
            drawPositions.push_back( positions[i] );
            drawPositions.push_back( (positions[i] + positions[after]) * 0.5f);
        }
        
        //Average twice
        for(int n = 0; n < 2; n++)
        {
            for(int i = 0; i < drawPositions.size(); i++)
            {
                int before = i - 1;
                if( before < 0 ) before = (int)drawPositions.size()-1;
                int after = i + 1;
                if( after == drawPositions.size() ) after = 0;
                vec2 p = (drawPositions.at(before) + drawPositions.at(after)) * 0.5f;
                drawPositions.at(i) = (drawPositions.at(i) + p) * 0.5f;
            }
        }
        
        
        
        
        Shape2d mShape;
        mShape.moveTo( toLocal(drawPositions.at(0)) );
        for(int i = 1; i < drawPositions.size(); i++)
            mShape.lineTo( toLocal(drawPositions.at(i)) );
        mShape.close();
        
        gl::color( ColorA (0.7,0.25,0.15,0.5) );
        gl::drawSolid( mShape );
        gl::draw( mShape );
        
    }
    
    
    addToMiniMap( global, ColorA (0.7,0.25,0.15,0.5), ratio * 0.5f, isOnScreen);
}

//
void CellRenderer::drawStarfish( const vec2 & global, const vector<FeelerStruct> & feelers, float contacts )
{
    
    vec2 pos = toLocal( global, 1 );
    bool isOnScreen = onScreen( pos, 100 );
    if( isOnScreen )
    {
        
        entityDrawCount++;
        
        for(int i = 0; i < feelers.size(); i++){
            Shape2d mShape = drawFeeler( feelers[i] );
            gl::color(ColorA8u(220,120 - contacts * 0.08f,6 + contacts * 0.05f,70));
            gl::drawSolid( mShape );
            gl::color(ColorA8u(220,120 - contacts * 0.08f,6 + contacts * 0.05f,140));
            gl::draw( mShape );
        }
        
    }
    
    addToMiniMap( global, Color(1.0,0.9,0.1), 50, isOnScreen );
    
}


void CellRenderer::drawJellyfish( const vec2 & global, float radius, const vector<FeelerStruct> & feelers, int jellyType, float counter, float baseWidth )
{
    
    vec2 pos = toLocal( global, 1 );
    bool isOnScreen = onScreen( pos, 150 );

    if( isOnScreen )
    {
        
        entityDrawCount++;
        
        gl::ScopedBlendAdditive additive;
        
        switch( jellyType )
        {
            case 0:
                gl::color(ColorA8u(150 + cos(counter) * 30,200 + sin(counter) * 30,200,155));
                break;
            case 1:
                gl::color(ColorA8u(150,200 + sin(counter) * 50,130,155));
                break;
            case 2:
                gl::color(ColorA8u(150,255,200,155));
                break;
        }
        
        
        //draw the feelers
        for(int i = 0; i < feelers.size(); i++){
            Shape2d mShape = drawFeeler( feelers[i] );
            gl::drawSolid( mShape );
        }
        
        //Draw texture body
        gl::ScopedBlendAlpha alpha;
        gl::color(Color(1,1,1));
        float width  = radius*2 + sin(counter)*2;
        float height = radius*2 + cos(counter)*3;
        gl::draw( jellyImgs[ jellyType ], Rectf(pos.x - width, pos.y - height, pos.x + width, pos.y + height )) ;
        
    }
    
    addToMiniMap( global, Color(0.6,0.3,1.0), radius, isOnScreen);
    
}

void CellRenderer::drawUrchin( const vec2 & global, float radius, const vector<FeelerStruct> & feelers )
{

    vec2 local = toLocal(global,1);
    
    bool isOnScreen = onScreen( local, radius + 50 );
    
    if(isOnScreen){
        
        entityDrawCount++;
        
        gl::color(ColorA(1,0.5,1,0.5));
        
        //draw the feelers
        for(int i = 0; i < feelers.size(); i++){
            Shape2d mShape = drawFeeler( feelers[i] );
            gl::drawSolid( mShape );
        }
        
        
        gl::color(Color(1,1,1));
        gl::draw( urchinImg, Rectf(local.x - radius*2.5, local.y - radius*2.5, local.x + radius*2.5, local.y + radius*2.5) );
        
    }
    
    addToMiniMap( global, Color(1,0.2,1.0), radius, isOnScreen);
}


Shape2d CellRenderer::drawFeeler( const FeelerStruct & feeler )
{
    vector<vec2> drawPositions;
    drawPositions.clear();
    
    vector<vec2> positions = feeler.mPoints;
    
    for(int i = 0; i < positions.size() - 1; i++)
    {
        drawPositions.push_back( toLocal(positions[i], 1) );
        drawPositions.push_back( toLocal((positions[i] + positions[i + 1]) * 0.5f, 1) );
    }
    
    //Average twice
    for(int n = 0; n < 2; n++)
    {
        for(int i = 1; i < drawPositions.size() - 1; i++)
        {
            vec2 p = (drawPositions.at(i - 1) + drawPositions.at(i + 1)) * 0.5f;
            drawPositions.at(i) = (drawPositions.at(i) + p) * 0.5f;
        }
    }
    
    
    vec2 original1 = drawPositions.at(0);
    vec2 next1 = drawPositions.at(1);
    vec2 toNext1 = glm::normalize(next1 - original1);
    vec2 mod1 = vec2( -toNext1.y, toNext1.x ) * feeler.mBaseWidth * 0.5f;
    vec2 right1= original1 - mod1;
    
    
    float currentWidth = feeler.mTipWidth;
    for(int n = (int)drawPositions.size() - 1; n >= 1; n--)
    {
        vec2 original = drawPositions.at(n);
        vec2 next = drawPositions.at(n-1);
        vec2 toNext = glm::normalize(next - original);
        vec2 mod = vec2( -toNext.y, toNext.x ) * currentWidth * 0.5f;
        vec2 right = original + mod;
        
        currentWidth += ( (feeler.mBaseWidth - feeler.mTipWidth) / drawPositions.size());
        
        drawPositions.at(n) -= mod;
        drawPositions.push_back( right );
        
    }
    
    
    drawPositions.push_back( right1 );
    drawPositions.at(0) += mod1;
    
    
    Shape2d mShape;
    mShape.moveTo( drawPositions.at(0) );
    for(int n = 0; n < drawPositions.size(); n++)
        mShape.lineTo( drawPositions.at(n) );
    mShape.close();
    
    
    return mShape;
    
}

