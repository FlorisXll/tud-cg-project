// Implements Object
#include "geometric_object.h"

void GeometricObject::generate() {
	for (int i = 0; i < 3; i++) {
		Vertex vertex = {};

		// Retrieve coordinates for vertex by index
		if (i == 0) {
			vertex.pos = { -1.0f, -1.0f, 0.0f};
		}
		if (i == 1) {
			vertex.pos = { 1.0f, -1.0f, 0.0f };
		}
		if (i == 2) {
			vertex.pos = { 1.0f, 1.0f, 0.0f };
		}
		

		// Retrieve components of normal by index
		vertex.normal = { 0.0f, 0.0f, 1.0f };

		// Set color (white by default)
		vertex.color = { 1.0f, 0.0f, 0.0f };

		// Set texture coordinates
		vertex.texCoords = { 0.0f, 0.0f };

		vertices.push_back(vertex);
	}
}

void GeometricObject::loadFromFile(const char* filename) {
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename)) {
		std::cerr << err << std::endl;
		return;
	}

	// Read triangle vertices from OBJ file
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex = {};

			// Retrieve coordinates for vertex by index
			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			// Retrieve components of normal by index
			vertex.normal = {
				attrib.normals[3 * index.normal_index + 0],
				attrib.normals[3 * index.normal_index + 1],
				attrib.normals[3 * index.normal_index + 2]
			};

			// Set color (white by default)
			vertex.color = { 1.0f, 1.0f, 1.0f };

			// Set texture coordinates
			vertex.texCoords = { 0.0f, 0.0f };

			vertices.push_back(vertex);
		}
	}
}

void GeometricObject::generateBufferObjects() {
	// generate VBO and VAO
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	
	// bind VBO and VAO
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// specify total size of buffer object
	glBufferData(GL_ARRAY_BUFFER, size() * sizeof(Vertex), data(), GL_STATIC_DRAW);

	// specify position vectors
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));
	glEnableVertexAttribArray(0);

	// specify normal vectors
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(1);

	// specify color vectors
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
	glEnableVertexAttribArray(2);

	// specify texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords)));
	glEnableVertexAttribArray(3);
}

std::vector<Vertex>* GeometricObject::getVertices() {
	return &vertices;
}

int GeometricObject::size() {
	return vertices.size();
}

Vertex* GeometricObject::data() {
	return vertices.data();
}

glm::mat4* GeometricObject::getModelMatrix() {
	return &model;
}
void GeometricObject::setModelMatrix(glm::mat4 matrix) {
	model = matrix;
}
void GeometricObject::clearModelMatrix() {
	model = glm::mat4();
}
void GeometricObject::pushModelMatrix() {
	modelStack.push_back(model);
}
void GeometricObject::popModelMatrix() {
	loadModelMatrix();
	modelStack.pop_back();
}
void GeometricObject::loadModelMatrix() {
	model = modelStack.back();
}
void GeometricObject::applyPosition() {
	translate(position);
}

void GeometricObject::translate(glm::vec3 translation) {
	model = glm::translate(model, translation);
}
void GeometricObject::translateX(float translation) {
	translate(glm::vec3(translation, 0, 0));
}
void GeometricObject::translateY(float translation) {
	translate(glm::vec3(0, translation, 0));
}
void GeometricObject::translateZ(float translation) {
	translate(glm::vec3(0, 0, translation));
}

void GeometricObject::rotate(float angle, glm::vec3 axis) {
	model = glm::rotate(model, angle, axis);
}
void GeometricObject::rotateX(float angle) {
	rotate(angle, glm::vec3(1.0, 0, 0));
}
void GeometricObject::rotateY(float angle) {
	rotate(angle, glm::vec3(0, 1.0, 0));
}
void GeometricObject::rotateZ(float angle) {
	rotate(angle, glm::vec3(0, 0, 1.0));
}

void GeometricObject::scale(glm::vec3 factors) {
	model = glm::scale(model, factors);
}
void GeometricObject::scaleX(float factor) {
	scale(glm::vec3(factor, 0, 0));
}
void GeometricObject::scaleY(float factor) {
	scale(glm::vec3(0, factor, 0));
}
void GeometricObject::scaleZ(float factor) {
	scale(glm::vec3(0, 0, factor));
}