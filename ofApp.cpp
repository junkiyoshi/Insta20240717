#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofNoFill();
	ofEnableDepthTest();

	auto ico_sphere = ofIcoSpherePrimitive(300, 4);
	this->triangle_list.insert(this->triangle_list.end(), ico_sphere.getMesh().getUniqueFaces().begin(), ico_sphere.getMesh().getUniqueFaces().end());

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->mesh.clear();
	this->frame.clear();
	this->random_walk.clear();

	glm::vec3 noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));

	for (int i = 0; i < 500; i++) {

		auto location = glm::vec3(
			ofMap(ofNoise(noise_seed.x, ofGetFrameNum() * 0.06 + i * 0.01), 0, 1, -320, 320),
			ofMap(ofNoise(noise_seed.y, ofGetFrameNum() * 0.06 + i * 0.01), 0, 1, -320, 320),
			ofMap(ofNoise(noise_seed.z, ofGetFrameNum() * 0.06 + i * 0.01), 0, 1, -320, 320));
		this->random_walk.push_back(location);
	}

	ofColor face_color, frame_color;
	for (auto radius = 200; radius <= 250; radius += 2) {

		for (int k = 0; k < this->triangle_list.size(); k++) {

			glm::vec3 avg = (this->triangle_list[k].getVertex(0) + this->triangle_list[k].getVertex(1) + this->triangle_list[k].getVertex(2)) / 3;
			avg = glm::normalize(avg) * radius;

			bool flag = true;
			for (auto& location : this->random_walk) {

				if (glm::distance(avg, location) < 80) {

					flag = false;
					break;
				}
			}

			if (flag == false) { continue; }

			vector<glm::vec3> vertices;
			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(0)) * radius);
			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(1)) * radius);
			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(2)) * radius);

			this->mesh.addVertices(vertices);
			this->frame.addVertices(vertices);

			face_color.setHsb(ofMap(radius, 200, 250, 0, 255), 100, 255);
			frame_color.setHsb(ofMap(radius, 200, 250, 0, 255), 255, 255);
			for (int k = 0; k < vertices.size(); k++) {

				this->mesh.addColor(face_color);
				this->frame.addColor(frame_color);
			}

			this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 2, this->mesh.getNumVertices() - 3);

			this->frame.addIndex(this->frame.getNumVertices() - 1); this->frame.addIndex(this->frame.getNumVertices() - 2);
			this->frame.addIndex(this->frame.getNumVertices() - 2); this->frame.addIndex(this->frame.getNumVertices() - 3);
			this->frame.addIndex(this->frame.getNumVertices() - 3); this->frame.addIndex(this->frame.getNumVertices() - 1);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 1.44);

	ofSetLineWidth(3);
	this->mesh.draw();
	this->frame.drawWireframe();

	ofSetColor(239);
	ofSetLineWidth(1);
	ofBeginShape();
	ofVertices(this->random_walk);
	ofEndShape();

	this->cam.end();

	/*
	int start = 145;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}

