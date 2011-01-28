#include "DGlobals.hpp"
#include "DGame.hpp"

namespace Distillate
{

float  DGlobals::elapsed;
const  std::string DGlobals::LIBRARY_NAME = "Distillate";
DPoint *DGlobals::scroll = new DPoint();
unsigned int DGlobals::width  = 0;
unsigned int DGlobals::height = 0;
bool DGlobals::_pause = false;


DGlobals::DGlobals()
{
    //ctor
}

DGlobals::~DGlobals()
{
    //dtor
}

bool DGlobals::pause()
{
    return _pause;
}

void DGlobals::pause(bool Pause)
{
    bool op = _pause;
    _pause = Pause;
    if(_pause != op)
    {
        if(_pause)
        {
            //_game->pauseGame();
            //pauseSounds();
        }
        else
        {
            //_game->unpauseGame();
            //playSounds();
        }
    }
}
/*



        static public function get framerate():uint
        {
            return _game._framerate;
        }

        static public function set frameratePaused(Framerate:uint):void
        {
            _game._frameratePaused = Framerate;
            if(_game._paused && (_game.stage != null))
                _game.stage.frameRate = Framerate;
        }

        static public function resetInput():void
        {
            keys.reset();
            mouse.reset();
        }

        static public function playMusic(Music:Class,Volume:Number=1.0):void
        {
            if(music == null)
                music = new FlxSound();
            else if(music.active)
                music.stop();
            music.loadEmbedded(Music,true);
            music.volume = Volume;
            music.survive = true;
            music.play();
        }

        static public function play(EmbeddedSound:Class,Volume:Number=1.0,Looped:Boolean=false):FlxSound
        {
            var sl:uint = sounds.length;
            for(var i:uint = 0; i < sl; i++)
                if(!(sounds[i] as FlxSound).active)
                    break;
            if(sounds[i] == null)
                sounds[i] = new FlxSound();
            var s:FlxSound = sounds[i];
            s.loadEmbedded(EmbeddedSound,Looped);
            s.volume = Volume;
            s.play();
            return s;
        }

        static public function stream(URL:String,Volume:Number=1.0,Looped:Boolean=false):FlxSound
        {
            var sl:uint = sounds.length;
            for(var i:uint = 0; i < sl; i++)
                if(!(sounds[i] as FlxSound).active)
                    break;
            if(sounds[i] == null)
                sounds[i] = new FlxSound();
            var s:FlxSound = sounds[i];
            s.loadStream(URL,Looped);
            s.volume = Volume;
            s.play();
            return s;
        }

        static public function get mute():Boolean
        {
            return _mute;
        }

        static public function set mute(Mute:Boolean):void
        {
            _mute = Mute;
            changeSounds();
        }

        static public function getMuteValue():uint
        {
            if(_mute)
                return 0;
            else
                return 1;
        }
       
        static public function get volume():Number { return _volume; }

        static public function set volume(Volume:Number):void
        {
            _volume = Volume;
            if(_volume < 0)
                _volume = 0;
            else if(_volume > 1)
                _volume = 1;
            changeSounds();
        }

        static internal function destroySounds(ForceDestroy:Boolean=false):void
        {
            if(sounds == null)
                return;
            if((music != null) && (ForceDestroy || !music.survive))
                music.destroy();
            var s:FlxSound;
            var sl:uint = sounds.length;
            for(var i:uint = 0; i < sl; i++)
            {
                s = sounds[i] as FlxSound;
                if((s != null) && (ForceDestroy || !s.survive))
                    s.destroy();
            }
        }

        static protected function changeSounds():void
        {
            if((music != null) && music.active)
                music.updateTransform();
            var s:FlxSound;
            var sl:uint = sounds.length;
            for(var i:uint = 0; i < sl; i++)
            {
                s = sounds[i] as FlxSound;
                if((s != null) && s.active)
                    s.updateTransform();
            }
        }

        static internal function updateSounds():void
        {
            if((music != null) && music.active)
                music.update();
            var s:FlxSound;
            var sl:uint = sounds.length;
            for(var i:uint = 0; i < sl; i++)
            {
                s = sounds[i] as FlxSound;
                if((s != null) && s.active)
                    s.update();
            }
        }
        
        static protected function pauseSounds():void
        {
            if((music != null) && music.active)
                music.pause();
            var s:FlxSound;
            var sl:uint = sounds.length;
            for(var i:uint = 0; i < sl; i++)
            {
                s = sounds[i] as FlxSound;
                if((s != null) && s.active)
                    s.pause();
            }
        }
        
        static protected function playSounds():void
        {
            if((music != null) && music.active)
                music.play();
            var s:FlxSound;
            var sl:uint = sounds.length;
            for(var i:uint = 0; i < sl; i++)
            {
                s = sounds[i] as FlxSound;
                if((s != null) && s.active)
                    s.play();
            }
        }

        static public function checkBitmapCache(Key:String):Boolean
        {
            return (_cache[Key] != undefined) && (_cache[Key] != null);
        }
        
        static public function createBitmap(Width:uint, Height:uint, Color:uint, Unique:Boolean=false, Key:String=null):BitmapData
        {
            var key:String = Key;
            if(key == null)
            {
                key = Width+"x"+Height+":"+Color;
                if(Unique && (_cache[key] != undefined) && (_cache[key] != null))
                {
                    //Generate a unique key
                    var inc:uint = 0;
                    var ukey:String;
                    do { ukey = key + inc++;
                    } while((_cache[ukey] != undefined) && (_cache[ukey] != null));
                    key = ukey;
                }
            }
            if(!checkBitmapCache(key))
                _cache[key] = new BitmapData(Width,Height,true,Color);
            return _cache[key];
        }

        static public function addBitmap(Graphic:Class, Reverse:Boolean=false, Unique:Boolean=false, Key:String=null):BitmapData
        {
            var needReverse:Boolean = false;
            var key:String = Key;
            if(key == null)
            {
                key = String(Graphic);
                if(Unique && (_cache[key] != undefined) && (_cache[key] != null))
                {
                    //Generate a unique key
                    var inc:uint = 0;
                    var ukey:String;
                    do { ukey = key + inc++;
                    } while((_cache[ukey] != undefined) && (_cache[ukey] != null));
                    key = ukey;
                }
            }
            //If there is no data for this key, generate the requested graphic
            if(!checkBitmapCache(key))
            {
                _cache[key] = (new Graphic).bitmapData;
                if(Reverse) needReverse = true;
            }
            var pixels:BitmapData = _cache[key];
            if(!needReverse && Reverse && (pixels.width == (new Graphic).bitmapData.width))
                needReverse = true;
            if(needReverse)
            {
                var newPixels:BitmapData = new BitmapData(pixels.width<<1,pixels.height,true,0x00000000);
                newPixels.draw(pixels);
                var mtx:Matrix = new Matrix();
                mtx.scale(-1,1);
                mtx.translate(newPixels.width,0);
                newPixels.draw(pixels,mtx);
                pixels = newPixels;
            }
            return pixels;
        }


        static public function follow(Target:FlxObject, Lerp:Number=1):void
        {
            followTarget = Target;
            followLerp = Lerp;
            _scrollTarget.x = (width>>1)-followTarget.x-(followTarget.width>>1);
            _scrollTarget.y = (height>>1)-followTarget.y-(followTarget.height>>1);
            scroll.x = _scrollTarget.x;
            scroll.y = _scrollTarget.y;
            doFollow();
        }

        static public function followAdjust(LeadX:Number = 0, LeadY:Number = 0):void
        {
            followLead = new Point(LeadX,LeadY);
        }

        static public function followBounds(MinX:int=0, MinY:int=0, MaxX:int=0, MaxY:int=0, UpdateWorldBounds:Boolean=true):void
        {
            followMin = new Point(-MinX,-MinY);
            followMax = new Point(-MaxX+width,-MaxY+height);
            if(followMax.x > followMin.x)
                followMax.x = followMin.x;
            if(followMax.y > followMin.y)
                followMax.y = followMin.y;
            if(UpdateWorldBounds)
                FlxU.setWorldBounds(MinX,MinY,MaxX-MinX,MaxY-MinY);
            doFollow();
        }

        static public function get stage():Stage
        {
            if((_game._state != null)  && (_game._state.parent != null))
                return _game._state.parent.stage;
            return null;
        }

        static public function get state():FlxState
        {
            return _game._state;
        }

        static public function set state(State:FlxState):void
        {
            _game.switchState(State);
        }


        static internal function setGameData(Game:FlxGame,Width:uint,Height:uint,Zoom:uint):void
        {
            _game = Game;
            _cache = new Object();
            width = Width;
            height = Height;
            _mute = false;
            _volume = 0.5;
            sounds = new Array();
            mouse = new FlxMouse();
            keys = new FlxKeyboard();
            scroll = null;
            _scrollTarget = null;
            unfollow();
            FlxG.levels = new Array();
            FlxG.scores = new Array();
            level = 0;
            score = 0;
            FlxU.seed = NaN;
            kong = null;
            pause = false;
            timeScale = 1.0;
            framerate = 60;
            frameratePaused = 10;
            maxElapsed = 0.0333333;
            FlxG.elapsed = 0;
            _showBounds = false;
            FlxObject._refreshBounds = false;

            panel = new FlxPanel();
            quake = new FlxQuake(Zoom);
            flash = new FlxFlash();
            fade = new FlxFade();

            FlxU.setWorldBounds(0,0,FlxG.width,FlxG.height);
        }


        static internal function doFollow():void
        {
            if(followTarget != null)
            {
                _scrollTarget.x = (width>>1)-followTarget.x-(followTarget.width>>1);
                _scrollTarget.y = (height>>1)-followTarget.y-(followTarget.height>>1);
                if((followLead != null) && (followTarget is FlxSprite))
                {
                    _scrollTarget.x -= (followTarget as FlxSprite).velocity.x*followLead.x;
                    _scrollTarget.y -= (followTarget as FlxSprite).velocity.y*followLead.y;
                }
                scroll.x += (_scrollTarget.x-scroll.x)*followLerp*FlxG.elapsed;
                scroll.y += (_scrollTarget.y-scroll.y)*followLerp*FlxG.elapsed;

                if(followMin != null)
                {
                    if(scroll.x > followMin.x)
                        scroll.x = followMin.x;
                    if(scroll.y > followMin.y)
                        scroll.y = followMin.y;
                }

                if(followMax != null)
                {
                    if(scroll.x < followMax.x)
                        scroll.x = followMax.x;
                    if(scroll.y < followMax.y)
                        scroll.y = followMax.y;
                }
            }
        }


        static internal function unfollow():void
        {
            followTarget = null;
            followLead = null;
            followLerp = 1;
            followMin = null;
            followMax = null;
            if(scroll == null)
                scroll = new Point();
            else
                scroll.x = scroll.y = 0;
            if(_scrollTarget == null)
                _scrollTarget = new Point();
            else
                _scrollTarget.x = _scrollTarget.y = 0;
        }


        static internal function unfollow():void
        {
            followTarget = null;
            followLead = null;
            followLerp = 1;
            followMin = null;
            followMax = null;
            if(scroll == null)
                scroll = new Point();
            else
                scroll.x = scroll.y = 0;
            if(_scrollTarget == null)
                _scrollTarget = new Point();
            else
                _scrollTarget.x = _scrollTarget.y = 0;
        }

        static internal function updateInput():void
        {
            keys.update();
            mouse.update(state.mouseX,state.mouseY,scroll.x,scroll.y);
        }

*/

}
