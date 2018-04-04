#pragma once

#include "pixelscene.h"

class SurfaceScene :public PixelScene{
private:
	class GrassPatch :public Image{
	public:
		static final int WIDTH = 16;
		static final int HEIGHT = 14;
	private:
		float tx;
		float ty;

		double a;
		double angle;

		boolean forward;
	public:
		GrassPatch(float tx, float ty, boolean forward);
		virtual void update();
	protected:
		virtual void updateMatrix();
	};

	class Avatar :public Image{
	private:
		static final int WIDTH = 24;
		static final int HEIGHT = 28;

	public:
		Avatar(HeroClass cl);
	};

	class Pet :public MovieClip,public MovieClip::Listener{
	private:
		Animation* idle;
		Animation* jump;

	public:
		Pet();
		void Jump() {
			play(jump);
		}
		virtual void onComplete(Animation* anim) {
			if (anim == jump) {
				play(idle);
			}
		}
	};
private:
	static final int FRAME_WIDTH = 88;
	static final int FRAME_HEIGHT = 125;

	static final int FRAME_MARGIN_TOP = 9;
	static final int FRAME_MARGIN_X = 4;

	static final int BUTTON_HEIGHT = 20;

	static final int SKY_WIDTH = 80;
	static final int SKY_HEIGHT = 112;

	static final int NSTARS = 100;
	static final int NCLOUDS = 5;

	Camera* viewport;

private:
	class Sky :public Visual{
	private:
		static final int _day[];
		static final int _night[];
		static std::vector<int> day;
		static std::vector<int> night;

		SmartTexture* texture;
		FloatBuffer* verticesBuffer;

	public:
		Sky(boolean dayTime);
		virtual void draw();
	};

	class Cloud :public Image{
	private:
		static int lastIndex;

	public:
		Cloud(float y, boolean dayTime);
		virtual void update();
	};
public:
	virtual void init();
};