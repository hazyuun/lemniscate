/*
	Lemniscate - Usage example
*/

#include "src/lemniscate.h"

/* Some math functions */
float exp_minus(float x) {
	return glm::exp(-x);
}
float square(float x) {
	return x * x;
}
float damp(float x) {
	return exp_minus(x) * glm::sin(10*x);
}
float r(float theta) {
	return 6 + (float) sin(2 * 3.1415 * theta);
}

using namespace grapher;

int main() {
	/* The first window */
	GrapherWIN damp_win = GrapherWIN(
									500, 250,				/* Window dimensions*/
									"Look at this Graph !"	/* Window title */
									);

	/* Curves */
	Grapher damp_sin_curve = Grapher(
								&damp_win,		/* Parent window		*/
								damp,			/* The function			*/
								-20, 20,		/* The interval to draw */
								100, 250/2,		/* Origin coordinates	*/
								500,			/* Smoothness			*/
								50, 50,			/* Scale for x and y	*/	
								true			/* Show axis ?			*/	
							);
			
	damp_sin_curve.setColor(glm::vec4(0.0, 1.0, 0.0, 1.0));
	
	Grapher exp_min_curve = Grapher(
								&damp_win,
								exp_minus,
								-20, 20,
								100, 250/2,
								500,
								50, 50,
								true
							);

	exp_min_curve.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
	
	/* The second window */
	GrapherWIN polar_win = GrapherWIN(500, 500, "Polar flower");

	/* Preparing the polar curve's points */
	std::vector<glm::vec2> pts;
	float theta = 0;
	while (theta < 20 * 3.1415) {
		theta += 0.01f;
		pts.push_back(glm::vec2(cos(theta) * (r(theta)), sin(theta) * (r(theta))));
	}

	Grapher flower_curve = Grapher(
								&polar_win, /* Parent window		*/
								pts,		/* Points				*/
								250, 250,	/* Origin coordinates	*/
								25, 25,		/* Scale for x and y	*/	
								true		/* Show axis ?			*/
							);

	flower_curve.setColor(glm::vec4(1.0, 0.0, 1.0, 1.0));

	while (damp_win.isOpen() && polar_win.isOpen()) {
		/* Show the first window and its curves */
		damp_win.activate();
		glClear(GL_COLOR_BUFFER_BIT);
		damp_sin_curve.show();
		exp_min_curve.show();
		damp_win.update();
		
		/* Same for the second one */
		polar_win.activate();
		glClear(GL_COLOR_BUFFER_BIT);
		flower_curve.show();
		polar_win.update();
	
	}
	glfwTerminate();
	return 0;
}