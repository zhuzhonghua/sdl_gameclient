#include "startscene.h"
#include "pixeldungeon.h"
#include "archs.h"
#include "bannersprites.h"
#include "bitmaskemitter.h"
#include "speck.h"
#include "icon.h"
#include "exitbutton.h"
#include "bpt.h"
#include "titlescene.h"
#include "badge.h"
#include "gamesinprogress.h"
#include "wndoptions.h"
#include "wndclass.h"

const char* StartScene::TXT_LOAD = "lang.loadgame";
const char* StartScene::TXT_NEW = "lang.newgame";

const char* StartScene::TXT_ERASE = "Erase current game";
const char* StartScene::TXT_DPTH_LVL = "Depth: %d, level: %d";

const char* StartScene::TXT_REALLY = "Do you really want to start new game?";
const char* StartScene::TXT_WARNING = "Your current game progress will be erased.";
const char* StartScene::TXT_YES = "Yes, start new game";
const char* StartScene::TXT_NO = "No, return to main menu";

const char* StartScene::TXT_UNLOCK = "To unlock this character class, slay the 3rd boss with any other class";

const char* StartScene::TXT_WIN_THE_GAME ="To unlock \"Challenges\", win the game with any character class.";

namespace{
	class NewWndOptions :public WndOptions{
	public:
		StartScene* _scene;
		NewWndOptions(StartScene* sce, const std::string& title, const std::string& message, const std::vector<std::string>& options)
			:WndOptions(title,message,options)
		{
			this->_scene = sce;
		}
	protected:
		virtual void onSelect(int index) 
		{
			if (index == 0) 
			{
				_scene->startNewGame();
			}
		}
	};
	class NewGameButton :public StartScene::GameButton{
	public:
		NewGameButton(const std::string& txt, StartScene* scene) :StartScene::GameButton(txt)
		{
			_scene = scene;
		}
	protected:
		StartScene* _scene;
		virtual void onClick()
		{
			GamesInProgress::Info info;

			if (GamesInProgress::check(_scene->curClass, info))
			{
				std::vector<std::string> options;
				options.push_back(StartScene::TXT_YES);
				options.push_back(StartScene::TXT_NO);

				_scene->add(new NewWndOptions(_scene, StartScene::TXT_REALLY, StartScene::TXT_WARNING, options));
			}
			else
			{
				_scene->startNewGame();
			}
		}
	};

	class LoadGameButton :public StartScene::GameButton{
	public:
		LoadGameButton(const std::string& txt, StartScene* scene) :StartScene::GameButton(txt)
		{
			_scene = scene;
		}
	protected:
		StartScene* _scene;
		virtual void onClick()
		{
			printf("test2");
		}
	};
}

StartScene::GameButton::GameButton(const std::string& primary) :RedButton(primary)
{
	init();

	_secondary->text("");
}

void StartScene::GameButton::createChildren()
{
	//RedButton::createChildren();
	_secondary = StartScene::createText(6);// new BitmapText();
	add(_secondary);
}

void StartScene::GameButton::layout()
{
	RedButton::layout();

	if (_secondary->text().length() > 0) 
	{
		_text->y = align(_y + (_height - _text->Height() - _secondary->baseLine()) / 2);

		_secondary->x = align(_x + (_width - _secondary->Width()) / 2);
		_secondary->y = align(_text->y + _text->Height());
	}
	else 
	{
		_text->y = align(_y + (_height - _text->baseLine()) / 2);
	}
}

void StartScene::GameButton::secondary(const std::string& text, bool highlighted)
{
	_secondary->text(text);
	_secondary->measure();

	_secondary->hardlight(highlighted ? SECONDARY_COLOR_H : SECONDARY_COLOR_N);
}

const float StartScene::ClassShield::MIN_BRIGHTNESS = 0.6f;
StartScene* StartScene::ClassShield::startScene = NULL;

StartScene::ClassShield::ClassShield(HeroClass clp) :cl(clp)
{
	init();
	
	avatar->frame(cl.ordinal() * WIDTH, 0, WIDTH, HEIGHT);
	GameMath::PointFSet(&avatar->scale, SCALE);
	//avatar->scale.set(SCALE);

	normal = BASIC_NORMAL;
	highlighted = BASIC_HIGHLIGHTED;

	//if (Badges.isUnlocked(cl.masteryBadge())) {
	//	normal = MASTERY_NORMAL;
	//	highlighted = MASTERY_HIGHLIGHTED;
	//}
	//else {
	//	normal = BASIC_NORMAL;
	//	highlighted = BASIC_HIGHLIGHTED;
	//}

	name->text(cl.name());
	name->measure();
	name->hardlight(normal);

	brightness = MIN_BRIGHTNESS;
	updateBrightness();
}

void StartScene::ClassShield::createChildren()
{
	Button::createChildren();

	avatar = new Image(Assets::AVATARS);
	add(avatar);

	name = StartScene::createText(9);// new BitmapText();
	add(name);

	emitter = new BitmaskEmitter(avatar);
	add(emitter);
}

