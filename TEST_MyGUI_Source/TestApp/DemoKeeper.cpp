/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "DemoKeeper.h"
#include <fstream>

DemoKeeper::DemoKeeper() :
	mGUI(0),
	mWidth(1), mHeight(1)
{
}

MyGUI::WidgetPtr but = 0;

void DemoKeeper::move(int _left, int _top)
{
	if (but != 0) but->setPosition(_left+2, _top+2);
}

void DemoKeeper::test(MyGUI::WidgetPtr _sender, bool _left)
{
	char * names [3] = {"core.png", "foto.png", "1.jpg"};
	static size_t num = 0;
	num++;
	if (num > 2) num = 0;

	_sender->_setTextureName(names[num]);
	
}

void DemoKeeper::start(MyGUI::Gui * _gui, size_t _width, size_t _height)
{
    mGUI = _gui;
    mWidth = _width;
    mHeight = _height;

	for (size_t pos=0; pos<1; pos++) {
		MyGUI::WindowPtr window = mGUI->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(10+pos*15, 10+pos*15, 300, 300), MyGUI::ALIGN_DEFAULT, "Overlapped");
		window->setCaption("WindowCSX");
		window->setAutoAlpha(true);

		/*MyGUI::ButtonPtr button = window->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(10, 10, 250, 250), MyGUI::ALIGN_DEFAULT);
		button->setCaption("Button 1");
		button->setTextAlign(MyGUI::ALIGN_DEFAULT);*/

		
		MyGUI::ListPtr list = window->createWidget<MyGUI::List>("List", MyGUI::IntCoord(MyGUI::IntPoint(), window->getClientRect().size()), MyGUI::ALIGN_STRETCH);
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		list->addItem("line 1");
		//wid->eventMouseButtonPressed = MyGUI::newDelegate(this, &DemoKeeper::test);

		/*wid = wid->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(10, 10, 200, 200), MyGUI::ALIGN_DEFAULT);
		wid->eventMouseButtonPressed = MyGUI::newDelegate(this, &DemoKeeper::test);

		wid = wid->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(10, 10, 160, 160), MyGUI::ALIGN_DEFAULT);
		wid->eventMouseButtonPressed = MyGUI::newDelegate(this, &DemoKeeper::test);

		wid = wid->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(10, 10, 120, 120), MyGUI::ALIGN_DEFAULT);
		wid->eventMouseButtonPressed = MyGUI::newDelegate(this, &DemoKeeper::test);

		wid = wid->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(10, 10, 50, 50), MyGUI::ALIGN_DEFAULT);
		wid->eventMouseButtonPressed = MyGUI::newDelegate(this, &DemoKeeper::test);*/
	}

	 //but = mGUI->createWidget<MyGUI::Widget>("Button", MyGUI::IntCoord(0, 0, 10, 10), MyGUI::ALIGN_DEFAULT, "Pointer");

	 /*MyGUI::WidgetPtr wid = mGUI->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(300, 100, 300, 300), MyGUI::ALIGN_DEFAULT, "Overlapped");
     but = wid->createWidget<MyGUI::Widget>("ButtonSmall", MyGUI::IntCoord(10, 10, 100, 100), MyGUI::ALIGN_LEFT | MyGUI::ALIGN_BOTTOM );
	 but->setCaption("trkluukbhgjkgkjgkjgkjgkjhgkj\njktuvucuyrcuyrcyruyrvyryrcuyrcuyrucyr\nuyrcuyucycu765i7to8r7rc7ri7ri7riu6\nuy76r4cu76cu76c76cu7c876rc876tr");

	 MyGUI::WidgetPtr but2 = but->createWidget<MyGUI::Widget>("ButtonSmall", MyGUI::IntCoord(10, 10, 10, 10), MyGUI::ALIGN_LEFT | MyGUI::ALIGN_BOTTOM );
     but2->createWidget<MyGUI::Widget>("ButtonSmall", MyGUI::IntCoord(1, 1, 3, 3), MyGUI::ALIGN_LEFT | MyGUI::ALIGN_BOTTOM );

	 mGUI->createWidget<MyGUI::Window>("DefaultClient", MyGUI::IntCoord(500, 200, 300, 300), MyGUI::ALIGN_DEFAULT, "Overlapped");//*/
	 //but->setAlpha(0.7);
	 //but->setColour(Ogre::ColourValue::White);
     //but->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(10, 10, 60, 26), MyGUI::ALIGN_LEFT | MyGUI::ALIGN_BOTTOM );

}

void DemoKeeper::end()
{
}
