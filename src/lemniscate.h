/* 
	Lemniscate - A little header only graphing library
	Author : computer-spectre

*/


#pragma once


#include <GL/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace grapher{
	/* A window, it can contain many graphs*/
	class GrapherWIN {
	private:
		GLFWwindow* mWIN;
		float mWIDTH, mHEIGHT;
	public:
		/* The constructor creates the window */
		inline GrapherWIN(float width, float height, std::string title) 
			:mWIDTH(width), mHEIGHT(height)
		{
			glfwInit();
			mWIN = glfwCreateWindow(mWIDTH, mHEIGHT, title.c_str(), 0, 0);
			glfwMakeContextCurrent(mWIN);
			glewInit();
			glClearColor(0.1f, 0.0f, 0.2f, 1.0f);
		}

		/* Getters for the dimensions */
		inline float getWidth() { return mWIDTH; }
		inline float getHeight() { return mHEIGHT; }

		/* Activates the OpenGL context of this window to render on */
		inline void activate() {
			glfwMakeContextCurrent(mWIN);
		}

		/* Converts to normalized device coordinates */
		inline glm::vec2 coords(float x, float y) {
			float newX = (x / mWIDTH * 2) - 1;
			float newY = -(y / mHEIGHT * 2) + 1;
			return glm::vec2(newX, newY);
		}

		/* Self explanatory ! */
		inline bool isOpen() {
			return !glfwWindowShouldClose(mWIN);
		};

		inline void update() {
			this->activate();
			glfwSwapBuffers(mWIN);
			glfwPollEvents();
		}

		inline ~GrapherWIN(){
			glfwDestroyWindow(mWIN);
		}
	};

	/* A single curve */
	class Grapher {
	private:
		bool mShowAxis;
		std::vector<glm::vec2> mPoints;
		glm::vec2 mPosition; // Origin's position 
		glm::vec4 mColor = glm::vec4(1.0, 0.0, 1.0, 1.0);
		float mXScale, mYScale;

		GrapherWIN* mParent;
	public:

		#define c(a,b) mParent->coords(a,b).x, mParent->coords(a,b).y

		/*
		The constructor :

		Grapher(
				Parent window,	
				Points,			
				Origin coordinates,	
				Scale for x and y,	
				Show axis ?			
			);
		*/
		Grapher(GrapherWIN* win, float function(float), float start, float end, float x, float y, int smoothness, float xScale, float yScale, bool showAxis)
			:mPosition(glm::vec2(x, y)), mXScale(xScale), mYScale(yScale), mShowAxis(showAxis)
		{
			this->mParent = win;
			this->mParent->activate();
			mPoints.push_back(glm::vec2(start, function(start)));
			float step = (end - start) / smoothness;
			for (int i = 1; i <= smoothness; i++) {
				mPoints.push_back(glm::vec2(start + i * step, function(start + i * step)));
				mPoints.push_back(glm::vec2(start + i * step, function(start + i * step)));
			}
			mPoints.push_back(glm::vec2(end, function(end)));
		}

		/* This one is with manual points instead of a function, it can be used for polar curves for example */
		Grapher(GrapherWIN* win, std::vector<glm::vec2> points, float x, float y, float xScale, float yScale, bool showAxis)
			:mPosition(glm::vec2(x, y)), mXScale(xScale), mYScale(yScale), mShowAxis(showAxis), mPoints(points)
		{
			this->mParent = win;
			this->mParent->activate();
		}

		/* Self explanatory !*/
		void setColor(glm::vec4 color) {
			mColor = color;
		}
		/* Renders the curve in the parent window */
		void show() {
			mParent->activate();
			glLineWidth(1);
			if (mShowAxis) {
				glBegin(GL_LINES);
				glColor3f(1.0, 1.0, 1.0);
				glVertex2f(c(0, mPosition.y)); glColor3f(1.0, 1.0, 1.0);
				glVertex2f(c(mParent->getWidth(), mPosition.y)); glColor3f(1.0, 1.0, 1.0);

				glVertex2f(c(mPosition.x, 0)); glColor3f(1.0, 1.0, 1.0);
				glVertex2f(c(mPosition.x, mParent->getHeight())); glColor3f(1.0, 1.0, 1.0);
				for (int i = -10; i <= 20; i++) {
					glVertex2f(c(mPosition.x + 4, mPosition.y + i * mYScale)); glColor3f(1.0, 1.0, 1.0);
					glVertex2f(c(mPosition.x - 4, mPosition.y + i * mYScale)); glColor3f(1.0, 1.0, 1.0);
					glVertex2f(c(mPosition.x + i * mXScale, mPosition.y + 4)); glColor3f(1.0, 1.0, 1.0);
					glVertex2f(c(mPosition.x + i * mXScale, mPosition.y - 4)); glColor3f(1.0, 1.0, 1.0);
				}
				glEnd();
			}
			glLineWidth(2);
			glBegin(GL_LINES);
			glColor4f(mColor.r, mColor.g, mColor.b, mColor.a);

			glVertex2f(c(mPosition.x + mPoints[0].x * mXScale, mPosition.y - mPoints[0].y * mYScale));
			for (unsigned int i = 0; i < mPoints.size(); i++) {
				glVertex2f(c(mPosition.x + mPoints[i].x * mXScale, mPosition.y - mPoints[i].y * mYScale));
				glVertex2f(c(mPosition.x + mPoints[i].x * mXScale, mPosition.y - mPoints[i].y * mYScale));
			}
			glEnd();
		}
		
	};

}
	