void StartScene::ClassShield::layout()
{
	Button::layout();

	avatar->x = align(_x + (_width - avatar->Width()) / 2);
	avatar->y = align(_y + (_height - avatar->Height() - name->Height()) / 2);

	name->x = align(_x + (_width - name->Width()) / 2);
	name->y = avatar->y + avatar->Height() + SCALE;
}

void StartScene::ClassShield::onTouchDown()
{
	emitter->revive();
	emitter->start(Speck::factory(Speck::LIGHT), 0.05f, 7);

	//Sample.INSTANCE.play(Assets.SND_CLICK, 1, 1, 1.2f);
	startScene->updateClass(cl);
}

void StartScene::ClassShield::update()
{
	Button::update();

	if (brightness < 1.0f && brightness > MIN_BRIGHTNESS) 
	{
		if ((brightness -= Game::elapsed) <= MIN_BRIGHTNESS) 
		{
			brightness = MIN_BRIGHTNESS;
		}
		updateBrightness();
	}
}

void StartScene::ClassShield::highlight(bool value)
{
	if (value) 
	{
		brightness = 1.0f;
		name->hardlight(highlighted);
	}
	else 
	{
		brightness = 0.999f;
		name->hardlight(normal);
	}

	updateBrightness();
}

void StartScene::ClassShield::updateBrightness()
{
	avatar->gm = avatar->bm = avatar->rm = avatar->am = brightness;
}

StartScene::ChallengeButton::ChallengeButton()
{
	init();

	_width = image->widthf;
	_height = image->heightf;

	image->am = 1.0f;// Badges.isUnlocked(Badges.Badge.VICTORY) ? 1.0f : 0.5f;
}

void StartScene::ChallengeButton::createChildren()
{
	//Button::createChildren();

	image = Icons::get(Icons::CHALLENGE_ON);//Icons.get(PixelDungeon.challenges() > 0 ? Icons::CHALLENGE_ON : Icons.CHALLENGE_OFF);
	add(image);
}

void StartScene::ChallengeButton::layout()
{
	Button::layout();

	image->x = align(_x);
	image->y = align(_y);
}

void StartScene::ChallengeButton::onClick()
{
	//if (Badges.isUnlocked(Badges.Badge.VICTORY)) {
	//	StartScene.this.add(new WndChallenges(PixelDungeon.challenges(), true){
	//		public void onBackPressed() {
	//			super.onBackPressed();
	//			image.copy(Icons.get(PixelDungeon.challenges() > 0 ?
	//				Icons.CHALLENGE_ON : Icons.CHALLENGE_OFF));
	//		};
	//	});
	//}
	//else {
	//	StartScene.this.add(new WndMessage(TXT_WIN_THE_GAME));
	//}
}

void StartScene::ChallengeButton::onTouchDown()
{
	//Sample.INSTANCE.play( Assets.SND_CLICK );
}

HeroClass StartScene::curClass("");
std::map<HeroClass, StartScene::ClassShield*> StartScene::shields;

