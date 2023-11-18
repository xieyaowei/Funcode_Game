/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <Stdio.h>
#include "CommonAPI.h"
#include "LessonX.h"
#include <math.h>
#include <mmsystem.h>
#include <windows.h>
#pragma comment(lib,"Winmm.lib")
////////////////////////////////////////////////////////////////////////////////
//
//玩家速度
float g_fSpeedwanjiaLeft	=	0.f;
float g_fSpeedwanjiaRight	=	0.f;
float g_fSpeedwanjiaTop		=	0.f;
float g_fSpeedwanjiaBottom	=	0.f;
//地图速度
float g_fSpeedMapLeft	=	0.f;
float g_fSpeedMapRight	=	0.f;
float g_fSpeedMapTop	=	0.f;
float g_fSpeedMapBottom	=	0.f;
//天空速度
float g_fSpeedskyLeft	=	0.f;
float g_fSpeedskyRight	=	0.f;
float g_fSpeedskyTop	=	0.f;
float g_fSpeedskyBottom	=	0.f;
//屏幕边界值
float g_fScreenLeft	    =	0.f;
float g_fScreenRight	=	0.f;
float g_fScreenTop	    =	0.f;
float g_fScreenBottom	=	0.f;
int	g_iGameState	=	0;               //游戏状态
int g_iguan1Count = 10;                  //怪物1的数量
int g_Scores = 0;                        //得分
int HP = 3;                              //血量
char *szNameguan2 = NULL;                //指向怪物2
int j = 0;                               //大蒜数量







// 游戏状态，0 -- 游戏结束等待开始状态；1 -- 按下空格键开始，初始化游戏；2 -- 游戏进行中
//
void		GameInit();
void		GameRun( float fDeltaTime );
void		GameEnd();



void OngongjiColguan1(const char* szgongjiName,const char* szguan1Name);//攻击与怪物1碰撞
void Createguan1();//创造怪物1
void CreateMoreguan1(const char* szTarName);//怪物1死后创造新怪物1
void Createguan2();//创造怪物2
//==============================================================================
//
// 大体的程序流程为：GameMainLoop函数为主循环函数，在引擎每帧刷新屏幕图像之后，都会被调用一次。


//==============================================================================
//
// 游戏主循环，此函数将被不停的调用，引擎每刷新一次屏幕，此函数即被调用一次
// 用以处理游戏的开始、进行中、结束等各种状态.
// 函数参数fDeltaTime : 上次调用本函数到此次调用本函数的时间间隔，单位：秒
void GameMainLoop( float	fDeltaTime )
{
    switch( g_iGameState )
    {
    // 初始化游戏，清空上一局相关数据
    case 1:
    {
        PlaySound("game/data/audio/GloriousMorning.wav",NULL,SND_FILENAME|SND_ASYNC|SND_LOOP);
        GameInit();
        g_iGameState	=	2; // 初始化之后，将游戏状态设置为进行中
    }
    break;

    // 游戏进行中，处理各种游戏逻辑
    case 2:
    {
        // TODO 修改此处游戏循环条件，完成正确游戏逻辑
        if( HP>0&&j<3 )
        {
            GameRun( fDeltaTime );


        }
        else
        {
            // 游戏结束。调用游戏结算函数，并把游戏状态修改为结束状态
            g_iGameState	=	0;
            GameEnd();
        }
    }
    break;
    case 3:
    {
         Createguan2();
         g_iGameState = 2;
    }

    // 游戏结束/等待按空格键开始
    case 0:
    default:
        break;
    };
}

