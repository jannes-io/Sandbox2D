#pragma once

#define S2D (Sandbox2D::GetSingleton())

struct BitmapWidget : Widget
{
	Bitmap* _bitmap;

	// Constructor
	BitmapWidget(Bitmap* bitmap, Pixel position)
	{
		_bitmap = bitmap;
		_position = position;
	}
	~BitmapWidget() {}

	void tick(double deltaTime) override {}

	void draw() override
	{
		S2D->_graphics->drawBitmap(_bitmap, absolutePosition());
	}
};