void StartScene::init()
{
	PixelScene::init();

	Badges::loadGlobal();

	uiCamera->visible = false;

	int w = Camera::mainCamera->width;
	int h = Camera::mainCamera->height;

	float width, height;
	if (PixelDungeon::landscape()) 
	{
		width = WIDTH_L;
		height = HEIGHT_L;
	}
	else 
	{
		width = WIDTH_P;
		height = HEIGHT_P;
	}

	float left = (w - width) / 2;
	float top = (h - height) / 2;
	float bottom = h - top;

	Archs* archs = new Archs();
	archs->setSize(w, h);
	add(archs);
	
	Image* title = BannerSprites::get(BannerSprites::Type::SELECT_YOUR_HERO);
	title->x = align((w - title->Width()) / 2);
	title->y = align(top);
	add(title);
	
	buttonX = left;
	buttonY = bottom - BUTTON_HEIGHT;
	
	btnNewGame = new NewGameButton(BPT::getText(TXT_NEW), this);
	add(btnNewGame);
	btnLoad = new LoadGameButton(BPT::getText(TXT_LOAD), this);
	add(btnLoad);
	
	float centralHeight = buttonY - title->y - title->Height();

	ClassShield::startScene = this;

	const HeroClass classes[] = {
		HeroClass(HeroClass::WARRIOR), 
		HeroClass(HeroClass::MAGE), 
		HeroClass(HeroClass::ROGUE), 
		HeroClass(HeroClass::HUNTRESS)
	};

	const int CLASSES_LEN = sizeof(classes) / sizeof(HeroClass);
	//const int CLASSES_LEN = 1;
	for (int i = 0; i < CLASSES_LEN; i++)
	{
		const HeroClass& cl = classes[i];
		ClassShield* shield = new ClassShield(cl);
		shields.insert(std::make_pair(cl, shield));
		add(shield);
	}
	
	if (PixelDungeon::landscape()) 
	{
		float shieldW = width / 4;
		float shieldH = std::min(centralHeight, shieldW);
		top = title->y + title->heightf + (centralHeight - shieldH) / 2;
		for (int i = 0; i < CLASSES_LEN; i++) 
		{
			ClassShield* shield = shields.find(classes[i])->second;
			shield->setRect(left + i * shieldW, top, shieldW, shieldH);
		}
	
		ChallengeButton* challenge = new ChallengeButton();
		challenge->setPos(
			w / 2 - challenge->width() / 2,
			top + shieldH - challenge->height() / 2);
		add(challenge);
	
	}
	else 
	{
		float shieldW = width / 2;
		float shieldH = std::min(centralHeight / 2, shieldW * 1.2f);
		top = title->y + title->Height() + centralHeight / 2 - shieldH;
		for (int i = 0; i < CLASSES_LEN; i++) 
		{
			ClassShield* shield = shields.find(classes[i])->second;
			shield->setRect(
				left + (i % 2) * shieldW,
				top + (i / 2) * shieldH,
				shieldW, shieldH);
		}
	
		ChallengeButton* challenge = new ChallengeButton();
		challenge->setPos(
			w / 2 - challenge->width() / 2,
			top + shieldH - challenge->height() / 2);
		add(challenge);
	}

	unlock = new Group();
	add(unlock);

	//if (!(huntressUnlocked = Badges.isUnlocked(Badges.Badge.BOSS_SLAIN_3))) {
	//
	//	BitmapTextMultiline text = PixelScene.createMultiline(TXT_UNLOCK, 9);
	//	text.maxWidth = (int)width;
	//	text.measure();
	//
	//	float pos = (bottom - BUTTON_HEIGHT) + (BUTTON_HEIGHT - text.height()) / 2;
	//	for (BitmapText line : text.new LineSplitter().split()) {
	//		line.measure();
	//		line.hardlight(0xFFFF00);
	//		line.x = PixelScene.align(w / 2 - line.width() / 2);
	//		line.y = PixelScene.align(pos);
	//		unlock.add(line);
	//
	//		pos += line.height();
	//	}
	//}

	ExitButton* btnExit = new ExitButton();
	btnExit->setPos(Camera::mainCamera->width - btnExit->width(), 0);
	add(btnExit);

	curClass.setNull();

	updateClass(classes[0]);

	fadeIn();

	//Badges.loadingListener = new Callback(){
	//	@Override
	//	public void call() {
	//		if (Game.scene() == StartScene.this) {
	//			PixelDungeon.switchNoFade(StartScene.class);
	//		}
	//	}
	//};
}

StartScene::StartScene()
{

}

StartScene::~StartScene()
{
	destroy();
}

void StartScene::destroy()
{
	//Badges.saveGlobal();
	//Badges.loadingListener = null;

	PixelScene::destroy();

	shields.clear();
}

void StartScene::updateClass(HeroClass cl)
{
	if (curClass == cl) {
		add(new WndClass(cl));
		return;
	}

	if (curClass.isNull() == false) 
	{
		shields.find(curClass)->second->highlight(false);
	}
	curClass = cl;
	shields.find(curClass)->second->highlight(true);

	if (cl.type() != HeroClass::E_HUNTRESS || huntressUnlocked) 
	{
		unlock->visible = false;
	
		//GamesInProgress.Info info = GamesInProgress.check(curClass);
		//if (info != null) {
		//
		//	btnLoad.visible = true;
		//	btnLoad.secondary(Utils.format(TXT_DPTH_LVL, info.depth, info.level), info.challenges);
		//
		//	btnNewGame.visible = true;
		//	btnNewGame.secondary(TXT_ERASE, false);
		//
		//	float w = (Camera.main.width - GAP) / 2 - buttonX;
		//
		//	btnLoad.setRect(
		//		buttonX, buttonY, w, BUTTON_HEIGHT);
		//	btnNewGame.setRect(
		//		btnLoad.right() + GAP, buttonY, w, BUTTON_HEIGHT);
		//
		//}
		//else {
			btnLoad->visible = false;
			
			btnNewGame->visible = true;
			btnNewGame->secondary("", false);
			btnNewGame->setRect(buttonX, buttonY, Camera::mainCamera->width - buttonX * 2, BUTTON_HEIGHT);
		//}
	
	}
	else 
	{	
		unlock->visible = true;
		btnLoad->visible = false;
		btnNewGame->visible = false;	
	}
}

void StartScene::startNewGame()
{
	//Dungeon.hero = null;
	//InterlevelScene.mode = InterlevelScene.Mode.DESCEND;
	//
	//if (PixelDungeon.intro()) {
	//	PixelDungeon.intro(false);
	//	Game.switchScene(IntroScene.class);
	//}
	//else {
	//	Game.switchScene(InterlevelScene.class);
	//}
}

void StartScene::onBackPressed()
{
	PixelDungeon::switchNoFade(new TitleScene());
}