//==============================================================================
//
// 每局开始前进行初始化，清空上一局相关数据
void GameInit()
{
    dSetSpriteVisible("GameBegin",0);
    g_fScreenLeft	 = 	dGetScreenLeft();
    g_fScreenRight  = 	dGetScreenRight();
    g_fScreenTop 	 = 	dGetScreenTop();
    g_fScreenBottom = 	dGetScreenBottom();
    Createguan1();


}
//==============================================================================
//
// 每局游戏进行中
void GameRun( float fDeltaTime )
{

    float fPoswanjiaX,fPoswanjiaY;
    fPoswanjiaX = dGetSpritePositionX( "wanjia" );
    fPoswanjiaY = dGetSpritePositionY( "wanjia" );

    char *szName=NULL;
    for(int i = 0; i<10; i++)
        {
        szName = dMakeSpriteName("guan1",i);
        dSpriteMoveTo( szName, fPoswanjiaX, fPoswanjiaY,4, 1 );
    }

    if(g_Scores%10 == 0&&g_Scores>=20&&g_Scores<50)
    {
        g_iGameState = 3;
        g_Scores++;
    }
    else if(g_Scores%5 == 0&&g_Scores>50)
    {
        g_iGameState = 3;
        g_Scores++;
    }









}
//==============================================================================
//
// 本局游戏结束
void GameEnd()
{   if(j == 3)
    dSetSpriteVisible("youwin",1);
    else
    dSetSpriteVisible("GameOver",1);
    char *szName = NULL;
    for(int i=0;i<10;i++){
        szName = dMakeSpriteName("guan1",i);
        dDeleteSprite(szName);
        dDeleteSprite("wanjia");
    }
    dDeleteSprite("ball");


}

//==========================================================================
//
// 鼠标移动
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseMove( const float fMouseX, const float fMouseY )
{

}
//==========================================================================
//
// 鼠标点击
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{
    float fPoswanjiaX,fPoswanjiaY;
    fPoswanjiaX = dGetSpritePositionX( "wanjia" );
    fPoswanjiaY = dGetSpritePositionY( "wanjia" );
    double y,x;
    dSetSpritePosition("ball",fPoswanjiaX,fPoswanjiaY);
    dSetSpriteVisible("ball",1);

    if( 2 != g_iGameState )
		return;
    if(iMouseType == MOUSE_LEFT){
    float fSpeedX = 0.f;
    float fSpeedY = 0.f;
    x = (double)fMouseX - (double)fPoswanjiaX;
    y = (double)fMouseY - (double)fPoswanjiaY;
    float ftan = atan2( y,x );
    fSpeedX = dRotationToVectorX( ftan/3.14*180 )*30;
    fSpeedY = dRotationToVectorY( ftan/3.14*180 )*30;
    dSetSpriteLinearVelocity("ball",fSpeedX,fSpeedY);
    }

}
//==========================================================================
//
// 鼠标弹起
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{

}
//==========================================================================
//
// 键盘按下
// 参数 iKey：被按下的键，值见 enum KeyCodes 宏定义
// 参数 iAltPress, iShiftPress，iCtrlPress：键盘上的功能键Alt，Ctrl，Shift当前是否也处于按下状态(0未按下，1按下)
void OnKeyDown( const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress )
{
    if( KEY_SPACE == iKey && 0 == g_iGameState )
    {
        g_iGameState	=	1;
        dSetTextValue("score",g_Scores);
        dSetSpriteVisible("ball",0);
    }
    else if( 2 == g_iGameState)
    {
        switch(iKey)
        {
        case KEY_W:
            g_fSpeedwanjiaTop = -10.f;
            dAnimateSpritePlayAnimation( "wanjia", "a02jes5lge_Animation3", 0 );
            break;
        case KEY_A:
            g_fSpeedwanjiaLeft = -15.f;
            dAnimateSpritePlayAnimation( "wanjia", "a02jes5lge_Animation1", 0 );
            break;
        case KEY_S:
            g_fSpeedwanjiaBottom = 10.f;
            dAnimateSpritePlayAnimation( "wanjia", "a02jes5lge_Animation", 0 );
            break;
        case KEY_D:
            g_fSpeedwanjiaRight = 15.f;
            dAnimateSpritePlayAnimation( "wanjia", "a02jes5lge_Animation2", 0 );
            break;
        }
        switch(iKey)
        {
        case KEY_W:
            g_fSpeedMapTop = 10.f;
            g_fSpeedskyTop = 10.f;
            break;
        case KEY_A:
            g_fSpeedMapLeft = 15.f;
            g_fSpeedskyLeft = 15.f;
            break;
        case KEY_S:
            g_fSpeedMapBottom = -10.f;
            g_fSpeedskyBottom = -10.f;
            break;
        case KEY_D:
            g_fSpeedMapRight = -10.f;
            g_fSpeedskyRight = -10.f;
            break;
        }


    }
    dSetSpriteLinearVelocity("wanjia",g_fSpeedwanjiaLeft + g_fSpeedwanjiaRight, g_fSpeedwanjiaTop + g_fSpeedwanjiaBottom);
    dSetSpriteLinearVelocity("Map",g_fSpeedMapLeft + g_fSpeedMapRight, g_fSpeedMapTop + g_fSpeedMapBottom);
    dSetSpriteLinearVelocity("sky",g_fSpeedskyLeft + g_fSpeedskyRight, g_fSpeedskyTop + g_fSpeedskyBottom);

}
//==========================================================================
//
// 键盘弹起
// 参数 iKey：弹起的键，值见 enum KeyCodes 宏定义
void OnKeyUp( const int iKey )
{
    switch(iKey)
    {
    case KEY_W:
        g_fSpeedwanjiaTop = 0.f;
        break;
    case KEY_A:
        g_fSpeedwanjiaLeft = 0.f;
        break;
    case KEY_S:
        g_fSpeedwanjiaBottom = 0.f;
        break;
    case KEY_D:
        g_fSpeedwanjiaRight = 0.f;
        break;

    default:
        break;
    }
    switch(iKey)
    {
    case KEY_W:
        g_fSpeedMapTop = 0.f;
        g_fSpeedskyTop = 0.f;
        break;
    case KEY_A:
        g_fSpeedMapLeft = 0.f;
        g_fSpeedskyLeft = 0.f;
        break;
    case KEY_S:
        g_fSpeedMapBottom = 0.f;
        g_fSpeedskyBottom = 0.f;
        break;
    case KEY_D:
        g_fSpeedMapRight = 0.f;
        g_fSpeedskyRight = 0.f;
        break;
    }
    dSetSpriteLinearVelocity("wanjia",g_fSpeedwanjiaLeft + g_fSpeedwanjiaRight, g_fSpeedwanjiaTop + g_fSpeedwanjiaBottom);
    dSetSpriteLinearVelocity("Map",g_fSpeedMapLeft + g_fSpeedMapRight, g_fSpeedMapTop + g_fSpeedMapBottom);
    dSetSpriteLinearVelocity("sky",g_fSpeedskyLeft + g_fSpeedskyRight, g_fSpeedskyTop + g_fSpeedskyBottom);

}
//===========================================================================
//
// 精灵与精灵碰撞
// 参数 szSrcName：发起碰撞的精灵名字
// 参数 szTarName：被碰撞的精灵名字
void OnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
    if( 2 != g_iGameState )
		return;
    for(int i=0; i<10; i++)
    {
        if(strcmp("wanjia",szSrcName)==0&&strcmp(szTarName,dMakeSpriteName("guan1",i))==0)
        {
            HP--;
            dDeleteSprite(szTarName);
            if(HP == 2)
            dDeleteSprite("xin1");
            if(HP == 1)
            dDeleteSprite("xin2");
            if(HP == 0)
                dDeleteSprite("xin3");
                return;

        }
        if(strcmp("wanjia",szSrcName)==0&&strcmp(szTarName,dMakeSpriteName("guan2",i))==0)
        {
            HP--;
            dDeleteSprite(szTarName);
            if(HP == 2)
            dDeleteSprite("xin1");
            if(HP == 1)
            dDeleteSprite("xin2");
            if(HP == 0)
                dDeleteSprite("xin3");
                return;

        }

        if(strcmp("gongji",szSrcName)==0&&strcmp(szTarName,dMakeSpriteName("guan1",i))==0)
        {
            dDeleteSprite(szTarName);
            g_Scores++;
            dSetTextValue("score",g_Scores);
            CreateMoreguan1(szTarName);


        }
        if(strcmp("gongji",szSrcName)==0&&strcmp(szTarName,dMakeSpriteName("guan2",i))==0)
        {
            dDeleteSprite(szTarName);
            g_Scores++;
            dSetTextValue("score",g_Scores);

        }
        if(strcmp("ball",szSrcName)==0&&strcmp(szTarName,dMakeSpriteName("guan1",i))==0)
        {
            dDeleteSprite(szTarName);
            g_Scores++;
            dSetTextValue("score",g_Scores);
            CreateMoreguan1(szTarName);
            dSetSpriteVisible("ball",0);
        }
        if(strcmp("ball",szSrcName)==0&&strcmp(szTarName,dMakeSpriteName("guan2",i))==0)
        {
            dDeleteSprite(szTarName);
            g_Scores++;
            dSetTextValue("score",g_Scores);
            dSetSpriteVisible("ball",0);
        }
        if(strcmp("zhu1",szSrcName)==0&&strcmp(szTarName,dMakeSpriteName("guan1",i)) == 0)
        {
            dDeleteSprite(szTarName);
            dAnimateSpritePlayAnimation("zhu1","imageMoleRedAnimation",0);
            CreateMoreguan1(szTarName);


        }
        if(strcmp("zhu2",szSrcName)==0&&strcmp(szTarName,dMakeSpriteName("guan1",i)) == 0)
        {
            dDeleteSprite(szTarName);
            dAnimateSpritePlayAnimation("zhu2","imageMoleLilacAnimation",0);
            CreateMoreguan1(szTarName);


        }
         if(strcmp("zhu3",szSrcName)==0&&strcmp(szTarName,dMakeSpriteName("guan1",i)) == 0)
        {
            dDeleteSprite(szTarName);
            dAnimateSpritePlayAnimation("zhu3","imageMoleGreenAnimation",0);
            CreateMoreguan1(szTarName);


        }





    }

    for(int m = 1;m<4;m++){
        if(strcmp("wanjia",szSrcName)==0&&strcmp(szTarName,dMakeSpriteName("garlic",m))==0)
        {
            dDeleteSprite(szTarName);
            j++;
            dSetTextValue("garlicNUM",j);

        }
    }

}
//===========================================================================
//
// 精灵与世界边界碰撞
// 参数 szName：碰撞到边界的精灵名字
// 参数 iColSide：碰撞到的边界 0 左边，1 右边，2 上边，3 下边
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{

}
void OngongjiColguan1(const char* szgongjiName,const char* szguan1Name)
{
    if(strstr(szguan1Name,"guan1")!= NULL)
    {
        dDeleteSprite(szguan1Name);


    }
}
void Createguan1()
{
    char *szName = NULL;
    int	iLoop = 0,iPosX = 0,iPosY = 0;
    for( iLoop = 0; iLoop < g_iguan1Count; iLoop++ )
    {
        szName	=	dMakeSpriteName( "guan1", iLoop );
        dCloneSprite( "guan1", szName );
        iPosX	=	dRandomRange( g_fScreenLeft, g_fScreenRight);//怪物生成
        iPosY	=	dRandomRange( g_fScreenTop, g_fScreenBottom);
        dSetSpritePosition( szName, (float)iPosX, (float)iPosY );
    }


}
void CreateMoreguan1(const char* szTarName)
{

    int	iPosX = 0,iPosY = 0;

    dCloneSprite( "guan1", szTarName);
    iPosX	=	dRandomRange( g_fScreenLeft, g_fScreenRight );//怪物生成
    iPosY	=	dRandomRange( g_fScreenTop, g_fScreenBottom );
    dSetSpritePosition( szTarName, (float)iPosX, (float)iPosY );
    dSetSpritePosition( "appear", (float)iPosX, (float)iPosY);
    dAnimateSpritePlayAnimation("appear","g777Animation",0);

}
void Createguan2(){

    int	iLoop = 0,iPosY = 0;
    for( iLoop = 0; iLoop < 6; iLoop++ )
    {
        szNameguan2	=	dMakeSpriteName( "guan2", iLoop );
        dCloneSprite( "guan2", szNameguan2 );
        iPosY	=	dRandomRange( g_fScreenTop, g_fScreenBottom );
        dSetSpritePosition( szNameguan2, g_fScreenLeft, (float)iPosY );
        dSetSpriteLinearVelocityX(szNameguan2,20);
    }

}

